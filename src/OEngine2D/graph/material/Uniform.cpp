#include "Uniform.h"

namespace oengine2d {
	void Uniform::Merge(const Uniform* rhs) {
		if (_type == rhs->_type)
			OnMerge(rhs);
	}
}
