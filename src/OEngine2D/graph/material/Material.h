#pragma once
#include "util.h"
#include "RenderOption.h"
#include "../Resource.h"
#include "../vertexdata/VertexDescription.h"

namespace oengine2d {
	class Technique;
	class CommandBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class RenderStage;
	struct ITechniqueSelector {
		virtual ~ITechniqueSelector() {}

		virtual Technique* SelectTechnique(const std::list<Technique*>& techniques) = 0;
	};

	class Material : public Resource {
	public:
		Material(const std::string& path) : Resource(ResourceType::RT_MATERIAL, path), _currentTech(nullptr) {
			_option.SetVertexDescription(&_vertexDesc);
		}
		~Material();

		void AddTechnique(Technique* technique) { _techniques.push_back(technique); }

		void SetRenderState(const RenderState& state) { _option.SetRenderState(state); }
		void SetUniform(Uniform* u) { _option.SetUniform(u); }
		void MergeOption();

		void SelectTechnique(ITechniqueSelector* selector);
		void RecordCommand(const std::string& stage, CommandBuffer& cmd, const VertexBuffer& vb, IndexBuffer* ib = nullptr);

	private:
		VertexDescription _vertexDesc;
		RenderOption _option;
		std::list<Technique*> _techniques;
		Technique* _currentTech;
	};
}
