#ifndef DISRUPTOR_CPP_INCLUDE_BLOCKING_WAIT_STRATEGY_H_
#define DISRUPTOR_CPP_INCLUDE_BLOCKING_WAIT_STRATEGY_H_

#include "sequence_barriers/sequence_barrier.h"
#include "wait_strategies/wait_strategy.h"
#include <mutex>
#include <condition_variable>

namespace disruptor_cpp {
/*
 * Lock and condition based blocking wait strategy for event processors.
 */
class BlockingWaitStrategy : public WaitStrategy {
public:
	int64 WaitFor(
		int64 request_sequence,
		Sequence *cursor,
		SequenceBarrier *barrier) throw(AlertException, TimeoutException) override {
		int64 available_sequence;
		while ((available_sequence = cursor->Get()) < request_sequence) {
			std::unique_lock<std::mutex> lk(m);
			processor_notify_condition.wait(lk);
			barrier->CheckAlert();
		}
		return available_sequence;
	}
	void SignalAllWhenBlocking() override {
		processor_notify_condition.notify_all();
	}
private:
	std::mutex m;
	std::condition_variable processor_notify_condition;
};
}

#endif // DISRUPTOR_CPP_INCLUDE_BLOCKING_WAIT_STRATEGY_H_
