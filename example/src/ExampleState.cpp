#include <Character.hpp>
#include <ContactSound.hpp>
#include "ExampleState.hpp"


#define PI 3.142
#define BALL_COUNT 8
#define COLUMN_COUNT 16


using std::cos;
using std::make_shared;
using std::make_unique;
using std::sin;
using std::sqrt;


ExampleState::ExampleState(Engine& _engine)
	:	engine(_engine),
		physics(),
		sound(scene),
		graphics(scene, engine.getGraphicsContext(), engine.getSwapChain(), engine.shaders) {
	for (size_t i = 0; i < BALL_COUNT; i++) {
		auto dist = sqrt(64.0f * i / (BALL_COUNT - 1));
		vec3 position;
		position.x = sin(2 * PI * i / (BALL_COUNT - 1)) * dist;
		position.y = 0;
		position.z = cos(2 * PI * i / (BALL_COUNT - 1)) * dist;
		createBall(position);
	}
	createBottom();
	createColumns();
	createSpotlight();
	createSun();
	createTop();
	createPlayer();
	setAmbientColors();
	setupControls();
}


ExampleState::~ExampleState() {
	scene.clear();
}


void ExampleState::update(float dt) {
	elapsedTime += dt;

	controls->update(dt);
	engine.cursor.setPosition(vec2(0, 0));
	scene.get<Transform>(spotlightIndex).setLocalPosition(vec3(22.0f * sin(elapsedTime / 8), 5.0f, 22.0f * cos(elapsedTime / 8)));
	scene.get<Spotlight>(spotlightIndex).setLocalDirection(vec3(0.57735 * -sin(elapsedTime / 8), -0.57735, 0.57735 * -cos(elapsedTime / 8)));
	scene.get<Sun>(sunIndex).setLocalDirection(vec3(0.666666 * cos(elapsedTime / 4), -0.333333, 0.666666 * sin(elapsedTime / 4)));

	physics.update(dt);
	scene.update<Character>();
	scene.update<ContactSound>();
	sound.update();
	graphics.update();
	graphics.render();
}


void ExampleState::createBall(vec3 position) {
	scene.create<Transform>(entityIndex);
	scene.get<Transform>(entityIndex).setLocalPosition(position);
	scene.create<RigidBody>(entityIndex, physics);
	scene.get<RigidBody>(entityIndex).setShape(make_shared<btSphereShape>(1));
	scene.get<RigidBody>(entityIndex).setMass(10);
	scene.get<RigidBody>(entityIndex).getBody().setRestitution(1);
	scene.create<SoundSource>(entityIndex, engine.sounds.load("ball.ogg"));
	scene.create<ContactSound>(entityIndex);
	scene.create<ModelRender>(entityIndex, graphics, engine.models.load("ball.mdl"));
	entityIndex++;
}


void ExampleState::createBottom() {
	scene.create<Transform>(entityIndex);
	scene.get<Transform>(entityIndex).setLocalPosition(vec3(0, -5, 0));
	scene.create<ModelRender>(entityIndex, graphics, engine.models.load("bottom.mdl"));
	scene.create<RigidBody>(entityIndex, physics);
	scene.get<RigidBody>(entityIndex).setShape(engine.meshes.load("bottom.obj"));
	scene.get<RigidBody>(entityIndex).getBody().setRestitution(0.2);
	entityIndex++;
}


void ExampleState::createColumns() {
	auto angleBetweenColumns = 2 * PI / COLUMN_COUNT;
	for (size_t i = entityIndex; i < entityIndex + COLUMN_COUNT; i++) {
		scene.create<Transform>(i);
		scene.get<Transform>(i).setLocalPosition(vec3(18 * sin(i * angleBetweenColumns), 3, 18 * cos(i * angleBetweenColumns)));
		scene.create<ModelRender>(i, graphics, engine.models.load("column.mdl"));
		scene.create<RigidBody>(i, physics);
		scene.get<RigidBody>(i).setShape(engine.meshes.load("column.obj"));
		scene.get<RigidBody>(i).getBody().setRestitution(0.4);
	}
	entityIndex += COLUMN_COUNT;
}


