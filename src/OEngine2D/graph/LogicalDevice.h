#pragma once

#include "util.h"

namespace oengine2d {
	class Instance;
	class PhysicalDevice;
	class Surface;

	class LogicalDevice {
	public:
		LogicalDevice(const Instance& instance, const PhysicalDevice& physicalDevice, const Surface& surface) : _instance(instance), _physicalDevice(physicalDevice), _surface(surface) {}
		~LogicalDevice();

		bool Create();

		operator const VkDevice& () const { return _device; }

		inline int32_t GetGraphicsFamily() const { return _graphicsFamily; }
		inline int32_t GetPresentFamily() const { return _presentFamily; }
		inline int32_t GetComputeFamily() const { return _computeFamily; }
		inline int32_t GetTransferFamily() const { return _transferFamily; }

	private:
		bool CreateDevice();
		void DestroyDevice();
		bool QueryQueues();

	private:
		const Instance& _instance;
		const PhysicalDevice& _physicalDevice;
		const Surface& _surface;

		VkDevice _device = nullptr;

		int32_t _graphicsFamily = -1;
		int32_t _presentFamily = -1;
		int32_t _computeFamily = -1;
		int32_t _transferFamily = -1;

		VkQueue _graphicsQueue = VK_NULL_HANDLE;
		VkQueue _presentQueue = VK_NULL_HANDLE;
		VkQueue _computeQueue = VK_NULL_HANDLE;
		VkQueue _transferQueue = VK_NULL_HANDLE;
	};
}