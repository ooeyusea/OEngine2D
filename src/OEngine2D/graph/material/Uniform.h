#ifndef __UNIFORM_H__
#define __UNIFORM_H__
#include "util.h"

namespace oengine2d {
	namespace uniform {
		enum Type {
			None = 0,
			INNNER,
			I1,
			I2,
			I3,
			I4,
			IV1,
			IV2,
			IV3,
			IV4,
			F1,
			F2,
			F3,
			F4,
			FV1,
			FV2,
			FV3,
			FV4,
			MATRIX2,
			MATRIX3,
			MATRIX4,
			TEXTURE2D,
			TEXTURE3D,
			TEXTURECUBE,
		};

		enum Usage
		{
			MODEL_MATRIX,
			VIEW_MATRIX,
			PROJECTION_MATRIX,
			VIEW_PROJECTION_MATRIX,
			WORLD_VIEW_PROJECTION_MATRIX,
			INVERSE_TRANSPOSE_WORLD_MATRIX,
			INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX,
			CAMERA_WORLD_POSITION,
		};
	}

	class Uniform {
	public:
		Uniform(uniform::Type type) : _type(type) {}
		virtual ~Uniform() {}

		void SetName(const std::string& name) { _name = name; }
		const std::string& GetName() const { return _name; }

		void Merge(const Uniform* rhs);

		virtual Uniform* Clone() = 0;
		virtual void Release() = 0;

	protected:
		virtual void OnMerge(const Uniform* rhs) = 0;

	protected:
		std::string _name;
		uniform::Type _type;
	};
}
#endif //__UNIFORM_H__
