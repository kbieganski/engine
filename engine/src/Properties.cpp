#include <regex>
#include <fstream>
#include "Logger.hpp"
#include "Properties.hpp"
#include "StringUtility.hpp"


using std::ifstream;
using std::ios;
using std::regex;
using std::regex_match;
using std::string;
using std::stoi;
using std::stoul;
using std::stof;
using std::to_string;


Properties::Properties() {

}


Properties::Properties(const string& filename) {
	ifstream file(filename, ios::ate | ios::binary);
	if (!file.is_open()) {
		ERROR("Failed to open file ", filename);
	}
	auto fileSize = file.tellg();
	string contents;
	contents.resize(fileSize);
	file.seekg(0);
	file.read(&contents[0], contents.size());
	read(contents);
}


void Properties::read(const string& propertiesAsString) {
	INFO("Reading properties");
	size_t lineStart = -1;
	do {
		lineStart++;
		size_t assignmentPosition = propertiesAsString.find('=', lineStart);
		size_t keyLength = assignmentPosition - lineStart;
		size_t valuePosition = assignmentPosition + 1;
		size_t lineEnd = propertiesAsString.find('\n', valuePosition);
		size_t valueLength = lineEnd - valuePosition;
		string key = propertiesAsString.substr(lineStart, keyLength);
		string value = propertiesAsString.substr(valuePosition, valueLength);
		set(key, value);
		lineStart = lineEnd;
	} while (lineStart != string::npos);
	INFO("Finished reading properties");
}


void Properties::write(string& propertiesAsString) {
	INFO("Writing properties to string");
	size_t capacity = 0;
	for (const auto& pair : internalMap) {
		capacity += pair.first.length();
		capacity += pair.second.asString.length();
		capacity += 2;
	}
	propertiesAsString.reserve(capacity);
	for (const auto& pair : internalMap) {
		propertiesAsString += pair.first + '=' + pair.second.asString + '\n';
	}
}


void Properties::set(const string& key, bool value) {
	if (value) {
		set(key, "true");
	} else {
		set(key, "false");
	}
}


void Properties::set(const string& key, int64_t value) {
	set(key, to_string(value));
}


void Properties::set(const string& key, uint64_t value) {
	set(key, to_string(value));
}


void Properties::set(const string& key, double value) {
	set(key, StringUtility::convertToString(value));
}


void Properties::set(const string& key, const char* value) {
	set(key, string(value));
}


void Properties::set(const string& key, const string& value) {
	string trimmedKey = StringUtility::trim(key);
	string trimmedValue = StringUtility::trim(value);
	INFO("Property '", trimmedKey, "' set to '", trimmedValue, '\'');
	auto it = internalMap.find(trimmedKey);
	if (it != internalMap.end()) {
		it->second.asString = trimmedValue;
	} else {
		internalMap.insert(make_pair(trimmedKey, Value{trimmedValue}));
	}
}


bool Properties::Value::get(bool alternative) const {
	if (asString == "true" || asString == "1") {
		return true;
	} else if (asString == "false" || asString == "0") {
		return false;
	}
	return alternative;
}



int8_t Properties::Value::get(int8_t alternative) const {
	return get(static_cast<int64_t>(alternative));
}


int16_t Properties::Value::get(int16_t alternative) const {
	return get(static_cast<int64_t>(alternative));
}


int32_t Properties::Value::get(int32_t alternative) const {
	return get(static_cast<int64_t>(alternative));
}


int64_t Properties::Value::get(int64_t alternative) const {
	static regex intRegex("(\\+|-)?[[:d:]]+");
	if (regex_match(asString, intRegex)) {
		return stoi(asString);
	}
	return alternative;
}


uint8_t Properties::Value::get(uint8_t alternative) const {
	return get(static_cast<uint64_t>(alternative));
}


uint16_t Properties::Value::get(uint16_t alternative) const {
	return get(static_cast<uint64_t>(alternative));
}


uint32_t Properties::Value::get(uint32_t alternative) const {
	return get(static_cast<uint64_t>(alternative));
}


uint64_t Properties::Value::get(uint64_t alternative) const {
	static regex uintRegex("\\+?[[:d:]]+");
	if (regex_match(asString, uintRegex)) {
		return stoul(asString);
	}
	return alternative;
}


double Properties::Value::get(double alternative) const {
	static regex floatRegex("(\\+|-)?[[:d:]]+(\\.[[:d:]]+)?");
	if (regex_match(asString, floatRegex)) {
		return stof(asString);
	}
	return alternative;
}


const string& Properties::Value::get(const string& alternative) const {
	return asString;
}
