/*
 * File: RSDef.h
 * Author: ooeyusea
 *
 * Created On October 31, 2015, 06:01 AM
 */

#ifndef __RSDEF_H__
#define __RSDEF_H__
#include "util.h"

namespace rs {
	enum BlendFactor {
		BF_DST_ALPHA = GL_DST_ALPHA,
		BF_DST_COLOR = GL_DST_COLOR,
		BF_ONE = GL_ONE,
		BF_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
		BF_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
		BF_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
		BF_SRC_ALPHA = GL_SRC_ALPHA,
		BF_SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE,
		BF_ZERO = GL_ZERO,
	};

	struct BlendFunc {
		BlendFactor src;
		BlendFactor dst;

		bool operator!=(const BlendFunc& rhs) const
		{
			return src != rhs.src || dst != rhs.dst;
		}
	};

	enum AlphaTestFunc {
		ATF_ALWAYS = GL_ALWAYS,
		ATF_NEVER = GL_NEVER,
		ATF_LESS = GL_LESS,
		ATF_LEQUAL = GL_LEQUAL,
		ATF_EQUAL = GL_EQUAL,
		ATF_GREATER = GL_GREATER,
		ATF_GEQUAL = GL_GEQUAL,
		ATF_NOTEQUAL = GL_NOTEQUAL,
	};

	enum StencilFuncType {
		SF_ALWAYS = GL_ALWAYS,
		SF_NEVER = GL_NEVER,
		SF_LESS = GL_LESS,
		SF_LEQUAL = GL_LEQUAL,
		SF_EQUAL = GL_EQUAL,
		SF_GREATER = GL_GREATER,
		SF_GEQUAL = GL_GEQUAL,
		SF_NOTEQUAL = GL_NOTEQUAL,
	};

	struct StencilFunc {
		StencilFuncType func;
		int ref;
		unsigned int mask;

		bool operator!=(const StencilFunc& rhs) const
		{
			return func != rhs.func || ref != rhs.ref || mask != rhs.mask;
		}
	};

	enum StencilOpType {
		SO_KEEP = GL_KEEP,
		SO_ZERO = GL_ZERO,
		SO_REPLACE = GL_REPLACE,
		SO_INCR = GL_INCR,
		SO_DECR = GL_DECR,
		SO_INVERT = GL_INVERT,
	};

	struct StencilOp {
		StencilOpType s_fail;
		StencilOpType d_fail;
		StencilOpType d_pass;

		bool operator!=(const StencilOp& rhs) const
		{
			return s_fail != rhs.s_fail || d_fail != rhs.d_fail || d_pass != rhs.d_pass;
		}
	};

	enum DepthFunc {
		DF_ALWAYS = GL_ALWAYS,
		DF_NEVER = GL_NEVER,
		DF_LESS = GL_LESS,
		DF_LEQUAL = GL_LEQUAL,
		DF_EQUAL = GL_EQUAL,
		DF_GREATER = GL_GREATER,
		DF_GEQUAL = GL_GEQUAL,
		DF_NOTEQUAL = GL_NOTEQUAL,
	};

	enum CullFaceSide {
		FRONT = GL_FRONT,
		BACK = GL_BACK,
	};

	enum FrontFace {
		CCW = GL_CCW,
		CW = GL_CW,
	};
}

#endif /*__RSDEF_H__ */
