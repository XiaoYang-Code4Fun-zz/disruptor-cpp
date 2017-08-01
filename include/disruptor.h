#ifndef DISRUPTOR_CPP_INCLUDE_DISRUPTOR_H_
#define DISRUPTOR_CPP_INCLUDE_DISRUPTOR_H_

#include "common.h"
#include "event_factory.h"
#include "ring_buffer.h"
#include "sequence.h"
#include "sequence_barriers/sequence_barrier.h"
#include "sequence_barriers/processing_sequence_barrier.h"
#include "sequencers/sequence_range.h"
#include "sequencers/sequencer.h"
#include "sequencers/single_producer_sequencer.h"
#include "sequencers/multiple_producer_sequencer.h"
#include "wait_strategies/wait_strategy.h"
#include "wait_strategies/blocking_wait_strategy.h"
#include "wait_strategies/spinning_wait_strategy.h"
#include "wait_strategies/yielding_wait_strategy.h"
#include "processors/event_processor.h"
#include "processors/batch_event_processor.h"
#include "sequence_barriers/sequence_barrier.h"
#include "handlers/event_handler.h"
#include <atomic>
#include <thread>
#include <vector>

namespace disruptor_cpp {

enum ProducerType {
	SINGLE,
	MULTIPLE
};

enum WaitStrategyType {
	BLOCKING,
	YIELDING,
	SPINNING
};

template <typename T>
class Disruptor {
public:
	Disruptor(int buffer_size, ProducerType producer_type, WaitStrategyType wait_strategy_type,
			EventFactory<T> *event_factory) :
			running_(false) {
		ring_buffer_ = new RingBuffer<T>(buffer_size, event_factory);
		switch (wait_strategy_type) {
		case BLOCKING:
			wait_strategy_ = new BlockingWaitStrategy();
			break;
		case SPINNING:
			wait_strategy_ = new SpinningWaitStrategy();
			break;
		case YIELDING:
		default:
			wait_strategy_ = new YieldingWaitStrategy();
		}
		switch (producer_type) {
		case SINGLE:
			sequencer_ = new SingleProducerSequencer(buffer_size, ring_buffer_->GetCursor(), wait_strategy_);
			break;
		case MULTIPLE:
		default:
			sequencer_ = new MultipleProducerSequencer<T>(ring_buffer_, ring_buffer_->GetCursor(), wait_strategy_);
		}
		sequence_barrier_ = new ProcessingSequenceBarrier(wait_strategy_, ring_buffer_->GetCursor());
	}
	~Disruptor() {
		Stop();
		for (int i = 0; i < event_processors_.size(); i ++) {
			delete event_processors_[i];
		}
		delete sequence_barrier_;
		delete sequencer_;
		delete wait_strategy_;
		delete ring_buffer_;
	}
	void HandleWith(EventHandler<T>* event_handler) {
		event_processors_.push_back(new BatchEventProcessor<T>(event_handler, sequence_barrier_, ring_buffer_));
	}
	void Start() {
		if (running_) {
			return;
		}
		running_ = true;
		for (int i = 0; i < event_processors_.size(); i ++) {
			sequencer_->AddGatingSequences(event_processors_[i]->GetSequence());
			thread_buffer_.push_back(event_processors_[i]->Spawn());
		}
	}
	void Stop() {
		if (!running_) {
			return;
		}
		running_ = false;
		for (int i = 0; i < event_processors_.size(); i ++) {
					event_processors_[i]->Halt();
		}
		// Wait up all consumers which will detect the alert and exit.
		wait_strategy_->SignalAllWhenBlocking();
		for (int i = 0; i < thread_buffer_.size(); i ++) {
			thread_buffer_[i].join();
		}
	}
	SequenceRange Next(int n) {
		return sequencer_->Next(n);
	}
	void Update(int64 sequence, T event) {
		ring_buffer_->StoreEvent(sequence, event);
	}
	void Publish(int64 sequence) {
		sequencer_->Publish(sequence);
	}
	void BatchPublish(const SequenceRange &range) {
		sequencer_->BatchPublish(range);
	}
	void WaitUntilFinish() {
		if (!running_) {
			return;
		}
		running_ = false;
		for (int i = 0; i < thread_buffer_.size(); i ++) {
			thread_buffer_[i].join();
		}
	}
private:
	std::atomic<bool> running_;
	RingBuffer<T> *ring_buffer_;
	WaitStrategy *wait_strategy_;
	Sequencer *sequencer_;
	SequenceBarrier *sequence_barrier_;
	std::vector<EventProcessor*> event_processors_;
	std::vector<std::thread> thread_buffer_;
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_DISRUPTOR_H_
