#pragma once

#include "util.h"

namespace oengine2d {
	class CommandBuffer;
	class RenderStage {
	public:
		RenderStage() {}
		~RenderStage();

		bool Create();
		void Update();

		inline VkSemaphore GetSemaphore(size_t frame) const { return _semaphores[frame]; }
		inline void SetPrev(RenderStage* prev) { _prev = prev; }
		inline void SetNext(RenderStage* next) { _next = next; }

	protected:
		virtual bool InnerCreate() = 0;
		virtual void RecordCommand(CommandBuffer& cmd) = 0;

	private:
		std::vector<CommandBuffer*> _commandBuffers;
		std::vector<VkSemaphore> _semaphores;

		RenderStage* _prev = nullptr;
		RenderStage* _next = nullptr;
	};
}