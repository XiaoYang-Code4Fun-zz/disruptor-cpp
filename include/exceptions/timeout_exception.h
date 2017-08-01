#ifndef DISRUPTOR_CPP_INCLUDE_TIMEOUT_EXCEPTION_H_
#define DISRUPTOR_CPP_INCLUDE_TIMEOUT_EXCEPTION_H_

#include <exception>

namespace disruptor_cpp {
class TimeoutException : public std::exception {
	const char* what() const throw()
	  {
	    return "Timeout Exception";
	  }
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_TIMEOUT_EXCEPTION_H_
