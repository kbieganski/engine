#pragma once
#include <map>


using std::string;
using std::map;


class Properties {
public:
	void read(const string& propertiesAsString);
	void write(string& propertiesAsString);

	void set(const string& key, bool value);
	void set(const string& key, int64_t value);
	void set(const string& key, uint64_t value);
	void set(const string& key, double value);
	void set(const string& key, const char* value);
	void set(const string& key, const string& value);

	template<typename Type>
	Type get(const string& key, const Type& alternative = Type()) const;

private:
	struct Value {
		string asString;

		bool get(bool alternative) const;
		int8_t get(int8_t alternative) const;
		int16_t get(int16_t alternative) const;
		int32_t get(int32_t alternative) const;
		int64_t get(int64_t alternative) const;
		uint8_t get(uint8_t alternative) const;
		uint16_t get(uint16_t alternative) const;
		uint32_t get(uint32_t alternative) const;
		uint64_t get(uint64_t alternative) const;
		double get(double alternative) const;
		const string& get(const string& alternative) const;
	};

	map<string, Value> internalMap;
};


template<typename Type>
Type Properties::get(const string& key, const Type& alternative) const {
	auto it = internalMap.find(key);
	if (it != internalMap.end()) {
		return internalMap.at(key).get(alternative);
	}
	return alternative;
}
