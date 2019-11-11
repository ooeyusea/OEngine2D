#ifndef __FLOAT2UNIFORM_H__
#define __FLOAT2UNIFORM_H__
#include "../Uniform.h"

namespace oengine2d {
	class Float2Uniform : public Uniform {
	public:
		Float2Uniform(float a, float b) : Uniform(uniform::Type::F2), _a(a), _b(b) {}
		virtual ~Float2Uniform() {}

		virtual Uniform* Clone() { return new Float2Uniform(_a, _b); }
		virtual void Release() { delete this; }

	protected:
		virtual void OnMerge(const Uniform* rhs);

	private:
		float _a;
		float _b;
	};
}
#endif //__FLOAT2UNIFORM_H__
