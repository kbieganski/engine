#pragma once
#include <unordered_map>


using std::make_pair;
using std::unordered_map;

typedef uint32_t EntityId;


template<typename Component>
class ComponentSystem {
public:
	template<typename... Arguments>
	void add(EntityId entityId, Arguments&&... arguments);
	void remove(EntityId entityId);
	void clear();

	typename unordered_map<EntityId, Component>::iterator begin();
	typename unordered_map<EntityId, Component>::iterator end();

	template<typename... Arguments>
	void update(Arguments&&... arguments);

	Component& operator[](EntityId entityId);
	const Component& operator[](EntityId entityId) const;


protected:
	unordered_map<EntityId, Component> components;
};


template<typename Component> template<typename... Arguments>
void ComponentSystem<Component>::add(EntityId entityId, Arguments&&... arguments) {
	components.insert(make_pair(entityId, Component(arguments...)));
}


template<typename Component>
void ComponentSystem<Component>::remove(EntityId entityId) {
	components.erase(entityId);
}


template<typename Component>
void ComponentSystem<Component>::clear() {
	components.clear();
}


template<typename Component>
typename unordered_map<EntityId, Component>::iterator ComponentSystem<Component>::begin() {
	return components.begin();
}


template<typename Component>
typename unordered_map<EntityId, Component>::iterator ComponentSystem<Component>::end() {
	return components.end();
}


template<typename Component> template<typename... Arguments>
void ComponentSystem<Component>::update(Arguments&&... arguments) {
	for (auto& component : components) {
		component.second.update(arguments...);
	}
}


template<typename Component>
Component& ComponentSystem<Component>::operator[](EntityId entityId) {
	return components.at(entityId);
}


template<typename Component>
const Component& ComponentSystem<Component>::operator[](EntityId entityId) const {
	return components.at(entityId);
}
