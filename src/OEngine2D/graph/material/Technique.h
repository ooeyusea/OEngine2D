#pragma once
#include "util.h"
#include "RenderOption.h"

namespace oengine2d {
	class Pass;
	class Technique {
	public:
		Technique() {}
		~Technique();

		void AddPass(Pass* pass) { _passes.push_back(pass); }

		void SetRenderState(const RenderState& state) { _option.SetRenderState(state); }
		void SetUniform(Uniform* u) { _option.SetUniform(u); }
		void MergeOption(const RenderOption& option);

	private:
		RenderOption _option;
		std::list<Pass*> _passes;
	};
}
