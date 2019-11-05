#pragma once

#include "util.h"

namespace oengine2d {
	class VertexDescription {
	public:
		VertexDescription() {}
		VertexDescription(std::vector<VkVertexInputAttributeDescription>&& descriptions) : _attributeDescriptions(descriptions) {
			CalcVertexSize();
		}

		inline uint32_t GetVertexSize() const { return _vertexSize; }
		std::vector<VkVertexInputAttributeDescription>& GetAttributes() { return _attributeDescriptions; }

		inline void AddAttribute(const VkVertexInputAttributeDescription& desc) {
			_attributeDescriptions.emplace_back(desc);
			CalcVertexSize();
		}

	protected:
		void CalcVertexSize();

	private:
		std::vector<VkVertexInputAttributeDescription> _attributeDescriptions;
		uint32_t _vertexSize = 0;
	};
}
