#include <Character.hpp>
#include <ContactSound.hpp>
#include "ExampleState.hpp"
#include "Orbiting.hpp"


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
		sound(scene),
		graphics(scene, engine.getGraphicsContext(), engine.getSwapChain(), engine.shaders) {
	createBalls();
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
	controls->update(dt);
	engine.cursor.setPosition(vec2(0, 0));
	scene.update<Orbiting>(dt);
	physics.update(dt);
	scene.update<Character>();
	scene.update<ContactSound>();
	sound.update();
	graphics.update();
	graphics.render();
}


void ExampleState::createBalls() {
	for (size_t i = 0; i < BALL_COUNT; i++) {
		auto dist = sqrt(64.0f * i / (BALL_COUNT - 1));
		vec3 position;
		position.x = sin(2 * PI * i / (BALL_COUNT - 1)) * dist;
		position.y = 0;
		position.z = cos(2 * PI * i / (BALL_COUNT - 1)) * dist;
		createBall(position);
	}
}


void ExampleState::createBall(vec3 position) {
	auto entity = scene.createEntity();
	scene.create<Transform>(entity);
	scene.get<Transform>(entity).setLocalPosition(position);
	scene.create<RigidBody>(entity, physics);
	scene.get<RigidBody>(entity).setShape(make_shared<btSphereShape>(1));
	scene.get<RigidBody>(entity).setMass(10);
	scene.get<RigidBody>(entity).getBody().setRestitution(1);
	scene.create<SoundSource>(entity, engine.sounds.load("ball.ogg"));
	scene.create<ContactSound>(entity);
	scene.create<ModelRender>(entity, graphics, engine.models.load("ball.mdl"));
}


void ExampleState::createBottom() {
	auto entity = scene.createEntity();
	scene.create<Transform>(entity);
	scene.get<Transform>(entity).setLocalPosition(vec3(0, -5, 0));
	scene.create<ModelRender>(entity, graphics, engine.models.load("bottom.mdl"));
	scene.create<RigidBody>(entity, physics);
	scene.get<RigidBody>(entity).setShape(engine.meshes.load("bottom.obj"));
	scene.get<RigidBody>(entity).getBody().setRestitution(0.2);
}


void ExampleState::createColumns() {
	auto angleBetweenColumns = 2 * PI / COLUMN_COUNT;
	for (size_t i = 0; i < COLUMN_COUNT; i++) {
		float angle = i * angleBetweenColumns;
		createColumn(vec3(18 * sin(angle), 3, 18 * cos(angle)));
	}
}


void ExampleState::createColumn(vec3 position) {
	auto entity = scene.createEntity();
	scene.create<Transform>(entity);
	scene.get<Transform>(entity).setLocalPosition(position);
	scene.create<ModelRender>(entity, graphics, engine.models.load("column.mdl"));
	scene.create<RigidBody>(entity, physics);
	scene.get<RigidBody>(entity).setShape(engine.meshes.load("column.obj"));
	scene.get<RigidBody>(entity).getBody().setRestitution(0.4);
}


void ExampleState::createPlayer() {
	auto character = scene.createEntity();
	scene.create<Transform>(character);
	scene.get<Transform>(character).setLocalPosition(vec3(0, -2, -15));
	scene.create<RigidBody>(character, physics);
	scene.create<Character>(character);
	auto camera = scene.createEntity();
	scene.create<Transform>(camera);
	scene.get<Transform>(camera).setParent(scene.get<Transform>(character));
	scene.get<Transform>(camera).setLocalPosition(vec3(0, 1.6, 0));
	scene.create<Camera>(camera);
	auto screenSize = engine.getSwapChain()->getScreenSize();
	auto aspectRatio = static_cast<float>(screenSize.x) / screenSize.y;
	scene.get<Camera>(camera).setAspectRatio(aspectRatio);
	graphics.setCurrentCamera(camera);
	sound.setCurrentCamera(camera);
	controls = make_unique<FirstPersonControls>(scene.get<Character>(character), graphics.getCurrentCamera(), engine.pressInput, engine.directionInput, engine.positionInput);
}


void ExampleState::createSpotlight() {
	auto entity = scene.createEntity();
	scene.create<Transform>(entity);
	auto& spotlight = scene.create<Spotlight>(entity, graphics, engine.shaders, 512);
	spotlight.setColor(vec3(0.3, 0.5, 0.6));
	spotlight.setLocalDirection(vec3(0, -1, 1));
	auto& satellite = scene.create<Orbiting>(entity);
	satellite.setPivot(vec3(0, 5, 0));
	satellite.setCycleDuration(15);
	satellite.setOrbitRadius(23);
}


void ExampleState::createSun() {
	auto entity = scene.createEntity();
	scene.create<Transform>(entity);
	scene.get<Transform>(entity).setLocalPosition(vec3(0, 2, 0));
	scene.create<Sun>(entity, graphics, engine.shaders, 2048);
	scene.get<Sun>(entity).setColor(vec3(0.7, 0.6, 0.5));
	scene.get<Sun>(entity).setLocalDirection(vec3(0, -2, 3));
	scene.create<Orbiting>(entity);
	scene.get<Orbiting>(entity).setCounterClockwise(true);
	scene.get<Orbiting>(entity).setCycleDuration(30);
}


void ExampleState::createTop() {
	auto entity = scene.createEntity();
	scene.create<Transform>(entity);
	scene.get<Transform>(entity).setLocalPosition(vec3(0, 12, 0));
	scene.create<ModelRender>(entity, graphics, engine.models.load("top.mdl"));
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
