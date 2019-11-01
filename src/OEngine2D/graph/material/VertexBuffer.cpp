#include "VertexBuffer.h"
#include "../LogicalDevice.h"
#include "../Graph.h"
#include "../PhysicalDevice.h"
#include "../CommandBuffer.h"
#include "../CommandPool.h"

namespace oengine2d {
	void VertexDescription::CalcVertexSize() {
		for (auto& desc : _attributeDescriptions) {
			switch (desc.format) {
			case VK_FORMAT_R32_UINT: _vertexSize += sizeof(uint32_t); break;
			case VK_FORMAT_R32_SINT: _vertexSize += sizeof(int32_t); break;
			case VK_FORMAT_R32_SFLOAT: _vertexSize += sizeof(float); break;
			case VK_FORMAT_R32G32_UINT: _vertexSize += sizeof(uint32_t) * 2; break;
			case VK_FORMAT_R32G32_SINT: _vertexSize += sizeof(int32_t) * 2; break;
			case VK_FORMAT_R32G32_SFLOAT: _vertexSize += sizeof(float) * 2; break;
			case VK_FORMAT_R32G32B32_UINT: _vertexSize += sizeof(uint32_t) * 3; break;
			case VK_FORMAT_R32G32B32_SINT: _vertexSize += sizeof(int32_t) * 3; break;
			case VK_FORMAT_R32G32B32_SFLOAT: _vertexSize += sizeof(float) * 4; break;
			case VK_FORMAT_R32G32B32A32_UINT: _vertexSize += sizeof(uint32_t) * 4; break;
			case VK_FORMAT_R32G32B32A32_SINT: _vertexSize += sizeof(int32_t) * 4; break;
			case VK_FORMAT_R32G32B32A32_SFLOAT: _vertexSize += sizeof(float) * 4; break;
			}
		}
	}

	VertexBuffer::~VertexBuffer() {
		if (_vertexBufferMemory)
			vkFreeMemory(Graph::GetInstance().GetLogicalDevice(), _vertexBufferMemory, nullptr);

		if (_vertexBuffer)
			vkDestroyBuffer(Graph::GetInstance().GetLogicalDevice(), _vertexBuffer, nullptr);

		if (_stagingBufferMemory)
			vkFreeMemory(Graph::GetInstance().GetLogicalDevice(), _stagingBufferMemory, nullptr);

		if (_stagingBuffer)
			vkDestroyBuffer(Graph::GetInstance().GetLogicalDevice(), _stagingBuffer, nullptr);
	}

	bool VertexBuffer::Create() {
		if (_type == VertexBufferMemoryType::VBMT_GENERAL) {
			CHECK(CreateBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _vertexBuffer, _vertexBufferMemory), "create vertex buffer failed");
		}
		else if (_type == VertexBufferMemoryType::VBMT_VIDEO) {
			CHECK(CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexBufferMemory), "create vertex buffer failed");
		}
		else {
			CHECK(CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexBufferMemory), "create vertex buffer failed");
			CHECK(CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _stagingBuffer, _stagingBufferMemory), "create staging buffer failed");
		}

		return true;
	}

	void VertexBuffer::Update() {
		if (_needCopy) {
			CopyBuffer();
			_needCopy = false;
		}
	}

	bool VertexBuffer::WriteBuffer(const void* p) {
		if (_type == VertexBufferMemoryType::VBMT_GENERAL) {
			void* data;
			vkMapMemory(Graph::GetInstance().GetLogicalDevice(), _vertexBufferMemory, 0, _size, 0, &data);
			memcpy(data, p, _size);
			vkUnmapMemory(Graph::GetInstance().GetLogicalDevice(), _vertexBufferMemory);
		}
		else if (_type == VertexBufferMemoryType::VBMT_VIDEO) {
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

	bool VertexBuffer::WriteBuffer(uint32_t offset, uint32_t size, const void* p) {
		if (_type == VertexBufferMemoryType::VBMT_VIDEO)
			return false;

		if (offset >= _size)
			return false;

		size = std::min(_size - offset, size);
		if (size > 0) {
			if (_type == VertexBufferMemoryType::VBMT_GENERAL) {
				void* data;
				vkMapMemory(Graph::GetInstance().GetLogicalDevice(), _vertexBufferMemory, offset, size, 0, &data);
				memcpy(data, p, size);
				vkUnmapMemory(Graph::GetInstance().GetLogicalDevice(), _vertexBufferMemory);
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

	bool VertexBuffer::CopyBuffer() {
		CommandBuffer commandBuffer(Graph::GetInstance().GetLogicalDevice());
		commandBuffer.Begin();

		VkBufferCopy copyRegion = {};
		copyRegion.size = _size;
		vkCmdCopyBuffer(commandBuffer, _stagingBuffer, _vertexBuffer, 1, &copyRegion);

		commandBuffer.SubmitIdle();
		return true;
	}

	bool VertexBuffer::CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VkBuffer& buffer, VkDeviceMemory& memory) {
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		CHECK(vkCreateBuffer(Graph::GetInstance().GetLogicalDevice(), &bufferInfo, nullptr, &buffer) == VK_SUCCESS, "failed to create vertex buffer!");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Graph::GetInstance().GetLogicalDevice(), buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Graph::GetInstance().GetPhysicalDevice().FindMemoryType(memRequirements.memoryTypeBits, flags);
		CHECK(allocInfo.memoryTypeIndex != -1, "unsupport memory type");
		CHECK(vkAllocateMemory(Graph::GetInstance().GetLogicalDevice(), &allocInfo, nullptr, &memory) == VK_SUCCESS, "failed to allocate vertex buffer memory!");

		vkBindBufferMemory(Graph::GetInstance().GetLogicalDevice(), buffer, memory, 0);
		return true;
	}
}
