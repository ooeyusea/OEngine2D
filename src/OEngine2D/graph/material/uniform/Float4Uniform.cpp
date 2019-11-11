#include "Float4Uniform.h"

namespace oengine2d {
	void Float4Uniform::OnMerge(const Uniform* rhs) {
		_a = ((Float4Uniform*)rhs)->_a;
		_b = ((Float4Uniform*)rhs)->_b;
		_c = ((Float4Uniform*)rhs)->_c;
		_d = ((Float4Uniform*)rhs)->_d;
	}
}
