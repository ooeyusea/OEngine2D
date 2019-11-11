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
}
