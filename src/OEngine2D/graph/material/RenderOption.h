#ifndef __RENDEROPTION_H__
#define __RENDEROPTION_H__
#include "util.h"
#include <unordered_map>
#include "RenderState.h"

namespace oengine2d {
	class Uniform;
	class RenderOption {
	public:
		RenderOption() {}
		~RenderOption();

		void SetUniform(Uniform* u);
		void SetRenderState(const RenderState& state) { _state = state; }

		void Merge(const RenderOption& rhs);

	private:
		RenderState _state;
		std::unordered_map<std::string, Uniform*> _uniforms;
	};
}

#endif //__RENDEROPTION_H__

