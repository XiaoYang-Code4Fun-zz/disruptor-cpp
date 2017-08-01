#ifndef DISRUPTOR_CPP_INCLUDE_INITIALIZATION_EXCEPTION_H_
#define DISRUPTOR_CPP_INCLUDE_INITIALIZATION_EXCEPTION_H_

#include <exception>
#include <string>

namespace disruptor_cpp {
class InitializationException : public std::exception {
public:
	InitializationException(const std::string &msg) : msg(msg) {}
	const char* what() const throw()
	  {
	    return msg.c_str();
	  }
private:
	std::string msg;
};
} // namespace disruptor_cpp

#endif // DISRUPTOR_CPP_INCLUDE_INITIALIZATION_EXCEPTION_H_
