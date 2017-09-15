#include "Exception.hpp"


const char* Exception::what() const noexcept {
	return message.c_str();
}
