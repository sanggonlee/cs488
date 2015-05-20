// #include <GL/glew.h>
#include <QtWidgets>
#include <QtOpenGL>
#include <QVector3D>
#include "Viewer.hpp"
#include <iostream>
#include <math.h>
// #include <GL/gl.h>
//#include <GL/glu.h>

#include "game.hpp"


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

#define MAXIMUM_SCALE_FACTOR 2.0
#define MINIMUM_SCALE_FACTOR 0.3

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

Viewer::Viewer(const QGLFormat& format, QWidget *parent)
    : QGLWidget(format, parent)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    , mVertexBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
#else
    , mVertexBufferObject(QGLBuffer::VertexBuffer)
#endif
	, mUnitLeng(0.4f)
	, mGlobalScaleFactor(1.0f)
	, mWireFrame(false)
	, mMultiColoured(false)
	, mColourMapper( {
		QColor(Qt::black),
		QColor(Qt::white),
		QColor(Qt::blue),
		QColor(Qt::red),
		QColor(Qt::cyan),
		QColor(Qt::yellow),
		QColor(Qt::green),
		QColor(Qt::magenta),
		QColor(Qt::gray),	// gray is only used for the well
	  })
	, mGame(new Game(BOARD_WIDTH,BOARD_HEIGHT))
	, mWasMouseMoving(false)
{
    mTimer = new QTimer(this);
    mPieceFallTimer = new QTimer(this);
    mPersistenceTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
    mTimer->start(1000/20);

    mModelMatrices[1].rotate(30, QVector3D(1,1,1));
    mModelMatrices[2].rotate(60, QVector3D(1,1,1));
    mModelMatrices[3].rotate(90, QVector3D(1,1,1));
    
    // set up translation matrices for the well
    mUShapedWellMatrices[0].translate(-5,10,0);
    for (int i=0; i<BOARD_HEIGHT; i++) {
    	mUShapedWellMatrices[i+1].translate(0,-2*mUnitLeng,0);
    	mUShapedWellMatrices[i+1] *= mUShapedWellMatrices[i];
    }
    for (int j=BOARD_HEIGHT; j<BOARD_HEIGHT+BOARD_WIDTH+1; j++) {
    	mUShapedWellMatrices[j+1].translate(2*mUnitLeng,0,0);
    	mUShapedWellMatrices[j+1] *= mUShapedWellMatrices[j];
    }
    for (int k=BOARD_HEIGHT+BOARD_WIDTH+1; k<2*BOARD_HEIGHT+BOARD_WIDTH+1; k++) {
    	mUShapedWellMatrices[k+1].translate(0,2*mUnitLeng,0);
    	mUShapedWellMatrices[k+1] *= mUShapedWellMatrices[k];
    }
    //mUShapedWellMatrices[51].translate(5*mUnitLeng,10*mUnitLeng,0);
    
    // set up translation matrices for the board
    for (int bRowIndex=0; bRowIndex<BOARD_HEIGHT; bRowIndex++) {
    	mBoardMatrices[bRowIndex*10].translate(2*mUnitLeng,0,0);
    	// borrow position from the well
    	mBoardMatrices[bRowIndex*10] *= mUShapedWellMatrices[BOARD_HEIGHT-bRowIndex-1];
    	for (int bColIndex=0; bColIndex<BOARD_WIDTH-1; bColIndex++) {
    		mBoardMatrices[bRowIndex*10+bColIndex+1].translate(2*mUnitLeng,0,0);
    		mBoardMatrices[bRowIndex*10+bColIndex+1] *= mBoardMatrices[bRowIndex*10+bColIndex];
    	}
    }

    connect(mPieceFallTimer, SIGNAL(timeout()), this, SLOT(tick()));
    connect(mPersistenceTimer, SIGNAL(timeout()), this, SLOT(rotateForPersistence()));
}

Viewer::~Viewer() {

}

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(300, 600);
}

