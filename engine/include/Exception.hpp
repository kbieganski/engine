#pragma once
#include <sstream>


using std::exception;
using std::endl;
using std::string;
using std::stringstream;


class Exception : public exception {
public:
	template<typename... T>
	Exception(T... message) {
		stringstream buffer;
		write(buffer, message...);
	}


	virtual const char* what() const noexcept;


private:
	template<typename Last>
	void write(stringstream& buffer, Last last) {
		buffer << last << endl;
		message = buffer.str();
	}


	template<typename First, typename... Rest>
	void write(stringstream& buffer, First first, Rest... rest) {
		buffer << first;
		write(buffer, rest...);
	}


	string message;
};
