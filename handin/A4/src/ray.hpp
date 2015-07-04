#ifndef CS488_RAY
#define CS488_RAY

#include "algebra.hpp"

struct Ray {
	Ray( Point3D o, Vector3D d ) : 
			origin(o), 
			direction(d) {
	}
	
	Point3D origin;
	Vector3D direction;
};

#endif
