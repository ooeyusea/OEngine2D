#include "RenderStage.h"
#include "CommandBuffer.h"
#include "Graph.h"
#include "SwapChain.h"
#include "CommandPool.h"
#include "LogicalDevice.h"

namespace oengine2d {
	RenderStage::~RenderStage() {
		for (auto commandBuffer : _commandBuffers)
			delete commandBuffer;
		_commandBuffers.clear();

		for (auto semaphore : _semaphores)
			vkDestroySemaphore(Graph::GetInstance().GetLogicalDevice(), semaphore, nullptr);
		_semaphores.clear();
	}

	bool RenderStage::Create() {
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		for (size_t i = 0; i < Graph::GetInstance().GetSwapChain().GetImageCount(); i++) {
			VkSemaphore semaphore = nullptr;
			CHECK(vkCreateSemaphore(Graph::GetInstance().GetLogicalDevice(), &semaphoreInfo, nullptr, &semaphore) == VK_SUCCESS, "failed to create synchronization objects for a frame in stage!");
			_semaphores.emplace_back(semaphore);

			CommandBuffer* commandBuffer = new CommandBuffer(Graph::GetInstance().GetLogicalDevice());
			CHECK(commandBuffer->Create(), "failed to create command buffer!");
			_commandBuffers.emplace_back(commandBuffer);
		}

		return InnerCreate();
	}

	void RenderStage::Update() {
		size_t frame = Graph::GetInstance().GetCurrentFrame();

		CommandBuffer * commandBuffer = _commandBuffers[Graph::GetInstance().GetSwapChain().GetImageIndex()];
		commandBuffer->Begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		RecordCommand(*commandBuffer);
		commandBuffer->End();
		commandBuffer->Submit(_prev ? _prev->GetSemaphore(frame) : Graph::GetInstance().GetReadyDrawSemaphore(frame), 
			                  _next ? GetSemaphore(frame) : Graph::GetInstance().GetRenderFinishSemaphore(frame),
			                  _next ? nullptr : Graph::GetInstance().GetFlightFence(frame));
	}
}
