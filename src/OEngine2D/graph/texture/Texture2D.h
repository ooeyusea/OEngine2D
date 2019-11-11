#pragma once

#include "util.h"
#include "../Resource.h"
#include "../Image.h"

namespace oengine2d {
	class Texture2D : public Image {
	public:
		Texture2D(const std::string& path, Image::Usage usage, VkFormat format, uint32_t width, uint32_t height)
			: Image(path, ResourceType::RT_TEXTURE2D, usage, format), _width(width), _height(height) {
			_size = CalcSize(format) * width * height;
		}
		Texture2D(const std::string& path, Image::Usage usage) : Image(path, ResourceType::RT_TEXTURE2D, usage) {}
		~Texture2D() {}

		bool Create();
		bool Load();
		void CopyBuffer(VkBuffer stagingBuffer);
		void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);

	private:
		bool CreateImage(VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		bool CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VkBuffer& buffer, VkDeviceMemory& memory);

	private:
		uint32_t _width = 0;
		uint32_t _height = 0;
	};
}
