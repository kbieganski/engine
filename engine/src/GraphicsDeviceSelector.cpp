#include <algorithm>
#include <functional>
#include "GraphicsDeviceSelector.hpp"
#include "Logger.hpp"


using std::find_if;
using std::transform;


GraphicsDeviceSelector::GraphicsDeviceSelector(const GraphicsDriver& driver) {
	acquirePhysicalDeviceList(driver.getInstance(), driver.getSurface());
}


GraphicsDeviceDescription GraphicsDeviceSelector::selectBest() const {
	vector<uint32_t> deviceScores(devices.size());
	transform(devices.begin(), devices.end(), deviceScores.begin(),
		[](const auto& device) { return device.rateDevice(); });
	uint32_t maxScoreIndex = 0;
	for (uint32_t i = 0; i < devices.size(); ++i) {
		if (deviceScores[i] > deviceScores[maxScoreIndex]) {
			maxScoreIndex = i;
		}
	}
	if (deviceScores[maxScoreIndex] == 0) {
		ERROR("No suitable device available");
	}
	return devices[maxScoreIndex];
}


void GraphicsDeviceSelector::acquirePhysicalDeviceList(VkInstance instance, VkSurfaceKHR surface) {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		ERROR("No device supporting Vulkan available");
	}
	vector<VkPhysicalDevice> deviceHandles(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, deviceHandles.data());
	devices.reserve(deviceCount);
	for (auto deviceHandle : deviceHandles) {
		devices.push_back(GraphicsDeviceDescription(deviceHandle, surface));
	}
}
