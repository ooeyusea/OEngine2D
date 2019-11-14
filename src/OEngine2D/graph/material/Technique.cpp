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

	void Technique::Install() {
		for (auto* pass : _passes)
			pass->CreatePipeline();
	}

	void Technique::Unstall() {
		for (auto* pass : _passes)
			pass->DestroyPipeline();
	}

	void Technique::RecordCommand(const std::string& stage, CommandBuffer& cmd, const VertexBuffer& vb, IndexBuffer* ib) {
		for (auto* pass : _passes) {
			if (pass->GetStage() == stage)
				pass->Draw(cmd, vb, ib);
		}
	}
}