void Viewer::initUnitCube() {
    float unitCubeData[] = {
    	/*
    	 *	The below data are triangles data used to support "fill" faces
    	 */
		-mUnitLeng, -mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 mUnitLeng, -mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 mUnitLeng,  mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 mUnitLeng,  mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-mUnitLeng,  mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-mUnitLeng, -mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-mUnitLeng, -mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 mUnitLeng, -mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 mUnitLeng,  mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 mUnitLeng,  mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-mUnitLeng,  mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-mUnitLeng, -mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-mUnitLeng,  mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-mUnitLeng,  mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-mUnitLeng, -mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-mUnitLeng, -mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-mUnitLeng, -mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-mUnitLeng,  mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		 mUnitLeng,  mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 mUnitLeng,  mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 mUnitLeng, -mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 mUnitLeng, -mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 mUnitLeng, -mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 mUnitLeng,  mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		-mUnitLeng, -mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 mUnitLeng, -mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 mUnitLeng, -mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 mUnitLeng, -mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-mUnitLeng, -mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-mUnitLeng, -mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-mUnitLeng,  mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 mUnitLeng,  mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 mUnitLeng,  mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 mUnitLeng,  mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-mUnitLeng,  mUnitLeng,  mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-mUnitLeng,  mUnitLeng, -mUnitLeng, //1.0f, 1.0f, 1.0f, 0.0f, 1.0f

    	/*
    	 * The below data are lines data used to support "wire-frame" mode
    	 * 12 edges (lines)
    	 */
    
    	// "-" part of x-axis face
    	-mUnitLeng, -mUnitLeng, -mUnitLeng, // ---
    	-mUnitLeng, -mUnitLeng,  mUnitLeng, // --+
    	
    	-mUnitLeng, -mUnitLeng, -mUnitLeng, // ---
    	-mUnitLeng,  mUnitLeng, -mUnitLeng, // -+-
    	
    	-mUnitLeng, -mUnitLeng,  mUnitLeng, // --+
    	-mUnitLeng,  mUnitLeng,  mUnitLeng, // -++
    	
    	-mUnitLeng,  mUnitLeng, -mUnitLeng, // -+-
    	-mUnitLeng,  mUnitLeng,  mUnitLeng, // -++

		// "+" part of x-axis face
    	 mUnitLeng, -mUnitLeng, -mUnitLeng, // +--
    	 mUnitLeng,  mUnitLeng, -mUnitLeng, // ++-
		
		 mUnitLeng, -mUnitLeng, -mUnitLeng, // +--
    	 mUnitLeng, -mUnitLeng,  mUnitLeng, // +-+
    	 
    	 mUnitLeng, -mUnitLeng,  mUnitLeng, // +-+
    	 mUnitLeng,  mUnitLeng,  mUnitLeng, // +++
    	 
    	 mUnitLeng,  mUnitLeng, -mUnitLeng, // ++-
    	 mUnitLeng,  mUnitLeng,  mUnitLeng, // +++
    	 
    	// "-" part of y-axis face
    	-mUnitLeng, -mUnitLeng, -mUnitLeng, // ---
    	-mUnitLeng, -mUnitLeng,  mUnitLeng, // --+
    	
    	-mUnitLeng, -mUnitLeng, -mUnitLeng, // ---
    	 mUnitLeng, -mUnitLeng, -mUnitLeng, // +--

    	-mUnitLeng, -mUnitLeng,  mUnitLeng, // --+
    	 mUnitLeng, -mUnitLeng,  mUnitLeng, // +-+

    	 mUnitLeng, -mUnitLeng, -mUnitLeng, // +--
    	 mUnitLeng, -mUnitLeng,  mUnitLeng, // +-+

		// "+" part of y-axis face
    	-mUnitLeng,  mUnitLeng, -mUnitLeng, // -+-
    	 mUnitLeng,  mUnitLeng, -mUnitLeng, // ++-

    	-mUnitLeng,  mUnitLeng, -mUnitLeng, // -+-
    	-mUnitLeng,  mUnitLeng,  mUnitLeng, // -++

    	-mUnitLeng,  mUnitLeng,  mUnitLeng, // -++
    	 mUnitLeng,  mUnitLeng,  mUnitLeng, // +++

    	 mUnitLeng,  mUnitLeng, -mUnitLeng, // ++-
    	 mUnitLeng,  mUnitLeng,  mUnitLeng, // +++

		// "-" part of z-axis face
    	-mUnitLeng, -mUnitLeng, -mUnitLeng, // ---
    	-mUnitLeng,  mUnitLeng, -mUnitLeng, // -+-

    	-mUnitLeng, -mUnitLeng, -mUnitLeng, // ---
    	 mUnitLeng, -mUnitLeng, -mUnitLeng, // +--

    	-mUnitLeng,  mUnitLeng, -mUnitLeng, // -+-
    	 mUnitLeng,  mUnitLeng, -mUnitLeng, // ++-

    	 mUnitLeng, -mUnitLeng, -mUnitLeng, // +--
    	 mUnitLeng,  mUnitLeng, -mUnitLeng, // ++-

		// "+" part of z-axis face
    	-mUnitLeng, -mUnitLeng,  mUnitLeng, // --+
    	-mUnitLeng,  mUnitLeng,  mUnitLeng, // -++

    	-mUnitLeng, -mUnitLeng,  mUnitLeng, // --+
    	 mUnitLeng, -mUnitLeng,  mUnitLeng, // +-+

    	-mUnitLeng,  mUnitLeng,  mUnitLeng, // -++
    	 mUnitLeng,  mUnitLeng,  mUnitLeng, // +++

    	 mUnitLeng, -mUnitLeng,  mUnitLeng, // +-+
    	 mUnitLeng,  mUnitLeng,  mUnitLeng, // +++
    };
    
    mVertexBufferObject.allocate(unitCubeData, 3 * (36+48) * sizeof(float));
    //mVertexBufferObject.allocate(unitCubeLines, 3 * 48 * sizeof(float));
    
    mProgram.bind();
    
    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);
    
    // set color buffer
    mProgram.enableAttributeArray("color");
    mProgram.setAttributeBuffer("color", GL_FLOAT, 0, 1);
}

void Viewer::initializeGL() {
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glClearColor(0.7, 0.7, 1.0, 0.0);

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

    glEnable(GL_DEPTH_TEST);

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
    mVertexBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
#endif

    if (!mVertexBufferObject.bind()) {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }

    initUnitCube();
    
    // request focus to this widget so that it listens to key events
    setFocus();

    // mPerspMatrixLocation = mProgram.uniformLocation("cameraMatrix");
    mMvpMatrixLocation = mProgram.uniformLocation("mvpMatrix");
    mColorLocation = mProgram.uniformLocation("color");
}

void Viewer::drawUnitCube(int colourIndex, QMatrix4x4 transform=QMatrix4x4()) {
	mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * transform);
	if (colourIndex == -1) {
		// empty cell, draw nothing
		return;
	} else if (colourIndex == 8) {
		// 8 is gray, used to draw well
		mProgram.setUniformValue(mColorLocation, QColor(Qt::gray));
		if (!mWireFrame) {
			glDrawArrays(GL_TRIANGLES, 0, 36);
		} else {
			glDrawArrays(GL_LINES, 36, 48);
		}
	} else if (mMultiColoured) {
		// different colour for each face; which piece doesn't matter here
		for (int cIndex=0; cIndex<6; cIndex++) {
			mProgram.setUniformValue(mColorLocation, mColourMapper[cIndex]);
			if (!mWireFrame) {
				glDrawArrays(GL_TRIANGLES, 6 * cIndex, 6);
			} else {
				glDrawArrays(GL_LINES, 36 + 8 * cIndex, 8);
			}
		}
	} else {
		// use different colour for different pieces
		mProgram.setUniformValue(mColorLocation, mColourMapper[colourIndex]);
		if (!mWireFrame) {
			glDrawArrays(GL_TRIANGLES, 0, 36);
		} else {
			glDrawArrays(GL_LINES, 36, 48);
		}
	}
}

void Viewer::drawUShapedWell() {
	QColor color = QColor(Qt::gray);
	mProgram.setUniformValue(mColorLocation, color);
	for (int i=0; i<52; i++) {
		drawUnitCube(8, mUShapedWellMatrices[i]); // 8 for gray
	}
}

