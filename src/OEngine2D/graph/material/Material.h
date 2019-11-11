#pragma once
#include "util.h"
#include "RenderOption.h"
#include "../Resource.h"

namespace oengine2d {
	class Technique;
	class Material : public Resource {
	public:
		Material(const std::string& path) : Resource(ResourceType::RT_MATERIAL, path), _currentTech(nullptr) {}
		~Material();

		void AddTechnique(Technique* technique) { _techniques.push_back(technique); }

		void SetRenderState(const RenderState& state) { _option.SetRenderState(state); }
		void SetUniform(Uniform* u) { _option.SetUniform(u); }
		void MergeOption();

	private:
		RenderOption _option;
		std::list<Technique*> _techniques;
		Technique* _currentTech;
	};
}
