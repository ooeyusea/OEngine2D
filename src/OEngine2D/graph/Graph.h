#pragma once
#include "util/util.h"
#include "util/singleton.h"

namespace oengine2d {
	class Surface;
	class Instance;
	class PhysicalDevice;
	class LogicalDevice;
	class SwapChain;
	class Graph : public Singleton<Graph> {
	public:
		Graph() {}
		~Graph() { Release(); }

		bool Init();
		void Release();
		void Update();

	private:
		Instance* _instance = nullptr;
		Surface* _surface = nullptr;
		PhysicalDevice* _physicalDevice = nullptr;
		LogicalDevice* _logicalDevice = nullptr;
		SwapChain* _swapChain = nullptr;

		uint32_t _currentImageIndex = 0;
		VkSemaphore _imageAvailableSemaphore = nullptr;
		VkSemaphore _renderFinishedSemaphore = nullptr;
	};
}