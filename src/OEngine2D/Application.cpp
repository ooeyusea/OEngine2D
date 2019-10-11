#include "Application.h"

#define INIT_STEP(f, msg) \
	if (!(f)) {\
		std::cout << msg << " failed" << std::endl; \
		break;\
	}\
	std::cout << msg << " success" << std::endl

#define CHECK(p, msg) \
	if (!(p)) {\
		std::cout << msg << std::endl; \
		return false;\
	}\

namespace oengine2d {
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}
	void Application::Run() {
		do {
			INIT_STEP(CreateGlfwWindow(), "create glfw window");
			INIT_STEP(CreateVkInstance(), "create vulkan instance");
#ifdef _DEBUG
			INIT_STEP(SetupMessageCallback(), "setup message callback");
#endif
			INIT_STEP(PickPhysicalDevice(), "pick physical device");
			INIT_STEP(CreateDevice(), "create logical device");

			MainLoop();
		} while (false);

#ifdef _DEBUG
		DestroyMessageCallback();
#endif
		DestroyDevice();
		DestroyVkInstance();
		DestroyWindow();
	}

	void Application::MainLoop() {
		while (!glfwWindowShouldClose(_window)) {
			glfwPollEvents();
		}

	}

	bool Application::CreateGlfwWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		_window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

		if (!_window) {
			return false;
		}
	}

	void Application::DestroyWindow() {
		if (_window)
			glfwDestroyWindow(_window);

		glfwTerminate();
	}

	std::vector<const char*> Application::GetRequiredExtensions() {
		std::vector<const char*> extensions;

		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (unsigned int i = 0; i < glfwExtensionCount; i++) {
			extensions.push_back(glfwExtensions[i]);
		}

#ifdef _DEBUG
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
		return extensions;
	}

	bool Application::CreateVkInstance() {
#ifdef _DEBUG
		CHECK(CheckValidationLayerSupport(), "not support validation layer");
#endif

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef _DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
		createInfo.ppEnabledLayerNames = _validationLayers.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#else
		createInfo.enabledLayerCount = 0;
#endif

		return vkCreateInstance(&createInfo, nullptr, &_vkInstance) == VK_SUCCESS;
	}

	void Application::DestroyVkInstance() {
		if (_vkInstance)
			vkDestroyInstance(_vkInstance, nullptr);
	}

	bool Application::CreateSurface() {
		VkWin32SurfaceCreateInfoKHR createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = glfwGetWin32Window(_window);
		createInfo.hinstance = GetModuleHandle(nullptr);

		auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(_vkInstance, "vkCreateWin32SurfaceKHR");
		if (!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR(_vkInstance, &createInfo, nullptr, &_surface) != VK_SUCCESS) {
			return false;
		}
	}

	void Application::DestroySurface() {

	}

	bool Application::SetupMessageCallback() {
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		return CreateDebugUtilsMessengerEXT(_vkInstance, &createInfo, nullptr, &_callback) == VK_SUCCESS;
	}

	bool Application::CheckValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : _validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	void Application::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
	}

	void Application::DestroyMessageCallback() {
		if (_callback)
			DestroyDebugUtilsMessengerEXT(_vkInstance, _callback, nullptr);
	}

	bool Application::PickPhysicalDevice() {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, nullptr);
		CHECK(deviceCount > 0, "failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, devices.data());

		for (const auto& device : devices) {
			if (IsDeviceSuitable(device)) {
				_physicalDevice = device;
				break;
			}
		}

		CHECK(_physicalDevice != VK_NULL_HANDLE, "failed to find suitable gpu!");

		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(_physicalDevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(_physicalDevice, &deviceFeatures);

		std::cout << "pick physical device: " << deviceProperties.deviceName << std::endl;

		return true;
	}

	bool Application::IsDeviceSuitable(VkPhysicalDevice device) {
		QueueFamilyIndices indices = FindQueueFamilies(device);

		return indices.isComplete();
	}

	QueueFamilyIndices Application::FindQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	bool Application::CreateDevice() {
		QueueFamilyIndices indices = FindQueueFamilies(_physicalDevice);

		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;

		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = 0;

#ifdef _DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
		createInfo.ppEnabledLayerNames = _validationLayers.data();
#else
		createInfo.enabledLayerCount = 0;
#endif

		if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
			return false;
		}

		vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);

		return true;
	}

	void Application::DestroyDevice() {
		if (_device)
			vkDestroyDevice(_device, nullptr);
	}
}
