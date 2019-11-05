#pragma once
#include "util.h"
#include "VertexDescription.h"
#include "tinyxml/tinyxml.h"
#include "shaderc/shaderc.hpp"

namespace oengine2d {
	class RenderStage;
	class Pipeline {
	public:
		Pipeline(const RenderStage& stage, uint32_t subpass) : _stage(stage), _subpass(subpass){}
		~Pipeline() { Release(); }

		bool Load(TiXmlElement * root);
		void Release();

		inline VkPipeline GetPipeline() const { return _graphicsPipeline; }

	private:
		VkShaderModule CreateShader(VkDevice device, const std::string& sourceName, shaderc_shader_kind kind, const char * entry, const std::string& source, const std::map<std::string, std::string>& defines);
		VkPrimitiveTopology ToTopology(const std::string& type);

	private:
		const RenderStage& _stage;
		uint32_t _subpass;

		VkPipelineLayout _pipelineLayout = nullptr;
		VkPipeline _graphicsPipeline = nullptr;

		VertexDescription * _vertexDescription = nullptr;
		VkPrimitiveTopology _topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		bool _depthClamp = false;
		bool _rasterizerDiscard = false;
		VkPolygonMode _polygonMode = VK_POLYGON_MODE_FILL;
		float _lineWidth = 1.0f;
		VkCullModeFlags _cullMode = VK_CULL_MODE_FRONT_BIT;
		VkFrontFace _frontFace = VK_FRONT_FACE_CLOCKWISE;
		bool _depthBias = false;
		float _depthBiasConstantFactor = 0.0f;
		float _depthBiasClamp = 0.0f;
		float _depthBiasSlopeFactor = 0.0f;
	};
}
