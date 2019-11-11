#ifndef __MATRIXUNIFORM_H__
#define __MATRIXUNIFORM_H__
#include "../Uniform.h"

namespace oengine2d {
	class MatrixUniform : public Uniform {
	public:
		MatrixUniform(float* ptr) : Uniform(uniform::Type::MATRIX4) { SafeMemcpy(_m, sizeof(_m), ptr, sizeof(_m)); }
		virtual ~MatrixUniform() {}

		virtual Uniform* Clone() { return new MatrixUniform((float*)_m); }
		virtual void Release() { delete this; }

	protected:
		virtual void OnMerge(const Uniform* rhs);

		float _m[4][4];
	};
}

#endif //__MATRIXUNIFORM_H__
