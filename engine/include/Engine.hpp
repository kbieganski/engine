#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "ApplicationState.hpp"
#include "AssetCache.hpp"
#include "CursorInput.hpp"
#include "GamepadInput.hpp"
#include "KeyboardInput.hpp"
#include "Model.hpp"
#include "MouseInput.hpp"
#include "SoundBuffer.hpp"
#include "SwapChain.hpp"
#include "Texture.hpp"


class Engine {
public:
	Engine(GLFWwindow* window, shared_ptr<const GraphicsContext> context, shared_ptr<const SwapChain> swapChain);
	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;

	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) = delete;

	void changeState(shared_ptr<ApplicationState> newState);
	void stop();

	shared_ptr<const GraphicsContext> getGraphicsContext() const;
	shared_ptr<const SwapChain> getSwapChain() const;

	AssetCache<Shader> shaders;
	AssetCache<Mesh> meshes;
	AssetCache<Texture> textures;
	AssetCache<SoundBuffer> sounds;
	AssetCache<Material> materials;
	AssetCache<Model> models;
	MouseInput mouse;
	CursorInput cursor;
	KeyboardInput keyboard;
	vector<GamepadInput> gamepads;
	UserInput<vec2> positionInput;
	UserInput<float> directionInput;
	UserInput<bool> pressInput;

private:
	void run();

	bool running;
	GLFWwindow* window;
	shared_ptr<ApplicationState> currentState;
	shared_ptr<ApplicationState> nextState;
	shared_ptr<const GraphicsContext> context;
	shared_ptr<const SwapChain> swapChain;
};
