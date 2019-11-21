#include "Renderable.h"
#include "../CommandBuffer.h"
#include "DescriptorSet.h"
#include "PropertyHandler.h"

namespace oengine2d {
	CommandBuffer* Renderable::GetCommandBuffer(uint32_t frame, const std::string& name, const std::string& stage) {
		return _commandBuffers[name][stage][frame];
	}

	CommandBuffer* Renderable::CreateCommandBuffer(uint32_t frame, const std::string& name, const std::string& stage) {
		auto& cmd = _commandBuffers[name][stage][frame];
		if (cmd != nullptr) {
			delete cmd;
		}
		else {
			cmd = new CommandBuffer();
			if (!cmd->Create(VK_COMMAND_BUFFER_LEVEL_SECONDARY)) {
				delete cmd;
				cmd = nullptr;
			}
		}
		
		return cmd;
	}

	DescriptorSet* Renderable::GetDescriptorSet(const std::string& name, const PropertyHandler& handler, const Pipeline& pipeline) {
		DescriptorSet* ds = _descriptorSets[name];
		if (ds == nullptr) {
			ds = new DescriptorSet(pipeline);

			std::vector<UniformBuffer*> ubs;
			auto* ub = handler.GetUniformBuffer();
			if (ub)
				ubs.emplace_back(ub);
			if (_uniformBuffer)
				ubs.emplace_back(_uniformBuffer);

			if (!ds->Create(ubs)) {
				delete ds;
				return nullptr;
			}

			_descriptorSets[name] = ds;
		}
		return ds;
	}
}
