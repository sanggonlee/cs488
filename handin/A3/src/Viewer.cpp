#include <QtWidgets>
#include <QtOpenGL>
//#include <QGuiApplication>
#include "Viewer.hpp"
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <unistd.h>


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

Viewer::Viewer(const QGLFormat& format, QWidget *parent) 
    : QGLWidget(format, parent) 
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    , mCircleBufferObject(QOpenGLBuffer::VertexBuffer)
    , mSphereBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
#else 
    , mCircleBufferObject(QGLBuffer::VertexBuffer)
    , mSphereBufferObject(QGLBuffer::VertexBuffer)
#endif
	, mPicking(false)
	, mMode('p')
	, mShowCircle(false)
	, mZBuffer(false)
	, mBackfaceCulling(false)
	, mFrontfaceCulling(false)
{
	mMatrixStack.push(QMatrix4x4());
	ObjectAttribute::initializePickerColour();
}

Viewer::~Viewer() {
    // Nothing to do here right now.
}

QSize Viewer::minimumSizeHint() const {
    return QSize(100, 100);
}

QSize Viewer::sizeHint() const {
    return QSize(600, 600);
}


void Viewer::initializeGL() {

    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glShadeModel(GL_SMOOTH);
    glClearColor( 0.7, 0.7, 1.0, 0.0 );
    glDisable(GL_DEPTH_TEST);
    
    if (!mProgram.addShaderFromSourceFile(QGLShader::Vertex, "shader.vert")) {
        std::cerr << "Cannot load vertex shader." << std::endl;
        return;
    }

    if (!mProgram.addShaderFromSourceFile(QGLShader::Fragment, "shader.frag")) {
        std::cerr << "Cannot load fragment shader." << std::endl;
        return;
    }

    if ( !mProgram.link() ) {
        std::cerr << "Cannot link shaders." << std::endl;
        return;
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.create();
    mVertexArrayObject.bind();

//    mCircleBufferObject.create();
//    mCircleBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
    
    mSphereBufferObject.create();
    mSphereBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
    
    mSphereNormalBufferObject.create();
#else 
    /*
     * if qt version is less than 5.1, use the following commented code
     * instead of QOpenGLVertexVufferObject. Also use QGLBuffer instead of 
     * QOpenGLBuffer.
     */

    uint vao;
     
    typedef void (APIENTRY *_glGenVertexArrays) (GLsizei, GLuint*);
    typedef void (APIENTRY *_glBindVertexArray) (GLuint);
     
    _glGenVertexArrays glGenVertexArrays;
    _glBindVertexArray glBindVertexArray;
     
    glGenVertexArrays = (_glGenVertexArrays) QGLWidget::context()->getProcAddress("glGenVertexArrays");
    glBindVertexArray = (_glBindVertexArray) QGLWidget::context()->getProcAddress("glBindVertexArray");
     
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);    

//    mCircleBufferObject.create();
//    mCircleBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
    
    mSphereBufferObject.create();
    mSphereBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
    
    mSphereNormalBufferObject.create();
#endif
/*
    if (!mCircleBufferObject.bind()) {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }
*/  
    if (!mSphereBufferObject.bind()) {
    	std::cerr << "could not bind vertex buffer for sphere to the context." << std::endl;
    }

    if (!mSphereNormalBufferObject.bind()) {
    	std::cerr << "could not bind buffer for sphere normal to the context." << std::endl;
    }
  
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	float theta = 0, phi = 0, deltaTheta = M_PI / 36, deltaPhi = M_PI / 36;
	//float z1, x1, y1, z2, x2, y2, z3, x3, y3, z4, x4, y4;
	float r = 1.0;
	int slice = 73, sec = 36;

	int sphereDataSize = slice*sec*6*3;
	float sphereData[sphereDataSize+120];
	
	// data for trackball
    double radius = width() < height() ? 
        (float)width() * 0.25 : (float)height() * 0.25;
        
    for(size_t i=0; i<40; ++i) {
        sphereData[i*3] = radius * cos(i*2*M_PI/40);
        sphereData[i*3 + 1] = radius * sin(i*2*M_PI/40);
        sphereData[i*3 + 2] = 0.0;
    }

	// data for sphere
	int i=0;
	for (theta = 0; theta <= 2*M_PI; theta += deltaTheta) {
		for (phi = 0; phi <= M_PI-0.001; phi += deltaPhi) {
			sphereData[120+i++] = r*sin(theta)*cos(phi);
			sphereData[120+i++] = r*sin(theta)*sin(phi);
			sphereData[120+i++] = r*cos(theta);
			
			sphereData[120+i++] = r*sin(theta + deltaTheta)*cos(phi);
			sphereData[120+i++] = r*sin(theta + deltaTheta)*sin(phi);
			sphereData[120+i++] = r*cos(theta + deltaTheta);
			
			sphereData[120+i++] = r*sin(theta)*cos(phi + deltaPhi);
			sphereData[120+i++] = r*sin(theta)*sin(phi + deltaPhi);
			sphereData[120+i++] = r*cos(theta);
			
			sphereData[120+i++] = r*sin(theta + deltaTheta)*cos(phi + deltaPhi);
			sphereData[120+i++] = r*sin(theta + deltaTheta)*sin(phi + deltaPhi);
			sphereData[120+i++] = r*cos(theta + deltaTheta);
			
			sphereData[120+i++] = r*sin(theta + deltaTheta)*cos(phi);
			sphereData[120+i++] = r*sin(theta + deltaTheta)*sin(phi);
			sphereData[120+i++] = r*cos(theta + deltaTheta);
			
			sphereData[120+i++] = r*sin(theta)*cos(phi + deltaPhi);
			sphereData[120+i++] = r*sin(theta)*sin(phi + deltaPhi);
			sphereData[120+i++] = r*cos(theta);
		}
	}

	mSphereBufferObject.allocate(sphereData, (sphereDataSize+120) * sizeof(float));
								
	float sphereNormalData[slice*sec*6*3];
	for (int j=0; j<slice*sec*6*3; j++) {
		sphereNormalData[j] = sphereData[j]; // since normal = position - origin; origin is 0
	}
 
    mSphereNormalBufferObject.allocate(sphereNormalData, slice*sec*6*3*sizeof(float));

    mProgram.bind();

    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);
    
    mProgram.enableAttributeArray("normal");
    mProgram.setAttributeBuffer("normal", GL_FLOAT, 0, 3);

    // set color buffer
    mProgram.enableAttributeArray("color");
    mProgram.setAttributeBuffer("color", GL_FLOAT, 0, 1);

    mMvpMatrixLocation = mProgram.uniformLocation("mvpMatrix");
    
    mShadingOrNotLocation = mProgram.uniformLocation("usingShading");
    mProgram.setUniformValue(mShadingOrNotLocation, true);
    mColorLocation = mProgram.uniformLocation("frag_color");
}


