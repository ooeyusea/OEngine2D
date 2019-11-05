#pragma once

#include "util.h"
#include "Resource.h"

namespace oengine2d {
	class Image : public Resource {
	public:
		enum Usage {
			TU_NORMAL,
			TU_RENDER_TARGET,
		};

		Image(const std::string& path, ResourceType type, Usage usage, VkFormat format) : Resource(type, path), _usage(usage), _format(format) {}
		Image(const std::string& path, ResourceType type, Usage usage) : Resource(type, path), _usage(usage) {}
		virtual ~Image();

		operator const VkImage& () const { return _texture; }

		VkFormat GetFormat() const { return _format; }

		VkDeviceSize CalcSize(VkFormat format);

	protected:
		Usage _usage;
		VkFormat _format = VK_FORMAT_UNDEFINED;
		VkDeviceSize _size = 0;
		bool _compress = false;

		VkImage _texture = nullptr;
		VkDeviceMemory _textureMemory = nullptr;
	};
}
