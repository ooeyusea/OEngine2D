#include "IndexBuffer.h"
#include "../LogicalDevice.h"
#include "../Graph.h"
#include "../PhysicalDevice.h"
#include "../CommandBuffer.h"
#include "../CommandPool.h"

namespace oengine2d {
	IndexBuffer::~IndexBuffer() {
		if (_indexBufferMemory)
			vkFreeMemory(Graph::GetInstance().GetLogicalDevice(), _indexBufferMemory, nullptr);

		if (_indexBuffer)
			vkDestroyBuffer(Graph::GetInstance().GetLogicalDevice(), _indexBuffer, nullptr);

		if (_stagingBufferMemory)
			vkFreeMemory(Graph::GetInstance().GetLogicalDevice(), _stagingBufferMemory, nullptr);

		if (_stagingBuffer)
			vkDestroyBuffer(Graph::GetInstance().GetLogicalDevice(), _stagingBuffer, nullptr);
	}

	bool IndexBuffer::Create() {
		if (_type == IndexBufferMemoryType::IBMT_GENERAL) {
			CHECK(CreateBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _indexBuffer, _indexBufferMemory), "create index buffer failed");
		}
		else if (_type == IndexBufferMemoryType::IBMT_VIDEO) {
			CHECK(CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexBufferMemory), "create index buffer failed");
		}
		else {
			CHECK(CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexBufferMemory), "create index buffer failed");
			CHECK(CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _stagingBuffer, _stagingBufferMemory), "create staging buffer failed");
		}

		return true;
	}

	void IndexBuffer::Update() {
		if (_needCopy) {
			CopyBuffer();
			_needCopy = false;
		}
	}

	bool IndexBuffer::WriteBuffer(const void* p) {
		if (_type == IndexBufferMemoryType::IBMT_GENERAL) {
			void* data;
			vkMapMemory(Graph::GetInstance().GetLogicalDevice(), _indexBufferMemory, 0, _size, 0, &data);
			memcpy(data, p, _size);
			vkUnmapMemory(Graph::GetInstance().GetLogicalDevice(), _indexBufferMemory);
		}
		else if (_type == IndexBufferMemoryType::IBMT_VIDEO) {
			CHECK(CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _stagingBuffer, _stagingBufferMemory), "create staging buffer failed");

			void* data;
			vkMapMemory(Graph::GetInstance().GetLogicalDevice(), _stagingBufferMemory, 0, _size, 0, &data);
			memcpy(data, p, _size);
			vkUnmapMemory(Graph::GetInstance().GetLogicalDevice(), _stagingBufferMemory);

			bool ret = CopyBuffer();

			vkFreeMemory(Graph::GetInstance().GetLogicalDevice(), _stagingBufferMemory, nullptr);
			vkDestroyBuffer(Graph::GetInstance().GetLogicalDevice(), _stagingBuffer, nullptr);

			_stagingBufferMemory = nullptr;
			_stagingBuffer = nullptr;

			return ret;
		}
		else {
			void* data;
			vkMapMemory(Graph::GetInstance().GetLogicalDevice(), _stagingBufferMemory, 0, _size, 0, &data);
			memcpy(data, p, _size);
			vkUnmapMemory(Graph::GetInstance().GetLogicalDevice(), _stagingBufferMemory);

			_needCopy = true;
		}
		return true;
	}

	bool IndexBuffer::WriteBuffer(uint32_t offset, uint32_t size, const void* p) {
		if (_type == IndexBufferMemoryType::IBMT_VIDEO)
			return false;

		if (offset >= _size)
			return false;

		size = std::min(_size - offset, size);
		if (size > 0) {
			if (_type == IndexBufferMemoryType::IBMT_GENERAL) {
				void* data;
				vkMapMemory(Graph::GetInstance().GetLogicalDevice(), _indexBufferMemory, offset, size, 0, &data);
				memcpy(data, p, size);
				vkUnmapMemory(Graph::GetInstance().GetLogicalDevice(), _indexBufferMemory);
			}
			else {
				void* data;
				vkMapMemory(Graph::GetInstance().GetLogicalDevice(), _stagingBufferMemory, offset, size, 0, &data);
				memcpy(data, p, size);
				vkUnmapMemory(Graph::GetInstance().GetLogicalDevice(), _stagingBufferMemory);
				_needCopy = true;
			}
		}

		return true;
	}

	bool IndexBuffer::CopyBuffer() {
		CommandBuffer commandBuffer(Graph::GetInstance().GetLogicalDevice());
		commandBuffer.Begin();

		VkBufferCopy copyRegion = {};
		copyRegion.size = _size;
		vkCmdCopyBuffer(commandBuffer, _stagingBuffer, _indexBuffer, 1, &copyRegion);

		commandBuffer.SubmitIdle();
		return true;
	}

	bool IndexBuffer::CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VkBuffer& buffer, VkDeviceMemory& memory) {
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
