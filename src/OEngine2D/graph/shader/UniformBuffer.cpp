#include "UniformBuffer.h"
#include "../LogicalDevice.h"
#include "../Graph.h"
#include "../PhysicalDevice.h"
#include "../CommandBuffer.h"
#include "../CommandPool.h"
#include "../SwapChain.h"
#include "Pipeline.h"
#include "DescriptorSet.h"

namespace oengine2d {
	UniformBuffer::~UniformBuffer() {
		for (auto& buffer : _buffers) {
			if (buffer.uniformBufferMemory)
				vkFreeMemory(Graph::GetInstance().GetLogicalDevice(), buffer.uniformBufferMemory, nullptr);

			if (buffer.uniformBuffer)
				vkDestroyBuffer(Graph::GetInstance().GetLogicalDevice(), buffer.uniformBuffer, nullptr);
		}
		_buffers.clear();
	}

	bool UniformBuffer::Create() {
		_buffers.resize(Graph::GetInstance().GetSwapChain().GetImageCount());
		for (auto& buffer : _buffers)
			CHECK(CreateBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer.uniformBuffer, buffer.uniformBufferMemory), "create uniform buffer failed");
		_created = true;
		return true;
	}

	void UniformBuffer::GetDescriptorBuffInfo(uint32_t frame, const Pipeline& pipeline, std::vector<WriteDescriptorSet>& writes) {
		std::vector<VkDescriptorBufferInfo> ret;

		for (auto itr = _descriptions.begin(); itr != _descriptions.end(); ++itr) {
			auto binding = pipeline.GetDescriptorLocation(itr->first);
			if (binding) {
				WriteDescriptorSet write;
				write.bufferInfo.buffer = _buffers[frame].uniformBuffer;
				write.bufferInfo.offset = itr->second.offset;
				write.bufferInfo.range = itr->second.size;

				write.write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.write.dstBinding = binding.value();
				write.write.dstArrayElement = 0;
				write.write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				write.write.descriptorCount = 1;
			}
		}
	}

	bool UniformBuffer::CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VkBuffer& buffer, VkDeviceMemory& memory) {
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

	void UniformBuffer::Update(uint32_t frame, const std::string& name, const char* data, const int32_t size) {
		auto itr = _descriptions.find(name);
		if (itr != _descriptions.end() && itr->second.size >= size) {
			void* targetData;
			vkMapMemory(Graph::GetInstance().GetLogicalDevice(), _buffers[frame].uniformBufferMemory, itr->second.offset, size, 0, &targetData);
			memcpy(targetData, data, size);
			vkUnmapMemory(Graph::GetInstance().GetLogicalDevice(), _buffers[frame].uniformBufferMemory);
		}
	}
}
