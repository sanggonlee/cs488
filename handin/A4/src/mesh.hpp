#ifndef CS488_MESH_HPP
#define CS488_MESH_HPP

#include <vector>
#include <iosfwd>
#include "primitive.hpp"
#include "algebra.hpp"

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces);

  typedef std::vector<int> Face;
  
  double computeIntersection(Ray ray, Vector3D &normal);
  double triangleIntersection(Ray ray, Point3D v0, Point3D v1, Point3D v2, Vector3D &normal);
  
  virtual void translate(Vector3D amount);
  virtual void rotate(Matrix4x4 mat);
  virtual void scale(Vector3D amount);
  
  virtual Mesh* make_copy();
  
private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;
  std::vector<Face> m_triangles;
  
  void triangulateFaces();

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif
