#pragma once

#include "util.h"

namespace oengine2d {
	struct WriteDescriptorSet {
		VkDescriptorBufferInfo bufferInfo;
		VkWriteDescriptorSet write;
	};
}
