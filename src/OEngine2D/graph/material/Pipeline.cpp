#include "Pipeline.h"
#include "../Graph.h"
#include "../LogicalDevice.h"
#include "../RenderStage.h"
#include "../../system/FileSystem.h"
#include "../../helper/StringHelper.h"
#include <filesystem>
namespace fs = std::experimental::filesystem;

namespace oengine2d {
	const std::vector<VkDynamicState> DYNAMIC_STATES = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH };
	class ShaderIncluder : public glslang::TShader::Includer {
	public:
		IncludeResult* includeLocal(const char* headerName, const char* includerName, size_t inclusionDepth) override {
			auto directory = std::filesystem::path(includerName).parent_path();
			std::string * buffer = new std::string;
			if (!FileSystem::GetInstance().ReadFile((directory / headerName).string(), *buffer)) {
				//Log::Error("Shader Include could not be loaded: ", std::quoted(headerName), '\n');
				return nullptr;
			}

			return new IncludeResult(headerName, buffer->c_str(), buffer->size(), buffer);
		}

		IncludeResult* includeSystem(const char* headerName, const char* includerName, size_t inclusionDepth) override {
			std::string* buffer = new std::string;
			if (!FileSystem::GetInstance().ReadFile(headerName, *buffer)) {
				//Log::Error("Shader Include could not be loaded: ", std::quoted(headerName), '\n');
				return nullptr;
			}

			return new IncludeResult(headerName, buffer->c_str(), buffer->size(), buffer);
		}

		void releaseInclude(IncludeResult* result) override {
			if (result) {
				delete static_cast<std::string*>(result->userData);
				delete result;
			}
		}
	};

	EShLanguage GetEshLanguage(const VkShaderStageFlags& stageFlag) {
		switch (stageFlag) {
		case VK_SHADER_STAGE_COMPUTE_BIT:
			return EShLangCompute;
		case VK_SHADER_STAGE_VERTEX_BIT:
			return EShLangVertex;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			return EShLangTessControl;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			return EShLangTessEvaluation;
		case VK_SHADER_STAGE_GEOMETRY_BIT:
			return EShLangGeometry;
		case VK_SHADER_STAGE_FRAGMENT_BIT:
			return EShLangFragment;
		default:
			return EShLangCount;
		}
	}


	TBuiltInResource GetResources() {
		TBuiltInResource resources = {};
		resources.maxLights = 32;
		resources.maxClipPlanes = 6;
		resources.maxTextureUnits = 32;
		resources.maxTextureCoords = 32;
		resources.maxVertexAttribs = 64;
		resources.maxVertexUniformComponents = 4096;
		resources.maxVaryingFloats = 64;
		resources.maxVertexTextureImageUnits = 32;
		resources.maxCombinedTextureImageUnits = 80;
		resources.maxTextureImageUnits = 32;
		resources.maxFragmentUniformComponents = 4096;
		resources.maxDrawBuffers = 32;
		resources.maxVertexUniformVectors = 128;
		resources.maxVaryingVectors = 8;
		resources.maxFragmentUniformVectors = 16;
		resources.maxVertexOutputVectors = 16;
		resources.maxFragmentInputVectors = 15;
		resources.minProgramTexelOffset = -8;
		resources.maxProgramTexelOffset = 7;
		resources.maxClipDistances = 8;
		resources.maxComputeWorkGroupCountX = 65535;
		resources.maxComputeWorkGroupCountY = 65535;
		resources.maxComputeWorkGroupCountZ = 65535;
		resources.maxComputeWorkGroupSizeX = 1024;
		resources.maxComputeWorkGroupSizeY = 1024;
		resources.maxComputeWorkGroupSizeZ = 64;
		resources.maxComputeUniformComponents = 1024;
		resources.maxComputeTextureImageUnits = 16;
		resources.maxComputeImageUniforms = 8;
		resources.maxComputeAtomicCounters = 8;
		resources.maxComputeAtomicCounterBuffers = 1;
		resources.maxVaryingComponents = 60;
		resources.maxVertexOutputComponents = 64;
		resources.maxGeometryInputComponents = 64;
		resources.maxGeometryOutputComponents = 128;
		resources.maxFragmentInputComponents = 128;
		resources.maxImageUnits = 8;
		resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		resources.maxCombinedShaderOutputResources = 8;
		resources.maxImageSamples = 0;
		resources.maxVertexImageUniforms = 0;
		resources.maxTessControlImageUniforms = 0;
		resources.maxTessEvaluationImageUniforms = 0;
		resources.maxGeometryImageUniforms = 0;
		resources.maxFragmentImageUniforms = 8;
		resources.maxCombinedImageUniforms = 8;
		resources.maxGeometryTextureImageUnits = 16;
		resources.maxGeometryOutputVertices = 256;
		resources.maxGeometryTotalOutputComponents = 1024;
		resources.maxGeometryUniformComponents = 1024;
		resources.maxGeometryVaryingComponents = 64;
		resources.maxTessControlInputComponents = 128;
		resources.maxTessControlOutputComponents = 128;
		resources.maxTessControlTextureImageUnits = 16;
		resources.maxTessControlUniformComponents = 1024;
		resources.maxTessControlTotalOutputComponents = 4096;
		resources.maxTessEvaluationInputComponents = 128;
		resources.maxTessEvaluationOutputComponents = 128;
		resources.maxTessEvaluationTextureImageUnits = 16;
		resources.maxTessEvaluationUniformComponents = 1024;
		resources.maxTessPatchComponents = 120;
		resources.maxPatchVertices = 32;
		resources.maxTessGenLevel = 64;
		resources.maxViewports = 16;
		resources.maxVertexAtomicCounters = 0;
		resources.maxTessControlAtomicCounters = 0;
		resources.maxTessEvaluationAtomicCounters = 0;
		resources.maxGeometryAtomicCounters = 0;
		resources.maxFragmentAtomicCounters = 8;
		resources.maxCombinedAtomicCounters = 8;
		resources.maxAtomicCounterBindings = 1;
		resources.maxVertexAtomicCounterBuffers = 0;
		resources.maxTessControlAtomicCounterBuffers = 0;
		resources.maxTessEvaluationAtomicCounterBuffers = 0;
		resources.maxGeometryAtomicCounterBuffers = 0;
		resources.maxFragmentAtomicCounterBuffers = 1;
		resources.maxCombinedAtomicCounterBuffers = 1;
		resources.maxAtomicCounterBufferSize = 16384;
		resources.maxTransformFeedbackBuffers = 4;
		resources.maxTransformFeedbackInterleavedComponents = 64;
		resources.maxCullDistances = 8;
		resources.maxCombinedClipAndCullDistances = 8;
		resources.maxSamples = 4;
		resources.limits.nonInductiveForLoops = true;
		resources.limits.whileLoops = true;
		resources.limits.doWhileLoops = true;
		resources.limits.generalUniformIndexing = true;
		resources.limits.generalAttributeMatrixVectorIndexing = true;
		resources.limits.generalVaryingIndexing = true;
		resources.limits.generalSamplerIndexing = true;
		resources.limits.generalVariableIndexing = true;
		resources.limits.generalConstantMatrixVectorIndexing = true;
		return resources;
	}

	bool Pipeline::Create() {
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &_desc->GetBindingDescription();
		vertexInputInfo.vertexAttributeDescriptionCount = _desc->GetAttributeDescription().size();
		vertexInputInfo.pVertexAttributeDescriptions = _desc->GetAttributeDescription().data();

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

		std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayouts;
		for (const auto& [uniformBlockName, uniformBlock] : _uniformBlocks) {
			VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
			descriptorSetLayoutBinding.binding = static_cast<uint32_t>(uniformBlock._binding);
			descriptorSetLayoutBinding.descriptorCount = 1;
			descriptorSetLayoutBinding.stageFlags = uniformBlock._stageFlags;
			descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

			switch (uniformBlock._type) {
			case UniformBlock::Type::Uniform:
				descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descriptorSetLayouts.emplace_back(descriptorSetLayoutBinding);
				break;
			case UniformBlock::Type::Storage:
				descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				descriptorSetLayouts.emplace_back(descriptorSetLayoutBinding);
				break;
			case UniformBlock::Type::Push:
				// Push constants are described in the pipeline.
				break;
			default:
				break;
			}
		}

		for (const auto& [uniformName, uniform] : _uniforms) {
			VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
			descriptorSetLayoutBinding.binding = uniform._binding;
			descriptorSetLayoutBinding.descriptorCount = 1;
			descriptorSetLayoutBinding.stageFlags = uniform._stageFlags;
			descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

			switch (uniform._glType) {
			case 0x8B5E: // GL_SAMPLER_2D
			case 0x904D: // GL_IMAGE_2D
			case 0x9108: // GL_SAMPLER_2D_MULTISAMPLE
			case 0x9055: // GL_IMAGE_2D_MULTISAMPLE
				descriptorSetLayoutBinding.descriptorType = uniform._writeOnly ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;;
				descriptorSetLayouts.emplace_back(descriptorSetLayoutBinding);
				break;
			case 0x8B60: // GL_SAMPLER_CUBE
			case 0x9050: // GL_IMAGE_CUBE
				descriptorSetLayoutBinding.descriptorType = uniform._writeOnly ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;;
				descriptorSetLayouts.emplace_back(descriptorSetLayoutBinding);
				break;
			default:
				break;
			}
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayouts.size());;
		layoutInfo.pBindings = descriptorSetLayouts.data();

		CHECK(vkCreateDescriptorSetLayout(Graph::GetInstance().GetLogicalDevice(), &layoutInfo, nullptr, &_descriptorSetLayout) == VK_SUCCESS, "create descriptor set layout failed");

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = 0; // Optional
		pipelineLayoutInfo.setLayoutCount = 1; // Optional
		pipelineLayoutInfo.pSetLayouts = &_descriptorSetLayout; // Optional

		CHECK(vkCreatePipelineLayout(Graph::GetInstance().GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) == VK_SUCCESS, "create pipe line layout failed");

		VkPipelineDynamicStateCreateInfo dynamicState;
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(DYNAMIC_STATES.size());
		dynamicState.pDynamicStates = DYNAMIC_STATES.data();

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = (uint32_t)_shaderStages.size();
		pipelineInfo.pStages = _shaderStages.data();
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

		if (_descriptorSetLayout)
			vkDestroyDescriptorSetLayout(Graph::GetInstance().GetLogicalDevice(), _descriptorSetLayout, nullptr);
	}

	void Pipeline::AddShader(std::string source, VkShaderStageFlags flag, const std::string& defines) {
		LoadShaders(source, flag, defines);
	}

	VkShaderModule Pipeline::CreateShader(VkDevice device, const std::string& sourceName, VkShaderStageFlags flag, const char* entry, const std::string& source, const std::string& defines) {
		auto language = GetEshLanguage(flag);
		glslang::TProgram program;
		glslang::TShader shader(language);

		auto messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);
