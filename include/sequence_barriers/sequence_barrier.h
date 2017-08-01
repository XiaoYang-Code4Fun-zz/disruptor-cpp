#ifndef DISRUPTOR_CPP_INCLUDE_SEQUENCE_BARRIER_H_
#define DISRUPTOR_CPP_INCLUDE_SEQUENCE_BARRIER_H_

#include "common.h"
#include "exceptions/alert_exception.h"
#include "exceptions/timeout_exception.h"

namespace disruptor_cpp {

/*
 * Sequence barrier to manage event processors' access to the ring buffer.
 */
class SequenceBarrier {
public:
	virtual ~SequenceBarrier() {}
	/*
	 * Waits for a given sequence to be available for processing. Returns the sequence up
	 * to which is available. Return value may be greater than the requested.
	 */
	virtual int64 WaitFor(int64 request_sequence) throw(AlertException, TimeoutException) = 0;
	/*
	 * Get the current state of whether it has been alerted.
	 */
	virtual bool IsAlerted() = 0;
	/*
	 * Alerts for a state change. Used to break out from the event processing loop.
	 */
	virtual void Alert() = 0;
	/*
	 * Clears alert state.
	 */
	virtual void ClearAlert() = 0;
	/*
	 * Throws AlertException if alerted.
	 */
	virtual void CheckAlert() throw (AlertException) = 0;
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_SEQUENCE_BARRIER_H_
