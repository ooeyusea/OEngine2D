#pragma once
#include "util.h"

namespace oengine2d {
	struct QueueFamilyIndices {
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isComplete() {
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class Application {
	public:
		Application() {}
		~Application() {}

		void Run();

	private:
		void MainLoop();

		bool CreateGlfwWindow();
		void DestroyWindow();

		bool CreateVkInstance();
		std::vector<const char*> GetRequiredExtensions();
		void DestroyVkInstance();

		bool CreateSurface();
		void DestroySurface();

		bool SetupMessageCallback();
		bool CheckValidationLayerSupport();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void DestroyMessageCallback();

		bool PickPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		bool CreateDevice();
		void DestroyDevice();

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		bool CreateSwapChain();
		void DestroySwapChain();

		bool CreateImageViews();
		void DestroyImageViews();

	private:
		uint32_t _width = 800;
		uint32_t _height = 600;

		GLFWwindow* _window = nullptr;
		VkSurfaceKHR _surface = nullptr;
		VkInstance _vkInstance = nullptr;

		VkDebugUtilsMessengerEXT _callback = nullptr;

		const std::vector<const char*> _validationLayers = { "VK_LAYER_LUNARG_standard_validation" };
		VkDebugUtilsMessengerEXT _debugMessenger = nullptr;

		const std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkPhysicalDevice _physicalDevice = nullptr;

		VkDevice _device = nullptr;
		VkQueue _graphicsQueue = nullptr;
		VkQueue _presentQueue = nullptr;

		VkSwapchainKHR _swapChain = nullptr;
		VkFormat _swapChainImageFormat;
		VkExtent2D _extent;
		std::vector<VkImage> _swapChainImages;

		std::vector<VkImageView> _swapChainImageViews;
	};
}