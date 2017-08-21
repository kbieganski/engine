#include "Logger.hpp"

#ifdef LOGGING_ENABLED
#include <iomanip>


#ifdef LOG_TO_STDOUT
using std::cout;
#endif
using std::endl;
#ifdef LOG_TO_FILE
using std::fstream;
#endif
using std::strlen;
using std::localtime;
using std::time;


namespace Logger {
#ifdef LOG_TO_FILE
	std::fstream outputFile;
#endif


	void initialize() {
		string startMessage = "App started on " + getTimestamp("%B %e, %Y at %H:%M:%S");
	#ifdef LOG_TO_FILE
		outputFile.open(getTimestamp("log%Y%m%d%H%M%S.txt"), fstream::out | fstream::trunc);
		outputFile << startMessage << endl;
	#endif
	#ifdef LOG_TO_STDOUT
		cout << startMessage << endl;
	#endif
	}


	void finalize() {
#ifdef LOG_TO_FILE
		outputFile.close();
#endif
	}


	string getTimestamp(const char* format) {
		auto t = time(nullptr);
		auto tm = *localtime(&t);
		string dateTime;
		size_t bufferLength = strlen(format) * 2;
		dateTime.resize(bufferLength);
		size_t actualLength = strftime(&dateTime.front(), bufferLength, format, &tm);
		dateTime.resize(actualLength);
		return dateTime;
	}
}

#endif
