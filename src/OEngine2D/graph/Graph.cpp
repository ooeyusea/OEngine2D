#include "Graph.h"
#include "Surface.h"
#include "Instance.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "SwapChain.h"
#include "Config.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "RenderStage.h"

namespace oengine2d {
	bool Graph::Init() {
		_instance = new Instance;
		CHECK(_instance->Create(), "create instance failed");

		_physicalDevice = new PhysicalDevice(*_instance);
		CHECK(_physicalDevice->Create(), "pick physical device failed");

		_surface = new Surface(*_instance, *_physicalDevice);
		CHECK(_surface->Create(), "create surface failed");

		_logicalDevice = new LogicalDevice(*_instance, *_physicalDevice, *_surface);
		CHECK(_logicalDevice->Create(), "create logical device failed");

		_swapChain = new SwapChain(*_surface, *_logicalDevice);
		CHECK(_swapChain->Create(), "create swap chain failed");

		CHECK(InitThread(), "thread local init failed");
		
		CHECK(CreateSyncObjects(), "create sync objects");
		return true;
	}

	void Graph::Release() {
		if (_logicalDevice)
			vkQueueWaitIdle(_logicalDevice->GetGraphicsQueue());

		DestroySyncObjects();

		ReleaseThread();
		SAFE_DELETE(_swapChain);
		SAFE_DELETE(_logicalDevice);
		SAFE_DELETE(_surface);
		SAFE_DELETE(_physicalDevice);
		SAFE_DELETE(_instance);
	}

	void Graph::Update() {
		_swapChain->AcquireNextImage(_imageAvailableSemaphores[_currentFrame], _inFlightFences[_currentFrame]);
		for (auto itr = _stages.begin(); itr != _stages.end(); ++itr)
			(*itr)->Update();
		_swapChain->Present(_renderFinishedSemaphores[_currentFrame]);
		_currentFrame = (_currentFrame + 1) % _inFlightFences.size();
	}

	bool Graph::InitThread() {
		GetCommandPool();
		return true;
	}

	void Graph::ReleaseThread() {
		delete GetCommandPool();
	}

	bool Graph::CreateSyncObjects() {
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		for (size_t i = 0; i < _swapChain->GetImageCount(); i++) {
			VkSemaphore imageAvailableSemaphore = nullptr;
			CHECK(vkCreateSemaphore(*_logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore) == VK_SUCCESS, "failed to create synchronization objects for a frame!");
			_imageAvailableSemaphores.emplace_back(imageAvailableSemaphore);

			VkSemaphore renderFinishedSemaphore = nullptr;
			CHECK(vkCreateSemaphore(*_logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore) == VK_SUCCESS, "failed to create synchronization objects for a frame!");
			_renderFinishedSemaphores.emplace_back(renderFinishedSemaphore);

			VkFence inFlightFence;
			CHECK(vkCreateFence(*_logicalDevice, &fenceInfo, nullptr, &inFlightFence) == VK_SUCCESS, "failed to create synchronization objects for a frame!");
			_inFlightFences.emplace_back(inFlightFence);
		}

		return true;
	}

	void Graph::DestroySyncObjects() {
		for (auto fence : _inFlightFences)
			vkDestroyFence(*_logicalDevice, fence, nullptr);
		_inFlightFences.clear();

		for (auto semaphore : _imageAvailableSemaphores)
			vkDestroySemaphore(*_logicalDevice, semaphore, nullptr);
		_imageAvailableSemaphores.clear();

		for (auto semaphore : _renderFinishedSemaphores)
			vkDestroySemaphore(*_logicalDevice, semaphore, nullptr);
		_renderFinishedSemaphores.clear();
	}

	bool Graph::AddRenderStage(RenderStage* stage) {
		CHECK(stage->Create(), "create stage failed");
		if (!_stages.empty()) {
			_stages.back()->SetNext(stage);
			stage->SetPrev(_stages.back());
		}

		_stages.emplace_back(stage);
		return true;
	}

	LogicalDevice& Graph::GetLogicalDevice() {
		return *_logicalDevice;
	}

	SwapChain& Graph::GetSwapChain() {
		return *_swapChain;
	}

	Graph::CommandPoolPtr& Graph::GetCommandPool() {
		static thread_local CommandPool* s_commandPool = nullptr;
		if (!s_commandPool) {
			std::lock_guard<std::mutex> lock(_mutex);
			s_commandPool = new CommandPool(*_logicalDevice);
			s_commandPool->Create();
		}
		return s_commandPool;
	}

	void Graph::DrawFrame(CommandBuffer& commandBuffer) {

	}
}
