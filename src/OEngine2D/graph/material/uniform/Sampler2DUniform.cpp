#include "Sampler2DUniform.h"

#define MERGE_SAMPLER2D_STATE(mask, mask_var, variable) \
	if ((((Sampler2DUniform*)rhs)->##mask_var & Mask::##mask) > 0)\
	{\
		if ((mask_var & Mask::##mask) == 0)\
		{\
			variable = ((Sampler2DUniform*)rhs)->##variable;\
			mask_var |= Mask::##mask; \
		}\
	}

namespace oengine2d {
	void Sampler2DUniform::OnMerge(const Uniform* rhs) {
		if (_tex != ((Sampler2DUniform*)rhs)->_tex) {
			_tex = ((Sampler2DUniform*)rhs)->_tex;
			_mipmap = ((Sampler2DUniform*)rhs)->_mipmap;
			_s = ((Sampler2DUniform*)rhs)->_s;
			_t = ((Sampler2DUniform*)rhs)->_t;
			_minFilter = ((Sampler2DUniform*)rhs)->_minFilter;
			_magFilter = ((Sampler2DUniform*)rhs)->_magFilter;
			_mask = ((Sampler2DUniform*)rhs)->_mask;
		}
		else {
			MERGE_SAMPLER2D_STATE(MIPMAP, _mask, _mipmap);
			MERGE_SAMPLER2D_STATE(WARP_S, _mask, _s);
			MERGE_SAMPLER2D_STATE(WARP_T, _mask, _t);
			MERGE_SAMPLER2D_STATE(MIN_FILTER, _mask, _minFilter);
			MERGE_SAMPLER2D_STATE(MAG_FILTER, _mask, _magFilter);
		}
	}
}
