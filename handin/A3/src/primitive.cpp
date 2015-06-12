#include "primitive.hpp"
#include <math.h>
#include <GL/gl.h>

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

void Sphere::walk_gl(bool picking) const
{
	//draw();
}
/*
void Sphere::draw() {
	if (!mViewer) {
		std::cerr << "Trying to draw a sphere but the Viewer is not set!" << std::endl;
	} else {
		mViewer->update();
	}
}*/
