#include "RenderOption.h"
#include "Uniform.h"

namespace oengine2d {
	RenderOption::~RenderOption() {
		for (auto itr = _uniforms.begin(); itr != _uniforms.end(); ++itr)
			itr->second->Release();
		_uniforms.clear();
	}

	void RenderOption::SetUniform(Uniform* u) {
		auto itr = _uniforms.find(u->GetName());
		if (itr != _uniforms.end())
			itr->second->Release();
		_uniforms[u->GetName()] = u;
	}

	void RenderOption::Merge(const RenderOption& rhs) {
		_state.Merge(rhs._state);
		_desc = rhs._desc;
		for (auto itr2 = rhs._uniforms.begin(); itr2 != rhs._uniforms.end(); ++itr2) {
			auto itr = _uniforms.find(itr2->first);
			if (itr != _uniforms.end())
				itr->second->Merge(itr2->second);
			else
				_uniforms[itr2->first] = itr2->second->Clone();
		}
	}
}
