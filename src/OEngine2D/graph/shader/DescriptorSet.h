#pragma once

#include "util.h"
#include "WriteDescriptorSet.h"

namespace oengine2d {
	class Pipeline;
	class UniformBuffer;

	class DescriptorSet {
	public:
		DescriptorSet(const Pipeline& pipeline) : _pipeline(pipeline) {}
		~DescriptorSet();

		bool Create(const std::vector<UniformBuffer*>& ubs);

		inline const VkDescriptorSet& operator[](uint32_t frame) const { return _descriptorSets[frame]; }

	private:
		const Pipeline& _pipeline;
		std::vector<VkDescriptorSet> _descriptorSets;
	};
}
