#include "Technique.h"
#include "Pass.h"

namespace oengine2d {
	Technique::~Technique() {
		for (auto* pass : _passes)
			delete pass;
		_passes.clear();
	}

	void Technique::MergeOption(const RenderOption& option) {
		_option.Merge(option);
		for (auto* pass : _passes)
			pass->MergeOption(_option);
	}
}
