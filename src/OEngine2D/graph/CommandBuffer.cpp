#include "CommandBuffer.h"
#include "LogicalDevice.h"
#include "CommandPool.h"
#include "Graph.h"

namespace oengine2d {
	CommandBuffer::~CommandBuffer() {
		if (_commandBuffer) {
			vkFreeCommandBuffers(Graph::GetInstance().GetLogicalDevice(), *_commandPool, 1, &_commandBuffer);
			_commandBuffer = nullptr;
		}
	}

	bool CommandBuffer::Create(VkCommandBufferLevel level) {
		_commandPool = Graph::GetInstance().GetCommandPool();

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = *_commandPool;
		allocInfo.level = level;
		allocInfo.commandBufferCount = 1;

		CHECK(vkAllocateCommandBuffers(Graph::GetInstance().GetLogicalDevice(), &allocInfo, &_commandBuffer) == VK_SUCCESS, "failed to allocate command buffers!");

		return true;
	}

	bool CommandBuffer::Begin(VkCommandBufferUsageFlags usage) {
		if (_running)
			return true;

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usage; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		CHECK(vkBeginCommandBuffer(_commandBuffer, &beginInfo) == VK_SUCCESS, "failed to begin recording command buffer!");
		_running = true;
		return true;
	}

	bool CommandBuffer::End() {
		if (!_running)
			return true;

		CHECK(vkEndCommandBuffer(_commandBuffer) == VK_SUCCESS, "failed to record command buffer!");
		_running = false;
		return true;
	}

	bool CommandBuffer::SubmitIdle() {
		if (_running) {
			if (!End())
				return false;
		}

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_commandBuffer;

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		auto& device = Graph::GetInstance().GetLogicalDevice();
		VkFence fence;
		vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);

		vkResetFences(device, 1, &fence);
		vkQueueSubmit(device.GetGraphicsQueue(), 1, &submitInfo, fence);
		vkWaitForFences(device, 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max());

		vkDestroyFence(device, fence, nullptr);
		return true;
	}

	bool CommandBuffer::Submit(VkSemaphore waitSemaphore, VkSemaphore singalSemaphore, VkFence inFlightFence) {
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_commandBuffer;

		if (waitSemaphore != nullptr) {
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = &waitSemaphore;
			submitInfo.pWaitDstStageMask = waitStages;
		}

		if (singalSemaphore != nullptr) {
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &singalSemaphore;
		}

		CHECK(vkQueueSubmit(Graph::GetInstance().GetLogicalDevice().GetGraphicsQueue(), 1, &submitInfo, inFlightFence) == VK_SUCCESS, "failed to create synchronization objects for a frame!");
		return true;
	}
}
