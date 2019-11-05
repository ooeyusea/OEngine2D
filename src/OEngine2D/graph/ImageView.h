#pragma once

#include "util.h"

namespace oengine2d {
	class Image;
	class ImageView {
	public:
		ImageView(const Image& image, VkImageViewType type);
		~ImageView();

		bool Create();

		operator const VkImageView& () const { return _imageView; }

	protected:
		const Image& _image;
		VkImageViewType _type;

		VkImageView _imageView = nullptr;
	};
}