void Viewer::paintGL() {
	// Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // enable or disable z-buffer
    if (mZBuffer) {
    	glEnable(GL_DEPTH_TEST);
    } else {
    	glDisable(GL_DEPTH_TEST);
    }

	// enable or disable back or front face culling
	if (mBackfaceCulling && mFrontfaceCulling) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	} else if (mBackfaceCulling) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	} else if (mFrontfaceCulling) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	} else {
		glDisable(GL_CULL_FACE);
	}

    // Set up lighting
    mProgram.setUniformValue(mShadingOrNotLocation, true);
    //QMatrix3x3 normalMatrix = (mViewMatrix*mTransformMatrix).normalMatrix();
    
    QMatrix4x4 lightTransformation;
    float lightAngle = M_PI/2;
    lightTransformation.rotate(lightAngle, 0, 1, 0);
    QVector3D lightPosition = lightTransformation * QVector3D(0, 1, 1);
    mProgram.setUniformValue("lightPosition", mViewMatrix * lightPosition);
    
	mProgram.setUniformValue("ambientReflection", (GLfloat)1.0);
    mProgram.setUniformValue("diffuseReflection", (GLfloat)1.0);
    mProgram.setUniformValue("specularReflection", (GLfloat)1.0);

    // Draw stuff
	mProgram.setUniformValue("drawingTrackball", mShowCircle);

	for (std::vector<ObjectAttribute>::iterator it=mObjects.begin(); it != mObjects.end(); ++it) {
		QMatrix4x4 T;
		QVector3D origin = it->getNode()->get_rotate_origin();
		T.translate(origin.x(), origin.y(), origin.z());
		mProgram.setUniformValue(mMvpMatrixLocation, 
			getCameraMatrix() * 
			it->getTransform());
		if (mPicking) {
			set_colour(it->getPickerColour());
		} else {
			it->setLighting(&mProgram);
		}
		
		QMatrix4x4 mvMatrix = 
			mViewMatrix * 
			mTransformMatrix * 
			it->getTransform();
		mProgram.setUniformValue("origin", origin);
		mProgram.setUniformValue("jointRotateMatrix", it->getJointTransform());
		
		mProgram.setUniformValue("mvMatrix", mvMatrix);
		
		QMatrix3x3 normalMatrix = mvMatrix.normalMatrix();
		mProgram.setUniformValue("normalMatrix", normalMatrix);
		
		
		//set_colour(QColor(0.0, 0.0, 1.0));
		glDrawArrays(GL_TRIANGLE_STRIP, 40, 73*36*6);
	}
	
	if (mShowCircle) {
		draw_trackball_circle();
	}
}

