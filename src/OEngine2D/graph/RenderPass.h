#pragma once

#include "util.h"

namespace oengine2d {
	struct Attachment {
		enum class Type { Image, Depth, Swapchain };

		Type type;
		VkFormat format;
		bool clear;
		bool multisampled;
	};

	struct Subpass {
		std::vector<uint32_t> attachmentBindings;
	};

	class RenderPass {
	public:
		RenderPass(std::vector<Attachment>&& attachments, std::vector<Subpass>&& subpasses, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT)
			: _samples(samples), _attachments(attachments), _subpasses(subpasses) {}
		~RenderPass();

		bool Create();

	private:
		VkRenderPass _renderPass = nullptr;

		VkSampleCountFlagBits _samples;
		std::vector<Attachment> _attachments;
		std::vector<Subpass> _subpasses;
	};
}