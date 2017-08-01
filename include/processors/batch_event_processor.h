#ifndef DISRUPTOR_CPP_INCLUDE_BATCH_EVENT_PROCESSOR_H_
#define DISRUPTOR_CPP_INCLUDE_BATCH_EVENT_PROCESSOR_H_

#include "common.h"
#include "handlers/event_handler.h"
#include "processors/event_processor.h"
#include "exceptions/alert_exception.h"
#include "exceptions/exit_exception.h"
#include "ring_buffer.h"
#include "sequence.h"
#include "sequence_barriers/sequence_barrier.h"
#include <algorithm>
#include <atomic>

namespace disruptor_cpp {

template <typename T>
class BatchEventProcessor : public EventProcessor {
public:
	BatchEventProcessor() = delete;
	BatchEventProcessor(EventHandler<T> *event_handler, SequenceBarrier *sequence_barrier,
			RingBuffer<T> *ring_buffer):
		running_(false),
		processed_value_(Sequence::InitialValue()),
		sequence_(Sequence::InitialValue()), event_handler_(event_handler), sequence_barrier_(sequence_barrier),
		ring_buffer_(ring_buffer) {

	}
	Sequence* GetSequence() override {
		return &sequence_;
	}
	/*
	 * Run forever until halted.
	 */
	void Run() override {
		if (running_) {
			// Already running.
			return;
		}
		event_handler_->OnStart();
		// int64 next_sequence;
		int64 available_sequence;
		while (true) {
			try {
				available_sequence = sequence_barrier_->WaitFor(processed_value_ + 1);
				for (int64 i = processed_value_ + 1; i <= available_sequence; i ++) {
					T event = ring_buffer_->GetEvent(i);
					event_handler_->OnEvent(event, i, i == available_sequence);
				}
				sequence_.Set(available_sequence);
				processed_value_ = available_sequence;
			} catch (AlertException &alert) {
				if (!running_) {
					break;
				}
			} catch (ExitException &exit) {
				running_ = false;
				break;
			}
		}
		event_handler_->OnShutDown();
		running_ = false;
	}
	void Halt() override {
		running_ = false;
		sequence_barrier_->Alert();
	}
	bool IsRunning() override {
		return running_;
	}
private:
	std::atomic<bool> running_;
	int64 processed_value_;
	// The sequence up to which this processor has already processed.
	Sequence sequence_;
	EventHandler<T> *event_handler_;
	SequenceBarrier *sequence_barrier_;
	RingBuffer<T> *ring_buffer_;
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_BATCH_EVENT_PROCESSOR_H_
