#ifndef DISRUPTOR_CPP_INCLUDE_ALERT_EXCEPTION_H_
#define DISRUPTOR_CPP_INCLUDE_ALERT_EXCEPTION_H_

#include <exception>

namespace disruptor_cpp {
class AlertException : public std::exception {
	const char* what() const throw()
	  {
	    return "Alert Exception";
	  }
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_ALERT_EXCEPTION_H_
