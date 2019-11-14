#pragma once
#include "util.h"
#include "RenderOption.h"

namespace oengine2d {
	class Pipeline;
	class CommandBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class Pass {
	public:
		Pass() {}
		~Pass() {}

		void SetUniform(Uniform* uniform) { _option.SetUniform(uniform); }
		void MergeOption(const RenderOption& option) { _option.Merge(option); }
		void SetStage(const std::string& stage) { _stage = stage; }
		const std::string& GetStage() const { return _stage; }

		bool CreatePipeline();
		void DestroyPipeline();

		void Draw(CommandBuffer& cmd, const VertexBuffer& vb, IndexBuffer* ib);

	private:
		RenderOption _option;
		std::string _stage;
		std::string _vertexShader;
		std::string _fragmentShader;

		Pipeline* _pipeline = nullptr;
	};
}
