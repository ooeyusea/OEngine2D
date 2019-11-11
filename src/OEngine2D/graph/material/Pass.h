#pragma once
#include "util.h"
#include "RenderOption.h"

namespace oengine2d {
	enum PassStageType {
		PST_,
	};

	class Pipeline;
	class Pass {
	public:
		Pass() {}
		~Pass() {}

		void SetUniform(Uniform* uniform) { _option.SetUniform(uniform); }
		void MergeOption(const RenderOption& option) { _option.Merge(option); }

		void CreatePipeline();
		void DestroyPipeline();

	private:
		RenderOption _option;
		Pipeline* _pipeline = nullptr;
	};
}
