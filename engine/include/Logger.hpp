#pragma once
#include "BuildConfiguration.hpp"
#include "Exception.hpp"

#ifdef LOG_LEVEL_TRACE
#define LOG_LEVEL_INFO
#endif

#ifdef LOG_LEVEL_INFO
#define LOG_LEVEL_WARNING
#endif

#ifdef LOG_LEVEL_WARNING
#define LOG_LEVEL_ERROR
#endif

#ifdef LOG_LEVEL_ERROR
#if defined LOG_TO_FILE || defined LOG_TO_STDOUT
#define LOGGING_ENABLED
#endif
#ifndef LOG_DATE_FORMAT
#define LOG_DATE_FORMAT "%d.%m.%Y %H:%M:%S"
#endif
#endif

#ifdef LOGGING_ENABLED
#include <cstring>

#ifdef LOG_TO_FILE
#include <fstream>
#endif
#ifdef LOG_TO_STDOUT
#include <iostream>
#endif


#ifdef LOG_TO_STDOUT
using std::cout;
#endif
using std::endl;
#ifdef LOG_TO_FILE
using std::fstream;
#endif


namespace Logger {
#ifdef LOG_TO_FILE
	extern fstream outputFile;
#endif


	template<typename Last>
	void print(Last last) {
#ifdef LOG_TO_FILE
		outputFile << last << endl;
#endif
#ifdef LOG_TO_STDOUT
		cout << last << endl;
#endif
	}


	template<typename First, typename... Rest>
	void print(First first, Rest... rest) {
#ifdef LOG_TO_FILE
		outputFile << first;
#endif
#ifdef LOG_TO_STDOUT
		cout << first;
#endif
		print(rest...);
	}


	extern void initialize();
	extern void finalize();
	extern string getTimestamp(const char* format);
}

#define INITIALIZE_LOGGER() Logger::initialize()
#define FINALIZE_LOGGER() Logger::finalize()
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#else
#define INITIALIZE_LOGGER()
#define FINALIZE_LOGGER()
#endif

#if defined LOGGING_ENABLED && defined LOG_LEVEL_TRACE
#define TRACE(...)														\
	Logger::print(Logger::getTimestamp(LOG_DATE_FORMAT), " | TRACE | ", \
				  __FILENAME__, '(', __LINE__, "):", __func__, " | ", __VA_ARGS__)
#else
#define TRACE(...)
#endif

#if defined LOGGING_ENABLED && defined LOG_LEVEL_INFO
#define INFO(...)\
	Logger::print(Logger::getTimestamp(LOG_DATE_FORMAT), " | INFO | ",	\
				  __FILENAME__, '(', __LINE__, "):", __func__, " | ", __VA_ARGS__)
#else
#define INFO(...)
#endif

#if defined LOGGING_ENABLED && defined LOG_LEVEL_WARNING
#define WARNING(...)\
	Logger::print(Logger::getTimestamp(LOG_DATE_FORMAT), " | WARNING | ", \
				  __FILENAME__, '(', __LINE__, "):", __func__, " | ", __VA_ARGS__)
#else
#define WARNING(...)
#endif

#if defined LOGGING_ENABLED && defined LOG_LEVEL_ERROR
#define ERROR(...)\
	Logger::print(Logger::getTimestamp(LOG_DATE_FORMAT), " | ERROR | ",\
				  __FILENAME__, '(', __LINE__, "):", __func__, " | ", __VA_ARGS__);\
	throw Exception(__VA_ARGS__)

#define ASSERT(check, ...)\
	if (!(check))\
		Logger::print(Logger::getTimestamp(LOG_DATE_FORMAT), " | ASSERTION ", #check, " FAILED | ",\
					  __FILENAME__, '(', __LINE__, "):", __func__, " | ", __VA_ARGS__); \
	if (!(check))\
		throw Exception(__VA_ARGS__)

#else
#define ERROR(...) throw Exception(__VA_ARGS__)
#define ASSERT(...)
	if (!(check))\
		throw Exception(__VA_ARGS__)
#endif