void Viewer::resizeGL(int width, int height) {

    if (height == 0) {
        height = 1;
    }

    mPerspMatrix.setToIdentity();
    mPerspMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
}

void Viewer::mousePressEvent ( QMouseEvent * event ) {
	glDisable(GL_LIGHTING);
    int x = event->x();
    int y = event->y();
   	
   	if (mMode == 'j' && (event->buttons() & Qt::LeftButton)) {
		// temporarily set colour of each object for sake of picking
		mProgram.setUniformValue(mShadingOrNotLocation, false);
		mPicking = true;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (std::vector<ObjectAttribute>::iterator it=mObjects.begin(); it != mObjects.end(); ++it) {
			mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * it->getTransform() * it->getJointTransform());
			set_colour(it->getPickerColour());
		
			QMatrix4x4 mvMatrix = mViewMatrix * mTransformMatrix * it->getTransform() * it->getJointTransform();
			mProgram.setUniformValue("mvMatrix", mvMatrix);
		
			QMatrix3x3 normalMatrix = mvMatrix.normalMatrix();
			mProgram.setUniformValue("normalMatrix", normalMatrix);
		
			glDrawArrays(GL_TRIANGLE_STRIP, 40, 73*36*6);
		}
		
	  	// Retrieve the colour of the pixel clicked
		unsigned char pixel[3];
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glReadPixels(x, viewport[3]-y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
		qDebug() << "r="<<pixel[0]<<", g="<<pixel[1]<<", b="<<pixel[2];
		QColor pixelColour = QColor(pixel[0], pixel[1], pixel[2]);
		
		for (std::vector<ObjectAttribute>::iterator it=mObjects.begin(); it != mObjects.end(); ++it) {
			if (it->getPickerColour().name() == pixelColour.name()) {
				if (it->isSelectable()) {
					it->onPicked();
				}
				break;
			}
		}
		
		// set back to shading
		glEnable(GL_LIGHTING);
		mPicking = false;
		mProgram.setUniformValue(mShadingOrNotLocation, true);
		update();
	} else if (mMode == 'p') {
		mCurrentTranslateMatrix.setToIdentity();
		mCurrentRotateMatrix.setToIdentity();
	}
	
	mLastX = x;
	mLastY = y;
}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
	if (!mCurrentTranslateMatrix.isIdentity()) {
		mTranslateStack.push_back(mCurrentTranslateMatrix);
		mOrderStack.push_back('t');
	} else if (!mCurrentRotateMatrix.isIdentity()) {
		mRotateStack.push_back(mCurrentRotateMatrix);
		mOrderStack.push_back('r');
	}
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    if (mMode == 'p') {
    	if (event->buttons() & Qt::LeftButton) {
			translateWorld((event->x()-mLastX)/5, 0, 0);
			translateWorld(0, (mLastY-event->y())/5, 0);
		} else if (event->buttons() & Qt::MiddleButton) {
			translateWorld(0, 0, (event->y()-mLastY)/2);
		} else if (event->buttons() & Qt::RightButton) {
			rotateWorld((event->x()-mLastX)/2, 0, 1, 0);
			rotateWorld((event->y()-mLastY)/2, 1, 0, 0);
		}
	} else if (mMode == 'j') {
		if (event->buttons() & Qt::MiddleButton) {
			std::vector<ObjectAttribute>::iterator it=mObjects.begin();
			for (; it != mObjects.end(); ++it) {
				if (it->isPicked()) {
					(it->getNode())->rotateJoint((event->y()-mLastY)/10.0, 1, 0, 0, 0);
				}
			}
		}
	}
	
	update();
	mLastX = event->x();
	mLastY = event->y();
}

QMatrix4x4 Viewer::getCameraMatrix() {
    // Todo: Ask if we want to keep this.
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0.0, 0.0, 20.0);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);
    mViewMatrix = vMatrix;

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void Viewer::translateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.translate(x, y, z);
    mModelTranslateMatrix.translate(x, y, z);
    mCurrentTranslateMatrix.translate(x, y, z);
}

void Viewer::rotateWorld(float angle, float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.rotate(angle, x, y, z);
    mModelRotateMatrix.rotate(angle, x, y, z);
    mCurrentRotateMatrix.rotate(angle, x, y, z);
}

void Viewer::scaleWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.scale(x, y, z);
}

void Viewer::set_colour(const QColor& col)
{
	mProgram.setUniformValue(mColorLocation, col.red()/255.0, col.green()/255.0, col.blue()/255.0);
}

