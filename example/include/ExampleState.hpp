#pragma once
#include <PhysicsSystem.hpp>
#include <GraphicsSystem.hpp>
#include <SoundSystem.hpp>
#include <CharacterComponent.hpp>
#include <ContactSoundComponent.hpp>
#include <FirstPersonControls.hpp>
#include <Engine.hpp>


class ExampleState : public ApplicationState {
public:
	ExampleState(Engine& _engine);

	virtual void update(float dt);


private:
	void createBall(vec3 position);
	void createBottom();
	void createColumns();
	void createPlayer();
	void createSpotlight();
	void createSun();
	void createTop();
	void setAmbientColors();
	void setupControls();

	size_t entityIndex = 0;
	size_t sunIndex;
	size_t spotlightIndex;
	float elapsedTime = 0;
	Engine& engine;
	shared_ptr<const GraphicsContext> context;
	ComponentSystem<TransformComponent> transforms;
	ComponentSystem<CameraComponent> cameras;
	ComponentSystem<CharacterComponent> characters;
	ComponentSystem<ContactSoundComponent> frictionSounds;
	PhysicsSystem physics;
	SoundSystem sound;
	GraphicsSystem graphics;
	unique_ptr<FirstPersonControls> controls;
};
