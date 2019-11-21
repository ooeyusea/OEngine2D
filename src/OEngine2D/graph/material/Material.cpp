#include "Material.h"
#include "../shader/Shader.h"

namespace oengine2d {
	Material::Material(const std::string& path, Shader* shader) : Resource(ResourceType::RT_MATERIAL, path), _shader(shader) {
		if (_shader) {
			_properties = _shader->GetProperties();
			_shader->AddRef();
		}
	}

	Material::~Material() {
		if (_shader)
			_shader->DecRef();
	}

	bool Material::Load(const char* data, const uint32_t size) {
		
		return true;
	}

	void Material::RecordCommand(uint32_t frame, Renderable& object, const std::string& stage, CommandBuffer& cmd) {
		_shader->RecordCommand(frame, object, *this, stage, cmd);
	}

	void Material::SetShader(Shader* shader) {
		if (shader == _shader)
			return;

		if (_shader)
			_shader->DecRef();

		_shader = shader;
		_shader->AddRef();
		_properties = _shader->GetProperties();
	}
}
