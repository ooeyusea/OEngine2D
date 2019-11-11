#ifndef __BUILDINUNIFORM_H__
#define __BUILDINUNIFORM_H__
#include "Uniform.h"

class BuildinUniform : public Uniform {
public:
	BuildinUniform(uniform::Usage usage) : Uniform(uniform::Type::INNNER), _usage(usage) {}
	virtual ~BuildinUniform() {}

	virtual void Use(IDevice * device, IRenderCamera * camera);

	virtual Uniform * Clone() { return NEW BuildinUniform(_usage); }
	virtual void Release() { DEL this; }

protected:
	virtual void OnMerge(const Uniform * rhs) { OASSERT(false, "wtf"); }

private:
	uniform::Usage _usage;
};

#endif //__BUILDINUNIFORM_H__
