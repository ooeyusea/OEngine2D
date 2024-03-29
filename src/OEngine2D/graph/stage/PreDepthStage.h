#pragma once

#include "util.h"
#include "../RenderStage.h"

namespace oengine2d {
	class RenderPass;
	class PreDepthStage : public RenderStage {
	public:
		PreDepthStage() {}
		~PreDepthStage();

		bool Create();

	private:
		RenderPass* _renderPass = nullptr;
	};
}