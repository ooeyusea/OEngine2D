#include "Float2Uniform.h"

namespace oengine2d {
	void Float2Uniform::OnMerge(const Uniform* rhs) {
		_a = ((Float2Uniform*)rhs)->_a;
		_b = ((Float2Uniform*)rhs)->_b;
	}
}