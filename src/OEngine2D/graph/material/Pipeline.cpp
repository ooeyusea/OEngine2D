#include "Pipeline.h"
#include "../Graph.h"
#include "../LogicalDevice.h"
#include "../RenderStage.h"

namespace oengine2d {
	const std::vector<VkDynamicState> DYNAMIC_STATES = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH };

	bool Pipeline::Load(TiXmlElement* root) {
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for (auto* elem = root->FirstChildElement("shader"); elem; elem = elem->NextSiblingElement("shader")) {
			if (strcmp(elem->Attribute("type"), "vert") == 0) {
				VkShaderModule shaderModule = CreateShader(Graph::GetInstance().GetLogicalDevice(), elem->Attribute("name"), shaderc_glsl_vertex_shader, "main", elem->FirstChild()->Value(), {});
				if (!shaderModule) {
					return false;
				}

				shaderStages.push_back({});
				shaderStages.back().sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderStages.back().stage = VK_SHADER_STAGE_VERTEX_BIT;
				shaderStages.back().module = shaderModule;
				shaderStages.back().pName = "main";
			}
			else if (strcmp(elem->Attribute("type"), "frag") == 0) {
				VkShaderModule shaderModule = CreateShader(Graph::GetInstance().GetLogicalDevice(), elem->Attribute("name"), shaderc_glsl_fragment_shader, "main", elem->FirstChild()->Value(), {});
				if (!shaderModule) {
					return false;
				}

				shaderStages.push_back({});
				shaderStages.back().sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderStages.back().stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				shaderStages.back().module = shaderModule;
				shaderStages.back().pName = "main";
			}
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		if (_vertexDescription) {
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = _vertexDescription->GetVertexSize();
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			auto& attributeDescriptions = _vertexDescription->GetAttributes();

			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
			vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
			vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		}
		else {
			vertexInputInfo.vertexBindingDescriptionCount = 0;
			vertexInputInfo.pVertexBindingDescriptions = nullptr;
			vertexInputInfo.vertexAttributeDescriptionCount = 0;
			vertexInputInfo.pVertexAttributeDescriptions = nullptr;
		}

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = _topology;
		inputAssembly.primitiveRestartEnable = VK_FALSE;


		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = nullptr;
		viewportState.scissorCount = 1;
		viewportState.pScissors = nullptr;

		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = _depthClamp ? VK_TRUE : VK_FALSE;
		rasterizer.rasterizerDiscardEnable = _rasterizerDiscard ? VK_TRUE : VK_FALSE;
		rasterizer.polygonMode = _polygonMode;
		rasterizer.lineWidth = _lineWidth;
		rasterizer.cullMode = _cullMode;
		rasterizer.frontFace = _frontFace;
		rasterizer.depthBiasEnable = _rasterizerDiscard ? VK_TRUE : VK_FALSE;
		rasterizer.depthBiasConstantFactor = _depthBiasConstantFactor;
		rasterizer.depthBiasClamp = _depthBiasClamp;
		rasterizer.depthBiasSlopeFactor = _depthBiasSlopeFactor;

		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = 0; // Optional

		CHECK(vkCreatePipelineLayout(Graph::GetInstance().GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) == VK_SUCCESS, "create pipe line layout failed");

		VkPipelineDynamicStateCreateInfo dynamicState;
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(DYNAMIC_STATES.size());
		dynamicState.pDynamicStates = DYNAMIC_STATES.data();

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = (uint32_t)shaderStages.size();
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState; // Optional
		pipelineInfo.layout = _pipelineLayout;
		pipelineInfo.renderPass = _stage;
		pipelineInfo.subpass = _subpass;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		CHECK(vkCreateGraphicsPipelines(Graph::GetInstance().GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline) == VK_SUCCESS, "create pipe line failed");
		return true;
	}

	void Pipeline::Release() {
		if (_graphicsPipeline)
			vkDestroyPipeline(Graph::GetInstance().GetLogicalDevice(), _graphicsPipeline, nullptr);

		if (_pipelineLayout)
			vkDestroyPipelineLayout(Graph::GetInstance().GetLogicalDevice(), _pipelineLayout, nullptr);
	}

	VkShaderModule Pipeline::CreateShader(VkDevice device, const std::string& sourceName, shaderc_shader_kind kind, const char* entry, const std::string& source, const std::map<std::string, std::string>& defines) {
		/*shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		for (auto& p : defines)
			options.AddMacroDefinition(p.first, p.second);
#ifndef _DEBUG
		options.SetOptimizationLevel(shaderc_optimization_level_performance);
#endif

		shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, kind, sourceName.c_str(), entry, options);
		if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
			std::cerr << module.GetErrorMessage() << std::endl;
			return nullptr;
		}

		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = (module.end() - module.begin()) * sizeof(uint32_t);
		createInfo.pCode = module.begin();

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			return nullptr;
		}
		return shaderModule;*/
		return nullptr;
	}

	VkPrimitiveTopology Pipeline::ToTopology(const std::string& type) {
		if (type == "point")
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		else if (type == "line")
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		else if (type == "line_strip")
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		else if (type == "triangle")
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		else if (type == "triangle_strip")
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		else if (type == "triangle_fan")
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
	}
}
