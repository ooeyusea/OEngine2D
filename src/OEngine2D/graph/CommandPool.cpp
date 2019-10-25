#include "CommandPool.h"
#include "LogicalDevice.h"

namespace oengine2d {
	CommandPool::~CommandPool() {
		if (_commandPool) {
			vkDestroyCommandPool(_device, _commandPool, nullptr);
			_commandPool = nullptr;
		}
	}

	bool CommandPool::Create() {
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = _device.GetGraphicsFamily();
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		CHECK(vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool) == VK_SUCCESS, "failed to create command pool!");
		return true;
	}
}
