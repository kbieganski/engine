#pragma once
#include "BuildConfiguration.hpp"

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
#include <string>


using std::string;


namespace Logger {
	extern std::fstream outputFile;


	template<typename Last>
	void print(Last last) {
#ifdef LOG_TO_FILE
		outputFile << last << std::endl;
#endif
#ifdef LOG_TO_STDOUT
		std::cout << last << std::endl;
#endif
	}


	template<typename First, typename... Rest>
	void print(First first, Rest... rest) {
#ifdef LOG_TO_FILE
		outputFile << first;
#endif
#ifdef LOG_TO_STDOUT
		std::cout << first;
#endif
		print(rest...);
	}


	extern void initialize();
	extern void finalize();
	extern std::string getTimestamp(const char* format);
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
	Logger::print(Logger::getTimestamp(LOG_DATE_FORMAT), " | TRACE | function ", \
				  __func__, " in ", __FILENAME__, '(', __LINE__, "): ", __VA_ARGS__)
#else
#define TRACE(...)
#endif

#if defined LOGGING_ENABLED && defined LOG_LEVEL_INFO
#define INFO(...)\
	Logger::print(Logger::getTimestamp(LOG_DATE_FORMAT), " | INFO | function ",	\
				  __func__, " in ", __FILENAME__, '(', __LINE__, "): ", __VA_ARGS__)
#else
#define INFO(...)
#endif

#if defined LOGGING_ENABLED && defined LOG_LEVEL_WARNING
#define WARNING(...)\
	Logger::print(Logger::getTimestamp(LOG_DATE_FORMAT), " | WARNING | function ", \
				  __func__, " in ", __FILENAME__, '(', __LINE__, "): ", __VA_ARGS__)
#else
#define WARNING(...)
#endif

#if defined LOGGING_ENABLED && defined LOG_LEVEL_ERROR
#define ERROR(...)\
	Logger::print(Logger::getTimestamp(LOG_DATE_FORMAT), " | ERROR | function ", \
				  __func__, " in ", __FILENAME__, '(', __LINE__, "): ", __VA_ARGS__)
#define ASSERT(check, ...)\
	if(!check)\
		Logger::print(Logger::getTimestamp(LOG_DATE_FORMAT), " | ASSERTION ", #check, " FAILED | function ", \
					  __func__, " in ", __FILENAME__, '(', __LINE__, "): ", __VA_ARGS__)
#else
#define ERROR(...)
#define ASSERT(...)
#endif
