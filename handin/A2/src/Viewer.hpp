#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>
#include "algebra.hpp"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif

class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
	enum TransformMode {
		VIEW_ROTATE,
		VIEW_TRANSLATE,
		VIEW_PERSPECTIVE,
		MODEL_ROTATE,
		MODEL_TRANSLATE,
		MODEL_SCALE,
		VIEWPORT
	};
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.

    // *** Fill in these functions (in viewer.cpp) ***

    // Set the parameters of the current perspective projection using
    // the semantics of gluPerspective().
    void set_perspective(double fov, double aspect,
                         double near, double far);

    // Restore all the transforms and perspective parameters to their
    // original state. Set the viewport to its initial size.
    Q_SLOT void reset_view();
    
    Q_SLOT void setToViewRotate();
    Q_SLOT void setToViewTranslate();
    Q_SLOT void setToViewPerspective();
    
    Q_SLOT void setToModelRotate();
    Q_SLOT void setToModelTranslate();
    Q_SLOT void setToModelScale();
    
    Q_SLOT void setToViewport();

signals:
	void statusChanged(QString msg);

protected:

    // Events we implement

    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when a mouse button is pressed
    virtual void mousePressEvent ( QMouseEvent * event );
    // Called when a mouse button is released
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    // Called when the mouse moves
    virtual void mouseMoveEvent ( QMouseEvent * event );
 
    // Draw a line -- call draw_init first!
    void draw_line(const QVector2D& p1, const QVector2D& p2) ;

    // Set the current colour
    void set_colour(const QColor& col);

    // Call this before you begin drawing. Width and height are the width
    // and height of the GL window.
    void draw_init();
private:

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mVertexBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mVertexBufferObject;
#endif
	void clipAndDraw(QVector2D a, QVector2D b);
	void updateStatus();
	void drawModelGnomon();
	void drawWorldGnomon();
	void drawCube();
	
	Vector4D mModelGnomonVertices[4];
	Vector4D mWorldGnomonVertices[4];
    Vector4D mCubeVertices[8];
    QVector2D mVerticesIn2D[8];
    
    Vector3D mEyePoint;
    
    TransformMode mMode;

	double mOldX;
	double mFov, mNear, mFar;
	double mViewportStartX, mViewportStartY;
	double mCurrentX, mCurrentY;
	bool mChangingViewport;
	double mVpFrameTop, mVpFrameBot, mVpFrameLeft, mVpFrameRight;

    QGLShaderProgram mProgram;

    int mColorLocation;
    
    Matrix4x4 m_projection;
    // mModelMatrix only maintains model rotations and translations, not scales
    Matrix4x4 mModelMatrix;
    // Maintain a separate matrix for model scale to handle unscaling model gnomon
    Matrix4x4 mScaleMatrix;
    Matrix4x4 mViewMatrix;
    Matrix4x4 mViewportMatrix;
};

#endif
