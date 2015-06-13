#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>
#include <stack>
#include "material.hpp"
#include "ObjectAttribute.hpp"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif

class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();/*
    struct ObjectAttribute {
    	QMatrix4x4 transform;
    	Material *material;
    	
    	ObjectAttribute(QMatrix4x4 t, Material *m):
    		transform(t)
    	,	material(m){}
    };*/
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.
    void drawSphere(QMatrix4x4 transform);
    void transformModel(QMatrix4x4 mat);
    void pushMatrix(QMatrix4x4 matrix);
    void popMatrix();
    void addTransform(QMatrix4x4 transform);
    void addObject(ObjectAttribute object);
  
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
    
    // Draw a circle for the trackball, with OpenGL commands.
    // Assumes the context for the viewer is active.
    void draw_trackball_circle();
    
    //void drawSphere();


private:

    QMatrix4x4 getCameraMatrix();
    void translateWorld(float x, float y, float z);
    void rotateWorld(float angle, float x, float y, float z);
    void scaleWorld(float x, float y, float z);
    void set_colour(const QColor& col);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mCircleBufferObject;
    QOpenGLBuffer mSphereBufferObject;
    QOpenGLBuffer mSphereNormalBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mCircleBufferObject;
    QGLBuffer mSphereBufferObject;
    QGLBuffer mSphereNormalBufferObject;
#endif
    
    int mMvpMatrixLocation;
    int mColorLocation;
    
    int mVmMatrixLocation;
    int m3x3InvTranspLocation;

    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mTransformMatrix;
    QMatrix4x4 mViewMatrix;
    QGLShaderProgram mProgram;
    
    std::stack<QMatrix4x4> mMatrixStack;

    std::vector<QMatrix4x4> mTransforms;
    std::vector<ObjectAttribute> mObjects;
};

#endif
