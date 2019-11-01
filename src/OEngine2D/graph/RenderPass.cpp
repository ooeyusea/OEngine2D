#include "RenderPass.h"
#include "Graph.h"
#include "LogicalDevice.h"
#include "SwapChain.h"

namespace oengine2d {
	RenderPass::~RenderPass() {
		if (_renderPass)
			vkDestroyRenderPass(Graph::GetInstance().GetLogicalDevice(), _renderPass, nullptr);
	}

	bool RenderPass::Create() {
		std::vector<VkAttachmentDescription> attachmentDescriptions;
		for (const auto& attachment : _attachments) {
			VkAttachmentDescription attachmentDescription = {};
			attachmentDescription.samples = attachment.multisampled ? _samples : VK_SAMPLE_COUNT_1_BIT;
			attachmentDescription.loadOp = attachment.clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
			attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // We don't care about initial layout of the attachment.

			switch (attachment.type) {
			case Attachment::Type::Image:
				attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				attachmentDescription.format = attachment.format;
				break;
			case Attachment::Type::Depth:
				attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				attachmentDescription.format = Graph::GetInstance().GetSwapChain().GetDepthFormat();
				break;
			case Attachment::Type::Swapchain:
				attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachmentDescription.format = Graph::GetInstance().GetSwapChain().GetColorFormat();
				break;
			}

			attachmentDescriptions.emplace_back(attachmentDescription);
		}

		std::vector<VkSubpassDependency> dependencies;
		std::vector<VkSubpassDescription> subpassDescriptions;
		uint32_t index = 0;
		for (const auto& subpassType : _subpasses) {
			// Attachments.
			std::vector<VkAttachmentReference> subpassColourAttachments;
			std::optional<VkAttachmentReference> subpassDepthAttachment;

			for (const auto& attachmentBinding : subpassType.attachmentBindings) {
				CHECK(attachmentBinding >= _attachments.size(), "Failed to find a renderpass attachment bound to: " << attachmentBinding);
				auto& attachment = _attachments[attachmentBinding];

				if (attachment.type == Attachment::Type::Depth) {
					subpassDepthAttachment = { attachmentBinding, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
				}
				else {
					VkAttachmentReference attachmentReference = {};
					attachmentReference.attachment = attachmentBinding;
					attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					subpassColourAttachments.emplace_back(attachmentReference);
				}
			}

			VkSubpassDescription subpass;
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = subpassColourAttachments.size();
			subpass.pColorAttachments = subpassColourAttachments.data();
			if (subpassDepthAttachment)
				subpass.pDepthStencilAttachment = &subpassDepthAttachment.value();

			// Subpass dependencies.
			VkSubpassDependency subpassDependency = {};
			subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			subpassDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			subpassDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			if (index == _subpasses.size() - 1) {
				subpassDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				subpassDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			}
			else {
				subpassDependency.dstSubpass = index;
			}

			if (index == 0) {
				subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				subpassDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			}
			else {
				subpassDependency.srcSubpass = index - 1;
			}

			dependencies.emplace_back(subpassDependency);
			++index;
		}

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = attachmentDescriptions.size();
		renderPassInfo.pAttachments = attachmentDescriptions.data();
		renderPassInfo.subpassCount = subpassDescriptions.size();
		renderPassInfo.pSubpasses = subpassDescriptions.data();
		renderPassInfo.dependencyCount = dependencies.size();
		renderPassInfo.pDependencies = dependencies.data();

		CHECK(vkCreateRenderPass(Graph::GetInstance().GetLogicalDevice(), &renderPassInfo, nullptr, &_renderPass) == VK_SUCCESS, "create render pass failed");
		return true;
	}
}