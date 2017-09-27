#pragma once
#include <memory>
#include "GraphicsContext.hpp"


using std::shared_ptr;
using std::string;


class Texture {
public:
	struct Description {
		VkFormat format;
		VkImageUsageFlags usageFlags;
		VkImageAspectFlags aspectFlags;
		VkImageLayout layout;
	};

	Texture(shared_ptr<const GraphicsContext> context, const string& filename);
	Texture(shared_ptr<const GraphicsContext> context, uvec2 size, Description description);
	Texture(shared_ptr<const GraphicsContext> context, VkImage image, uvec2 size, VkFormat format, VkImageAspectFlags aspectFlags, VkImageLayout layout);
	Texture(const Texture&) = delete;
	Texture(Texture&& moved);
	~Texture();

	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&& moved);

	VkImage getImage() const;
	VkImageView getView() const;
	uvec2 getSize() const;

	void changeLayout(VkImageLayout newLayout);


private:
	void createImage(VkFormat format, VkImageUsageFlags usageFlags);
	void createView(VkFormat format);
	void allocateMemory();

	bool ownsImage;
	uvec2 size;
	VkImage image;
	VkImageView view;
	VkDeviceMemory memory;
	VkImageAspectFlags aspectFlags;
	VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	shared_ptr<const GraphicsContext> context;
};
