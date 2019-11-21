#pragma once

#include "util.h"
#include "ShaderProperty.h"
#include "../Resource.h"
#include "tinyxml/tinyxml.h"

namespace oengine2d {
	class SubShader;
	class Renderable;
	class CommandBuffer;
	class PropertyHandler;
	class VertexDescription;
	class Pass;
	class RenderState;
	class Shader : public Resource {
	public:
		Shader(const std::string& path) : Resource(ResourceType::RT_SHADER, path) {}
		~Shader() {}

		virtual bool Load(const char* data, const uint32_t size);

		void RecordCommand(uint32_t frame, Renderable& object, const PropertyHandler& handler, const std::string& stage, CommandBuffer& cmd);
		
		inline void SetVertexDescription(VertexDescription* desc) { _desc = desc; }
		inline VertexDescription* GetVertexDescription() const { return _desc; }

		void AddSubShader(SubShader* subShader);

		inline const ShaderPropertyMap& GetProperties() { return _properties; }

	private:
		bool ParseProperty(TiXmlElement* el);
		bool ParseSubshader(TiXmlElement* el);
		Pass * ParsePass(TiXmlElement* el);
		bool ParseRenderState(TiXmlElement* el, RenderState& state);

	private:
		ShaderPropertyMap _properties;

		VertexDescription* _desc = nullptr;
		std::vector<SubShader*> _subshaders;
	};
}
