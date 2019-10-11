#pragma once
#include "util.h"

namespace oengine2d {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;

		inline bool isComplete() {
			return graphicsFamily.has_value();
		}
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

		bool CreateDevice();
		void DestroyDevice();

	private:
		GLFWwindow* _window = nullptr;
		VkSurfaceKHR _surface = nullptr;
		VkInstance _vkInstance = nullptr;

		VkDebugUtilsMessengerEXT _callback = nullptr;

		const std::vector<const char*> _validationLayers = { "VK_LAYER_LUNARG_standard_validation" };
		VkDebugUtilsMessengerEXT _debugMessenger = nullptr;

		VkPhysicalDevice _physicalDevice = nullptr;

		VkDevice _device = nullptr;
		VkQueue _graphicsQueue = nullptr;
	};
}