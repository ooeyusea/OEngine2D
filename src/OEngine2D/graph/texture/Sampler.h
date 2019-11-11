#pragma once

#include "util.h"
#include "../Image.h"

namespace oengine2d {
	class Image;
	class Sampler {
	public:
		Sampler(const Image& image);
		~Sampler();

		bool Create();

	private:
		const Image& _image;
		VkSampler _sampler = nullptr;
	};
}
