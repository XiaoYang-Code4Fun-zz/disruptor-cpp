#ifndef DISRUPTOR_CPP_INCLUDE_YIELDING_WAIT_STRATEGY_H_
#define DISRUPTOR_CPP_INCLUDE_YIELDING_WAIT_STRATEGY_H_

#include "sequence_barriers/sequence_barrier.h"
#include "wait_strategies/wait_strategy.h"
#include <thread>

namespace disruptor_cpp {
namespace {
constexpr int SPIN_INTERVAL = 100;
}
/*
 * Yielding wait strategy for event processors.
 */
class YieldingWaitStrategy : public WaitStrategy {
public:
	int64 WaitFor(
		int64 request_sequence,
		Sequence *cursor,
		SequenceBarrier *barrier) throw(AlertException, TimeoutException) override {
		int64 available_sequence;
		int counter = SPIN_INTERVAL;
		while ((available_sequence = cursor->Get()) < request_sequence) {
			if (counter == 0) {
				std::this_thread::yield();
			} else {
				counter --;
			}
			barrier->CheckAlert();
		}
		return available_sequence;
	}
	void SignalAllWhenBlocking() override {
	}
};
}

#endif // DISRUPTOR_CPP_INCLUDE_YIELDING_WAIT_STRATEGY_H_
