#pragma once

#include "util.h"

namespace oengine2d {
	class Instance;
	class Surface;
	class PhysicalDevice {
	public:
		PhysicalDevice(const Instance& instance) : _instance(instance) {}
		~PhysicalDevice() {}

		bool Create();

		int32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		operator const VkPhysicalDevice& () const { return _physicalDevice; }
		const VkPhysicalDeviceProperties& GetProperties() const { return _deviceProperties; }
		const VkPhysicalDeviceFeatures& GetFeatures() const { return _deviceFeatures; }

		const std::vector<const char*>& GetExtensions() const { return _deviceExtensions; }

	private:
		int32_t ScorePhysicalDevice(const VkPhysicalDevice& device);

	private:
		const Instance& _instance;

		const std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkPhysicalDevice _physicalDevice = nullptr;

		VkPhysicalDeviceProperties _deviceProperties = {};
		VkPhysicalDeviceFeatures _deviceFeatures = {};
		VkPhysicalDeviceMemoryProperties _memProperties = {};
	};
}