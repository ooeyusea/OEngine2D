#include "LogicalDevice.h"
#include "Instance.h"
#include "PhysicalDevice.h"
#include "Surface.h"

namespace oengine2d {
	LogicalDevice::~LogicalDevice() {
		DestroyDevice();
	}

	bool LogicalDevice::Create() {
		CHECK(QueryQueues(), "failed to get queues!");
		CHECK(CreateDevice(), "failed to create logical device!");
		return true;
	}

	bool LogicalDevice::CreateDevice() {
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<int> uniqueQueueFamilies = { _graphicsFamily, _presentFamily };
		if (_computeFamily >= 0)
			uniqueQueueFamilies.insert(_computeFamily);

		if (_transferFamily >= 0)
			uniqueQueueFamilies.insert(_transferFamily);

		float queuePriority = 1.0f;
		for (int queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(_physicalDevice.GetExtensions().size());
		createInfo.ppEnabledExtensionNames = _physicalDevice.GetExtensions().data();

#ifdef _DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(_instance.GetValidationLayers().size());
		createInfo.ppEnabledLayerNames = _instance.GetValidationLayers().data();
#else
		createInfo.enabledLayerCount = 0;
#endif

		if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
			return false;
		}

		vkGetDeviceQueue(_device, _graphicsFamily, 0, &_graphicsQueue);
		vkGetDeviceQueue(_device, _presentFamily, 0, &_presentQueue);
		vkGetDeviceQueue(_device, _computeFamily, 0, &_computeQueue);
		vkGetDeviceQueue(_device, _transferFamily, 0, &_transferQueue);

		return true;
	}

	void LogicalDevice::DestroyDevice() {
		if (_device)
			vkDestroyDevice(_device, nullptr);
	}
	
	bool LogicalDevice::QueryQueues() {
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				_graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, i, _surface, &presentSupport);

			if (queueFamily.queueCount > 0 && presentSupport) {
				_presentFamily = i;
			}

			if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
				_computeFamily = i;
			}

			if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
				_transferFamily = i;
			}

			if (_graphicsFamily >= 0 && _presentFamily >= 0 && _computeFamily >= 0 && _transferFamily >= 0)
				break;

			i++;
		}

		if (_graphicsFamily == -1 || _presentFamily == -1)
			return false;
		return true;
	}
}
