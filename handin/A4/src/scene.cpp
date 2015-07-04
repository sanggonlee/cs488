#include "scene.hpp"
#include <iostream>

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
  ,	mIsGeometry(false)
  , m_scale(Vector3D(1.0, 1.0, 1.0))
{
}

SceneNode::~SceneNode()
{
}
/*
void SceneNode::add_child(SceneNode* child)
  {
  	// When adding a child, create a unique node even if it points to the same node.
  	// This is to ensure preserving the tree properties.
  	if (child->is_geometry()) {
  		GeometryNode* c = dynamic_cast<GeometryNode*>(child)->make_copy();
  		m_children.push_back(c);
  	} else {
  		SceneNode* c = child->make_copy();
  		m_children.push_back(c);
  	}
    //m_children.push_back(child);
  }
*/
void SceneNode::rotate(char axis, double angle)
{
  std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
	double angleRadian = angle / (180/M_PI);
	Matrix4x4 mat;
	switch(axis) {
		case 'z':
			mat.assign(0, cos(angleRadian));
			mat.assign(1, -sin(angleRadian));
			mat.assign(4, sin(angleRadian));
			mat.assign(5, cos(angleRadian));
			/*
			mat = Matrix4x4(
				cos(angle),	-sin(angle),	0,		0,
				sin(angle),	 cos(angle),	0,		0,
				0,			 0,				1,		0,
				0,			 0,				0,		1
			);*/
			break;
		case 'x':
			mat.assign(5, cos(angleRadian));
			mat.assign(6, -sin(angleRadian));
			mat.assign(9, sin(angleRadian));
			mat.assign(10, cos(angleRadian));
			/*
			rotationMatrix = Matrix4x4(
				1,	0,				0,			0,
				0,	cos(angle),	   -sin(angle), 0,
				0,	sin(angle),		cos(angle), 0,
				0,	0,				0,			1
			);*/
			break;
		case 'y':
			mat.assign(0, cos(angleRadian));
			mat.assign(2, sin(angleRadian));
			mat.assign(8, -sin(angleRadian));
			mat.assign(10, cos(angleRadian));
			/*
			rotationMatrix = Matrix4x4(
				 cos(angle),	0,	sin(angle),		0,
				 0,				1,	0,				0,
				-sin(angle),	0,	cos(angle),		0,
				0,				0,	0,				1
			);*/
			break;
		default:
			std::cerr << "Invalid axis!" << std::endl;
			break;
	}
	m_rotate = m_rotate * mat;
	m_non_inherited.push_back(Transform(Transform::ROTATE, Vector3D(0,0,0), mat, Vector3D(1,1,1)));
	//m_non_inherited.insert(m_non_inherited.begin(),Transform(Transform::ROTATE, Vector3D(0,0,0), mat, Vector3D(1,1,1)));
}

void SceneNode::scale(const Vector3D& amount)
{
  //std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  Matrix4x4 mat;
  mat.assign(0, amount[0]);
  mat.assign(5, amount[1]);
  mat.assign(10, amount[2]);
  //m_scale = m_scale * mat;
  
  m_scale[0] = m_scale[0] * amount[0];
  m_scale[1] = m_scale[1] * amount[1];
  m_scale[2] = m_scale[2] * amount[2];
  m_non_inherited.push_back(Transform(Transform::SCALE, Vector3D(0,0,0), Matrix4x4(), amount));
  //m_non_inherited.insert(m_non_inherited.begin(),Transform(Transform::SCALE, Vector3D(0,0,0), Matrix4x4(), amount));
}

void SceneNode::translate(const Vector3D& amount)
{
  //std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
  Matrix4x4 mat;
  mat.assign(3, amount[0]);
  mat.assign(7, amount[1]);
  mat.assign(11, amount[2]);
  //m_translate = m_translate * mat;
  
  m_translate = m_translate + amount;
  m_non_inherited.push_back(Transform(Transform::TRANSLATE, amount, Matrix4x4(), Vector3D(1,1,1)));
  //m_non_inherited.insert(m_non_inherited.begin(),Transform(Transform::TRANSLATE, amount, Matrix4x4(), Vector3D(1,1,1)));
}

