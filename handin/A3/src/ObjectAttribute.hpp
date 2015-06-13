#ifndef CS488_OBJECT_ATTRIBUTE_HPP
#define CS488_OBJECT_ATTRIBUTE_HPP

#include <QMatrix4x4>
#include <QGLShaderProgram>
#include "material.hpp"

class ObjectAttribute {
public:
	ObjectAttribute(QMatrix4x4 t, Material *m):
		mTransform(t)
	,	mMaterial(m){}
	void setTransform(QMatrix4x4 t) {
		mTransform = t;
	}
	void setMaterial(Material* m) {
		mMaterial = m;
	}
	
	QMatrix4x4 getTransform() {
		return mTransform;
	}
	Material* getMaterial() {
		return mMaterial;
	}
	
	void setLighting(QGLShaderProgram* program) {
		mMaterial->apply_gl(program);
	}
private:
	QMatrix4x4 mTransform;
	Material *mMaterial;
};

#endif
