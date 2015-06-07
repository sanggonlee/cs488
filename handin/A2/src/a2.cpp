#include <cmath>
#include <math.h>
#include <QMatrix4x4>
#include <QVector3D>
#include "algebra.hpp"

Matrix4x4 getViewMatrix(Vector3D eye, Vector3D center, Vector3D up) {
	Vector3D zaxis = center-eye;
	zaxis.normalize();
	Vector3D xaxis = cross(up, zaxis);
	xaxis.normalize();
	Vector3D yaxis = cross(zaxis, xaxis);
	
	Matrix4x4 translateToOriginMat(
		1.0,		0.0,		0.0,		-eye[0],
		0.0,		1.0,		0.0,		-eye[1],
		0.0,		0.0,		1.0,		-eye[2],
		0.0,		0.0,		0.0,		1.0
	);

	Matrix4x4 rotateViewMat(
		xaxis[0],	xaxis[1],	xaxis[2],	0,
		yaxis[0],	yaxis[1],	yaxis[2],	0,
		zaxis[0],	zaxis[1],	zaxis[2],	0,
		0,			0,			0,			1
	);
	
	return rotateViewMat*translateToOriginMat ;
}


Matrix4x4 getTranslationMatrix(double dist, char axis) {
	double x = 0;
	double y = 0; 
	double z = 0;
	if (axis == 'x') {
		x = dist;
	} else if (axis == 'y') {
		y = dist;
	} else if (axis == 'z') {
		z = dist;
	}
	
	Matrix4x4 translationMatrix(
		1,	0,	0,	x,
		0,	1,	0,	y,
		0,	0,	1,	z,
		0,	0,	0,	1
	);
	return translationMatrix;
}

Matrix4x4 getScaleMatrix(double factor, char axis) {
	double sX = 1.0;
	double sY = 1.0; 
	double sZ = 1.0;
	if (axis == 'x') {
		sX += factor;
	} else if (axis == 'y') {
		sY += factor;
	} else if (axis == 'z') {
		sZ += factor;
	}
	
	Matrix4x4 scaleMatrix(
		sX,	0,	0,	0,
		0,	sY,	0,	0,
		0,	0,	sZ,	0,
		0,	0,	0,	1
	);
	return scaleMatrix;
}

Matrix4x4 getRotationMatrix(double angle, char axis) {
	Matrix4x4 rotationMatrix;
	
	switch(axis) {
		case 'z':
			rotationMatrix = Matrix4x4(
				cos(angle),	-sin(angle),	0,		0,
				sin(angle),	 cos(angle),	0,		0,
				0,			 0,				1,		0,
				0,			 0,				0,		1
			);
			break;
		case 'x':
			rotationMatrix = Matrix4x4(
				1,	0,				0,			0,
				0,	cos(angle),	   -sin(angle), 0,
				0,	sin(angle),		cos(angle), 0,
				0,	0,				0,			1
			);
			break;
		case 'y':
			rotationMatrix = Matrix4x4(
				 cos(angle),	0,	sin(angle),		0,
				 0,				1,	0,				0,
				-sin(angle),	0,	cos(angle),		0,
				0,				0,	0,				1
			);
	}
	return rotationMatrix;
}
