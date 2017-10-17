#pragma once
#include <functional>
#include <string>
#include <unordered_map>


using std::function;
using std::make_pair;
using std::move;
using std::string;
using std::unordered_map;


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

	void setCallback(const string& binding, function<void (Type)> callback);
	void setGetter(const string& binding, function<Type ()> getter);

	Type get(const string& binding) const;


private:
	struct Binding {
		function<void (Type)> callback;
		function<Type ()> getter;
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
	bindings.insert(make_pair(name, Binding { [](Type) {}, []() { return Type(); } }));
}


template<typename Type>
void UserInput<Type>::invokeCallback(const string& binding, Type value) const {
	auto it = bindings.find(binding);
	if (it != bindings.end()) {
		it->second.callback(value);
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
void UserInput<Type>::setGetter(const string& binding, function<Type ()> getter) {
	auto it = bindings.find(binding);
	if (it != bindings.end()) {
		it->second.getter = getter;
	}
}


template<typename Type>
Type UserInput<Type>::get(const string& binding) const {
	return bindings.at(binding).getter();
}
