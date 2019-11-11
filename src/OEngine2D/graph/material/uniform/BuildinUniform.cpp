#include "BuildinUniform.h"

void BuildinUniform::Use(IDevice * device, IRenderCamera * camera) {
	switch (_usage) {
	case uniform::Usage::WORLD_VIEW_PROJECTION_MATRIX: break;
	}
}

