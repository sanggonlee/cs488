#include "primitive.hpp"
#include <math.h>
#include "algebra.hpp"

Primitive::~Primitive()
{
}

double Primitive::computeIntersection(Ray ray, Matrix4x4 &trans) {
	// Let subclasses implement this
	std::cout << "Primitive::computeIntersection"<<std::endl;
	return (double)0.0;
}

Vector3D Primitive::computeNormal(Vector3D pointAtTime) {
	// Let subclasses implement this
	return Vector3D(0,0,0);
}
void Primitive::translate(Vector3D amount) {
	Matrix4x4 mat;
	mat.assign(3, amount[0]);
	mat.assign(7, amount[1]);
	mat.assign(11, amount[2]);
	
	transform = transform * mat;
}
void Primitive::rotate(Matrix4x4 m) {
	transform = transform * m;
}
void Primitive::scale(Vector3D amount) {
	Matrix4x4 mat;
	mat.assign(0, amount[0]);
	mat.assign(5, amount[1]);
	mat.assign(10, amount[2]);
	
	transform = transform * mat;
	rotateScale = rotateScale * mat;
}

Primitive* Primitive::make_copy() {
	std::cerr << "Trying to make a copy of base primitive type. You're doing something wrong.."
		<< std::endl;
	Primitive *p = new Primitive();
	p->setMaterial(getMaterial());
	p->type = Primitive::UNDEFINED;
	p->transform = transform;
	return p;
}

/**************************************
 *	Sphere
 **************************************/

Sphere::~Sphere()
{
}
/*
void Sphere::translate(Vector3D amount) {
	//m_pos = m_pos + amount;
}

void Sphere::rotate(Matrix4x4 m) {
	// nothing to do here!
}

void Sphere::scale(Vector3D amount) {
	//m_size = m_size * amount[0];
}
*/
Sphere* Sphere::make_copy() {
	Sphere *p = new Sphere();
	p->setMaterial(getMaterial());
	p->type = Primitive::SPHERE;
	p->transform = transform;
	return p;
}

/**************************************
 *	Cube
 **************************************/

Cube::~Cube()
{
}
/*
void Cube::translate(Vector3D amount) {
	//m_pos = m_pos + amount;
}

void Cube::rotate(Matrix4x4 m) {
	// nothing to do here!
}

void Cube::scale(Vector3D amount) {
	//m_size = m_size * amount[0];
}
*/
Cube* Cube::make_copy() {
	Cube *p = new Cube();
	p->setMaterial(getMaterial());
	p->type = Primitive::CUBE;
	p->transform = transform;
	return p;
}

/**************************************
 *	NonhierSphere
 **************************************/

NonhierSphere::~NonhierSphere()
{
}

double NonhierSphere::computeIntersection(Ray ray, Matrix4x4 &trans) {
	//std::cout << "NonhierSphere::computeIntersection"<<std::endl;

	Point3D transformedOrigin = rotateScale.invert() * ray.origin;
	Vector3D transformedDirection = rotateScale.invert() * ray.direction;
	transformedDirection.normalize();

#if HIER
	Vector3D eyeToCenter = m_pos - transformedOrigin;
	double vecLeng = eyeToCenter.dot(transformedDirection);
#else
	Vector3D eyeToCenter = m_pos - ray.origin;
	double vecLeng = eyeToCenter.dot(ray.direction);
#endif
	double cameraToCenterLeng = eyeToCenter.dot(eyeToCenter);
	
	double discriminant = (m_radius*m_radius) - cameraToCenterLeng + (vecLeng*vecLeng);
	if (discriminant < 0) {
		return -1; // ray didn't hit object
	} else {
		//std::cout << "ray hit an object"<<std::endl;
		trans = rotateScale;
		return vecLeng - sqrt(discriminant);
	}
}

Vector3D NonhierSphere::computeNormal(Point3D pointAtTime) {
	Vector3D normal = (pointAtTime - m_pos);
	//normal.normalize();
#if HIER
	normal = rotateScale.invert().transpose() * normal;
#endif
	normal.normalize();
	return normal;
}

void NonhierSphere::translate(Vector3D amount) {
	m_pos = m_pos + amount;
}

