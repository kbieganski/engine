#include <gli/gli.hpp>
#include "Logger.hpp"
#include "StagingBuffer.hpp"
#include "Texture.hpp"


using std::move;


Texture::Texture(shared_ptr<const GraphicsContext> context, const string& filename) {
	ownsImage = true;
	this->context = context;
	aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	gli::texture2d loadedTexture(gli::load_dds(filename));
	if (loadedTexture.empty()) {
		ERROR("Failed to load image ", filename);
	}
	INFO("Loaded image ", filename);
	size = uvec2(loadedTexture[0].extent().x, loadedTexture.extent().y);
	StagingBuffer stagingBuffer(context, loadedTexture.size(), loadedTexture.data());
	createImage(VK_FORMAT_BC1_RGB_UNORM_BLOCK, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
	allocateMemory();
	changeLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	stagingBuffer.copyTo(*this);
	changeLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	createView(VK_FORMAT_BC1_RGB_UNORM_BLOCK);
}


Texture::Texture(shared_ptr<const GraphicsContext> context, uvec2 size, Description description) {
	ownsImage = true;
	this->size = size;
	aspectFlags = description.aspectFlags;
	this->context = context;
	createImage(description.format, description.usageFlags);
	allocateMemory();
	changeLayout(description.layout);
	createView(description.format);
}


Texture::Texture(shared_ptr<const GraphicsContext> context, VkImage image, uvec2 size, VkFormat format, VkImageAspectFlags aspectFlags, VkImageLayout layout) {
	ownsImage = false;
	this->context = context;
	this->image = image;
	this->memory = VK_NULL_HANDLE;
	this->size = size;
	this->aspectFlags = aspectFlags;
	this->currentLayout = layout;
	createView(format);
}


Texture::Texture(Texture&& moved) {
	*this = move(moved);
}


Texture::~Texture() {
	if (view != VK_NULL_HANDLE) {
		vkDestroyImageView(context->getDevice(), view, nullptr);
		INFO("Destroyed view ", view, " for texture ", image);
	}
	if (ownsImage && image != VK_NULL_HANDLE) {
		vkDestroyImage(context->getDevice(), image, nullptr);
		INFO("Destroyed texture ", image);
	}
	if (memory != VK_NULL_HANDLE) {
		vkFreeMemory(context->getDevice(), memory, nullptr);
		INFO("Freed memory for texture ", image);
	}
}


Texture& Texture::operator=(Texture&& moved) {
	ownsImage = moved.ownsImage;
	size = moved.size;
	image = moved.image;
	view = moved.view;
	memory = moved.memory;
	aspectFlags = moved.aspectFlags;
	currentLayout = moved.currentLayout;
	context = moved.context;
	moved.image = VK_NULL_HANDLE;
	moved.view = VK_NULL_HANDLE;
	moved.memory = VK_NULL_HANDLE;
	return *this;
}


VkImage Texture::getImage() const {
	return image;
}


VkImageView Texture::getView() const {
	return view;
}


uvec2 Texture::getSize() const {
	return size;
}


void Texture::createImage(VkFormat format, VkImageUsageFlags usageFlags) {
	VkImageCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.imageType = VK_IMAGE_TYPE_2D;
	createInfo.extent.width = size.x;
	createInfo.extent.height = size.y;
	createInfo.extent.depth = 1;
	createInfo.mipLevels = 1;
	createInfo.arrayLayers = 1;
	createInfo.format = format;
	createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	createInfo.usage = usageFlags;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	if (vkCreateImage(context->getDevice(), &createInfo, nullptr, &image) != VK_SUCCESS) {
		ERROR("Failed to create texture");
	}
	INFO("Created texture ", image);
}


void Texture::createView(VkFormat format) {
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = format;
	createInfo.subresourceRange.aspectMask = aspectFlags;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;
	if (vkCreateImageView(context->getDevice(), &createInfo, nullptr, &view) != VK_SUCCESS) {
		ERROR("Failed to create texture view");
	}
	INFO("Created view ", view, " for texture ", image);
}


void Texture::allocateMemory() {
	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(context->getDevice(), image, &memoryRequirements);
	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = context->findMemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryRequirements.memoryTypeBits);
	if (vkAllocateMemory(context->getDevice(), &memoryAllocateInfo, nullptr, &memory) != VK_SUCCESS) {
		ERROR("Failed to allocate texture memory");
	}
	INFO("Allocated ", memoryAllocateInfo.allocationSize, " B of memory for texture ", image);
	vkBindImageMemory(context->getDevice(), image, memory, 0);
}


void Texture::changeLayout(VkImageLayout newLayout) {
	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = currentLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = aspectFlags;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;
	if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		ERROR("Unsupported layout transition");
	}
	auto commandBuffer = context->createCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	vkEndCommandBuffer(commandBuffer);
	context->submit(commandBuffer);
	context->waitForGraphicsQueue();
	context->destroy(commandBuffer);
	currentLayout = newLayout;
}