#if defined(ACID_DEBUG)
		messages = static_cast<EShMessages>(messages | EShMsgDebugInfo);
#endif
		auto shaderNameCstr = sourceName.c_str();
		auto shaderSource = source.c_str();
		shader.setStringsWithLengthsAndNames(&shaderSource, nullptr, &shaderNameCstr, 1);
		shader.setPreamble(defines.c_str());

		shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 110);
		shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_1);
		shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_3);

		ShaderIncluder includer;
		auto resources = GetResources();

		std::string str;
		CHECK(shader.preprocess(&resources, glslang::EShTargetVulkan_1_1, ENoProfile, false, false, messages, &str, includer), "SPRIV shader preprocess failed!\n" << shader.getInfoLog() << "\n" << shader.getInfoDebugLog());
		CHECK(shader.parse(&resources, glslang::EShTargetVulkan_1_1, true, messages, includer), "SPRIV shader parse failed!\n" << shader.getInfoLog() << "\n" << shader.getInfoDebugLog());

		program.addShader(&shader);
		CHECK((program.link(messages) && !program.mapIO()), "Error while linking shader program.");

		program.buildReflection();
		for (uint32_t dim = 0; dim < 3; ++dim) {
			auto localSize = program.getLocalSize(dim);

			//if (localSize > 1) {
			//	_localSizes[dim] = localSize;
			//}
		}

		LoadUniformBlock(program, flag);
		LoadUniform(program, flag);

		if (flag == VK_SHADER_STAGE_VERTEX_BIT) {
			LoadAttribute(program);
		}

		glslang::SpvOptions spvOptions;
