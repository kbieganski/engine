#pragma once
#include "GraphicsDeviceDescription.hpp"
#include "GraphicsDriver.hpp"


class GraphicsDeviceSelector {
public:
	GraphicsDeviceSelector(const GraphicsDriver& driver);

	GraphicsDeviceDescription selectBest() const;


private:
	void acquirePhysicalDeviceList(VkInstance instance, VkSurfaceKHR surface);

	vector<GraphicsDeviceDescription> devices;
};
