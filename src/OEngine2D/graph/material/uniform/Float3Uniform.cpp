#include "Float3Uniform.h"

namespace oengine2d {
	void Float3Uniform::OnMerge(const Uniform* rhs) {
		_a = ((Float3Uniform*)rhs)->_a;
		_b = ((Float3Uniform*)rhs)->_b;
		_c = ((Float3Uniform*)rhs)->_c;
	}
}