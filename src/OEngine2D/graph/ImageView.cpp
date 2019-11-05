#include "ImageView.h"
#include "Graph.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "Image.h"

namespace oengine2d {
	ImageView::ImageView(const Image& image, VkImageViewType type) : _image(image), _type(type) {
		image.AddRef();
	}

	ImageView::~ImageView() {
		_image.DecRef();

		if (_imageView)
			vkDestroyImageView(Graph::GetInstance().GetLogicalDevice(), _imageView, nullptr);
	}

	bool ImageView::Create() {
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = _image;
		viewInfo.viewType = _type;
		viewInfo.format = _image.GetFormat();
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		CHECK(vkCreateImageView(Graph::GetInstance().GetLogicalDevice(), &viewInfo, nullptr, &_imageView) == VK_SUCCESS, "failed to create texture image view!");
		return true;
	}
}