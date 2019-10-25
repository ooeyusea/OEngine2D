#pragma once

#include "util.h"

namespace oengine2d {
	class LogicalDevice;
	class CommandPool {
	public:
		CommandPool(const LogicalDevice& device) : _device(device) {}
		~CommandPool();

		bool Create();

		operator const VkCommandPool& () const { return _commandPool; }

	private:
		const LogicalDevice& _device;

		VkCommandPool _commandPool = nullptr;
	};
}