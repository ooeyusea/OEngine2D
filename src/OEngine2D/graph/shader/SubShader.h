#pragma once
#include "util.h"

namespace oengine2d {
	class CommandBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class Renderable;
	class Pass;
	class Shader;
	class PropertyHandler;
	class SubShader {
	public:
		SubShader(Shader* shader) : _shader(shader) {}
		~SubShader() {}

		void RecordCommand(uint32_t frame, Renderable& object, const PropertyHandler& handler, const std::string& stage, CommandBuffer& cmd);

		inline void SetLodNeed(uint32_t lod) { _lodNeed = lod; }
		inline uint32_t GetLodNeed() const { return _lodNeed; }

		void SetShader(Shader* shader);
		void AddPass(Pass* pass);

	private:
		Shader* _shader = nullptr;
		std::vector<Pass*> _passes;
		int32_t _lodNeed = 0;
	};
}
