#pragma once

#include "util.h"

namespace oengine2d {
	class LogicalDevice;
	class CommandPool;
	class CommandBuffer {
	public:
		CommandBuffer(const LogicalDevice& device, const CommandPool& commandPool) : _device(device), _commandPool(commandPool) {}
		~CommandBuffer();

		bool Create(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		operator const VkCommandBuffer& () const { return _commandBuffer; }
		bool IsRunning() const { return _running; }

		bool Begin(VkCommandBufferUsageFlags usage = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		bool End();
		bool SubmitIdle();
		bool Submit(VkSemaphore waitSemaphore, VkSemaphore singalSemaphore, VkFence inFlightFence);

	private:
		const LogicalDevice& _device;
		const CommandPool& _commandPool;

		VkCommandBuffer _commandBuffer = nullptr;
		bool _running = false;
	};
}