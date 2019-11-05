#pragma once

#include "util.h"

namespace oengine2d {
	class CommandBuffer;
	class RenderStage {
	public:
		RenderStage() {}
		virtual ~RenderStage();

		bool Create();
		void Update();

		inline VkSemaphore GetSemaphore(size_t frame) const { return _semaphores[frame]; }
		inline void SetPrev(RenderStage* prev) { _prev = prev; }
		inline void SetNext(RenderStage* next) { _next = next; }

		operator const VkRenderPass& () const { return _renderPass; }

	protected:
		virtual bool InnerCreate() = 0;
		virtual void RecordCommand(CommandBuffer& cmd) = 0;

	protected:
		VkRenderPass _renderPass = nullptr;

	private:
		std::vector<CommandBuffer*> _commandBuffers;
		std::vector<VkSemaphore> _semaphores;

		RenderStage* _prev = nullptr;
		RenderStage* _next = nullptr;
	};
}