bool SceneNode::is_joint() const
{
  return false;
}

bool SceneNode::is_geometry() const {
	return mIsGeometry;
}

void SceneNode::traverseAndCollectPrimitives(std::set<Primitive*> &prims) {
	m_transforms.reserve(m_non_inherited.size() + m_transforms.size());
	m_transforms.insert(m_transforms.end(), m_non_inherited.begin(), m_non_inherited.end());
	//std::cout << "SceneNode::traverseAndCollectPrimitives" << std::endl;
	for (std::list<SceneNode*>::const_iterator it = m_children.begin(); it != m_children.end(); ++it) {
		transferTransforms(*it);
		if ((*it)->is_geometry()) {
			dynamic_cast<GeometryNode*>(*it)->traverseAndCollectPrimitives(prims);
		} else {
			(*it)->traverseAndCollectPrimitives(prims);
		}
		//(*it)->set_transform(m_trans * (*it)->get_transform());
	}
}

void SceneNode::transferTransforms(SceneNode* node) {
	//std::cout << "Transfer transformations from "<<m_name
	//	<<" to "<<node->get_name()<<"; size is "<<node->getTransforms()->size()<<std::endl;
	std::vector<Transform> *transforms = node->getTransforms();
/*
	transforms->reserve(transforms->size() + m_transforms.size());
	transforms->insert(transforms->end(), m_transforms.begin(), m_transforms.end());
*/
	//for (std::vector<Transform>::const_iterator it=m_transforms.begin(); it!=m_transforms.end(); ++it) {
	//	transforms->push_back(*it);
	//}
	*transforms = m_transforms;
}

/*
void SceneNode::setNonInherited(std::vector<Transform> trans) {
	m_non_inherited = trans;
}*/
/*
SceneNode* SceneNode::make_copy() {
	SceneNode* node = new SceneNode(m_name);
	node->setNonInherited(m_non_inherited);
	
	//std::list<SceneNode*> children = node->getChildren();
	for (std::list<SceneNode*>::const_iterator it=node->getChildren().begin(); 
			it!= node->getChildren().end(); ++it) {
		node->add_child(*it);
	}
	
	return node;
}*/

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
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
  : SceneNode(name)
  , m_primitive(primitive)
{
	mIsGeometry = true;
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::traverseAndCollectPrimitives(std::set<Primitive*> &prims) {
	//std::cout << "GeometryNode::traverseAndCollectPrimitives" << std::endl;
	m_transforms.reserve(m_non_inherited.size() + m_transforms.size());
	m_transforms.insert(m_transforms.end(), m_non_inherited.begin(), m_non_inherited.end());
	
	if (m_primitive) {
		Primitive* p = m_primitive->make_copy();
	
		//std:: cout << "TRANSFORM FOR "<<m_name<<" BEGIN"<<std::endl;
		std::vector<Transform>::const_iterator it = m_transforms.end()-1;
		for ( ; it != m_transforms.begin()-1; --it) {
			switch((*it).type) {
				case Transform::TRANSLATE:
					//std::cout << "--Translate "<< m_name <<" by "<<(*it).translate<<std::endl;
					p->translate((*it).translate);
					break;
				case Transform::ROTATE:
					//std::cout << "--Rotate "<< m_name <<" by"<<std::endl<<(*it).rotate<<std::endl;
					p->rotate((*it).rotate);
					break;
				case Transform::SCALE:
					//std::cout << "--Scale "<< m_name <<" by "<<(*it).scale<<std::endl;
					p->scale((*it).scale);
					break;
				default:
					std::cerr << "Invalid transform type" << std::endl;
					break;
			}
		}
		
		prims.insert(p);
	}
	for (std::list<SceneNode*>::const_iterator it = m_children.begin(); it != m_children.end(); ++it) {
		transferTransforms(*it);
		if ((*it)->is_geometry()) {
			dynamic_cast<GeometryNode*>(*it)->traverseAndCollectPrimitives(prims);
		} else {
			(*it)->traverseAndCollectPrimitives(prims);
		}
		//(*it)->set_transform(m_trans * (*it)->get_transform());
	}

}

