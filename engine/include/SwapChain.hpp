#pragma once
#include "Framebuffer.hpp"
#include "RenderDescription.hpp"


class SwapChain {
public:
	SwapChain(shared_ptr<const GraphicsContext> context, VkSurfaceKHR surface, uvec2 screenSize);
	SwapChain(const SwapChain&) = delete;
	SwapChain(SwapChain&& moved);
	~SwapChain();

	SwapChain& operator=(const SwapChain&) = delete;
	SwapChain& operator=(SwapChain&& moved);

	void present() const;

	uvec2 getScreenSize() const;
	shared_ptr<const RenderPass> getRenderPass() const;
	const Framebuffer& getCurrentFramebuffer() const;


	void acquireNextImage() const;
private:
	void createSwapChain(VkSurfaceKHR surface, VkSurfaceFormatKHR surfaceFormat);
	void createRenderPass(VkFormat format);
	void createSemaphores();
	void createFramebuffers(VkFormat format);

	mutable uint32_t currentImage;
	uvec2 screenSize;
	VkSwapchainKHR handle;
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	shared_ptr<const GraphicsContext> context;
	shared_ptr<RenderPass> renderPass;
	vector<Framebuffer> framebuffers;
};
