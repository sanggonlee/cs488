#include <QtWidgets>
#include <iostream>
#include "paintwindow.hpp"

PaintWindow::PaintWindow() {
    setWindowTitle("488 Paint");

    QVBoxLayout *layout = new QVBoxLayout;

    m_canvas = new PaintCanvas(this);
    layout->addWidget(m_canvas);

	m_quitButton = new QPushButton(tr("&Quit"));
	connect(m_quitButton, SIGNAL(clicked()), this, SLOT(close()));
	layout->addWidget(m_quitButton);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    createMenu();
}

void PaintWindow::createMenu() {
    // Adding the drop down menu to the menubar
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    m_menu_tools = menuBar()->addMenu(tr("&Tools"));
    m_menu_colour = menuBar()->addMenu(tr("&Colour"));
    m_menu_help = menuBar()->addMenu(tr("&Help"));

    // Adding the menu items for each drop down menu
    QAction* quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Exits the program"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
    m_menu_app->addAction(quitAct);
    
    QAction* clearAct = new QAction(tr("&Clear"), this);
    clearAct->setShortcut(QKeySequence(Qt::Key_C));
    connect(clearAct, SIGNAL(triggered()), m_canvas, SLOT(clearCanvas()));
    m_menu_app->addAction(clearAct);

    QAction* drawLineAct = new QAction(tr("&Line"), this);
    drawLineAct->setStatusTip(tr("Draws a line"));
    drawLineAct->setShortcut(QKeySequence(Qt::Key_L));
    drawLineAct->setCheckable(true);
    connect(drawLineAct, SIGNAL(triggered()), this, SLOT(set_line()));

    QAction* drawOvalAct = new QAction(tr("&Oval"), this);
    drawOvalAct->setStatusTip(tr("Draws an Oval"));
    drawOvalAct->setShortcut(QKeySequence(Qt::Key_O));
    drawOvalAct->setCheckable(true);
    connect(drawOvalAct, SIGNAL(triggered()), this, SLOT(set_oval()));

    QAction* drawRectangleAct = new QAction(tr("&Rectangle"), this);
    drawRectangleAct->setStatusTip(tr("Draws a rectangle"));
    drawRectangleAct->setShortcut(QKeySequence(Qt::Key_R));
    drawRectangleAct->setCheckable(true);
    connect(drawRectangleAct, SIGNAL(triggered()), this, SLOT(set_rect()));
    
	QActionGroup *toolsActionGroup = new QActionGroup(this);
	toolsActionGroup->addAction(drawLineAct);
	toolsActionGroup->addAction(drawOvalAct);
	toolsActionGroup->addAction(drawRectangleAct);
	toolsActionGroup->setExclusive(true);
	drawLineAct->setChecked(true);
	
    m_menu_tools->addAction(drawLineAct);
    m_menu_tools->addAction(drawOvalAct);
    m_menu_tools->addAction(drawRectangleAct);

    QAction* setColourBlackAct = new QAction(tr("&Black"), this);
    connect(setColourBlackAct, SIGNAL(triggered()), this, SLOT(setColourBlack()));
    
    QAction* setColourRedAct = new QAction(tr("&Red"), this);
    connect(setColourRedAct, SIGNAL(triggered()), this, SLOT(setColourRed()));
    
    QAction* setColourGreenAct = new QAction(tr("&Green"), this);
    connect(setColourGreenAct, SIGNAL(triggered()), this, SLOT(setColourGreen()));
    
    QAction* setColourBlueAct = new QAction(tr("&Blue"), this);
    connect(setColourBlueAct, SIGNAL(triggered()), this, SLOT(setColourBlue()));
    
    m_menu_colour->addAction(setColourBlackAct);
    m_menu_colour->addAction(setColourRedAct);
    m_menu_colour->addAction(setColourGreenAct);
    m_menu_colour->addAction(setColourBlueAct);

    QAction* helpLineAct = new QAction(tr("&Line Help"), this);
    helpLineAct->setStatusTip(tr("Help Instructions"));
    connect(helpLineAct, SIGNAL(triggered()), this, SLOT(help_line()));
    m_menu_help->addAction(helpLineAct);
    
    QAction* helpRectangleAct = new QAction(tr("&Rectangle Help"), this);
    helpRectangleAct->setStatusTip(tr("Help Instructions for rectangles"));
    connect(helpRectangleAct, SIGNAL(triggered()), this, SLOT(help_rectangle()));
    m_menu_help->addAction(helpRectangleAct);

    QAction* helpOvalAct = new QAction(tr("&Oval Help"), this);
    helpOvalAct->setStatusTip(tr("Help Instructions for ovals"));
    connect(helpOvalAct, SIGNAL(triggered()), this, SLOT(help_oval()));
    m_menu_help->addAction(helpOvalAct);
}

void PaintWindow::help_line() {
    const char* message =
    "Drawing a Line\n"
    "\n"
    "To draw a line, press the left mouse button to mark the beginning of the line.  Drag the mouse to the end of the line and release the button.";

    QMessageBox msgBox;
    msgBox.setText(QString(message));
    msgBox.exec();
}

void PaintWindow::help_rectangle() {
    const char* message =
    "Drawing a Rectangle\n"
    "\n"
    "To draw a rectangle, press the left mouse button to mark the first vertex of the rectangle.  Drag the mouse to the diagonally opposite vertex and release the button.";

    QMessageBox msgBox;
    msgBox.setText(QString(message));
    msgBox.exec();
}

void PaintWindow::help_oval() {
    const char* message =
    "Drawing an Oval\n"
    "\n"
    "To draw an oval, press the left mouse button and drag the mouse to set the size of the oval and release the button.";

    QMessageBox msgBox;
    msgBox.setText(QString(message));
    msgBox.exec();
}

void PaintWindow::set_line() {
    m_canvas->set_mode(PaintCanvas::DRAW_LINE);
}

void PaintWindow::set_oval() {
    m_canvas->set_mode(PaintCanvas::DRAW_OVAL);
}

void PaintWindow::set_rect() {
    m_canvas->set_mode(PaintCanvas::DRAW_RECTANGLE);
}

void PaintWindow::setColourBlack() {
	m_canvas->setColour(QColor(Qt::GlobalColor::black));
}

void PaintWindow::setColourRed() {
	m_canvas->setColour(QColor(Qt::GlobalColor::red));
}

void PaintWindow::setColourGreen() {
	m_canvas->setColour(QColor(Qt::GlobalColor::green));
}

void PaintWindow::setColourBlue() {
	m_canvas->setColour(QColor(Qt::GlobalColor::blue));
}
