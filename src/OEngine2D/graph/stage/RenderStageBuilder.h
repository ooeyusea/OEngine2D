#pragma once

#include "util.h"

namespace oengine2d {
	class RenderStageBuilder {
	public:
		RenderStageBuilder() {}
		~RenderStageBuilder() {}

		bool Build();
	};
}