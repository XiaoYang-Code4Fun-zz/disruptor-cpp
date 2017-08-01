#ifndef DISRUPTOR_CPP_INCLUDE_PROCESSING_SEQUENCE_BARRIER_H_
#define DISRUPTOR_CPP_INCLUDE_PROCESSING_SEQUENCE_BARRIER_H_

#include "common.h"
#include "sequence.h"
#include "sequencers/sequencer.h"
#include "sequence_barriers/sequence_barrier.h"
#include "wait_strategies/wait_strategy.h"
#include "exceptions/alert_exception.h"
#include <atomic>

namespace disruptor_cpp {

/*
 * Gates event processors by cursor and optional dependencies.
 */
class ProcessingSequenceBarrier : public SequenceBarrier {
public:
	ProcessingSequenceBarrier(
		WaitStrategy *wait_strategy,
		Sequence *cursor) :
		wait_strategy_(wait_strategy),
		cursor_(cursor),
		alerted_(false) {
		// TODO: support dependencies of even processors.
	}
	int64 WaitFor(int64 request_sequence) throw(AlertException) override {
		CheckAlert();
		int64 available_sequence = wait_strategy_->WaitFor(request_sequence, cursor_, this);
		return available_sequence;
	}
	void CheckAlert() throw (AlertException) override {
		if (alerted_) {
			throw AlertException();
		}
	}
	void ClearAlert() override {
		alerted_ = false;
	}
	bool IsAlerted() override {
		return alerted_;
	}
	void Alert() override {
		alerted_ = true;
		wait_strategy_->SignalAllWhenBlocking();
	}
private:
	WaitStrategy *wait_strategy_;
	Sequence *cursor_;
	std::atomic<bool> alerted_;
};
}

#endif // DISRUPTOR_CPP_INCLUDE_PROCESSING_SEQUENCE_BARRIER_H_
