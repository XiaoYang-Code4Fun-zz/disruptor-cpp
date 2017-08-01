#ifndef DISRUPTOR_CPP_INCLUDE_EXIT_EXCEPTION_H_
#define DISRUPTOR_CPP_INCLUDE_EXIT_EXCEPTION_H_

#include <exception>

namespace disruptor_cpp {
class ExitException : public std::exception {
	const char* what() const throw()
	  {
	    return "Exit Exception";
	  }
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_EXIT_EXCEPTION_H_
