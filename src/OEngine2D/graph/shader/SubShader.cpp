#include "SubShader.h"
#include "Renderable.h"
#include "Pass.h"
#include "../CommandBuffer.h"
#include "Shader.h"

namespace oengine2d {
	void SubShader::RecordCommand(uint32_t frame, Renderable& object, const PropertyHandler& handler, const std::string& stage, CommandBuffer& cmd) {
		CommandBuffer* secondaryCommandBuffer = nullptr;
		bool first = true;
		for (auto* pass : _passes) {
			if (pass->GetStage() == stage) {
				if (first) {
					secondaryCommandBuffer = object.GetCommandBuffer(frame, _shader->GetPath(), stage);

					if (!object.IsChanged() && secondaryCommandBuffer) {
						vkCmdExecuteCommands(cmd, 1, &(const VkCommandBuffer&)(*secondaryCommandBuffer));
						return;
					}
					else if (object.IsChanged() && secondaryCommandBuffer)
						vkResetCommandBuffer(*secondaryCommandBuffer, 0);
					else
						secondaryCommandBuffer = object.CreateCommandBuffer(frame, _shader->GetPath(), stage);

					first = false;
				}

				if (secondaryCommandBuffer)
					pass->Draw(frame, object, handler, *secondaryCommandBuffer);
			}
		}

		if (secondaryCommandBuffer)
			vkCmdExecuteCommands(cmd, 1, &(const VkCommandBuffer&)(*secondaryCommandBuffer));
	}

	void SubShader::SetShader(Shader* shader) {
		_shader = shader;
		for (auto* pass : _passes)
			pass->SetShader(shader);
	}

	void SubShader::AddPass(Pass* pass) {
		pass->SetShader(_shader);
		_passes.emplace_back(pass);
	}
}
