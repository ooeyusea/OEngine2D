#pragma once
#include "util.h"
#include "VertexDescription.h"
#include "tinyxml/tinyxml.h"
#include "SPIRV/GlslangToSpv.h"
#include "glslang/Public/ShaderLang.h"

namespace oengine2d {
	class RenderStage;

	class Uniform {
		friend class Pipeline;
	public:
		explicit Uniform(int32_t binding = -1, int32_t offset = -1, int32_t size = -1, int32_t glType = -1, bool readOnly = false,
			bool writeOnly = false, const VkShaderStageFlags & stageFlags = 0) :
			_binding(binding),
			_offset(offset),
			_size(size),
			_glType(glType),
			_readOnly(readOnly),
			_writeOnly(writeOnly),
			_stageFlags(stageFlags) {
		}

		int32_t GetBinding() const { return _binding; }
		int32_t GetOffset() const { return _offset; }
		int32_t GetSize() const { return _size; }
		int32_t GetGlType() const { return _glType; }
		bool IsReadOnly() const { return _readOnly; }
		bool IsWriteOnly() const { return _writeOnly; }
		const VkShaderStageFlags& GetStageFlags() const { return _stageFlags; }

		bool operator==(const Uniform& other) const {
			return _binding == other._binding && _offset == other._offset && _size == other._size && _glType == other._glType && _readOnly == other._readOnly &&
				_writeOnly == other._writeOnly && _stageFlags == other._stageFlags;
		}

		bool operator!=(const Uniform& other) const {
			return !operator==(other);
		}

	private:
		int32_t _binding;
		int32_t _offset;
		int32_t _size;
		int32_t _glType;
		bool _readOnly;
		bool _writeOnly;
		VkShaderStageFlags _stageFlags;
	};

	class UniformBlock {
		friend class Pipeline;
	public:
		enum class Type { None, Uniform, Storage, Push };

		explicit UniformBlock(int32_t binding = -1, int32_t size = -1, VkShaderStageFlags stageFlags = 0, Type type = Type::Uniform) :
			_binding(binding),
			_size(size),
			_stageFlags(stageFlags),
			_type(type) {
		}

		int32_t GetBinding() const { return _binding; }
		int32_t GetSize() const { return _size; }
		VkShaderStageFlags GetStageFlags() const { return _stageFlags; }
		Type GetType() const { return _type; }
		const std::map<std::string, Uniform>& GetUniforms() const { return _uniforms; }

		std::optional<Uniform> GetUniform(const std::string& name) const {
			auto it = _uniforms.find(name);

			if (it == _uniforms.end()) {
				return std::nullopt;
			}

			return it->second;
		}

		bool operator==(const UniformBlock& other) const {
			return _binding == other._binding && _size == other._size && _stageFlags == other._stageFlags && _type == other._type && _uniforms == other._uniforms;
		}

		bool operator!=(const UniformBlock& other) const {
			return !operator==(other);
		}


	private:
		int32_t _binding;
		int32_t _size;
		VkShaderStageFlags _stageFlags;
		Type _type;
		std::map<std::string, Uniform> _uniforms;
	};

	class Pipeline {
	public:
		Pipeline(const RenderStage& stage, uint32_t subpass) : _stage(stage), _subpass(subpass){}
		~Pipeline() { Release(); }

		bool Load(TiXmlElement * root);
		void Release();

		inline VkPipeline GetPipeline() const { return _graphicsPipeline; }

	private:
		VkShaderModule CreateShader(VkDevice device, const std::string& sourceName, VkShaderStageFlags flag, const char * entry, const std::string& source, const std::string& defines);
		VkPrimitiveTopology ToTopology(const std::string& type);

		bool LoadShaders(TiXmlElement* root, std::vector<VkPipelineShaderStageCreateInfo>& shaderStages);

		void LoadUniformBlock(glslang::TProgram& program, VkShaderStageFlags flag);
		void LoadUniform(glslang::TProgram& program, VkShaderStageFlags flag);
		void LoadAttribute(glslang::TProgram& program);

		VkFormat GlTypeToVk(int32_t glType);
		int32_t ComputeSize(const glslang::TType* ttype);

	private:
		const RenderStage& _stage;
		uint32_t _subpass;

		VkDescriptorSetLayout _descriptorSetLayout = nullptr;
		VkPipelineLayout _pipelineLayout = nullptr;
		VkPipeline _graphicsPipeline = nullptr;

		VertexDescription _vertexDescription;
		std::map<std::string, Uniform> _uniforms;
		std::map<std::string, UniformBlock> _uniformBlocks;
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
