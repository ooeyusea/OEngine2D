#include "SwapChain.h"
#include "Window.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "Surface.h"

namespace oengine2d {
	SwapChain::~SwapChain() {
		DestroyImageViews();
		DestroySwapChain();
	}

	bool SwapChain::Create() {
		CHECK(CreateSwapChain(), "create swap chain");
		CHECK(CreateImageViews(), "create image views");
		return true;
	}

	VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
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

	VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
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

	VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			VkExtent2D actualExtent = { Window::GetInstance().GetSize().x, Window::GetInstance().GetSize().y };

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	bool SwapChain::CreateSwapChain() {
		VkSurfaceFormatKHR surfaceFormat = _surface.GetFormat();
		VkPresentModeKHR presentMode = _surface.GetPresentMode();
		_extent = ChooseSwapExtent(_surface.GetCapabilities());

		uint32_t imageCount = _surface.GetCapabilities().minImageCount + 1;
		if (_surface.GetCapabilities().maxImageCount > 0 && imageCount > _surface.GetCapabilities().maxImageCount) {
			imageCount = _surface.GetCapabilities().maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = _surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = _extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t queueFamilyIndices[] = { (uint32_t)_device.GetGraphicsFamily(), (uint32_t)_device.GetPresentFamily() };
		if (_device.GetGraphicsFamily() != _device.GetPresentFamily()) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}
		createInfo.preTransform = _surface.GetCapabilities().currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
			return false;
		}

		vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, nullptr);
		_images.resize(imageCount);
		vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, _images.data());
		_imageFormat = surfaceFormat.format;

		return true;
	}

	void SwapChain::DestroySwapChain() {
		if (_swapChain) {
			vkDestroySwapchainKHR(_device, _swapChain, nullptr);
		}
	}

	bool SwapChain::CreateImageViews() {
		for (size_t i = 0; i < _images.size(); i++) {
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = _images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = _imageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			VkImageView imageView = nullptr;
			if (vkCreateImageView(_device, &createInfo, nullptr, &imageView) != VK_SUCCESS) {
				return false;
			}

			_imageViews.emplace_back(imageView);
		}
		return true;
	}

	void SwapChain::DestroyImageViews() {
		for (VkImageView imageView : _imageViews) {
			vkDestroyImageView(_device, imageView, nullptr);
		}
	}
}
