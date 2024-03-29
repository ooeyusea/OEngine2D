#include "Sampler.h"
#include "../Graph.h"
#include "../LogicalDevice.h"
#include "../Image.h"

namespace oengine2d {
	Sampler::Sampler(const Image& image) : _image(image) {
		image.AddRef();
	}

	Sampler::~Sampler() {
		_image.DecRef();

		if (_sampler)
			vkDestroySampler(Graph::GetInstance().GetLogicalDevice(), _sampler, nullptr);
	}

	bool Sampler::Create() {
		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		CHECK(vkCreateSampler(Graph::GetInstance().GetLogicalDevice(), &samplerInfo, nullptr, &_sampler) == VK_SUCCESS, "failed to create texture sampler!");
		return true;
	}
}