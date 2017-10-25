#include "UserInput.hpp"


template<>
bool UserInput<bool>::get(const string& bindingName) const {
	auto& binding = bindings.at(bindingName);
	for (auto getter : binding.getters) {
		if (getter()) {
			return true;
		}
	}
	return false;
}
