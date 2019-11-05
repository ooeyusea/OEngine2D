#include "Texture2D.h"
#include "../Graph.h"
#include "../LogicalDevice.h"
#include "../PhysicalDevice.h"
#include "Bitmap.h"
#include "../CommandBuffer.h"

namespace oengine2d {
	bool Texture2D::Create() {
		CHECK(_format != VK_FORMAT_UNDEFINED, "create texture" << "but format is undefined");
		CHECK(_size > 0, "create texture" << "but space is empty");
		CHECK(CreateImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _texture, _textureMemory), "");
		return true;
	}

	bool Texture2D::Load() {
		Bitmap image;
		CHECK(image.Load(_path), "load texture from file failed :" << _path);

		if (_format == VK_FORMAT_UNDEFINED)
			_format = image.GetColor();
		else {
			CHECK(image.Convert(_format), "convert texture format failed:" << _path);
		}

		_width = image.GetWidth();
		_height = image.GetHeight();
		_size = image.GetSize();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CHECK(CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory), "create stagingBuffer failed");

		void* data;
		vkMapMemory(Graph::GetInstance().GetLogicalDevice(), stagingBufferMemory, 0, _size, 0, &data);
		memcpy(data, image.GetData(), static_cast<size_t>(_size));
		vkUnmapMemory(Graph::GetInstance().GetLogicalDevice(), stagingBufferMemory);

		CreateImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _texture, _textureMemory);

		TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBuffer(stagingBuffer);
		TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(Graph::GetInstance().GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(Graph::GetInstance().GetLogicalDevice(), stagingBufferMemory, nullptr);

		return true;
	}

	void Texture2D::CopyBuffer(VkBuffer stagingBuffer) {
		CommandBuffer commandBuffer(Graph::GetInstance().GetLogicalDevice());
		commandBuffer.Begin();

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { _width, _height, 1 };

		vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, _texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		commandBuffer.SubmitIdle();
	}

	void Texture2D::TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout) {
		CommandBuffer commandBuffer(Graph::GetInstance().GetLogicalDevice());
		commandBuffer.Begin();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = _texture;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		commandBuffer.SubmitIdle();
	}

	bool Texture2D::CreateImage(VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = _width;
		imageInfo.extent.height = _height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = _format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		CHECK(vkCreateImage(Graph::GetInstance().GetLogicalDevice(), &imageInfo, nullptr, &image) == VK_SUCCESS, "failed to create image!");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(Graph::GetInstance().GetLogicalDevice(), image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Graph::GetInstance().GetPhysicalDevice().FindMemoryType(memRequirements.memoryTypeBits, properties);
		CHECK(allocInfo.memoryTypeIndex != -1, "unsupport memory type");

		CHECK(vkAllocateMemory(Graph::GetInstance().GetLogicalDevice(), &allocInfo, nullptr, &imageMemory) == VK_SUCCESS, "failed to allocate image memory!");

		vkBindImageMemory(Graph::GetInstance().GetLogicalDevice(), image, imageMemory, 0);
		return true;
	}

	bool Texture2D::CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VkBuffer& buffer, VkDeviceMemory& memory) {
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		CHECK(vkCreateBuffer(Graph::GetInstance().GetLogicalDevice(), &bufferInfo, nullptr, &buffer) == VK_SUCCESS, "failed to create index buffer!");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Graph::GetInstance().GetLogicalDevice(), buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Graph::GetInstance().GetPhysicalDevice().FindMemoryType(memRequirements.memoryTypeBits, flags);
		CHECK(allocInfo.memoryTypeIndex != -1, "unsupport memory type");
		CHECK(vkAllocateMemory(Graph::GetInstance().GetLogicalDevice(), &allocInfo, nullptr, &memory) == VK_SUCCESS, "failed to allocate index buffer memory!");

		vkBindBufferMemory(Graph::GetInstance().GetLogicalDevice(), buffer, memory, 0);
		return true;
	}
}