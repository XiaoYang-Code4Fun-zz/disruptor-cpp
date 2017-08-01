#ifndef DISRUPTOR_CPP_INCLUDE_SPINNING_WAIT_STRATEGY_H_
#define DISRUPTOR_CPP_INCLUDE_SPINNING_WAIT_STRATEGY_H_

#include "sequence_barriers/sequence_barrier.h"
#include "wait_strategies/wait_strategy.h"

namespace disruptor_cpp {
/*
 * Spinning wait strategy for event processors.
 */
class SpinningWaitStrategy : public WaitStrategy {
public:
	int64 WaitFor(
		int64 request_sequence,
		Sequence *cursor,
		SequenceBarrier *barrier) throw(AlertException, TimeoutException) override {
		int64 available_sequence;
		while ((available_sequence = cursor->Get()) < request_sequence) {
			barrier->CheckAlert();
		}
		return available_sequence;
	}
	void SignalAllWhenBlocking() override {
	}
};
}

#endif // DISRUPTOR_CPP_INCLUDE_SPINNING_WAIT_STRATEGY_H_