void Viewer::draw_trackball_circle()
{
	glDisable(GL_DEPTH_TEST);
    int current_width = width();
    int current_height = height();
    std::cout << "w:"<<width()<<", h:"<<height()<<std::endl;

    // Set up for orthogonal drawing to draw a circle on screen.
    // You'll want to make the rest of the function conditional on
    // whether or not we want to draw the circle this time around.

	mProgram.setUniformValue(mShadingOrNotLocation, false);
    mProgram.setUniformValue(mColorLocation, 0.0, 0.0, 0.0);

    // Set orthographic Matrix
    QMatrix4x4 orthoMatrix;
    orthoMatrix.ortho(0.0, (float)current_width, 
           0.0, (float)current_height, -0.1, 0.1);

    // Translate the view to the middle
    QMatrix4x4 transformMatrix;
    transformMatrix.translate(width()/2.0, height()/2.0, 0.0);

    // Bind buffer object
    mSphereBufferObject.bind();
    mProgram.setUniformValue("drawingTrackball", true);
    mProgram.setUniformValue(mMvpMatrixLocation, orthoMatrix * transformMatrix);

    // Draw buffer
    glDrawArrays(GL_LINE_LOOP, 0, 40);
}


void Viewer::pushMatrix(QMatrix4x4 matrix) {
	mMatrixStack.push(mMatrixStack.top() * matrix);
}

void Viewer::popMatrix() {
	mMatrixStack.pop();
}

void Viewer::transformModel(QMatrix4x4 mat) {
	mTransformMatrix = mTransformMatrix * mat;
}

void Viewer::drawSphere(QMatrix4x4 transform) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * transform);
	
	set_colour(QColor(0.0, 0.0, 1.0));
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 19*9*6);
}

void Viewer::addTransform(QMatrix4x4 transform) {
	mTransforms.push_back(transform);
}

void Viewer::addObject(ObjectAttribute object) {
	for (std::vector<ObjectAttribute>::iterator it=mObjects.begin(); it != mObjects.end(); ++it) {
		if (*it == object) {
			object.setPicked(it->isPicked());
			object.setSelectable(it->isSelectable());
			mObjects.erase(it);
			mObjects.insert(it, object);
			return;
		}
	}
	mObjects.push_back(object);
}

void Viewer::resetPosition() {
	//mTransformMatrix = mTransformMatrix * mModelTranslateMatrix.inverted();
	mTranslateStack.clear();
	mOrderStack.clear();
	mTransformMatrix.setToIdentity();
	for (std::vector<QMatrix4x4>::iterator it=mRotateStack.begin(); it != mRotateStack.end(); ++it) {
		mTransformMatrix = mTransformMatrix * (*it);
		mOrderStack.push_back('r');
	}
	update();
}

void Viewer::resetOrientation() {
	mRotateStack.clear();
	mOrderStack.clear();
	mTransformMatrix.setToIdentity();
	for (std::vector<QMatrix4x4>::iterator it=mTranslateStack.begin(); it != mTranslateStack.end(); ++it) {
		mTransformMatrix = mTransformMatrix * (*it);
		mOrderStack.push_back('t');
	}
	update();
}

void Viewer::resetAll() {
	mRotateStack.clear();
	mTranslateStack.clear();
	mOrderStack.clear();
	mTransformMatrix.setToIdentity();
}

void Viewer::setToPositionOrientation() {
	mMode = 'p';
}

void Viewer::setToJoints() {
	mMode = 'j';
}

void Viewer::undo() {
	if (mOrderStack.empty()) {
		return;
	}
	char lastAction = mOrderStack.back();
	mOrderStack.pop_back();
	
	QMatrix4x4 lastTransform;
	if (lastAction == 'r') {
		lastTransform = mRotateStack.back();
		mTransformMatrix = mTransformMatrix * lastTransform.inverted();
		mRotateStack.pop_back();
	} else if (lastAction == 't') {
		lastTransform = mTranslateStack.back();
		mTransformMatrix = mTransformMatrix * lastTransform.inverted();
		mTranslateStack.pop_back();
	}
	update();
}

void Viewer::redo() {
	if (mOrderStack.empty()) {
		return;
	}
	char lastAction = mOrderStack.back();
	
	QMatrix4x4 lastTransform;
	if (lastAction == 'r') {
		lastTransform = mRotateStack.back();
		mRotateStack.push_back(lastTransform);
		mOrderStack.push_back('r');
		mTransformMatrix = mTransformMatrix * lastTransform;
	} else if (lastAction == 't') {
		lastTransform = mTranslateStack.back();
		mTranslateStack.push_back(lastTransform);
		mOrderStack.push_back('t');
		mTransformMatrix = mTransformMatrix * lastTransform.inverted();
	}
	update();
}

void Viewer::setToCircle() {
	mShowCircle = !mShowCircle;
	update();
}

void Viewer::setToZBuffer() {
	mZBuffer = !mZBuffer;
	update();
}

void Viewer::setToBackfaceCull() {
	mBackfaceCulling = !mBackfaceCulling;
	update();
}

void Viewer::setToFrontfaceCull() {
	mFrontfaceCulling = !mFrontfaceCulling;
	update();
}
