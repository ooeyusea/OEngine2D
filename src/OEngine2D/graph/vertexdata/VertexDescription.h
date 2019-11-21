#pragma once

#include "util.h"

namespace oengine2d {
	class VertexDescription {
	public:
		VertexDescription() {}

		inline uint32_t GetVertexSize() const { return _vertexSize; }
		inline std::vector<VkVertexInputAttributeDescription>& GetAttributeDescription() { return _attributeDescriptions; }
		inline const VkVertexInputBindingDescription& GetBindingDescription() { return _bindingDescription; }

		inline VkPrimitiveTopology GetTopology() const { return _topology; }
		inline void SetTopology(VkPrimitiveTopology topology) { _topology = topology; }

		inline void AddAttribute(const VkVertexInputAttributeDescription& desc) {
			_attributeDescriptions.emplace_back(desc);
		}

		void Fix();

	protected:
		int32_t GetFormatSize(VkFormat format);

	private:
		VkPrimitiveTopology _topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;

		VkVertexInputBindingDescription _bindingDescription = {};
		std::vector<VkVertexInputAttributeDescription> _attributeDescriptions;
		uint32_t _vertexSize = 0;
	};
}
