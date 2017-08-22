#include <regex>
#include "Logger.hpp"
#include "Settings.hpp"
#include "StringUtility.hpp"


using std::regex;
using std::regex_match;
using std::string;
using std::stoi;
using std::stoul;
using std::stof;
using std::to_string;


void Settings::read(const string& settingsAsString) {
	INFO("Reading settings");
	size_t lineStart = -1;
	do {
		lineStart++;
		INFO(lineStart);
		size_t assignmentPosition = settingsAsString.find('=', lineStart);
		size_t keyLength = assignmentPosition - lineStart;
		size_t valuePosition = assignmentPosition + 1;
		size_t lineEnd = settingsAsString.find('\n', valuePosition);
		size_t valueLength = lineEnd - valuePosition;
		string key = settingsAsString.substr(lineStart, keyLength);
		string value = settingsAsString.substr(valuePosition, valueLength);
		INFO('\'', settingsAsString.substr(lineStart, keyLength), '\'');
		set(key, value);
		lineStart = lineEnd;
	} while (lineStart != string::npos);
	INFO("Finished reading settings");
}


void Settings::write(string& settingsAsString) {
	INFO("Writing settings to string");
	size_t capacity = 0;
	for (const auto& pair : internalMap) {
		capacity += pair.first.length();
		capacity += pair.second.asString.length();
		capacity += 2;
	}
	settingsAsString.reserve(capacity);
	for (const auto& pair : internalMap) {
		settingsAsString += pair.first + '=' + pair.second.asString + '\n';
	}
}


void Settings::set(const string& key, bool value) {
	if (value) {
		set(key, "true");
	} else {
		set(key, "false");
	}
}


void Settings::set(const string& key, int64_t value) {
	set(key, to_string(value));
}


void Settings::set(const string& key, uint64_t value) {
	set(key, to_string(value));
}


void Settings::set(const string& key, double value) {
	set(key, StringUtility::convertToString(value));
}


void Settings::set(const string& key, const char* value) {
	set(key, string(value));
}


void Settings::set(const string& key, const string& value) {
	string trimmedKey = StringUtility::trim(key);
	string trimmedValue = StringUtility::trim(value);
	INFO("Setting '", trimmedKey, "' set to '", trimmedValue, '\'');
	auto it = internalMap.find(trimmedKey);
	if (it != internalMap.end()) {
		it->second.asString = trimmedValue;
	} else {
		internalMap.insert(make_pair(trimmedKey, Value{trimmedValue}));
	}
}


bool Settings::Value::get(bool alternative) const {
	if (asString == "true" || asString == "1") {
		return true;
	} else if (asString == "false" || asString == "0") {
		return false;
	}
	return alternative;
}



int8_t Settings::Value::get(int8_t alternative) const {
	return get(static_cast<int64_t>(alternative));
}


int16_t Settings::Value::get(int16_t alternative) const {
	return get(static_cast<int64_t>(alternative));
}


int32_t Settings::Value::get(int32_t alternative) const {
	return get(static_cast<int64_t>(alternative));
}


int64_t Settings::Value::get(int64_t alternative) const {
	static regex intRegex("(\\+|-)?[[:d:]]+");
	if (regex_match(asString, intRegex)) {
		return stoi(asString);
	}
	return alternative;
}


uint8_t Settings::Value::get(uint8_t alternative) const {
	return get(static_cast<uint64_t>(alternative));
}


uint16_t Settings::Value::get(uint16_t alternative) const {
	return get(static_cast<uint64_t>(alternative));
}


uint32_t Settings::Value::get(uint32_t alternative) const {
	return get(static_cast<uint64_t>(alternative));
}


uint64_t Settings::Value::get(uint64_t alternative) const {
	static regex uintRegex("\\+?[[:d:]]+");
	if (regex_match(asString, uintRegex)) {
		return stoul(asString);
	}
	return alternative;
}


double Settings::Value::get(double alternative) const {
	static regex floatRegex("(\\+|-)?[[:d:]]+(\\.[[:d:]]+)?");
	if (regex_match(asString, floatRegex)) {
		return stof(asString);
	}
	return alternative;
}


const string& Settings::Value::get(const string& alternative) const {
	return asString;
}
