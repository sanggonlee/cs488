#ifndef PAINTCANVAS_HPP
#define PAINTCANVAS_HPP

#include <list>
#include <QWidget>
#include <QPainter>
//#include <QColor>
#include "algebra.hpp"

// An abstract base class that defines shapes we'll store and
// draw.  Each shape is currently defined from two corners m_from 
// and m_to, and a method that asks the shape to draw itself.
// 
class PaintShape
{
public:
    virtual ~PaintShape() {}

    virtual void draw( QPainter& painter ) = 0;
    void setColour(QColor colour) { m_colour = colour; }
    QColor getColour() { return m_colour; }

protected:
    PaintShape( const Point2D& from, const Point2D& to )
        : m_from( from )
        , m_to( to )
        , m_colour( Qt::GlobalColor::black )
    {}

protected:
    Point2D     m_from;
    Point2D     m_to;
    
    QColor		m_colour;
};

class PaintLine
    : public PaintShape
{
public:
    PaintLine( const Point2D& from, const Point2D& to )
        : PaintShape( from, to )
    {}

    virtual void draw( QPainter& painter );
};

class PaintRect
    : public PaintShape
{
public:
    PaintRect( const Point2D& from, const Point2D& to )
        : PaintShape( from, to )
    {}

    virtual void draw( QPainter& painter );
};

class PaintOval
    : public PaintShape
{
public:
    PaintOval( const Point2D& from, const Point2D& to )
        : PaintShape( from, to )
    {}

    virtual void draw( QPainter& painter );
};

class PaintCanvas : public QWidget {

    Q_OBJECT

public:
    enum Mode {
    DRAW_LINE,
    DRAW_OVAL,
    DRAW_RECTANGLE
    };

    PaintCanvas(QWidget *parent = 0);
    virtual ~PaintCanvas();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void set_mode(Mode mode) { m_mode = mode; }
    void setColour(QColor colour) { m_colour = colour; }

	Q_SLOT void clearCanvas();
	
protected:

    // Events we implement

    // Called when our window needs to be redrawn
    virtual void paintEvent( QPaintEvent * event );
    // Called when a mouse button is pressed
    virtual void mousePressEvent ( QMouseEvent * event );
    // Called when a mouse button is released
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    // Called when mouse is pressed and dragged
    virtual void mouseMoveEvent( QMouseEvent * event );


private:
    Mode m_mode; // what to do when a user clicks

    Point2D m_start_pos; // position where the user last clicked

    std::list<PaintShape*> m_shapes;

	bool m_willClearCanvas;
	QColor m_colour;
	int m_numCompletedShapes;
};

#endif
