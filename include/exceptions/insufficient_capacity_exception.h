#ifndef DISRUPTOR_CPP_INCLUDE_INSUFFICIENT_CAPACITY_EXCEPTION_H_
#define DISRUPTOR_CPP_INCLUDE_INSUFFICIENT_CAPACITY_EXCEPTION_H_

#include <exception>

namespace disruptor_cpp {
class InsufficientException : public std::exception {
	const char* what() const throw()
	  {
	    return "Insufficient Capacity Exception";
	  }
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_INSUFFICIENT_CAPACITY_EXCEPTION_H_
