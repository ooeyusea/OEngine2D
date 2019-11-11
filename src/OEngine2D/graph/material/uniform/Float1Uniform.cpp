#include "Float1Uniform.h"

namespace oengine2d {
	void Float1Uniform::OnMerge(const Uniform* rhs) {
		_a = ((Float1Uniform*)rhs)->_a;
	}

}
