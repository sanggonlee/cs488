#include <QtWidgets>
#include <QtOpenGL>
#include "Viewer.hpp"
#include <iostream>
#include <math.h>
#include <GL/glut.h>


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
{
	mMatrixStack.push(QMatrix4x4());
}

Viewer::~Viewer() {
    // Nothing to do here right now.
}

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(300, 300);
}

void Viewer::initializeGL() {

    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glShadeModel(GL_SMOOTH);
    glClearColor( 0.7, 0.7, 1.0, 0.0 );
    glEnable(GL_DEPTH_TEST);
    
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

    float circleData[120];

    double radius = width() < height() ? 
        (float)width() * 0.25 : (float)height() * 0.25;
        
    for(size_t i=0; i<40; ++i) {
        circleData[i*3] = radius * cos(i*2*M_PI/40);
        circleData[i*3 + 1] = radius * sin(i*2*M_PI/40);
        circleData[i*3 + 2] = 0.0;
    }


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.create();
    mVertexArrayObject.bind();

//    mCircleBufferObject.create();
//    mCircleBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
    
    mSphereBufferObject.create();
    mSphereBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float theta = 0, phi = 0, deltaTheta = M_PI / 9, deltaPhi = M_PI / 9;
	//float z1, x1, y1, z2, x2, y2, z3, x3, y3, z4, x4, y4;
	float r = 1.0;
	int slice = 19, sec = 9;

	float sphereData[slice*sec*6*3];
	int i=0;
	for (theta = 0; theta <= 2*M_PI; theta += M_PI/9) {
		for (phi = 0; phi <= M_PI-0.001; phi += M_PI/9) {
			sphereData[i++] = r*sin(theta)*cos(phi);
			sphereData[i++] = r*sin(theta)*sin(phi);
			sphereData[i++] = r*cos(theta);
			
			sphereData[i++] = r*sin(theta + deltaTheta)*cos(phi);
			sphereData[i++] = r*sin(theta + deltaTheta)*sin(phi);
			sphereData[i++] = r*cos(theta + deltaTheta);
			
			sphereData[i++] = r*sin(theta)*cos(phi + deltaPhi);
			sphereData[i++] = r*sin(theta)*sin(phi + deltaPhi);
			sphereData[i++] = r*cos(theta);
			
			
			sphereData[i++] = r*sin(theta + deltaTheta)*cos(phi + deltaPhi);
			sphereData[i++] = r*sin(theta + deltaTheta)*sin(phi + deltaPhi);
			sphereData[i++] = r*cos(theta + deltaTheta);
			
			sphereData[i++] = r*sin(theta + deltaTheta)*cos(phi);
			sphereData[i++] = r*sin(theta + deltaTheta)*sin(phi);
			sphereData[i++] = r*cos(theta + deltaTheta);
			
			sphereData[i++] = r*sin(theta)*cos(phi + deltaPhi);
			sphereData[i++] = r*sin(theta)*sin(phi + deltaPhi);
			sphereData[i++] = r*cos(theta);
		}
	}
	mSphereBufferObject.allocate(sphereData, 
								slice*sec*6*3
								* sizeof(float));
 
    
//    mCircleBufferObject.allocate(circleData, 40 * 3 * sizeof(float));

    mProgram.bind();

    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    // set color buffer
    mProgram.enableAttributeArray("color");
    mProgram.setAttributeBuffer("color", GL_FLOAT, 0, 1);

    mMvpMatrixLocation = mProgram.uniformLocation("mvpMatrix");
    mColorLocation = mProgram.uniformLocation("frag_color");
}


void Viewer::paintGL() {
	qDebug() << "[Viewer::paintGL] enter, transformMatrix="<<mTransformMatrix;
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up lighting

    // Draw stuff

//    draw_trackball_circle();

	for (std::vector<QMatrix4x4>::iterator it=mTransforms.begin(); it != mTransforms.end(); ++it) {
		mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * (*it));
		set_colour(QColor(0.0, 0.0, 1.0));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 19*9*6);
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
    std::cerr << "Stub: button " << event->button() << " pressed\n";
}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
    std::cerr << "Stub: button " << event->button() << " released\n";
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    std::cerr << "Stub: Motion at " << event->x() << ", " << event->y() << std::endl;
    if (event->buttons() & Qt::LeftButton) {
		//rotateWorld(0, 0, 2);
		//translateWorld(0.1,0.1,0.1);
		rotateWorld(2, 1, 0, 0);
	} else if ( event->buttons() & Qt::RightButton) {
		scaleWorld(1.1, 1.1, 1.1);
	}
	update();
}

QMatrix4x4 Viewer::getCameraMatrix() {
    // Todo: Ask if we want to keep this.
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0.0, 0.0, 20.0);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void Viewer::translateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.translate(x, y, z);
}

void Viewer::rotateWorld(float angle, float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.rotate(angle, x, y, z);
}

void Viewer::scaleWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.scale(x, y, z);
}

void Viewer::set_colour(const QColor& col)
{
  mProgram.setUniformValue(mColorLocation, col.red(), col.green(), col.blue());
}

void Viewer::draw_trackball_circle()
{
    int current_width = width();
    int current_height = height();
    std::cout << "w:"<<width()<<", h:"<<height()<<std::endl;

    // Set up for orthogonal drawing to draw a circle on screen.
    // You'll want to make the rest of the function conditional on
    // whether or not we want to draw the circle this time around.

    set_colour(QColor(0.0, 0.0, 0.0));

    // Set orthographic Matrix
    QMatrix4x4 orthoMatrix;
    orthoMatrix.ortho(0.0, (float)current_width, 
           0.0, (float)current_height, -0.1, 0.1);

    // Translate the view to the middle
    QMatrix4x4 transformMatrix;
    transformMatrix.translate(width()/2.0, height()/2.0, 0.0);

    // Bind buffer object
    mCircleBufferObject.bind();
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
/*
QMatrix4x4 topMatrix() {
	return mMatrixStack.top();
}*/

void Viewer::transformModel(QMatrix4x4 mat) {
	mTransformMatrix = mTransformMatrix * mat;
}

void Viewer::drawSphere(QMatrix4x4 transform) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * transform);
	
	//mProgram.setUniformValue(mColorLocation, QColor(Qt::blue));
	set_colour(QColor(0.0, 0.0, 1.0));
	
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 18*9*6);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 19*9*6);
}

void Viewer::addTransform(QMatrix4x4 transform) {
	mTransforms.push_back(transform);
}
