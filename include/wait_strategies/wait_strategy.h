#ifndef DISRUPTOR_CPP_INCLUDE_WAIT_STRATEGY_H_
#define DISRUPTOR_CPP_INCLUDE_WAIT_STRATEGY_H_

#include "common.h"
#include "sequence.h"
#include "sequence_barriers/sequence_barrier.h"
#include "exceptions/alert_exception.h"
#include "exceptions/timeout_exception.h"

namespace disruptor_cpp {

/*
 * Wait strategy to make event processes wait.
 */
class WaitStrategy {
public:
	virtual ~WaitStrategy() {}
	/*
	 * Wait for a given sequence to be available. Returns the available sequence, which
	 * may be greater than the requested.
	 */
	virtual int64 WaitFor(
			int64 request_sequence, // sequence to be waited on
			Sequence *cursor, // the main sequence from the ring buffer
			SequenceBarrier *barrier // processor barrier
	) throw(AlertException, TimeoutException) = 0;
	/*
	 * Signal all event processors that the cursor has advanced.
	 */
	virtual void SignalAllWhenBlocking() = 0;
};

}

#endif // DISRUPTOR_CPP_INCLUDE_WAIT_STRATEGY_H_
