#include "Material.h"
#include "../shader/Shader.h"
#include "../../system/FileSystem.h"
#include "../resource/ResourceMgr.h"

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

	bool Material::Load() {
		std::string data;
		if (!FileSystem::GetInstance().ReadFile(_path, data)) {
			return false;
		}

		TiXmlDocument doc;
		doc.Parse(data.c_str());
		if (doc.Error()) {
			return false;
		}

		const char* shaderPath = doc.FirstChildElement()->Attribute("shader");
		if (!shaderPath) {
			return false;
		}

		Shader* shader = ResourceMgr::GetInstance().GetShader(shaderPath, true);
		if (!shader) {
			return false;
		}

		_shader = shader;
		_properties = _shader->GetProperties();
		_shader->AddRef();

		for (TiXmlElement* el = doc.FirstChildElement()->FirstChildElement("property"); el != nullptr; el = el->NextSiblingElement("property")) {
			const char* propName = el->Attribute("name");
			auto itr = _properties.find(propName);
			if (itr != _properties.end()) {
				if (!ShaderPropertyLoader::GetInstance().Load(el, itr->second)) {
					return false;
				}
			}
		}

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
