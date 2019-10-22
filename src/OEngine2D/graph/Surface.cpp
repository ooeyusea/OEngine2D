#include "Surface.h"
#include "Window.h"
#include "Instance.h"
#include "PhysicalDevice.h"

namespace oengine2d {
	Surface::~Surface() {
		if (_surface) {
			vkDestroySurfaceKHR(_instance, _surface, nullptr);
			_surface = nullptr;
		}
	}

	bool Surface::Create() {
#ifdef WIN32
		VkWin32SurfaceCreateInfoKHR createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = Window::GetInstance().GetWin32Window();
		createInfo.hinstance = GetModuleHandle(nullptr);
		createInfo.pNext = nullptr;
		createInfo.flags = 0;

		auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(_instance, "vkCreateWin32SurfaceKHR");
		if (!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR(_instance, &createInfo, nullptr, &_surface) != VK_SUCCESS) {
			return false;
		}
#endif

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, _surface, &_capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &formatCount, nullptr);
		if (formatCount <= 0)
			return false;

		std::vector<VkSurfaceFormatKHR> formats;
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &formatCount, formats.data());
		_format = ChooseSwapSurfaceFormat(formats);


		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, _surface, &presentModeCount, nullptr);
		if (presentModeCount <= 0)
			return false;

		std::vector<VkPresentModeKHR> presentModes;
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, _surface, &presentModeCount, presentModes.data());
		_presentMode = ChooseSwapPresentMode(presentModes);
		return true;
	}

	VkSurfaceFormatKHR Surface::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}

		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR Surface::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				bestMode = availablePresentMode;
			}
		}

		return bestMode;
	}
}
