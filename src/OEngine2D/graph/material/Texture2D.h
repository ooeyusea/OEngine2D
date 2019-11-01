#pragma once

#include "util.h"
#include "../Resource.h"

namespace oengine2d {
	class Texture2D : public Resource {
	public:
		enum TextureUsage {
			TU_NORMAL,
			TU_RENDER_TARGET,
		};

		Texture2D(const std::string& path, TextureUsage usage, VkFormat format, uint32_t width, uint32_t height) 
			: Resource(ResourceType::RT_TEXTURE2D, path), _usage(usage), _format(format), _width(width), _height(height) {
			_size = CalcSize(format) * width * height;
		}
		Texture2D(const std::string& path, TextureUsage usage) : Resource(ResourceType::RT_TEXTURE2D, path), _usage(usage) {}
		~Texture2D();

		bool Create();
		bool Load();
		void CopyBuffer(VkBuffer stagingBuffer);
		void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);

	private:
		bool CreateImage(VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		bool CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VkBuffer& buffer, VkDeviceMemory& memory);
		VkDeviceSize CalcSize(VkFormat format);

	private:
		TextureUsage _usage;
		VkFormat _format = VK_FORMAT_UNDEFINED;
		uint32_t _width = 0;
		uint32_t _height = 0;
		VkDeviceSize _size = 0;
		bool _compress = false;

		VkImage _texture = nullptr;
		VkDeviceMemory _textureMemory = nullptr;
	};
}
