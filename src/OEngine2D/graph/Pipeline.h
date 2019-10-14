#pragma once
#include "Resource.h"
#include "shaderc/shaderc.hpp"

namespace oengine2d {
	class Pipeline : public Resource {
	public:
		Pipeline() : Resource(ResourceType::RT_PIPELINE) {}
		~Pipeline() {}

		virtual bool Load(VkDevice device);
		virtual void Release(VkDevice device);

	private:
		VkShaderModule CreateShader(VkDevice device, const std::string& sourceName, shaderc_shader_kind kind, const char * entry, const std::string& source, const std::map<std::string, std::string>& defines);

	private:
		VkPipelineLayout _pipelineLayout = nullptr;
		VkRenderPass _renderPass = nullptr;
		VkPipeline _graphicsPipeline = nullptr;
		std::vector<VkFramebuffer> _swapChainFramebuffers;
	};
}