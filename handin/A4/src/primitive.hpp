#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include "ray.hpp"
#include "material.hpp"

#define HIER 0

class Primitive {
public:
  enum Type {
  	UNDEFINED,
  	SPHERE,
  	CUBE,
  	NH_SPHERE,
  	NH_BOX,
  	MESH
  };
  Primitive() 
    : type(UNDEFINED)
  {
  }
  virtual ~Primitive();
  double computeIntersection(Ray ray, Matrix4x4 &trans);
  Vector3D computeNormal(Vector3D pointAtTime);
  
  void setMaterial(Material* material) {
  	mMaterial = material;
  }
  Material* getMaterial() {
  	return mMaterial;
  }
  
  virtual void translate(Vector3D amount);
  virtual void rotate(Matrix4x4 m);
  virtual void scale(Vector3D amount);
  
  virtual Primitive* make_copy();
  
  Type type;
  Matrix4x4 transform;
  Matrix4x4 rotateScale;
private:
  Material* mMaterial;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  //double computeIntersection(Ray ray);
  //Vector3D computeNormal(Vector3D pointAtTime);
/*  
  virtual void translate(Vector3D amount);
  virtual void rotate(Matrix4x4 m);
  virtual void scale(Vector3D amount);
*/  
  virtual Sphere* make_copy();
};

class Cube : public Primitive {
public:
  virtual ~Cube();
/*  
  virtual void translate(Vector3D amount);
  virtual void rotate(Matrix4x4 m);
  virtual void scale(Vector3D amount);
*/  
  virtual Cube* make_copy();
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos)
    , m_radius(radius)
    , m_scale_matrix(Matrix4x4())
  {
  	type = NH_SPHERE;
  }
  virtual ~NonhierSphere();
  
  double computeIntersection(Ray ray, Matrix4x4 &trans);
  Vector3D computeNormal(Point3D pointAtTime);
  
  Point3D getPos() {
  	return m_pos;
  }

  virtual void translate(Vector3D amount);
  virtual void rotate(Matrix4x4 m);
/*  virtual void scale(Vector3D amount);
*/  
  virtual NonhierSphere* make_copy();

private:
  Point3D m_pos;
  double m_radius;
  Matrix4x4 m_scale_matrix;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size)
    : m_pos(pos)
    , m_size(size)
    , m_xsize(size)
    , m_ysize(size)
    , m_zsize(size)
  {
  	type = NH_BOX;
  }
  
  virtual ~NonhierBox();
  
  double computeIntersection(Ray ray, Matrix4x4 &trans);
  Vector3D computeNormal(Point3D pointAtTime);
  
  Point3D getPos() {
  	return m_pos;
  }

  virtual void translate(Vector3D amount);
  virtual void rotate(Matrix4x4 m);
/*  virtual void scale(Vector3D amount);
*/  
  virtual NonhierBox* make_copy();

private:
  Point3D m_pos;
  double m_size;
  double m_xsize;
  double m_ysize;
  double m_zsize;
};

#endif
