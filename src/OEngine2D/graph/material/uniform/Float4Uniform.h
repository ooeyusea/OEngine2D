#ifndef __FLOAT4UNIFORM_H__
#define __FLOAT4UNIFORM_H__
#include "../Uniform.h"

namespace oengine2d {
	class Float4Uniform : public Uniform {
	public:
		Float4Uniform(float a, float b, float c, float d) : Uniform(uniform::Type::F4), _a(a), _b(b), _c(c), _d(d) {}
		virtual ~Float4Uniform() {}

		virtual Uniform* Clone() { return new Float4Uniform(_a, _b, _c, _d); }
		virtual void Release() { delete this; }

	protected:
		virtual void OnMerge(const Uniform* rhs);

	private:
		float _a;
		float _b;
		float _c;
		float _d;
	};
}

#endif //__FLOAT4UNIFORM_H__