void ExampleState::createPlayer() {
	scene.create<Transform>(entityIndex);
	scene.get<Transform>(entityIndex).setLocalPosition(vec3(0, -2, -15));
	scene.create<RigidBody>(entityIndex, physics);
	scene.create<Character>(entityIndex);
	entityIndex++;
	scene.create<Transform>(entityIndex);
	scene.get<Transform>(entityIndex).setParent(scene.get<Transform>(entityIndex - 1));
	scene.get<Transform>(entityIndex).setLocalPosition(vec3(0, 1.6, 0));
	scene.create<Camera>(entityIndex);
	auto screenSize = engine.getSwapChain()->getScreenSize();
	scene.get<Camera>(entityIndex).setAspectRatio(static_cast<float>(screenSize.x) / screenSize.y);
	graphics.setCurrentCamera(entityIndex);
	sound.setCurrentCamera(entityIndex);
	controls = make_unique<FirstPersonControls>(scene.get<Character>(entityIndex - 1), graphics.getCurrentCamera(), engine.pressInput, engine.directionInput, engine.positionInput);
	controls->setMouseSensitivity(0.5);
	entityIndex++;
}


void ExampleState::createSpotlight() {
	spotlightIndex = entityIndex;
	scene.create<Transform>(entityIndex);
	scene.create<Spotlight>(entityIndex, graphics, engine.shaders, 512);
	scene.get<Spotlight>(entityIndex).setColor(vec3(0.3, 0.5, 0.6));
	entityIndex++;
}


void ExampleState::createSun() {
	sunIndex = entityIndex;
	scene.create<Transform>(entityIndex);
	scene.get<Transform>(entityIndex).setLocalPosition(vec3(0, 2, 0));
	scene.create<Sun>(entityIndex, graphics, engine.shaders, 2048);
	scene.get<Sun>(entityIndex).setColor(vec3(0.7, 0.6, 0.5));
	scene.get<Sun>(entityIndex).setLocalDirection(vec3(1, -1, 1));
	entityIndex++;
}


void ExampleState::createTop() {
	scene.create<Transform>(entityIndex);
	scene.get<Transform>(entityIndex).setLocalPosition(vec3(0, 12, 0));
	scene.create<ModelRender>(entityIndex, graphics, engine.models.load("top.mdl"));
	entityIndex++;
}


void ExampleState::setAmbientColors() {
	graphics.setAmbientLightColor(vec3(0.08, 0.13, 0.18));
	graphics.setSkyColor(vec3(0.68, 0.82, 1));
}


void ExampleState::setupControls() {
	engine.directionInput.createBinding("movement_horizontal");
	engine.directionInput.createBinding("movement_vertical");
	engine.directionInput.createBinding("look_horizontal");
	engine.directionInput.createBinding("look_vertical");
	engine.positionInput.createBinding("mouselook");
	engine.pressInput.createBinding("jump");
	engine.keyboard.bindKey("movement_horizontal", KeyboardInput::POSITIVE, GLFW_KEY_D);
	engine.keyboard.bindKey("movement_horizontal", KeyboardInput::NEGATIVE, GLFW_KEY_A);
	engine.keyboard.bindKey("movement_vertical", KeyboardInput::POSITIVE, GLFW_KEY_W);
	engine.keyboard.bindKey("movement_vertical", KeyboardInput::NEGATIVE, GLFW_KEY_S);
	engine.keyboard.bindKey("jump", KeyboardInput::POSITIVE, GLFW_KEY_SPACE);
	if (!engine.gamepads.empty()) {
		engine.gamepads[0].bindAxis("movement_horizontal", GamepadInput::POSITIVE, 0);
		engine.gamepads[0].bindAxis("movement_vertical", GamepadInput::NEGATIVE, 1);
		engine.gamepads[0].bindAxis("look_horizontal", GamepadInput::POSITIVE, 3);
		engine.gamepads[0].bindAxis("look_vertical", GamepadInput::NEGATIVE, 4);
		engine.gamepads[0].bindButton("jump", GamepadInput::POSITIVE, 0);
	}
	engine.cursor.bindMovement("mouselook");
}
