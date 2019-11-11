#ifndef __FLOAT1UNIFORM_H__
#define __FLOAT1UNIFORM_H__
#include "../Uniform.h"

namespace oengine2d {
	class Float1Uniform : public Uniform {
	public:
		Float1Uniform(float a) : Uniform(uniform::Type::F1), _a(a) {}
		virtual ~Float1Uniform() {}

		virtual Uniform* Clone() { return new Float1Uniform(_a); }
		virtual void Release() { delete this; }

	protected:
		virtual void OnMerge(const Uniform* rhs);

	private:
		float _a;
	};
}
#endif //__FLOAT1UNIFORM_H__
