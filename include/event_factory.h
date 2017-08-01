#ifndef DISRUPTOR_CPP_INCLUDE_EVENT_FACTORY_H_
#define DISRUPTOR_CPP_INCLUDE_EVENT_FACTORY_H_

#include "common.h"

template <typename T>
class EventFactory {
public:
	virtual ~EventFactory() {}
	virtual T NewInstance() = 0;
	virtual void CopyEvent(T &dst, T src) = 0;
	virtual void ClearEvent(T event) = 0;
};

#endif // DISRUPTOR_CPP_INCLUDE_EVENT_FACTORY_H_
