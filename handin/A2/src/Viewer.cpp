#include <QtWidgets>
#include <QtOpenGL>
#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Viewer.hpp"
// #include "draw.hpp"
#include "algebra.hpp"
#include "a2.cpp"
#include <stdlib.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

using namespace std;

Viewer::Viewer(const QGLFormat& format, QWidget *parent) 
    : QGLWidget(format, parent) 
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    , mVertexBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
#else 
    , mVertexBufferObject(QGLBuffer::VertexBuffer)
#endif
	, mModelGnomonVertices{
		Vector4D(0.0, 0.0, 0.0, 1.0),
		Vector4D(0.5, 0.0, 0.0, 1.0),
		Vector4D(0.0,-0.5, 0.0, 1.0),
		Vector4D(0.0, 0.0, 0.5, 1.0),
	}
	, mWorldGnomonVertices{
		Vector4D(0.0, 0.0, 0.0, 1.0),
		Vector4D(0.5, 0.0, 0.0, 1.0),
		Vector4D(0.0,-0.5, 0.0, 1.0),
		Vector4D(0.0, 0.0, 0.5, 1.0),
	}
    , mCubeVertices{
    	Vector4D(-1.0, -1.0, -1.0, 1.0),
    	Vector4D(-1.0, -1.0,  1.0, 1.0),
    	Vector4D(-1.0,  1.0, -1.0, 1.0),
    	Vector4D(-1.0,  1.0,  1.0, 1.0),
    	Vector4D( 1.0, -1.0, -1.0, 1.0),
    	Vector4D( 1.0, -1.0,  1.0, 1.0),
    	Vector4D( 1.0,  1.0, -1.0, 1.0),
    	Vector4D( 1.0,  1.0,  1.0, 1.0),
    }
    , mEyePoint(Vector3D(0.0, 0.0, 4.0))
    , mMode(MODEL_ROTATE)
    , mFov(60.0* M_PI/180.0)
    , mNear(0.5)
    , mFar(2.0)
    , mChangingViewport(false)
    , mVpFrameTop(0.9)
    , mVpFrameBot(-0.9)
    , mVpFrameLeft(-0.9)
    , mVpFrameRight(0.9)
{
	mModelMatrix = Matrix4x4();
	mScaleMatrix = Matrix4x4();
	
	mViewMatrix = getViewMatrix(mEyePoint,
								Vector3D(0.0, 0.0, 0.0),
								Vector3D(0.0, 1.0, 0.0));
	mViewportMatrix = Matrix4x4();
}

Viewer::~Viewer() {
    // Nothing to do here right now.
}

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(400, 400);
}

void Viewer::set_perspective(double fov, double aspect,
                             double near, double far)
{
    double cotVal = 1.0/tan(fov/2.0);
    
    m_projection = Matrix4x4(
    	cotVal*aspect,	0.0,		0.0,					0.0,
    	0.0,			cotVal,		0.0,					0.0,
    	0.0,			0.0,		(far+near)/(far-near),  1.0,
    	0.0,			0.0,	   -2.0*near*far/(far-near),0.0
    );
}

void Viewer::reset_view()
{
	mFov = 60.0*M_PI/180.0;
	mNear = 0.5;
	mFar = 2.0;
	mViewMatrix = getViewMatrix(mEyePoint,
								Vector3D(0.0, 0.0, 0.0),
								Vector3D(0.0, 1.0, 0.0));
    mModelMatrix = Matrix4x4();
    update();
    updateStatus();
}

void Viewer::initializeGL() {
    // Do some OpenGL setup
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glClearColor(0.7, 0.7, 0.7, 0.0);
    
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

    mVertexBufferObject.create();
    mVertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
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

    mVertexBufferObject.create();
    mVertexBufferObject.setUsagePattern(QGLBuffer::DynamicDraw);
#endif

    if (!mVertexBufferObject.bind()) {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }

    mProgram.bind();

    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mColorLocation = mProgram.uniformLocation("frag_color");
    
    updateStatus();
}

