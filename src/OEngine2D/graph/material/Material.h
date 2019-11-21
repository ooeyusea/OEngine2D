#pragma once
#include "util.h"
#include "../Resource.h"
#include "../shader/PropertyHandler.h"

namespace oengine2d {
	class CommandBuffer;
	class Shader;
	class Renderable;
	class Material : public Resource, public PropertyHandler {
	public:
		Material(const std::string& path, Shader* shader);
		~Material();

		virtual bool Load(const char* data, const uint32_t size);

		void RecordCommand(uint32_t frame, Renderable& object, const std::string& stage, CommandBuffer& cmd);

		void SetShader(Shader* shader);

	private:
		Shader* _shader = nullptr;
	};
}
