#ifndef DISRUPTOR_CPP_INCLUDE_EVENT_PROCESSOR_H_
#define DISRUPTOR_CPP_INCLUDE_EVENT_PROCESSOR_H_

#include "sequence.h"
#include <thread>

namespace disruptor_cpp {
class EventProcessor {
public:
	virtual ~EventProcessor() {}
	/*
	 * Returns the sequence owned by this event processor.
	 */
	virtual Sequence* GetSequence() = 0;
	/*
	 * Creates a new thread and run forever until halted.
	 */
	virtual void Run() = 0;
	virtual void Halt() = 0;
	virtual bool IsRunning() = 0;
	virtual std::thread Spawn() {
		return std::thread([this] {Run();});
	}
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_EVENT_PROCESSOR_H_