void Viewer::paintGL() {    
    draw_init();
    
    set_perspective(mFov, (double)width()/(double)height(), mNear, mFar);

    // Here is where your drawing code should go.
    set_colour(QColor(1.0, 1.0, 1.0));
	drawCube();
	
	set_colour(QColor(0.0, 1.0, 0.0));
	drawModelGnomon();
	
	set_colour(QColor(1.0, 0.0, 0.0));
	drawWorldGnomon();
	
	// Draw the viewport frame
	set_colour(QColor(0.0, 0.0, 0.0));
	draw_line(QVector2D(mVpFrameLeft, mVpFrameBot), QVector2D(mVpFrameLeft, mVpFrameTop));
	draw_line(QVector2D(mVpFrameLeft, mVpFrameBot), QVector2D(mVpFrameRight, mVpFrameBot));
	draw_line(QVector2D(mVpFrameRight, mVpFrameTop), QVector2D(mVpFrameLeft, mVpFrameTop));
	draw_line(QVector2D(mVpFrameRight, mVpFrameTop), QVector2D(mVpFrameRight,  mVpFrameBot));
}

void Viewer::drawModelGnomon() {
	QVector2D transModelGnomonVertices[4];
	for (int vIndex=0; vIndex<4; vIndex++) {
		Vector4D transModelGnomonVertex = 
			mViewportMatrix * m_projection * mViewMatrix * mModelMatrix * mModelGnomonVertices[vIndex];
		transModelGnomonVertices[vIndex] = 
			QVector2D(transModelGnomonVertex[0]/transModelGnomonVertex[3],
					  transModelGnomonVertex[1]/transModelGnomonVertex[3]);
	}
	clipAndDraw(transModelGnomonVertices[0], transModelGnomonVertices[1]);
	clipAndDraw(transModelGnomonVertices[0], transModelGnomonVertices[2]);
	clipAndDraw(transModelGnomonVertices[0], transModelGnomonVertices[3]);
}

void Viewer::drawWorldGnomon() {
	QVector2D transWorldGnomonVertices[4];
	for (int vIndex=0; vIndex<4; vIndex++) {
		Vector4D transWorldGnomonVertex = 
			mViewportMatrix * m_projection * mViewMatrix * mWorldGnomonVertices[vIndex];
		transWorldGnomonVertices[vIndex] = 
			QVector2D(transWorldGnomonVertex[0]/transWorldGnomonVertex[3],
					  transWorldGnomonVertex[1]/transWorldGnomonVertex[3]);
	}
	clipAndDraw(transWorldGnomonVertices[0], transWorldGnomonVertices[1]);
	clipAndDraw(transWorldGnomonVertices[0], transWorldGnomonVertices[2]);
	clipAndDraw(transWorldGnomonVertices[0], transWorldGnomonVertices[3]);
}

void Viewer::drawCube() {
	for (int vIndex=0; vIndex<8; vIndex++) {
		Vector4D transformedVertex = mViewportMatrix * m_projection * mViewMatrix * (mModelMatrix * mScaleMatrix) * mCubeVertices[vIndex];

		transformedVertex[0] = transformedVertex[0]/transformedVertex[3];
		transformedVertex[1] = transformedVertex[1]/transformedVertex[3];
		transformedVertex[2] = transformedVertex[2]/transformedVertex[3];
		
		mVerticesIn2D[vIndex] = QVector2D(
			transformedVertex[0],
			transformedVertex[1]
		);
	}

	// Actually draw lines from cube vertices to cube vertices here
	clipAndDraw(mVerticesIn2D[0], mVerticesIn2D[1]);
	clipAndDraw(mVerticesIn2D[0], mVerticesIn2D[2]);
	clipAndDraw(mVerticesIn2D[0], mVerticesIn2D[4]);
	clipAndDraw(mVerticesIn2D[1], mVerticesIn2D[3]);
	clipAndDraw(mVerticesIn2D[1], mVerticesIn2D[5]);
	clipAndDraw(mVerticesIn2D[2], mVerticesIn2D[3]);
	clipAndDraw(mVerticesIn2D[2], mVerticesIn2D[6]);
	clipAndDraw(mVerticesIn2D[3], mVerticesIn2D[7]);
	clipAndDraw(mVerticesIn2D[4], mVerticesIn2D[5]);
	clipAndDraw(mVerticesIn2D[4], mVerticesIn2D[6]);
	clipAndDraw(mVerticesIn2D[5], mVerticesIn2D[7]);
	clipAndDraw(mVerticesIn2D[6], mVerticesIn2D[7]);
}

void Viewer::clipAndDraw(QVector2D a, QVector2D b) {
	QVector2D P[4] = {QVector2D(0.0, mVpFrameTop),
					  QVector2D(mVpFrameLeft, 0.0), 
					  QVector2D(mVpFrameRight, 0.0),
					  QVector2D(0.0, mVpFrameBot)};
	QVector2D n[4] = {QVector2D(0.0, -1.0),
					  QVector2D(1.0, 0.0),
					  QVector2D(-1.0, 0.0),
					  QVector2D(0.0, 1.0)};
	
	for (int i=0; i<4; i++) {
		double wecA = QVector2D::dotProduct(a-P[i], n[i]);
		double wecB = QVector2D::dotProduct(b-P[i], n[i]);
		if (wecA < 0.0 && wecB < 0.0) {
			return;
		} else if (wecA >= 0.0 && wecB >= 0.0) {
			continue;
		}
		double t = wecA / (wecA - wecB);
		
		if (wecA < 0) {
			a = a + t*(b-a);
			break;
		} else {
			b = a + t*(b-a);
			break;
		}
	}
	
	draw_line(a, b);
}

void Viewer::mousePressEvent ( QMouseEvent * event ) {
    if (mMode == VIEWPORT) {
    	mChangingViewport = true;
    	mViewportStartX = event->x() - (double)width()/2;
    	mViewportStartY = event->y() - (double)height()/2;
    }
    
    mOldX = event->x();
}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
	(void)event;
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    
    mCurrentX = event->x() - (double)width()/2;
    mCurrentY = event->y() - (double)height()/2;
    
	double w = mCurrentX - mViewportStartX;
	double h = mCurrentY - mViewportStartY;
    
    switch(mMode) {
    	case VIEW_ROTATE:
    		if (event->buttons() & Qt::LeftButton) {
    			mViewMatrix = getRotationMatrix((event->x()-mOldX)/width(), 'x') * mViewMatrix;
    		}
    		if (event->buttons() & Qt::MiddleButton) {
    			mViewMatrix = getRotationMatrix((event->x()-mOldX)/height(), 'y') * mViewMatrix;
    		}
    		if (event->buttons() & Qt::RightButton) {
    			mViewMatrix = getRotationMatrix((event->x()-mOldX)/width(), 'z') * mViewMatrix;
    		}
    		break;
    	case VIEW_TRANSLATE:
    		if (event->buttons() & Qt::LeftButton) {
    			mViewMatrix = getTranslationMatrix((event->x()-mOldX)/width(), 'x') * mViewMatrix;
    		}
    		if (event->buttons() & Qt::MiddleButton) {
    			mViewMatrix = getTranslationMatrix((event->x()-mOldX)/height(), 'y') * mViewMatrix;
    		}
    		if (event->buttons() & Qt::RightButton) {
    			mViewMatrix = getTranslationMatrix((event->x()-mOldX)/width(), 'z') * mViewMatrix;
    		}
    		break;
    	case VIEW_PERSPECTIVE:
    		if (event->buttons() & Qt::LeftButton) {
    			mFov += (event->x()-mOldX)/60;
    			if (mFov < 5.0*M_PI/180.0) {
    				mFov = 5.0*M_PI/180.0;
    			}
    			if (mFov > 160.0*M_PI/180.0) {
    				mFov = 160.0*M_PI/180.0;
    			}
    		}
    		if(event->buttons() & Qt::MiddleButton) {
    			mNear += (event->x()-mOldX)/10;
    			updateStatus();
    		}
    		if(event->buttons() & Qt::RightButton) {
    			mFar += (event->x()-mOldX)/10;
    			updateStatus();
    		}
    		break;
    	case MODEL_ROTATE:
    		if (event->buttons() & Qt::LeftButton) {
    			mModelMatrix = mModelMatrix * getRotationMatrix((event->x()-mOldX)/10, 'x');
    		}
    		if (event->buttons() & Qt::MiddleButton) {
    			mModelMatrix = mModelMatrix * getRotationMatrix((event->x()-mOldX)/10, 'y');
    		}
    		if (event->buttons() & Qt::RightButton) {
    			mModelMatrix = mModelMatrix * getRotationMatrix((event->x()-mOldX)/10, 'z');
    		}
    		break;
    	case MODEL_TRANSLATE:
    		if (event->buttons() & Qt::LeftButton) {
    			mModelMatrix = mModelMatrix * getTranslationMatrix((event->x()-mOldX)/10, 'x');
    		}
    		if (event->buttons() & Qt::MiddleButton) {
    			mModelMatrix = mModelMatrix * getTranslationMatrix((event->x()-mOldX)/10, 'y');
    		}
    		if (event->buttons() & Qt::RightButton) {
    			mModelMatrix = mModelMatrix * getTranslationMatrix((event->x()-mOldX)/10, 'z');
    		}
    		break;
    	case MODEL_SCALE:
    		if (event->buttons() & Qt::LeftButton) {
    			mScaleMatrix = mScaleMatrix * getScaleMatrix((event->x()-mOldX)/10, 'x');
    		}
    		if (event->buttons() & Qt::MiddleButton) {
    			mScaleMatrix = mScaleMatrix * getScaleMatrix((event->x()-mOldX)/10, 'y');
    		}
    		if (event->buttons() & Qt::RightButton) {
    			mScaleMatrix = mScaleMatrix * getScaleMatrix((event->x()-mOldX)/10, 'z');
    		}
    		break;
    	case VIEWPORT:
    		mVpFrameLeft = mViewportStartX/(width()/2);
    		mVpFrameRight = mCurrentX/(width()/2);
    		mVpFrameBot = -mCurrentY/(height()/2);
    		mVpFrameTop = -mViewportStartY/(height()/2);
    		
			glViewport(mViewportStartX, mViewportStartY, w, h);
    		mViewportMatrix = Matrix4x4(
    			w/(double)width(), 	0.0, 	0.0,	(w+mViewportStartX*2)/(double)width(),
				0.0,	h/(double)height(), 0.0,   -(h+mViewportStartY*2)/(double)height(),
				0.0, 		 0.0, 		1.0, 	0.0,
				0.0, 	 	0.0, 		0.0, 	1.0);
			break;
    	default:
    		qWarning() << "Undefined transform mode";
    		break;
    }
    
    mOldX = event->x();
    update();
}

void Viewer::updateStatus() {
	QString mode;
	switch(mMode) {
		case VIEW_ROTATE:
			mode = "View Rotate";
			break;
		case VIEW_TRANSLATE:
			mode = "View Translate";
			break;
		case VIEW_PERSPECTIVE:
			mode = "View Perspective";
			break;
		case MODEL_ROTATE:
			mode = "Model Rotate";
			break;
		case MODEL_TRANSLATE:
			mode = "Model Translate";
			break;
		case MODEL_SCALE:
			mode = "Model Scale";
			break;
		case VIEWPORT:
			mode = "Viewport";
			break;
	}
	emit statusChanged(QString("Mode:"+mode
							+", Near:"+QString::number(mNear)
							+", Far:"+QString::number(mFar)));
}

void Viewer::setToViewRotate() {
	mMode = VIEW_ROTATE;
	updateStatus();
}

void Viewer::setToViewTranslate() {
	mMode = VIEW_TRANSLATE;
	updateStatus();
}

void Viewer::setToViewPerspective() {
	mMode = VIEW_PERSPECTIVE;
	updateStatus();
}

void Viewer::setToModelRotate() {
	mMode = MODEL_ROTATE;
	updateStatus();
}

void Viewer::setToModelTranslate() {
	mMode = MODEL_TRANSLATE;
	updateStatus();
}

void Viewer::setToModelScale() {
	mMode = MODEL_SCALE;
	updateStatus();
}

void Viewer::setToViewport() {
	mMode = VIEWPORT;
	updateStatus();
}

// Drawing Functions
// ******************* Do Not Touch ************************ // 

void Viewer::draw_line(const QVector2D& p1, const QVector2D& p2) {

  const GLfloat lineVertices[] = {
    p1.x(), p1.y(), 1.0,
    p2.x(), p2.y(), 1.0
  };

  mVertexBufferObject.allocate(lineVertices, 3 * 2 * sizeof(float));

  glDrawArrays(GL_LINES, 0, 2);
}

void Viewer::set_colour(const QColor& col)
{
  mProgram.setUniformValue(mColorLocation, col.red(), col.green(), col.blue());
}

void Viewer::draw_init() {
    glClearColor(0.7, 0.7, 0.7, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width(), 0.0, (double)height());
    glViewport(0, 0, width(), height());

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.0);
}
