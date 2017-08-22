#include "StringUtility.hpp"


using std::to_string;


namespace StringUtility {
	string trim(const string& untrimmed) {
		size_t start = untrimmed.find_first_not_of(" \t\n");
		if (start == string::npos) {
			return "";
		}
		size_t end = untrimmed.find_last_not_of(" \t\n");
		return untrimmed.substr(start, end - start + 1);
	}


	string convertToString(double value) {
		string valueAsString = to_string(value);
		size_t offset = 1;
		if (valueAsString.find_last_not_of('0') == valueAsString.find('.')) {
			offset = 0;
		}
		return valueAsString.substr(0, valueAsString.find_last_not_of('0') + offset);
	}
}
