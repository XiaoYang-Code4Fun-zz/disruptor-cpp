#ifndef DISRUPTOR_CPP_INCLUDE_SINGLE_PRODUCER_SEQUENCER_H_
#define DISRUPTOR_CPP_INCLUDE_SINGLE_PRODUCER_SEQUENCER_H_

#include "common.h"
#include "sequence.h"
#include "sequencers/sequence_range.h"
#include "exceptions/insufficient_capacity_exception.h"
#include "sequencers/sequencer.h"
#include "wait_strategies/wait_strategy.h"
#include <vector>
#include <thread>
#include <math.h>

namespace disruptor_cpp {
/*
 * Sequencer to manage slot claiming for one producer thread.
 * If you have only one producer, you should use this class even though the multiple
 * producers class is also generically applicable since this class contains optimization
 * under the one producer consumption.
 * The publish method in this class cannot skip sequence - meaning you can only publish
 * sequence from low to high.
 */
class SingleProducerSequencer : public Sequencer {
public:
	SingleProducerSequencer(int buffer_size, Sequence *cursor, WaitStrategy *wait_strategy) :
		buffer_size_(buffer_size),
		cursor_(cursor), wait_strategy_(wait_strategy),
	  claimed_value_(cursor->Get()), cached_gating_value_(Sequence::InitialValue()) {}
	void AddGatingSequences(Sequence* sequence) override {
		gating_sequences_.push_back(sequence);
	}
	SequenceRange Next(int n) override {
		int64 request_n = std::min(n, buffer_size_);
		int64 current_value = claimed_value_;
		int64 request_sequence = current_value + request_n;
		int64 wrap_point = request_sequence - buffer_size_;
		while (wrap_point > cached_gating_value_) {
			wait_strategy_->SignalAllWhenBlocking();
			std::this_thread::yield();
			// Recalculate the slowest event processor slot.
			cached_gating_value_ = Sequence::GetMinimumSequence(gating_sequences_, Sequence::InitialValue());
		}
		claimed_value_ = request_sequence;
		return SequenceRange(current_value + 1, request_sequence);
	}
	void Publish(int64 sequence) override {
		cursor_->Set(sequence);
		wait_strategy_->SignalAllWhenBlocking();
	}
	void BatchPublish(const SequenceRange &range) override {
		Publish(range.High());
	}
private:
	int buffer_size_;
	Sequence* cursor_;
	WaitStrategy *wait_strategy_;
	// The currently requested slot.
	int64 claimed_value_;
	// Remember the slowest event processor slot.
	int64 cached_gating_value_;
	std::vector<Sequence*> gating_sequences_;

};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_SINGLE_PRODUCER_SEQUENCER_H_
