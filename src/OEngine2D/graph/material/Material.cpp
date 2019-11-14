#include "Material.h"
#include "Technique.h"

namespace oengine2d {
	Material::~Material() {
		for (auto* tech : _techniques)
			delete tech;
		_techniques.clear();
	}

	void Material::MergeOption() {
		for (auto* tech : _techniques)
			tech->MergeOption(_option);
	}

	void Material::SelectTechnique(ITechniqueSelector* selector) {
		auto * technique = selector->SelectTechnique(_techniques);
		if (_currentTech != technique) {
			if (nullptr != _currentTech)
				_currentTech->Unstall();

			_currentTech = technique;
			_currentTech->Install();
		}
	}

	void Material::RecordCommand(const std::string& stage, CommandBuffer& cmd, const VertexBuffer& vb, IndexBuffer* ib) {
		if (_currentTech)
			_currentTech->RecordCommand(stage, cmd, vb, ib);
	}
}
