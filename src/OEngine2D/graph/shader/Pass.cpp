#include "Pass.h"
#include "../Graph.h"
#include "Pipeline.h"
#include "../CommandBuffer.h"
#include "../CommandPool.h"
#include "../vertexdata/VertexBuffer.h"
#include "../vertexdata/IndexBuffer.h"
#include "DescriptorSet.h"
#include "Renderable.h"
#include "Shader.h"

namespace oengine2d {
	bool Pass::CreatePipeline() {
		auto [stage, subpass] = Graph::GetInstance().GetStage(_stage);
		if (stage) {
			_pipeline = new Pipeline(*stage, subpass, _renderState);
			_pipeline->AddShader(_vertexShader, VK_SHADER_STAGE_VERTEX_BIT);
			_pipeline->AddShader(_fragmentShader, VK_SHADER_STAGE_FRAGMENT_BIT);
			_pipeline->SetVertex(_shader->GetVertexDescription());
			if (_pipeline->Create())
				return true;
		}

		SAFE_DELETE(_pipeline);
		return false;
	}

	void Pass::DestroyPipeline() {
		SAFE_DELETE(_pipeline);
	}

	void Pass::Draw(uint32_t frame, Renderable& object, const PropertyHandler& handler, CommandBuffer& cmd) {
		vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, *_pipeline);
		DescriptorSet * descriptorSet = object.GetDescriptorSet(_name, handler, *_pipeline);
		vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipelineLayout(), 0, 1, &(*descriptorSet)[frame], 0, nullptr);

		auto * vb = object.GetVertexBuffer();
		VkBuffer vertexBuffers[] = { *vb };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);

		IndexBuffer* ib = object.GetIndexBuffer();
		if (ib != nullptr) {
			vkCmdBindIndexBuffer(cmd, *ib, 0, ib->GetIndexType());
			vkCmdDrawIndexed(cmd, ib->GetCount(), 1, 0, 0, 0);
		}
		else
			vkCmdDraw(cmd, vb->GetCount(), 1, 0, 0);
	}
}
