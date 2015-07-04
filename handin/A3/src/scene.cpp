#include "scene.hpp"
#include <iostream>
#include "ObjectAttribute.hpp"
#include "material.hpp"

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
  , mSelectable(false)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(bool picking)
{
	mViewer->pushMatrix(m_trans);
	for (ChildList::const_iterator it=m_children.begin(); it != m_children.end(); ++it) {
		(*it)->set_transform(m_rotate_translate * (*it)->get_transform());
		(*it)->set_rotate_translate(m_rotate_translate * (*it)->get_rotate_translate());
		(*it)->walk_gl(picking);
	}
	mViewer->popMatrix();
}

const QMatrix4x4& SceneNode::get_transform() const {
	return m_trans;
}
const QMatrix4x4& SceneNode::get_inverse() const {
	return m_invtrans;
}

void SceneNode::set_transform(const QMatrix4x4& m)
{
	m_trans = m;
	m_invtrans = m.inverted();
}

void SceneNode::set_transform(const QMatrix4x4& m, const QMatrix4x4& i)
{
	m_trans = m;
	m_invtrans = i;
}

QMatrix4x4 SceneNode::get_transform() {
	return m_trans;
}

void SceneNode::setJointTransform(QMatrix4x4 j) {
	mJointTransform = j;
}

QMatrix4x4 SceneNode::getJointTransform() {
	return mJointTransform;
}


// rotate joint
void SceneNode::rotateJoint(float angle, int x, int y, int z, int depth) {
	mAccJointTransform = m_translate.inverted() * mAccJointTransform;
	mAccJointTransform.rotate(angle, x, y, z); //= mAccJointTransform * mJointTransform;
	mAccJointTransform = m_translate * mAccJointTransform;
	
	for (ChildList::const_iterator it=m_children.begin(); it != m_children.end(); ++it) {
		if (is_joint()) {
			((GeometryNode*)(*it))->rotateJoint(angle, x, y, z, depth+1);
		} else {
			(*it)->rotateJoint(angle, x, y, z, depth+1);
		}
	}
}



void SceneNode::setAccJointTransform(QMatrix4x4 j) {
	mAccJointTransform = j;
}

QMatrix4x4 SceneNode::getAccJointTransform() {
	return mAccJointTransform;
}

void SceneNode::set_rotate_translate(QMatrix4x4 m) {
	m_rotate_translate = m;
}

QMatrix4x4 SceneNode::get_rotate_translate() {
	return m_rotate_translate;
}

QMatrix4x4 SceneNode::get_translate() {
	return m_translate;
}

QVector3D SceneNode::get_rotate_origin() {
	return m_rotate_origin;
}

void SceneNode::add_child(SceneNode* child)
{
	m_children.push_back(child);
}

void SceneNode::remove_child(SceneNode* child)
{
	m_children.remove(child);
}

void SceneNode::setSelectable(bool selectable) {
	mSelectable = selectable;
}


void SceneNode::rotate(char axis, double angle)
{
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
  m_trans = m_trans * mat;
  m_rotate_translate = m_rotate_translate * mat;
}

void SceneNode::scale(const Vector3D& amount)
{
  QMatrix4x4 mat;
  mat.scale(QVector3D(amount[0], amount[1], amount[2]));
  m_trans = m_trans * mat;
  m_scale = mat;
}

void SceneNode::translate(const Vector3D& amount)
{
  QMatrix4x4 mat;
  mat.translate(QVector3D(amount[0], amount[1], amount[2]));
  m_trans = m_trans * mat;
  m_rotate_translate = m_rotate_translate * mat;
  
  QMatrix4x4 trMat;
  trMat.translate(QVector3D(amount[0], amount[1], amount[2]));
  m_translate = m_translate * trMat;
  
  m_rotate_origin = QVector3D(-amount[0], -amount[1], -amount[2]);
}

bool SceneNode::is_joint() const
{
  return false;
}

void SceneNode::setViewer(Viewer* viewer) {
	mViewer = viewer;
	for (ChildList::const_iterator it=m_children.begin(); it != m_children.end(); ++it) {
		(*it)->setViewer(viewer);
	}
}


/*
 *	JointNode
 */

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(bool picking)
{
	for (ChildList::const_iterator it=m_children.begin(); it != m_children.end(); ++it) {
		(*it)->set_transform(m_rotate_translate * (*it)->get_transform());
		(*it)->set_rotate_translate(m_rotate_translate * (*it)->get_rotate_translate());
		(*it)->setSelectable(true);
		(*it)->walk_gl(picking);
	}
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


void GeometryNode::walk_gl(bool picking) {
	if (m_primitive) {
		ObjectAttribute objAttr = ObjectAttribute(QString(m_name.c_str()), m_trans, mAccJointTransform, m_scale, m_material);
		objAttr.setSelectable(mSelectable);
		objAttr.setNode(this);
		mViewer->addObject(objAttr);
	}
	
	for (ChildList::const_iterator it=m_children.begin(); it != m_children.end(); ++it) {
		(*it)->set_transform(m_rotate_translate * (*it)->get_transform());
		(*it)->set_rotate_translate(m_rotate_translate * (*it)->get_rotate_translate());
		(*it)->walk_gl(picking);
	}
}

void GeometryNode::rotateJoint(float angle, int x, int y, int z, int depth) {
	mAccJointTransform = m_translate.inverted() * mAccJointTransform;
	mAccJointTransform.rotate(angle, x, y, z);
	mAccJointTransform = m_translate * mAccJointTransform;
	
	if (m_primitive) {
		ObjectAttribute objAttr = ObjectAttribute(QString(m_name.c_str()), m_trans, mAccJointTransform, m_scale, m_material);
		objAttr.setTranslate(m_translate);
		objAttr.setSelectable(mSelectable);
		objAttr.setNode(this);
		mViewer->addObject(objAttr);
	}
	
	
	for (ChildList::const_iterator it=m_children.begin(); it != m_children.end(); ++it) {
		(*it)->rotateJoint(angle, x, y, z, depth+1);
	}
}

void GeometryNode::set_material(Material* material)
{
  m_material = material;
}

void GeometryNode::setViewer(Viewer* viewer) {
 	mViewer = viewer;
  	for (ChildList::const_iterator it=m_children.begin(); it != m_children.end(); ++it) {
  		(*it)->setViewer(viewer);
  	}
}

