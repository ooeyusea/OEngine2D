#pragma once
#include "util.h"
#include "RenderState.h"

namespace oengine2d {
	class Pipeline;
	class CommandBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class Renderable;
	class Shader;
	class PropertyHandler;
	class Pass {
	public:
		Pass() {}
		~Pass() { DestroyPipeline(); }

		inline void SetName(const std::string& name) { _name = name; }
		inline const std::string& GetName() const { return _name; }

		inline void SetStage(const std::string& stage) { _stage = stage; }
		inline const std::string& GetStage() const { return _stage; }

		inline void SetVertexShader(const std::string& content) { _vertexShader = content; }
		inline void SetFragmentShader(const std::string& content) { _fragmentShader = content; }

		inline void SetRenderState(const RenderState& rs) { _renderState = rs; }

		inline void SetShader(Shader* shader) { _shader = shader; }

		bool CreatePipeline();
		void DestroyPipeline();

		void Draw(uint32_t frame, Renderable& object, const PropertyHandler& handler, CommandBuffer& cmd);

	private:
		std::string _name;
		Shader* _shader = nullptr;
		RenderState _renderState;
		std::string _stage;
		std::string _vertexShader;
		std::string _fragmentShader;

		Pipeline* _pipeline = nullptr;
	};
}
