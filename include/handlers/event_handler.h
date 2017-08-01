#ifndef DISRUPTOR_CPP_INCLUDE_EVENT_HANDLER_H_
#define DISRUPTOR_CPP_INCLUDE_EVENT_HANDLER_H_

#include "common.h"
#include "exceptions/exit_exception.h"

namespace disruptor_cpp {
template <typename T>
class EventHandler {
public:
	virtual ~EventHandler() {}
	virtual void OnStart() = 0;
	/*
	 * Called when publisher publishes an event to the ring buffer.
	 * Throws ExitException to indicate this event processor should exit.
	 */
	virtual void OnEvent(T event, int64 sequence, bool end_of_batch) throw(ExitException) = 0;
	virtual void OnShutDown() = 0;
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_EVENT_HANDLER_H_
