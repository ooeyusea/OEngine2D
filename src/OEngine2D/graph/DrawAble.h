#pragma once

#include "util.h"

namespace oengine2d {
	class Pipeline;
	struct DrawAble {
		virtual ~DrawAble() {}
		virtual bool RecordCommand(Pipeline& pipeline) = 0;
		virtual void Summit() = 0;
	};

	class TestDraw : public DrawAble {
	public:
		TestDraw() {}
		~TestDraw() {}

		virtual bool RecordCommand(Pipeline& pipeline);
		virtual void Summit();

	private:
		std::vector<VkCommandBuffer> _commandBuffers;
	};
}