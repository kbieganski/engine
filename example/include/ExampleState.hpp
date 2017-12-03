#pragma once
#include <Engine.hpp>
#include <PhysicsSystem.hpp>
#include <FirstPersonControls.hpp>
#include <GraphicsSystem.hpp>
#include <SoundSystem.hpp>


class ExampleState : public ApplicationState {
public:
	ExampleState(Engine& _engine);
	~ExampleState();

	virtual void update(float dt);


private:
	void createBalls();
	void createBall(vec3 position);
	void createBottom();
	void createColumns();
	void createColumn(vec3 position);
	void createPlayer();
	void createSpotlight();
	void createSun();
	void createTop();
	void setAmbientColors();
	void setupControls();

	Engine& engine;
	Scene scene;
	PhysicsSystem physics;
	SoundSystem sound;
	GraphicsSystem graphics;
	unique_ptr<FirstPersonControls> controls;
};
