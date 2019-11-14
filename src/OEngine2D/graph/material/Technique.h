#pragma once
#include "util.h"
#include "RenderOption.h"

namespace oengine2d {
	class Pass;
	class CommandBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class Technique {
	public:
		Technique() {}
		~Technique();

		void AddPass(Pass* pass) { _passes.push_back(pass); }

		void SetRenderState(const RenderState& state) { _option.SetRenderState(state); }
		void SetUniform(Uniform* u) { _option.SetUniform(u); }
		void MergeOption(const RenderOption& option);

		void Install();
		void Unstall();

		void RecordCommand(const std::string& stage, CommandBuffer& cmd, const VertexBuffer& vb, IndexBuffer* ib);

	private:
		RenderOption _option;
		std::list<Pass*> _passes;
	};
}
