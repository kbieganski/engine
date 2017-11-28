#pragma once
#include <unordered_map>
#include <functional>
#include <memory>

using std::unordered_map;
using std::shared_ptr;
using std::unique_ptr;
using std::function;
using std::make_shared;
using std::enable_if;
using std::is_same;

typedef size_t EntityId;
typedef size_t TypeId;


template<typename Component>
TypeId getTypeId() {
	return reinterpret_cast<TypeId>(getTypeId<Component>);
}


template<typename Dependency, typename... Dependencies>
struct DependentOn {};


class Scene {
public:
	void clear();
	EntityId createEntity();
	void destroyEntity(EntityId id);

	template<typename Component, typename... Args>
	Component& create(EntityId id, Args&&... args);

	template<typename Component>
	void destroy(EntityId id);

	template<typename Component>
	unordered_map<TypeId, Component>& get();

	template<typename Component>
	const unordered_map<TypeId, Component>& get() const;

	template<typename Component>
	Component& get(EntityId id);

	template<typename Component>
	const Component& get(EntityId id) const;

	template<typename Component, typename... Args>
	void update(Args&&... args);

private:
	template<typename Component, typename... Dependencies, typename... Args>
	Component& create(DependentOn<Dependencies...>*, EntityId id, Args&&... args);

	template<typename Component, typename... Args>
	Component& create(void*, EntityId id, Args&&... args);

	EntityId nextEntity;
	unordered_map<TypeId, shared_ptr<void>> componentSets;
	unordered_map<TypeId, function<void(EntityId)>> destroyFunctions;
};


template<typename Component, typename... Args>
Component& Scene::create(EntityId id, Args&&... args) {
	Component* _ = nullptr;
	return create<Component>(_, id, args...);
}


template<typename Component>
void Scene::destroy(EntityId id) {
	auto& components = get<Component>();
	components.erase(id);
}


template<typename Component>
unordered_map<TypeId, Component>& Scene::get() {
	auto it = componentSets.find(getTypeId<Component>());
	if (it == componentSets.end()) {
		auto components = make_shared<unordered_map<TypeId, Component>>();
		componentSets.insert(make_pair(getTypeId<Component>(), components));
		destroyFunctions.insert(make_pair(getTypeId<Component>(), [components] (EntityId id) { components->erase(id); }));
		return *components;
	} else {
		unordered_map<TypeId, Component>* components = reinterpret_cast<unordered_map<TypeId, Component>*>(it->second.get());
		return *components;
	}
}


template<typename Component>
const unordered_map<TypeId, Component>& Scene::get() const {
	auto pointer = componentSets.at(getTypeId<Component>());
	unordered_map<TypeId, Component>* components = reinterpret_cast<unordered_map<TypeId, Component>*>(pointer.get());
	return *components;
}


template<typename Component>
Component& Scene::get(EntityId id) {
	auto& components = get<Component>();
	return components.at(id);
}


template<typename Component>
const Component& Scene::get(EntityId id) const {
	auto& components = get<Component>();
	return components.at(id);
}


template<typename Component, typename... Args>
void Scene::update(Args&&... args) {
	auto& components = get<Component>();
	for (auto& component : components) {
		component.second.update(args...);
	}
}


template<typename Component, typename... Dependencies, typename... Args>
Component& Scene::create(DependentOn<Dependencies...>*, EntityId id, Args&&... args) {
	auto& components = get<Component>();
	auto result = components.insert(make_pair(id, Component(get<Dependencies>(id)..., args...)));
	return result.first->second;
}


template<typename Component, typename... Args>
Component& Scene::create(void*, EntityId id, Args&&... args) {
	auto& components = get<Component>();
	auto result = components.insert(make_pair(id, Component(args...)));
	return result.first->second;
}
