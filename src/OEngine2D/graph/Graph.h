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
	class RenderStage;
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

		bool AddRenderStage(RenderStage * stage);

		LogicalDevice& GetLogicalDevice();
		SwapChain& GetSwapChain();
		CommandPoolPtr& GetCommandPool();

		size_t GetCurrentFrame() const { return _currentFrame; }
		inline VkSemaphore GetReadyDrawSemaphore(size_t frame) const { return _imageAvailableSemaphores[frame]; }
		inline VkSemaphore GetRenderFinishSemaphore(size_t frame) const { return _renderFinishedSemaphores[frame]; }
		inline VkFence GetFlightFence(size_t frame) const { return _inFlightFences[frame]; }

	private:
		void DrawFrame(CommandBuffer& commandBuffer);

	private:
		Instance* _instance = nullptr;
		Surface* _surface = nullptr;
		PhysicalDevice* _physicalDevice = nullptr;
		LogicalDevice* _logicalDevice = nullptr;
		SwapChain* _swapChain = nullptr;

		std::vector<VkSemaphore> _imageAvailableSemaphores;
		std::vector<VkSemaphore> _renderFinishedSemaphores;
		std::vector<VkFence> _inFlightFences;
		size_t _currentFrame = 0;
		
		std::vector<RenderStage*> _stages;

		std::mutex _mutex;
	};
}