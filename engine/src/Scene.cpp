#include <Scene.hpp>


void Scene::clear() {
	destroyFunctions.clear();
	componentSets.clear();
}


EntityId Scene::createEntity() {
	return nextEntity++;
}


void Scene::destroyEntity(EntityId id) {
	for (auto& destroyFunction : destroyFunctions) {
		destroyFunction.second(id);
	}
}
