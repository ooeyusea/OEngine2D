#ifndef __RENDEROPTION_H__
#define __RENDEROPTION_H__
#include "util.h"
#include <unordered_map>
#include "RenderState.h"
#include "../vertexdata/VertexDescription.h"

namespace oengine2d {
	class Uniform;
	class RenderOption {
	public:
		RenderOption() {}
		~RenderOption();

		void SetUniform(Uniform* u);
		void SetRenderState(const RenderState& state) { _state = state; }
		const RenderState& GetRenderState() const { return _state; }

		void SetVertexDescription(VertexDescription * desc) { _desc = desc; }
		VertexDescription * GetVertexDescription() const { return _desc; }

		void Merge(const RenderOption& rhs);

	private:
		VertexDescription * _desc = nullptr;
		RenderState _state;
		std::unordered_map<std::string, Uniform*> _uniforms;
	};
}

#endif //__RENDEROPTION_H__

