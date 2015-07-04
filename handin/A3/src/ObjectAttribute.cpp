#include "ObjectAttribute.hpp"

ObjectAttribute::ObjectAttribute(QString name, QMatrix4x4 t, QMatrix4x4 j, QMatrix4x4 s, Material *m):
		mObjectName(name)
		, mTransform(t)
		, mJointTransform(j)
		, mScale(s)
		, mMaterial(m)
		, mSelectable(false)
		, mPicked(false) {
	static int pickerColourIdentifier;
	
	// generate unique colour for each object, max 255*255*255 objects
	int r = (pickerColourIdentifier/255)/255;
	int g = (pickerColourIdentifier/255)%255;
	int b = pickerColourIdentifier%255;
	mPickerColour = QColor(r,g,b);
	pickerColourIdentifier += 1;
}

ObjectAttribute::~ObjectAttribute() {
}
	
void ObjectAttribute::initializePickerColour() {
	static int pickerColourIdentifier;
	pickerColourIdentifier = 0;
}

void ObjectAttribute::setNode(GeometryNode* node) {
	mNode = node;
}

void ObjectAttribute::setTransform(QMatrix4x4 t) {
	mTransform = t;
}
void ObjectAttribute::setJointTransform(QMatrix4x4 j) {
	mJointTransform = j;
}
void ObjectAttribute::setScale(QMatrix4x4 s) {
	mScale = s;
}
void ObjectAttribute::setTranslate(QMatrix4x4 t) {
	mTranslate = t;
}
void ObjectAttribute::setMaterial(Material* m) {
	mMaterial = m;
}
void ObjectAttribute::setSelectable(bool selectable) {
	mSelectable = selectable;
}
void ObjectAttribute::setPicked(bool picked) {
	mPicked = picked;
}


QString ObjectAttribute::getName() {
	return mObjectName;
}

GeometryNode* ObjectAttribute::getNode() {
	return mNode;
}

QMatrix4x4 ObjectAttribute::getTransform() {
	return mTransform;
}
QMatrix4x4 ObjectAttribute::getJointTransform() {
	return mJointTransform;
}
QMatrix4x4 ObjectAttribute::getScale() {
	return mScale;
}
QMatrix4x4 ObjectAttribute::getTranslate() {
	return mTranslate;
}
Material* ObjectAttribute::getMaterial() {
	return mMaterial;
}
bool ObjectAttribute::isSelectable() {
	return mSelectable;
}
QColor ObjectAttribute::getPickerColour() {
	return mPickerColour;
}

void ObjectAttribute::setLighting(QGLShaderProgram* program) {
	mMaterial->apply_gl(program, mPicked);
}

void ObjectAttribute::rotateJoint(float angle, int x, int y, int z) {
	mJointTransform.rotate(angle, x, y, z);
}

// Highlight this object upon picking, and undo if clicked again
void ObjectAttribute::onPicked() {
	mPicked = !mPicked;
}

bool ObjectAttribute::isPicked() {
	return mPicked;
}

bool ObjectAttribute::operator==(ObjectAttribute o) {
	return (mObjectName == o.getName());
}
