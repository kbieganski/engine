#pragma once
#include "Framebuffer.hpp"
#include "RenderDescription.hpp"


class SwapChain {
public:
	SwapChain(shared_ptr<const GraphicsContext> context, const GraphicsDeviceDescription& deviceDescription, VkSurfaceKHR surface, uvec2 screenSize);
	SwapChain(const SwapChain&) = delete;
	SwapChain(SwapChain&& moved);
	~SwapChain();

	SwapChain& operator=(const SwapChain&) = delete;
	SwapChain& operator=(SwapChain&& moved);

	void bind(const RenderDescription& renderDescription);
	void draw();

	uvec2 getScreenSize() const;
	shared_ptr<const RenderPass> getRenderPass() const ;


private:
	void present(uint32_t imageIndex);
	void createSwapChain(const GraphicsDeviceDescription& deviceDescription, VkSurfaceKHR surface, VkSurfaceFormatKHR surfaceFormat);
	void createRenderPass(VkFormat format);
	void createFramebuffers(VkFormat format);
	void createSemaphores();

	uvec2 screenSize;
	VkSwapchainKHR handle;
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	shared_ptr<const GraphicsContext> context;
	shared_ptr<RenderPass> renderPass;
	vector<Framebuffer> framebuffers;
};
