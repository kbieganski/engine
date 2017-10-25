#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>


using std::function;
using std::make_pair;
using std::move;
using std::string;
using std::unordered_map;
using std::vector;


template<typename Type>
class UserInput {
public:
	UserInput();
	UserInput(const UserInput&) = delete;
	UserInput(UserInput&& moved);

	UserInput<Type>& operator=(const UserInput&) = delete;
	UserInput<Type>& operator=(UserInput&& moved);

	void createBinding(const string& name);
	void invokeCallback(const string& binding, Type value) const;

	void addGetter(const string& binding, function<Type ()> getter);
	void setCallback(const string& binding, function<void (Type)> callback);

	Type get(const string& binding) const;


private:
	struct Binding {
		function<void (Type)> callback;
		vector<function<Type ()>> getters;
	};


	unordered_map<string, Binding> bindings;
};


template<typename Type>
UserInput<Type>::UserInput() {

}


template<typename Type>
UserInput<Type>::UserInput(UserInput&& moved) {
	bindings = move(moved.bindings);
}


template<typename Type>
UserInput<Type>& UserInput<Type>::operator=(UserInput&& moved) {
	bindings = move(moved.bindings);
}


template<typename Type>
void UserInput<Type>::createBinding(const string& name) {
	bindings.insert(make_pair(name, Binding { [](Type) {} }));
}


template<typename Type>
void UserInput<Type>::invokeCallback(const string& binding, Type value) const {
	auto it = bindings.find(binding);
	if (it != bindings.end()) {
		it->second.callback(value);
	}
}


template<typename Type>
void UserInput<Type>::addGetter(const string& binding, function<Type ()> getter) {
	auto it = bindings.find(binding);
	if (it != bindings.end()) {
		it->second.getters.push_back(getter);
	}
}


template<typename Type>
void UserInput<Type>::setCallback(const string& binding, function<void (Type)> callback) {
	auto it = bindings.find(binding);
	if (it != bindings.end()) {
		it->second.callback = callback;
	}
}


template<typename Type>
Type UserInput<Type>::get(const string& bindingName) const {
	auto& binding = bindings.at(bindingName);
	Type value = Type();
	for (auto getter : binding.getters) {
		value += getter();
	}
	return value;
}


template<>
bool UserInput<bool>::get(const string& bindingName) const;
