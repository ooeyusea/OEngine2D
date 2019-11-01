#include "PreDepthStage.h"
#include "../RenderPass.h"

namespace oengine2d {
	PreDepthStage::~PreDepthStage() {
		SAFE_DELETE(_renderPass);
	}

	bool PreDepthStage::Create() {
		_renderPass = new RenderPass({ {Attachment::Type::Depth, VK_FORMAT_UNDEFINED, true, false} }, { {{0}} });
		CHECK(_renderPass, "create pre depth stage render pass failed");
		return true;
	}
}
