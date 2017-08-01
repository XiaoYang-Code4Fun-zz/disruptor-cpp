#ifndef DISRUPTOR_CPP_INCLUDE_TIMEOUT_HANDLER_H_
#define DISRUPTOR_CPP_INCLUDE_TIMEOUT_HANDLER_H_

#include "common.h"

namespace disruptor_cpp {
class TimeoutHandler {
public:
	virtual ~TimeoutHandler() {}
  virtual void OnTimeout(int64 sequence) = 0;
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_TIMEOUT_HANDLER_H_
