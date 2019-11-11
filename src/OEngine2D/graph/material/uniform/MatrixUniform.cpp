#include "MatrixUniform.h"

namespace oengine2d {
	void MatrixUniform::OnMerge(const Uniform* rhs) {
		SafeMemcpy(_m, sizeof(_m), ((MatrixUniform*)rhs)->_m, sizeof(_m));
	}
}
