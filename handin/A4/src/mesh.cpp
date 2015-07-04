#include "mesh.hpp"
#include <iostream>
#include <limits>
//#include <algorithm>

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces)
{
	type = Primitive::MESH;
	triangulateFaces();
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  std::cerr << "mesh({";
  for (std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
    if (I != mesh.m_verts.begin()) std::cerr << ",\n      ";
    std::cerr << *I;
  }
  std::cerr << "},\n\n     {";
  
  for (std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
    if (I != mesh.m_faces.begin()) std::cerr << ",\n      ";
    std::cerr << "[";
    for (Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
      if (J != I->begin()) std::cerr << ", ";
      std::cerr << *J;
    }
    std::cerr << "]";
  }
  std::cerr << "});" << std::endl;
  return out;
}

void Mesh::triangulateFaces() {
	if (!m_triangles.empty()) {
		//std::cerr << "Already triangulated this mesh. Returning." << std::endl;
		return;
	}
	
	for (std::vector<Face>::const_iterator it = m_faces.begin(); it != m_faces.end(); ++it) {
		for (int i=0; i<(*it).size()-2; i++) {
			std::vector<int> triangle;
			triangle.push_back(it->at(0));
			triangle.push_back(it->at(i+1));
			triangle.push_back(it->at(i+2));
			m_triangles.push_back(triangle);
		}
	}
}

double Mesh::computeIntersection(Ray ray, Vector3D &normal) {
	//triangulateFaces();
	double dist = std::numeric_limits<double>::infinity();
	for (std::vector<Face>::const_iterator it=m_triangles.begin(); it!=m_triangles.end();++it) {
		Point3D v0 = m_verts.at((*it).at(0));
		Point3D v1 = m_verts.at((*it).at(1));
		Point3D v2 = m_verts.at((*it).at(2));
		double d = triangleIntersection(ray, v0, v1, v2, normal);
		if (d >= 0) {
			if (d < dist) {
				dist = d;
			}
		}/*
		if (dist >= 0) {
			return dist;
		}*/
	}
	return dist;
}

double Mesh::triangleIntersection(Ray ray, Point3D v0, Point3D v1, Point3D v2, Vector3D &n) {
	Vector3D edge1 = v1 - v0;
	Vector3D edge2 = v2 - v0;
	Vector3D normal = edge1.cross(edge2);
	normal.normalize();
	
	Vector3D pvec = ray.direction.cross(edge2);
	double det = edge1.dot(pvec);
	if (det > -1e-8 && det < 1e-8) {
		return -1;
	}
	double invDet = 1/det;
	Vector3D tvec = ray.origin - v0;
	double u = tvec.dot(pvec) * invDet;
	if (u<0 || u>1) {
		return -1;
	}
	
	Vector3D qvec = tvec.cross(edge1);
	double v = ray.direction.dot(qvec) * invDet;
	if (v < 0 || u+v > 1) {
		return -1;
	}

//	double t = edge2.dot(qvec) * invDet;
//	n = normal;
//	return t;
	
	n = normal;
	double t = -(normal.dot(ray.origin-v0))/(normal.dot(ray.direction));
	return t;
}

void Mesh::translate(Vector3D amount) {
	//std::cout << "Mesh::translate"<<std::endl;
	for (int i=0; i<m_verts.size(); i++) {
		//m_verts.assign(it, (*it) + amount);
		m_verts[i] = m_verts[i] + amount;
	}
}

void Mesh::rotate(Matrix4x4 mat) {
	for (int i=0; i<m_verts.size(); i++) {
		m_verts[i] = mat * m_verts[i];
	}
}

void Mesh::scale(Vector3D amount) {
	//std::cout << "Mesh::scale"<<std::endl;
	for (int i=0; i<m_verts.size(); i++) {
		m_verts[i] = Point3D(m_verts[i][0] * amount[0],
							 m_verts[i][1] * amount[1],
							 m_verts[i][2] * amount[2]);
	}
}

Mesh* Mesh::make_copy() {
	Mesh *p = new Mesh(m_verts, m_faces);
	p->setMaterial(getMaterial());
	p->type = Primitive::MESH;
	return p;
}




