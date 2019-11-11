#ifndef __SAMPLER2DUNIFORM_H__
#define __SAMPLER2DUNIFORM_H__
#include "../Uniform.h"
#include "../../texture/Texture2D.h"

namespace oengine2d {
	namespace tex {
		enum MinFilter {
			MIN_NEAREST = GL_NEAREST,
			MIN_LINEAR = GL_LINEAR,
			MIN_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			MIN_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			MIN_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			MIN_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
		};

		enum MagFilter {
			MAG_NEAREST = GL_NEAREST,
			MAG_LINEAR = GL_LINEAR,
		};

		enum Warp {
			REPEAT = GL_REPEAT,
			CLAMP = GL_CLAMP,
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		};
	}

	class Sampler2DUniform : public Uniform {
	public:
		enum Mask
		{
			MIPMAP = 0x01,
			WARP_S = 0x02,
			WARP_T = 0x04,
			WARP_R = 0x08,
			MIN_FILTER = 0x10,
			MAG_FILTER = 0x20,
		};

		Sampler2DUniform(Texture2D * tex, bool mipmap, tex::Warp s, tex::Warp t, tex::MinFilter minFilter, tex::MagFilter magFilter, uint32_t mask)
			: Uniform(uniform::Type::TEXTURE2D)
			, _tex(tex)
			, _mipmap(mipmap)
			, _s(s)
			, _t(t)
			, _minFilter(minFilter)
			, _magFilter(magFilter)
			, _mask(mask) {
			_tex->AddRef();
		}
		virtual ~Sampler2DUniform() {
			if (_tex)
				_tex->DecRef();
		}

		virtual Uniform* Clone() { return new Sampler2DUniform(_tex, _mipmap, _s, _t, _minFilter, _magFilter, _mask); }
		virtual void Release() { delete this; }

	protected:
		virtual void OnMerge(const Uniform* rhs);

	private:
		Texture2D * _tex;
		bool _mipmap;
		tex::Warp _s;
		tex::Warp _t;
		tex::MinFilter _minFilter;
		tex::MagFilter _magFilter;
		uint32_t _mask;
	};
}

#endif //__SAMPLER2DUNIFORM_H__
