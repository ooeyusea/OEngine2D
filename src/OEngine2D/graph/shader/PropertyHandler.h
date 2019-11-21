#pragma once

#include "util.h"
#include "UniformBuffer.h"
#include "ShaderProperty.h"

namespace oengine2d {
	class PropertyHandler {
	public:
		PropertyHandler() {}
		virtual ~PropertyHandler() {}

		bool Build();
		inline UniformBuffer* GetUniformBuffer() const { _uniformBuffer.IsCreated() ? &_uniformBuffer : nullptr; }

		inline void SetProperties(const ShaderPropertyMap& properties) { _properties = properties; }

	protected:
		UniformBuffer _uniformBuffer;
		ShaderPropertyMap _properties;
	};
}
