#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include "algebra.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include <set>
#include "transform.hpp"
#include <vector>

class GeometryNode;

class SceneNode {
public:
  SceneNode(const std::string& name);
  virtual ~SceneNode();

  std::string get_name() {
  	return m_name;
  }

  const Matrix4x4& get_transform() const { return m_trans; }
  const Matrix4x4& get_inverse() const { return m_invtrans; }
  
  void set_transform(const Matrix4x4& m)
  {
    m_trans = m;
    m_invtrans = m.invert();
  }

  void set_transform(const Matrix4x4& m, const Matrix4x4& i)
  {
    m_trans = m;
    m_invtrans = i;
  }

  void add_child(SceneNode* child) {
  	m_children.push_back(child);
  }

  void remove_child(SceneNode* child)
  {
    m_children.remove(child);
  }

  // Callbacks to be implemented.
  // These will be called from Lua.
  void rotate(char axis, double angle);
  void scale(const Vector3D& amount);
  void translate(const Vector3D& amount);

  // Returns true if and only if this node is a JointNode
  virtual bool is_joint() const;
  
  virtual bool is_geometry() const;
  
  void traverseAndCollectPrimitives(std::set<Primitive*> &prims);
  std::vector<Transform> *getTransforms() {
  	return &m_transforms;
  }
  //void setNonInherited(std::vector<Transform> trans);
  //std::list<SceneNode*> getChildren() {
  //	return m_children;
  //}
  
  void transferTransforms(SceneNode* node);
  //SceneNode* make_copy();
  
protected:
  
  // Useful for picking
  int m_id;
  std::string m_name;

  // Transformations
  Matrix4x4 m_trans;
  Matrix4x4 m_invtrans;
  
  Vector3D m_translate;
  Matrix4x4 m_rotate;
  Vector3D m_scale;
  
  std::vector<Transform> m_transforms;
  std::vector<Transform> m_non_inherited;
  
  bool mIsGeometry;

  // Hierarchy
  typedef std::list<SceneNode*> ChildList;
  ChildList m_children;
};

class JointNode : public SceneNode {
public:
  JointNode(const std::string& name);
  virtual ~JointNode();

  virtual bool is_joint() const;

  void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);

  struct JointRange {
    double min, init, max;
  };

  
protected:

  JointRange m_joint_x, m_joint_y;
};

class GeometryNode : public SceneNode {
public:
  GeometryNode(const std::string& name,
               Primitive* primitive);
  virtual ~GeometryNode();

  const Material* get_material() const;
  Material* get_material();

  void set_material(Material* material)
  {
    m_material = material;
    m_primitive->setMaterial(material);
  }
  
  void traverseAndCollectPrimitives(std::set<Primitive*> &prims);
  //GeometryNode* make_copy();

protected:
  Material* m_material;
  Primitive* m_primitive;
};

#endif
