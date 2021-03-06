#include <algorithm>
#include "GraphicsDeviceSelector.hpp"
#include "Logger.hpp"
#include "RenderPassBuilder.hpp"
#include "SwapChain.hpp"


using std::make_shared;
using std::move;


SwapChain::SwapChain(shared_ptr<const GraphicsContext> context, VkSurfaceKHR surface, uvec2 screenSize) {
	this->context = context;
	auto& deviceDescription = context->getDeviceDescription();
	this->screenSize = deviceDescription.chooseScreenSize(screenSize);
	VkSurfaceFormatKHR surfaceFormat = deviceDescription.getBestSurfaceFormat();
	createSwapChain(surface, surfaceFormat);
	createRenderPass(surfaceFormat.format);
	createSemaphores();
	createFramebuffers(surfaceFormat.format);
}


SwapChain::SwapChain(SwapChain&& moved) {
	*this = move(moved);
}


SwapChain::~SwapChain() {
	if (imageAvailableSemaphore != VK_NULL_HANDLE) {
		vkDestroySemaphore(context->getDevice(), imageAvailableSemaphore, nullptr);
		INFO("Destroyed image available semaphore");
	}
	if (renderFinishedSemaphore != VK_NULL_HANDLE) {
		vkDestroySemaphore(context->getDevice(), renderFinishedSemaphore, nullptr);
		INFO("Destroyed render finished semaphore");
	}
	framebuffers.clear();
	if (handle != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(context->getDevice(), handle, nullptr);
		INFO("Destroyed swap chain ", handle);
	}
}


SwapChain& SwapChain::operator=(SwapChain&& moved) {
	currentImage = moved.currentImage;
	screenSize = moved.screenSize;
	handle = moved.handle;
	imageAvailableSemaphore = moved.renderFinishedSemaphore;
	renderFinishedSemaphore = moved.renderFinishedSemaphore;
	context = moved.context;
	renderPass = move(moved.renderPass);
	framebuffers = move(moved.framebuffers);
	moved.handle = VK_NULL_HANDLE;
	moved.imageAvailableSemaphore = VK_NULL_HANDLE;
	moved.renderFinishedSemaphore = VK_NULL_HANDLE;
	return *this;
}


void SwapChain::present() const {
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &handle;
	presentInfo.pImageIndices = &currentImage;
	context->present(presentInfo);
//	acquireNextImage();
}


uvec2 SwapChain::getScreenSize() const {
	return screenSize;
}


shared_ptr<const RenderPass> SwapChain::getRenderPass() const {
	return renderPass;
}


const Framebuffer& SwapChain::getCurrentFramebuffer() const {
	return framebuffers[currentImage];
}


void SwapChain::acquireNextImage() const {
	vkAcquireNextImageKHR(context->getDevice(), handle, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &currentImage);
}


void SwapChain::createSwapChain(VkSurfaceKHR surface, VkSurfaceFormatKHR surfaceFormat) {
	auto& deviceDescription = context->getDeviceDescription();
	VkPresentModeKHR presentationMode = deviceDescription.getBestPresentationMode();
	uint32_t imageCount = deviceDescription.getSurfaceCapabilities().minImageCount + 1;
	uint32_t maxImageCount = deviceDescription.getSurfaceCapabilities().maxImageCount;
	if (maxImageCount > 0 && imageCount > maxImageCount) {
		imageCount = maxImageCount;
	}
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = { screenSize.x, screenSize.y };
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = deviceDescription.getSurfaceCapabilities().currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentationMode;
	createInfo.oldSwapchain = VK_NULL_HANDLE;
	if (vkCreateSwapchainKHR(context->getDevice(), &createInfo, nullptr, &handle) != VK_SUCCESS) {
		ERROR("Failed to create swap chain");
	}
	INFO("Created swap chain ", handle);
}


void SwapChain::createRenderPass(VkFormat format) {
	RenderPassBuilder builder(context);
	builder.addColorAttachment(format, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	renderPass = make_shared<RenderPass>(builder.buildRenderPass());
}


void SwapChain::createSemaphores() {
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	if (vkCreateSemaphore(context->getDevice(), &semaphoreCreateInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS) {
		ERROR("Failed to create image available semaphore");
	}
	INFO("Created image available semaphore");
	if (vkCreateSemaphore(context->getDevice(), &semaphoreCreateInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {
		ERROR("Failed to create render finished semaphore");
	}
	INFO("Created render finished semaphore");
}


void SwapChain::createFramebuffers(VkFormat format) {
	uint32_t imageCount;
	vkGetSwapchainImagesKHR(context->getDevice(), handle, &imageCount, nullptr);
	vector<VkImage> images(imageCount);
	vkGetSwapchainImagesKHR(context->getDevice(), handle, &imageCount, images.data());
	framebuffers.reserve(images.size());
	for (auto image : images) {
		auto renderTexture = make_shared<Texture>(context, image, screenSize, format, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		auto renderTarget = make_shared<RenderTarget>(context, renderPass, renderTexture);
		framebuffers.push_back(Framebuffer(context, renderTarget));
		framebuffers.back().setClearColor(0, {{{ 0, 0, 0, 1 }}});
		framebuffers.back().addWaitSemaphore(imageAvailableSemaphore);
		framebuffers.back().addSignalSemaphore(renderFinishedSemaphore);
	}
}
