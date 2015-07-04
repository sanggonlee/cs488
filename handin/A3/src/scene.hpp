#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <QMatrix4x4>
#include "algebra.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include "Viewer.hpp"
#include <QMutex>

class Viewer;
class ObjectAttribute;
class SceneNode {
public:
  SceneNode(const std::string& name);
  virtual ~SceneNode();

  Q_SLOT virtual void walk_gl(bool picking = false);

  const QMatrix4x4& get_transform() const;
  const QMatrix4x4& get_inverse() const;
  
  void set_transform(const QMatrix4x4& m);
  void set_transform(const QMatrix4x4& m, const QMatrix4x4& i);
  QMatrix4x4 get_transform();
  
  void setJointTransform(QMatrix4x4 j);
  QMatrix4x4 getJointTransform();
  
  void rotateJoint(float angle, int x, int y, int z, int depth);
  
  void setAccJointTransform(QMatrix4x4 j);
  QMatrix4x4 getAccJointTransform();
  
  void set_rotate_translate(QMatrix4x4 m);
  QMatrix4x4 get_rotate_translate();
  
  QMatrix4x4 get_translate();
  
  QVector3D get_rotate_origin();

  void add_child(SceneNode* child);

  void remove_child(SceneNode* child);
  
  void setSelectable(bool selectable);

  // Callbacks to be implemented.
  // These will be called from Lua.
  void rotate(char axis, double angle);
  void scale(const Vector3D& amount);
  void translate(const Vector3D& amount);

  // Returns true if and only if this node is a JointNode
  virtual bool is_joint() const;
  
  void setViewer(Viewer* viewer);
  
protected:
  
  // Useful for picking
  int m_id;
  std::string m_name;

  // Transformations
  QMatrix4x4 m_trans;
  QMatrix4x4 m_invtrans;
  QMatrix4x4 mJointTransform;
  QMatrix4x4 mAccJointTransform;
  
  QMatrix4x4 m_rotate_translate;
  QMatrix4x4 m_scale;
  QMatrix4x4 m_translate;
  QVector3D m_rotate_origin;

  // Hierarchy
  typedef std::list<SceneNode*> ChildList;
  ChildList m_children;
  
  Viewer* mViewer;
  
  bool mSelectable;
};

class JointNode : public SceneNode {
public:
  JointNode(const std::string& name);
  virtual ~JointNode();

  virtual void walk_gl(bool bicking = false);

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

  virtual void walk_gl(bool picking = false);

  const Material* get_material() const;
  Material* get_material();

  void rotateJoint(float angle, int x, int y, int z, int depth);

  void set_material(Material* material);
  void setViewer(Viewer* viewer);

protected:
  Material* m_material;
  Primitive* m_primitive;
};

#endif
