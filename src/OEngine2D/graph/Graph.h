#pragma once
#include "util/util.h"
#include "util/singleton.h"
#include <mutex>

namespace oengine2d {
	class Surface;
	class Instance;
	class PhysicalDevice;
	class LogicalDevice;
	class SwapChain;
	class CommandPool;
	class CommandBuffer;
	class Graph : public Singleton<Graph> {
		typedef CommandPool* CommandPoolPtr;
	public:
		Graph() {}
		~Graph() { Release(); }

		bool Init();
		void Release();
		void Update();

		bool InitThread();
		void ReleaseThread();

		bool CreateSyncObjects();
		void DestroySyncObjects();

		LogicalDevice& GetLogicalDevice();
		CommandPoolPtr& GetCommandPool();

	private:
		void DrawFrame(CommandBuffer& commandBuffer);

	private:
		Instance* _instance = nullptr;
		Surface* _surface = nullptr;
		PhysicalDevice* _physicalDevice = nullptr;
		LogicalDevice* _logicalDevice = nullptr;
		SwapChain* _swapChain = nullptr;

		std::vector<CommandBuffer*> _commandBuffers;

		uint32_t _currentImageIndex = 0;
		std::vector<VkSemaphore> _imageAvailableSemaphores;
		std::vector<VkSemaphore> _renderFinishedSemaphores;
		std::vector<VkFence> _inFlightFences;
		size_t _currentFrame = 0;

		std::mutex _mutex;
	};
}