void NonhierSphere::rotate(Matrix4x4 m) {
	m_pos = m * m_pos;
}
/*
void NonhierSphere::scale(Vector3D amount) {
	m_scale_matrix.assign(0, m_scale_matrix.getRow(0)[0] * amount[0]);
	m_scale_matrix.assign(5, m_scale_matrix.getRow(1)[1] * amount[1]);
	m_scale_matrix.assign(10, m_scale_matrix.getRow(2)[2] * amount[2]);
	//std::cout << "NonhierSphere::scale"<<std::endl;
	
	//m_pos = Point3D(m_pos[0]*amount[0], m_pos[1]*amount[1], m_pos[2]*amount[2]);
	//m_radius = m_radius * amount[1];
}
*/
NonhierSphere* NonhierSphere::make_copy() {
	NonhierSphere *p = new NonhierSphere(m_pos, m_radius);
	p->setMaterial(getMaterial());
	p->type = Primitive::NH_SPHERE;
	p->transform = transform;
	p->rotateScale = rotateScale;
	return p;
}

/**************************************
 *	NonhierBox
 **************************************/

NonhierBox::~NonhierBox()
{
}

double NonhierBox::computeIntersection(Ray ray, Matrix4x4 &trans) {
	double tmin = (m_pos[0] - ray.origin[0]) / ray.direction[0];
	double tmax = (m_pos[0]+m_xsize - ray.origin[0]) / ray.direction[0];
	if (tmin > tmax) {
		double tempx = tmax;
		tmax = tmin;
		tmin = tempx;
	}
	
	double tymin = (m_pos[1] - ray.origin[1]) / ray.direction[1];
	double tymax = (m_pos[1]+m_ysize - ray.origin[1]) / ray.direction[1];
	if (tymin > tymax) {
		double tempy = tymax;
		tymax = tymin;
		tymin = tempy;
	}
	
	if ((tmin > tymax) || (tymin > tmax)) {
		return -1; // didn't hit the object
	}
	
	if (tymin > tmin) {
		tmin = tymin;
	}
	if (tymax < tmax) {
		tmax = tymax;
	}
	
	double tzmin = (m_pos[2] - ray.origin[2]) / ray.direction[2];
	double tzmax = (m_pos[2]+m_zsize - ray.origin[2]) / ray.direction[2];
	if (tzmin > tzmax) {
		double tempz = tzmax;
		tzmax = tzmin;
		tzmin = tempz;
	}
	
	if ((tmin > tzmax) || (tzmin > tmax)) {
		return -1; // didn't hit the object
	}
	
	if (tzmin > tmin) {
		tmin = tzmin;
	}
	if (tzmax < tmax) {
		tmax = tzmax;
	}
	
	if (tmin < 0) {
		if (tmax < 0) {
			return -1;
		}
		return tmax;
	}
	return tmin;
}

Vector3D NonhierBox::computeNormal(Point3D pointAtTime) {
	Vector3D N;
	if (pointAtTime[0] == m_pos[0]) {				// on x-min surf
		N = Vector3D(-1, 0, 0);
	} else if (pointAtTime[0] == m_pos[0]+m_xsize) {	// on x-max surf
		N = Vector3D(1, 0, 0);
	} else if (pointAtTime[1] == m_pos[1]) {		// on y-min surf
		N = Vector3D(0, -1, 0);
	} else if (pointAtTime[1] == m_pos[1]+m_ysize) {	// on y-max surf
		N = Vector3D(0, 1, 0);
	} else if (pointAtTime[2] == m_pos[2]) {		// on z-min surf
		N = Vector3D(0, 0, -1);
	} else if (pointAtTime[2] == m_pos[2]+m_zsize) {	// on z-max surf
		N = Vector3D(0, 0, 1);
	}
	return N;
}

void NonhierBox::translate(Vector3D amount) {
	m_pos = m_pos + amount;
}

void NonhierBox::rotate(Matrix4x4 m) {
	m_pos = m * m_pos;
}
/*
void NonhierBox::scale(Vector3D amount) {
	//std::cout << "NonhierBox::scale"<<std::endl;
	m_pos = Point3D(m_pos[0]*amount[0], m_pos[1]*amount[0], m_pos[2]*amount[2]);
	m_xsize = m_xsize * amount[0];
	m_ysize = m_ysize * amount[1];
	m_zsize = m_zsize * amount[2];
}
*/
NonhierBox* NonhierBox::make_copy() {
	NonhierBox *p = new NonhierBox(m_pos, m_size);
	p->setMaterial(getMaterial());
	p->type = Primitive::NH_BOX;
	p->transform = transform;
	return p;
}