void Viewer::paintGL() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.bind();
#endif
/*
    for (int i = 0; i < 4; i++) {
		//qDebug() << i << ": " << mModelMatrices[i];
        mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * mModelMatrices[i]);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
*/
	drawUShapedWell();
	
	for (int rIndex=0; rIndex<BOARD_HEIGHT; rIndex++) {
		for (int cIndex=0; cIndex<BOARD_WIDTH; cIndex++) {
			//qDebug() << "cId=" << mGame->get(rowIndex, colIndex);
			drawUnitCube(mGame->get(rIndex, cIndex), 
				mBoardMatrices[rIndex*BOARD_WIDTH+cIndex]);
		}
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
    setFocus();
    mWasMouseMoving = false;
    mClickedButtons = event->buttons();
    mPersistenceTimer->stop();
    mLastXPos = event->x();
}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
    if (mWasMouseMoving) {
    	mPersistenceTimer->start(10);
    }
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    // handle scaling via shift + mouse buttons
    if (event->modifiers() & Qt::ShiftModifier) {
    	float scaleFactor = event->x() < mLastXPos ? 0.95 : 1.05;
    	// set minimum & maximum scale range
    	if (mGlobalScaleFactor * scaleFactor <= MAXIMUM_SCALE_FACTOR
    			&& mGlobalScaleFactor * scaleFactor >= MINIMUM_SCALE_FACTOR) {
			mGlobalScaleFactor *= scaleFactor;
			scaleWorld(scaleFactor, scaleFactor, scaleFactor);
    	}
    }
    // handle rotation via mouse buttons
    else if (event->buttons() & Qt::LeftButton) {
		rotateWorld(event->x() - mLastXPos, 1, 0, 0);
	} else if (event->buttons() & Qt::RightButton) {
		rotateWorld(event->x() - mLastXPos, 0, 1, 0);
	} else if (event->buttons() & Qt::MiddleButton) {
		rotateWorld(event->x() - mLastXPos, 0, 0, 1);
	}
	
	// for rotation persistence, detect if mouse has been moving
	mWasMouseMoving = (mLastXPos - event->x() > 1 || mLastXPos - event->x() < -1);
	
	mLastXPos = event->x();
}

void Viewer::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Left) {
		mGame->moveLeft();
	} else if (event->key() == Qt::Key_Right) {
		mGame->moveRight();
	} else if (event->key() == Qt::Key_Up) {
		mGame->rotateCCW();
	} else if (event->key() == Qt::Key_Down) {
		mGame->rotateCW();
	} else if (event->key() == Qt::Key_Space) {
		mGame->drop();
	}
}

QMatrix4x4 Viewer::getCameraMatrix() {
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(3.0, 3.0, 20.0);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void Viewer::translateWorld(float x, float y, float z) {
    mTransformMatrix.translate(x, y, z);
}

void Viewer::rotateWorld(float angle, float x, float y, float z) {
    mTransformMatrix.rotate(angle, x, y, z);
}

void Viewer::scaleWorld(float x, float y, float z) {
    mTransformMatrix.scale(x, y, z);
}

void Viewer::setToWireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mWireFrame = true;
	mMultiColoured = false;
}

void Viewer::fillFaces() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mWireFrame = false;
	mMultiColoured = false;
}

void Viewer::setToMulticoloured() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mWireFrame = false;
	mMultiColoured = true;
}

void Viewer::startNewGame() {
	mGame->reset();
}

void Viewer::resetView() {
	mPersistenceTimer->stop();
	mTransformMatrix.setToIdentity();
}

void Viewer::tick() {
	mGame->tick();
}

void Viewer::rotateForPersistence() {
	if (mClickedButtons & Qt::LeftButton) {
		rotateWorld(2, 1, 0, 0);
	} else if (mClickedButtons & Qt::RightButton) {
		rotateWorld(2, 0, 1, 0);
	} else if (mClickedButtons & Qt::MiddleButton) {
		rotateWorld(2, 0, 0, 1);
	}
}

void Viewer::setToSlow() {
	if (mPieceFallTimer->isActive()) {
		mPieceFallTimer->setInterval(1000/2);
	} else {
		mPieceFallTimer->start(1000/2);
	}
}

void Viewer::setToMedium() {
	if (mPieceFallTimer->isActive()) {
		mPieceFallTimer->setInterval(1000/5);
	} else {
		mPieceFallTimer->start(1000/5);
	}
}

void Viewer::setToFast() {
	if (mPieceFallTimer->isActive()) {
		mPieceFallTimer->setInterval(1000/10);
	} else {
		mPieceFallTimer->start(1000/10);
	}
}
