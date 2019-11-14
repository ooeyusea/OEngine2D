#pragma once

#include "util.h"
#include "RenderPass.h"

namespace oengine2d {
	class CommandBuffer;
	class RenderStage {
	public:
		RenderStage() {}
		virtual ~RenderStage();

		bool Create();
		bool ReCreate();
		void Update();

		inline VkSemaphore GetSemaphore(size_t frame) const { return _semaphores[frame]; }
		inline void SetPrev(RenderStage* prev) { _prev = prev; }
		inline void SetNext(RenderStage* next) { _next = next; }

		operator const VkRenderPass& () const { return *_renderPass; }

		inline const std::unordered_map<std::string, uint32_t>& GetDict() const { return _dict; }

	protected:
		virtual bool InnerCreate() = 0;
		virtual void RecordCommand(CommandBuffer& cmd) = 0;

	protected:
		RenderPass * _renderPass = nullptr;

		std::vector<CommandBuffer*> _commandBuffers;
		std::vector<VkSemaphore> _semaphores;
		std::unordered_map<std::string, uint32_t> _dict;

		RenderStage* _prev = nullptr;
		RenderStage* _next = nullptr;
	};
}