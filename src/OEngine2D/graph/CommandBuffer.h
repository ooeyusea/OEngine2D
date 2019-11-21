#pragma once

#include "util.h"

namespace oengine2d {
	class CommandPool;
	class CommandBuffer {
	public:
		CommandBuffer() {}
		~CommandBuffer();

		bool Create(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		operator const VkCommandBuffer& () const { return _commandBuffer; }
		bool IsRunning() const { return _running; }

		bool Begin(VkCommandBufferUsageFlags usage = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		bool End();
		bool SubmitIdle();
		bool Submit(VkSemaphore waitSemaphore, VkSemaphore singalSemaphore, VkFence inFlightFence);

	private:
		CommandPool * _commandPool = nullptr;

		VkCommandBuffer _commandBuffer = nullptr;
		bool _running = false;
	};
}