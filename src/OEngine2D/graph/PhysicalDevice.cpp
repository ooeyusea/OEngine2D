#include "PhysicalDevice.h"
#include "Instance.h"

namespace oengine2d {
	bool PhysicalDevice::Create() {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
		CHECK(deviceCount > 0, "failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

		std::multimap<int32_t, VkPhysicalDevice> rankedDevices;
		for (const auto& device : devices)
			rankedDevices.emplace(ScorePhysicalDevice(device), device);

		if (rankedDevices.rbegin()->first > 0)
			_physicalDevice = rankedDevices.rbegin()->second;
		CHECK(_physicalDevice != VK_NULL_HANDLE, "failed to find suitable gpu!");

		vkGetPhysicalDeviceProperties(_physicalDevice, &_deviceProperties);
		vkGetPhysicalDeviceFeatures(_physicalDevice, &_deviceFeatures);

		std::cout << "pick physical device: " << _deviceProperties.deviceName << std::endl;
		return true;
	}

	int32_t PhysicalDevice::ScorePhysicalDevice(const VkPhysicalDevice& device) {
		int32_t score = 0;

		uint32_t extensionPropertyCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties(extensionPropertyCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, extensionProperties.data());

		for (const char* currentExtension : _deviceExtensions) {
			bool extensionFound = false;

			for (const auto& extension : extensionProperties) {
				if (strcmp(currentExtension, extension.extensionName) == 0) {
					extensionFound = true;
					break;
				}
			}

			if (!extensionFound) {
				return 0;
			}
		}

		VkPhysicalDeviceProperties physicalDeviceProperties;
		VkPhysicalDeviceFeatures physicalDeviceFeatures;
		vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(device, &physicalDeviceFeatures);

		if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}

		score += physicalDeviceProperties.limits.maxImageDimension2D;

		return score;
	}
}
