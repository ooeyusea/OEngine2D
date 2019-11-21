#include "DescriptorSet.h"
#include "../Graph.h"
#include "../SwapChain.h"
#include "Pipeline.h"
#include "../LogicalDevice.h"
#include "UniformBuffer.h"

namespace oengine2d {
	DescriptorSet::~DescriptorSet() {
		if (!_descriptorSets.empty()) {
			vkFreeDescriptorSets(Graph::GetInstance().GetLogicalDevice(), _pipeline.GetDescriptorPool(), static_cast<uint32_t>(_descriptorSets.size()), _descriptorSets.data());
			_descriptorSets.clear();
		}
	}

	bool DescriptorSet::Create(const std::vector<UniformBuffer*>& ubs) {
		std::vector<VkDescriptorSetLayout> layouts(Graph::GetInstance().GetSwapChain().GetImageCount(), _pipeline.GetDescriptorSetLayout());

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _pipeline.GetDescriptorPool();
		allocInfo.descriptorSetCount = static_cast<uint32_t>(layouts.size());
		allocInfo.pSetLayouts = layouts.data();

		_descriptorSets.resize(layouts.size());
		if (vkAllocateDescriptorSets(Graph::GetInstance().GetLogicalDevice(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS) {
			_descriptorSets.clear();
			return false;
		}

		std::vector<VkDescriptorImageInfo> images;

		for (size_t i = 0; i < layouts.size(); i++) {
			std::vector<WriteDescriptorSet> writes;
			for (auto* ub : ubs) {
				ub->GetDescriptorBuffInfo(static_cast<uint32_t>(i), _pipeline, writes);
			}

			std::vector<VkWriteDescriptorSet> descriptorWrites;
			for (auto& w : writes) {
				w.write.pBufferInfo = &w.bufferInfo;
				w.write.dstSet = _descriptorSets[i];

				descriptorWrites.emplace_back(w.write);
			}

			vkUpdateDescriptorSets(Graph::GetInstance().GetLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
		return true;
	}
}