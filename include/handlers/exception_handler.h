#ifndef DISRUPTOR_CPP_INCLUDE_EXCEPTION_HANDLER_H_
#define DISRUPTOR_CPP_INCLUDE_EXCEPTION_HANDLER_H_

#include "common.h"
#include <exception>

namespace disruptor_cpp {
template <typename T>
class ExceptionHandler {
public:
	virtual ~ExceptionHandler() {}
	virtual void HandleEventException(std::exception e, int64 sequence, T event) = 0;
	virtual void HandleOnStartException(std::exception e) = 0;
	virtual void HandleOnShutDownException(std::exception e) = 0;
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_EXCEPTION_HANDLER_H_
