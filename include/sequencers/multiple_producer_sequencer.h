#ifndef DISRUPTOR_CPP_INCLUDE_VARIOUS_PRODUCER_SEQUENCER_H_
#define DISRUPTOR_CPP_INCLUDE_VARIOUS_PRODUCER_SEQUENCER_H_

#include "common.h"
#include "ring_buffer.h"
#include "sequence.h"
#include "sequencers/sequencer.h"
#include "wait_strategies/wait_strategy.h"
#include <thread>
#include <vector>
#include <math.h>

namespace disruptor_cpp {

/*
 * Sequencer to manage slot claiming for multiple producers.
 */
template <typename T>
class MultipleProducerSequencer : public Sequencer {
public:
	MultipleProducerSequencer(RingBuffer<T> *ring_buffer, Sequence *cursor, WaitStrategy *wait_strategy) :
		ring_buffer_(ring_buffer), cursor_(cursor), claimed_(Sequence::InitialValue()),
		cached_minimum_gating_sequence_(Sequence::InitialValue()),
		wait_strategy_(wait_strategy) {
		slot_flags_ = new std::atomic<bool>[ring_buffer->GetBufferSize()];
	}
	~MultipleProducerSequencer() {
		delete slot_flags_;
	}
	void AddGatingSequences(Sequence* sequence) override {
		gating_sequences_.push_back(sequence);
	}
	SequenceRange Next(int n) override {
		int64 current_claimed = claimed_.Get();
		int request_n = std::min(n, ring_buffer_->GetBufferSize());
		int64 new_claimed;
		do {
			new_claimed = current_claimed + request_n;
			int64 wrap_point = new_claimed - ring_buffer_->GetBufferSize();
			while (wrap_point > cached_minimum_gating_sequence_.Get()) {
				wait_strategy_->SignalAllWhenBlocking();
				std::this_thread::yield();
				cached_minimum_gating_sequence_.Set(Sequence::GetMinimumSequence(gating_sequences_, Sequence::InitialValue()));
			}
		} while (!claimed_.CompareAndSet(&current_claimed, new_claimed));
		SequenceRange range(current_claimed + 1, new_claimed);
		UpdateSlotFlags(range, false);
		return range;
	}
	void Publish(int64 sequence) override {
		SequenceRange range(sequence, sequence);
		BatchPublish(range);
	}
	void BatchPublish(const SequenceRange &range) override {
		UpdateSlotFlags(range, true);
		AdvanceCursorIfApplicable(range.High());
		wait_strategy_->SignalAllWhenBlocking();
	}
private:
	void UpdateSlotFlags(const SequenceRange &range, bool value) {
		for (int64 i = range.Low(); i <= range.High(); i ++) {
			slot_flags_[ring_buffer_->CalculateIndex(i)] = value;
		}
	}
	void AdvanceCursorIfApplicable(int64 sequence) {
		int64 current_cursor = cursor_->Get();
		if (sequence <= current_cursor) {
			return;
		} else {
			int64 new_cursor = current_cursor;
			for (int64 i = current_cursor + 1; i <= sequence; i ++) {
				if (slot_flags_[ring_buffer_->CalculateIndex(i)] == true) {
					new_cursor = i;
					continue;
				} else {
					break;
				}
			}
			while (!cursor_->CompareAndSet(&current_cursor, new_cursor)) {
				if (new_cursor <= current_cursor) {
					// Other producers have already advance the cursor beyond this new cursor.
					break;
				}
			}
		}
	}
	RingBuffer<T> *ring_buffer_;
	std::atomic<bool> *slot_flags_;
	Sequence *cursor_;
	Sequence claimed_;
	Sequence cached_minimum_gating_sequence_;
	WaitStrategy *wait_strategy_;
	std::vector<Sequence*> gating_sequences_;
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_VARIOUS_PRODUCER_SEQUENCER_H_
