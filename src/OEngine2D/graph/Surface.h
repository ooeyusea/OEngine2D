#pragma once

#include "util.h"

namespace oengine2d {
	class Instance;
	class PhysicalDevice;

	class Surface {
	public:
		Surface(const Instance& instance, const PhysicalDevice& physicalDevice) : _instance(instance), _physicalDevice(physicalDevice) {}
		~Surface();

		bool Create();

		operator const VkSurfaceKHR& () const { return _surface; }

		const VkSurfaceKHR& GetSurface() const { return _surface; }
		const VkSurfaceCapabilitiesKHR& GetCapabilities() const { return _capabilities; }
		const VkSurfaceFormatKHR& GetFormat() const { return _format; }
		const VkPresentModeKHR& GetPresentMode() const { return _presentMode; }

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

	private:
		const Instance& _instance;
		const PhysicalDevice& _physicalDevice;

		VkSurfaceKHR _surface = nullptr;
		VkSurfaceCapabilitiesKHR _capabilities = {};
		VkSurfaceFormatKHR _format = {};
		VkPresentModeKHR _presentMode = VK_PRESENT_MODE_FIFO_KHR;
	};
}