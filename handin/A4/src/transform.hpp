#ifndef CS488_TRANSFORM
#define CS488_TRANSFORM

#include "algebra.hpp"

struct Transform {
	enum Type {
		TRANSLATE,
		ROTATE,
		SCALE
	};

	Transform(Type t_type, Vector3D t, Matrix4x4 r, Vector3D s)
		: type(t_type)
		, translate(t)
		, rotate(r)
		, scale(s) {
	}

	Type type;
	Vector3D translate;
	Matrix4x4 rotate;
	Vector3D scale;
};

#endif
