#ifndef __FLOAT3UNIFORM_H__
#define __FLOAT3UNIFORM_H__
#include "../Uniform.h"

namespace oengine2d {
	class Float3Uniform : public Uniform {
	public:
		Float3Uniform(float a, float b, float c) : Uniform(uniform::Type::F3), _a(a), _b(b), _c(c) {}
		virtual ~Float3Uniform() {}

		virtual Uniform* Clone() { return new Float3Uniform(_a, _b, _c); }
		virtual void Release() { delete this; }

	protected:
		virtual void OnMerge(const Uniform* rhs);

	private:
		float _a;
		float _b;
		float _c;
	};
}
#endif //__FLOAT3UNIFORM_H__
