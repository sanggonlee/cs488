#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif

#include "game.hpp"

class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.
    
    Q_SLOT void setToWireframe();
    Q_SLOT void fillFaces();
    Q_SLOT void setToMulticoloured();
    
    Q_SLOT void startNewGame();
    Q_SLOT void resetView();
    
    Q_SLOT void tick();
    
    Q_SLOT void setToSlow();
    Q_SLOT void setToMedium();
    Q_SLOT void setToFast();
  
protected:

    // Events we implement

    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when the window is resized (formerly on_configure_event)
    virtual void resizeGL(int width, int height);
    // Called when a mouse button is pressed
    virtual void mousePressEvent ( QMouseEvent * event );
    // Called when a mouse button is released
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    // Called when the mouse moves
    virtual void mouseMoveEvent ( QMouseEvent * event );
    
    virtual void keyPressEvent(QKeyEvent *event);

private:

    QMatrix4x4 getCameraMatrix();
    void translateWorld(float x, float y, float z);
    void rotateWorld(float angle, float x, float y, float z);
    void scaleWorld(float x, float y, float z);

    void initUnitCube();
    void drawUnitCube(int colourIndex, QMatrix4x4 transform);
    void drawUShapedWell();
    Q_SLOT void rotateForPersistence();


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mVertexBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mVertexBufferObject;
#endif

    int mVertexLocation;
    int mMvpMatrixLocation;
    int mColorLocation;
    
    float mUnitLeng; // unit length for cubes
    
    float mLastXPos;
    float mGlobalScaleFactor;
    
	const QColor mColourMapper[9];
	
	bool mWireFrame;
	bool mMultiColoured;
	bool mWasMouseMoving;
	
	Qt::MouseButtons mClickedButtons;

    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mModelMatrices[4];

    QMatrix4x4 mUShapedWellMatrices[52]; // 21 for left and right, 10 for bottom
    QMatrix4x4 mBoardMatrices[200];
    QMatrix4x4 mTransformMatrix;

    QTimer* mTimer;
    QTimer* mPersistenceTimer;
    QTimer* mPieceFallTimer;
    QGLShaderProgram mProgram;
    Game* mGame;
};

#endif
