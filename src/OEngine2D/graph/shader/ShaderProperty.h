#pragma once

#include "util.h"

namespace oengine2d {
	class ShaderProperty {
	public:
		enum Type {
			None = 0,
			I1,
			I2,
			I3,
			I4,
			F1,
			F2,
			F3,
			F4,
			MATRIX2,
			MATRIX3,
			MATRIX4,
			TEXTURE2D,
			TEXTURE3D,
			TEXTURECUBE,
		};

	public:
		ShaderProperty(Type type, const std::string& name) : _type(type), _name(name) {}
		virtual ~ShaderProperty() {}

		inline Type GetType() const { return _type; }

		virtual ShaderProperty* Clone() = 0;

	protected:
		Type _type;
		std::string _name;
	};

	template <typename T, ShaderProperty::Type type>
	class TShaderProperty : public ShaderProperty {
	public:
		TShaderProperty(const std::string& name, const T& a) : ShaderProperty(type, name), _a(a) {}
		virtual ~TShaderProperty() {}

		virtual ShaderProperty* Clone() { return new TShaderProperty(_name, _a); }

		inline const T& Get() const { return _a; }

	private:
		T _a;
	};

	using I1Property = TShaderProperty<int32_t, ShaderProperty::I1>;
	using I2Property = TShaderProperty<glm::ivec2, ShaderProperty::I2>;
	using I3Property = TShaderProperty<glm::ivec3, ShaderProperty::I3>;
	using I4Property = TShaderProperty<glm::ivec4, ShaderProperty::I4>;

	using F1Property = TShaderProperty<float, ShaderProperty::F1>;
	using F2Property = TShaderProperty<glm::vec2, ShaderProperty::F2>;
	using F3Property = TShaderProperty<glm::vec3, ShaderProperty::F3>;
	using F4Property = TShaderProperty<glm::vec4, ShaderProperty::F4>;

	using Matrix2Property = TShaderProperty<glm::mat2x2, ShaderProperty::MATRIX2>;
	using Matrix3Property = TShaderProperty<glm::mat2x2, ShaderProperty::MATRIX3>;
	using Matrix4Property = TShaderProperty<glm::mat2x2, ShaderProperty::MATRIX4>;

	class Texture2DProperty : ShaderProperty {
	public:

	private:
	};

	class ShaderPropertyMap : public std::map<std::string, ShaderProperty*> {
	public:
		ShaderPropertyMap() {}
		ShaderPropertyMap(const ShaderPropertyMap& other) {
			for (auto itr = other.begin(); itr != other.end(); ++itr)
				insert(std::make_pair(itr->first, itr->second->Clone()));
		}
		ShaderPropertyMap(ShaderPropertyMap&& other) noexcept {
			std::swap(*this, other);
		}
		~ShaderPropertyMap() {
			for (auto itr = begin(); itr != end(); ++itr) {
				delete itr->second;
			}
			clear();
		}

		ShaderPropertyMap& operator=(const ShaderPropertyMap& other) {
			for (auto itr = begin(); itr != end(); ++itr) {
				delete itr->second;
			}
			clear();

			for (auto itr = other.begin(); itr != other.end(); ++itr)
				insert(std::make_pair(itr->first, itr->second->Clone()));
			return *this;
		}

		ShaderPropertyMap& operator=(ShaderPropertyMap&& other) {
			std::swap(*this, other);
			return *this;
		}
	};
}
