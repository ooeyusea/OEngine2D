#pragma once

#include "util.h"
#include "singleton.h"

namespace oengine2d {
	class Instance {
	public:
		Instance() {}
		~Instance();

		bool Create();

		bool CreateVkInstance();
		void DestroyVkInstance();

		bool SetupMessageCallback();
		bool CheckValidationLayerSupport();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void DestroyMessageCallback();

		operator const VkInstance& () const { return _vkInstance; }
		const std::vector<const char*>& GetValidationLayers() const { return _validationLayers; }

	private:
		VkInstance _vkInstance = nullptr;
		VkDebugUtilsMessengerEXT _callback = nullptr;

		const std::vector<const char*> _validationLayers = { "VK_LAYER_LUNARG_standard_validation" };
		VkDebugUtilsMessengerEXT _debugMessenger = nullptr;
	};
}