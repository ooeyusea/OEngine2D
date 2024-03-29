#pragma once

#include "util.h"

namespace oengine2d {
	class LogicalDevice;
	class Surface;
	class SwapChain {
	public:
		SwapChain(const Surface& surface, const LogicalDevice& device) : _surface(surface), _device(device) {}
		~SwapChain();

		bool Create();

		bool AcquireNextImage(VkSemaphore waitSemaphore, VkFence fence);
		bool Present(VkSemaphore waitSemaphore);

		inline uint32_t GetImageIndex() const { return _imageIndex; }
		inline size_t GetImageCount() const { return _images.size(); }

		inline VkFormat GetColorFormat() const { return _imageFormat; }
		inline VkFormat GetDepthFormat() const { return _depthFormat; }

	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		bool CreateSwapChain();
		void DestroySwapChain();

		bool CreateImageViews();
		void DestroyImageViews();

	private:
		const Surface& _surface;
		const LogicalDevice& _device;

		VkExtent2D _extent = {};
		VkFormat _imageFormat = VK_FORMAT_UNDEFINED;
		VkFormat _depthFormat = VK_FORMAT_UNDEFINED;

		VkSwapchainKHR _swapChain = nullptr;

		std::vector<VkImage> _images;
		std::vector<VkImageView> _imageViews;

		uint32_t _imageIndex = 0;
	};
}