#include "PropertyHandler.h"

namespace oengine2d {
	bool PropertyHandler::Build() {
		for (auto itr = _properties.begin(); itr != _properties.end(); ++itr) {
			switch (itr->second->GetType()) {
			case ShaderProperty::I1: _uniformBuffer.AddUniform<int32_t>(itr->first); break;
			case ShaderProperty::I2: _uniformBuffer.AddUniform<glm::ivec2>(itr->first); break;
			case ShaderProperty::I3: _uniformBuffer.AddUniform<glm::ivec3>(itr->first); break;
			case ShaderProperty::I4: _uniformBuffer.AddUniform<glm::ivec4>(itr->first); break;
			case ShaderProperty::F1: _uniformBuffer.AddUniform<float>(itr->first); break;
			case ShaderProperty::F2: _uniformBuffer.AddUniform<glm::vec2>(itr->first); break;
			case ShaderProperty::F3: _uniformBuffer.AddUniform<glm::vec3>(itr->first); break;
			case ShaderProperty::F4: _uniformBuffer.AddUniform<glm::vec4>(itr->first); break;
			case ShaderProperty::MATRIX2: _uniformBuffer.AddUniform<glm::mat2x2>(itr->first); break;
			case ShaderProperty::MATRIX3: _uniformBuffer.AddUniform<glm::mat3x3>(itr->first); break;
			case ShaderProperty::MATRIX4: _uniformBuffer.AddUniform<glm::mat4x4>(itr->first); break;
			case ShaderProperty::TEXTURE2D: break;
			case ShaderProperty::TEXTURE3D: break;
			case ShaderProperty::TEXTURECUBE: break;
			}
		}

		CHECK(_uniformBuffer.Create(), "create uniform buffer failed");
		return true;
	}
}