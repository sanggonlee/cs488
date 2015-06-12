#include "scene.hpp"
#include <iostream>

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(bool picking) const
{
	std::cout << "[SceneNode::walk_gl] node " << m_name << std::endl;
	mViewer->pushMatrix(m_trans);
	std::cout << "[SceneNode::walk_gl] children size = " << m_children.size() << std::endl;
	for (ChildList::const_iterator it=m_children.begin(); it != m_children.end(); ++it) {
		(*it)->walk_gl(picking);
	}
	mViewer->popMatrix();
}

void SceneNode::rotate(char axis, double angle)
{
  std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
  // Fill me in
  
  QMatrix4x4 mat;
  switch(axis) {
  	case 'x':
  		mat.rotate(angle, 1, 0, 0);
  		break;
  	case 'y':
  		mat.rotate(angle, 0, 1, 0);
  		break;
  	case 'z':
  		mat.rotate(angle, 0, 0, 1);
  		break;
  	default:
  		std::cerr << "[SceneNode::rotate] Invalid axis given" << std::endl;
  		break;
  }
  set_transform(m_trans * mat);
  
  //m_viewer->pushMatrix(mat);
}

void SceneNode::scale(const Vector3D& amount)
{
  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  // Fill me in
  
  QMatrix4x4 mat;
  mat.scale(QVector3D(amount[0], amount[1], amount[2]));
  set_transform(m_trans * mat);
  
  //m_viewer->pushMatrix(mat);
}

void SceneNode::translate(const Vector3D& amount)
{
  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
  // Fill me in
  
  QMatrix4x4 mat;
  mat.translate(QVector3D(amount[0], amount[1], amount[2]));
  set_transform(m_trans * mat);
  
  //m_viewer->pushMatrix(mat);
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(bool picking) const
{
  // Fill me in
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::walk_gl(bool picking) const
{
	std::cout << "[GeometryNode::walk_gl] node " << m_name << std::endl;
	mViewer->pushMatrix(m_trans);
	
	std::cout << "[GeometryNode::walk_gl] children size = " << m_children.size() << std::endl;
//	for (ChildList::const_iterator it=m_children.begin(); it != m_children.end(); ++it) {
//		(*it)->walk_gl(picking);
//	}
	
	if (m_primitive) {
		std::cout << "[GeometryNode::walk_gl] drawing a primitive" << std::endl;
		//mViewer->transformModel(m_trans);
	//mViewer->drawSphere(m_trans);
		//mViewer->update();
		//mViewer->transformModel(m_invtrans);
		mViewer->addTransform(m_trans);
	}
	mViewer->popMatrix();
}

