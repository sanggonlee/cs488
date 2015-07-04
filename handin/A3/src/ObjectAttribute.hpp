#ifndef CS488_OBJECT_ATTRIBUTE_HPP
#define CS488_OBJECT_ATTRIBUTE_HPP

#include <QMatrix4x4>
#include <QGLShaderProgram>
#include "material.hpp"
#include "scene.hpp"

class GeometryNode;
class ObjectAttribute {
public:
	ObjectAttribute(QString name, QMatrix4x4 t, QMatrix4x4 j, QMatrix4x4 s, Material *m);
	~ObjectAttribute();
	
	static void initializePickerColour();
	
	void setNode(GeometryNode* node);
	void setTransform(QMatrix4x4 t);
	void setJointTransform(QMatrix4x4 t);
	void setScale(QMatrix4x4 s);
	void setTranslate(QMatrix4x4 t);
	void setMaterial(Material* m);
	void setSelectable(bool selectable);
	void setPicked(bool picked);
	
	QString getName();
	GeometryNode* getNode();
	QMatrix4x4 getTransform();
	QMatrix4x4 getJointTransform();
	QMatrix4x4 getScale();
	QMatrix4x4 getTranslate();
	Material* getMaterial();
	bool isSelectable();
	QColor getPickerColour();
	bool isPicked();
	
	void setLighting(QGLShaderProgram* program);
	
	void rotateJoint(float angle, int x, int y, int z);
	
	// Highlight this object upon picking, and undo if clicked again
	void onPicked();
	bool operator==(ObjectAttribute o);

private:
	QString mObjectName;
	GeometryNode* mNode;
	QMatrix4x4 mTransform;
	QMatrix4x4 mJointTransform;
	QMatrix4x4 mScale;
	QMatrix4x4 mTranslate;
	Material *mMaterial;
	bool mSelectable;
	bool mPicked;
	QColor mPickerColour;
	static int pickerColourIdentifier;
};

#endif