#if _DEBUG
		spvOptions.generateDebugInfo = true;
		spvOptions.disableOptimizer = true;
		spvOptions.optimizeSize = false;
#else
		spvOptions.generateDebugInfo = false;
		spvOptions.disableOptimizer = false;
		spvOptions.optimizeSize = true;
#endif

		spv::SpvBuildLogger logger;
		std::vector<uint32_t> spirv;
		GlslangToSpv(*program.getIntermediate(static_cast<EShLanguage>(language)), spirv, &logger, &spvOptions);

		VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.codeSize = spirv.size() * sizeof(uint32_t);
		shaderModuleCreateInfo.pCode = spirv.data();

		VkShaderModule shaderModule;
		CHECK(vkCreateShaderModule(Graph::GetInstance().GetLogicalDevice(), &shaderModuleCreateInfo, nullptr, &shaderModule) == VK_SUCCESS, "create shader module failed");
		return shaderModule;
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

	bool Pipeline::LoadShaders(const std::string& source, VkShaderStageFlags flag, const std::string& defines) {
		const char* entry = "";
		switch (flag) {
		case VK_SHADER_STAGE_VERTEX_BIT: entry = "vert"; break;
		case VK_SHADER_STAGE_FRAGMENT_BIT: entry = "frag"; break;
		}

		VkShaderModule shaderModule = CreateShader(Graph::GetInstance().GetLogicalDevice(), "SHADER", flag, entry, source.c_str(), defines);
		if (!shaderModule) {
			return false;
		}

		_shaderStages.push_back({});
		_shaderStages.back().sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		_shaderStages.back().stage = (VkShaderStageFlagBits)flag;
		_shaderStages.back().module = shaderModule;
		_shaderStages.back().pName = entry;

		return true;
	}

	void Pipeline::LoadUniformBlock(glslang::TProgram& program, VkShaderStageFlags stageFlag) {
		for (int32_t i = program.getNumLiveUniformBlocks() - 1; i >= 0; i--) {
			auto reflection = program.getUniformBlock(i);

			for (auto& [uniformBlockName, uniformBlock] : _uniformBlocks) {
				if (uniformBlockName == reflection.name) {
					uniformBlock._stageFlags |= stageFlag;
					return;
				}
			}

			auto type = UniformBlock::Type::None;

			if (reflection.getType()->getQualifier().storage == glslang::EvqUniform) {
				type = UniformBlock::Type::Uniform;
			}

			if (reflection.getType()->getQualifier().storage == glslang::EvqBuffer) {
				type = UniformBlock::Type::Storage;
			}

			if (reflection.getType()->getQualifier().layoutPushConstant) {
				type = UniformBlock::Type::Push;
			}

			_uniformBlocks.emplace(reflection.name, UniformBlock(reflection.getBinding(), reflection.size, stageFlag, type));
		}
	}

	void Pipeline::LoadUniform(glslang::TProgram& program, VkShaderStageFlags stageFlag) {
		for (int32_t i = 0; i < program.getNumLiveUniformVariables(); i++) {
			auto reflection = program.getUniform(i);

			if (reflection.getBinding() == -1) {
				auto splitName = StringHelper::Explode(".", reflection.name);

				if (splitName.size() > 1) {
					for (auto& [uniformBlockName, uniformBlock] : _uniformBlocks) {
						if (uniformBlockName == splitName.at(0)) {

							uniformBlock._uniforms.emplace(StringHelper::Implode(".", splitName, 1),
								Uniform(reflection.getBinding(), reflection.offset, ComputeSize(reflection.getType()), reflection.glDefineType, false, false, stageFlag));
							return;
						}
					}
				}
			}

			for (auto& [uniformName, uniform] : _uniforms) {
				if (uniformName == reflection.name) {
					uniform._stageFlags |= stageFlag;
					return;
				}
			}

			auto& qualifier = reflection.getType()->getQualifier();
			_uniforms.emplace(reflection.name, Uniform(reflection.getBinding(), reflection.offset, -1, reflection.glDefineType, qualifier.readonly, qualifier.writeonly, stageFlag));
		}
	}

	void Pipeline::LoadAttribute(glslang::TProgram& program) {
	}

	VkFormat Pipeline::GlTypeToVk(int32_t type) {
		switch (type) {
		case 0x1406: // GL_FLOAT
			return VK_FORMAT_R32_SFLOAT;
		case 0x8B50: // GL_FLOAT_VEC2
			return VK_FORMAT_R32G32_SFLOAT;
		case 0x8B51: // GL_FLOAT_VEC3
			return VK_FORMAT_R32G32B32_SFLOAT;
		case 0x8B52: // GL_FLOAT_VEC4
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case 0x1404: // GL_INT
			return VK_FORMAT_R32_SINT;
		case 0x8B53: // GL_INT_VEC2
			return VK_FORMAT_R32G32_SINT;
		case 0x8B54: // GL_INT_VEC3
			return VK_FORMAT_R32G32B32_SINT;
		case 0x8B55: // GL_INT_VEC4
			return VK_FORMAT_R32G32B32A32_SINT;
		case 0x1405: // GL_UNSIGNED_INT
			return VK_FORMAT_R32_SINT;
		case 0x8DC6: // GL_UNSIGNED_INT_VEC2
			return VK_FORMAT_R32G32_SINT;
		case 0x8DC7: // GL_UNSIGNED_INT_VEC3
			return VK_FORMAT_R32G32B32_SINT;
		case 0x8DC8: // GL_UNSIGNED_INT_VEC4
			return VK_FORMAT_R32G32B32A32_SINT;
		default:
			return VK_FORMAT_UNDEFINED;
		}
	}

	int32_t Pipeline::ComputeSize(const glslang::TType* ttype) {
		// TODO: glslang::TType::computeNumComponents is available but has many issues resolved in this method.
		int32_t components = 0;

		if (ttype->getBasicType() == glslang::EbtStruct || ttype->getBasicType() == glslang::EbtBlock) {
			for (const auto& tl : *ttype->getStruct()) {
				components += ComputeSize(tl.type);
			}
		}
		else if (ttype->getMatrixCols() != 0) {
			components = ttype->getMatrixCols() * ttype->getMatrixRows();
		}
		else {
			components = ttype->getVectorSize();
		}

		if (ttype->getArraySizes()) {
			int32_t arraySize = 1;

			for (int32_t d = 0; d < ttype->getArraySizes()->getNumDims(); ++d) {
				auto dimSize = ttype->getArraySizes()->getDimSize(d);

				// This only makes sense in paths that have a known array size.
				if (dimSize != glslang::UnsizedArraySize) {
					arraySize *= dimSize;
				}
			}

			components *= arraySize;
		}

		return sizeof(float) * components;
	}
}
