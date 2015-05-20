#include <QtWidgets>
#include <QGLFormat>
#include <iostream>
#include "AppWindow.hpp"

AppWindow::AppWindow() {
    setWindowTitle("488 Tetrominoes on the Wall");

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    // m_menubar = new QMenuBar;
    m_viewer = new Viewer(glFormat, this);
    layout->addWidget(m_viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);
    m_viewer->show();

    createActions();
    createMenu();
    startNewGame();
}

void AppWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        QCoreApplication::instance()->quit();
    } else if (event->key() == Qt::Key_T) {
        std::cerr << "Hello!" << std::endl;
    } else {
        QWidget::keyPressEvent(event);
    }
}

void AppWindow::createActions() {
    // Creates a new action for quiting and pushes it onto the menu actions vector 
    QAction* quitAct = new QAction(tr("&Quit"), this);
    
    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P); 
    quitAct->setShortcuts(QKeySequence::Quit);

    // Set the tip
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
    
    
  	// Acts for Application menu
  	QAction* newGameAct = new QAction(tr("&New game"), this);
  	newGameAct->setShortcut(QKeySequence(Qt::Key_N));
  	connect(newGameAct, SIGNAL(triggered()), this, SLOT(startNewGame()));
  	m_menu_actions.push_back(newGameAct);
  	
  	QAction* resetAct = new QAction(tr("&Reset"), this);
  	resetAct->setShortcut(QKeySequence(Qt::Key_R));
  	connect(resetAct, SIGNAL(triggered()), m_viewer, SLOT(resetView()));
  	m_menu_actions.push_back(resetAct);
  	
  	m_menu_actions.push_back(quitAct);
    
    // Acts for Draw Mode menu
    QAction* wireframeAct = new QAction(tr("&Wire-frame"), this);
    wireframeAct->setShortcut(QKeySequence(Qt::Key_W));
    connect(wireframeAct, SIGNAL(triggered()), m_viewer, SLOT(setToWireframe()));
    m_menu_drawMode_actions.push_back(wireframeAct);
    
    QAction* faceAct = new QAction(tr("&Face"), this);
    faceAct->setShortcut(QKeySequence(Qt::Key_F));
    connect(faceAct, SIGNAL(triggered()), m_viewer, SLOT(fillFaces()));
    m_menu_drawMode_actions.push_back(faceAct);
    
    QAction* multicolouredAct = new QAction(tr("&Multicoloured"), this);
    multicolouredAct->setShortcut(QKeySequence(Qt::Key_M));
    connect(multicolouredAct, SIGNAL(triggered()), m_viewer, SLOT(setToMulticoloured()));
    m_menu_drawMode_actions.push_back(multicolouredAct);
    
    // Acts for Speed menu
    QAction *slowAct = new QAction(tr("&Slow (1)"), this);
    slowAct->setShortcut(QKeySequence(Qt::Key_1));
    slowAct->setCheckable(true);
    connect(slowAct, SIGNAL(triggered()), m_viewer, SLOT(setToSlow()));
    m_menu_speed_actions.push_back(slowAct);
    
    QAction *mediumAct = new QAction(tr("&Medium (2)"), this);
    mediumAct->setShortcut(QKeySequence(Qt::Key_2));
    mediumAct->setCheckable(true);
    connect(mediumAct, SIGNAL(triggered()), m_viewer, SLOT(setToMedium()));
    m_menu_speed_actions.push_back(mediumAct);
    
    QAction *fastAct = new QAction(tr("&Fast (3)"), this);
    fastAct->setShortcut(QKeySequence(Qt::Key_3));
    fastAct->setCheckable(true);
    connect(fastAct, SIGNAL(triggered()), m_viewer, SLOT(setToFast()));
    m_menu_speed_actions.push_back(fastAct);
    
    mSpeedActionGroup = new QActionGroup(this);
    mSpeedActionGroup->addAction(slowAct);
    mSpeedActionGroup->addAction(mediumAct);
    mSpeedActionGroup->addAction(fastAct);
    mSpeedActionGroup->setExclusive(true);
    mediumAct->setChecked(true);
}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    m_menu_drawMode = menuBar()->addMenu(tr("Draw Mode"));
    m_menu_speed = menuBar()->addMenu(tr("Speed"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }
    
    for (auto& action : m_menu_drawMode_actions) {
    	m_menu_drawMode->addAction(action);
    }
    
    for (auto& action : m_menu_speed_actions) {
    	m_menu_speed->addAction(action);
    }
}

void AppWindow::startNewGame() {
	if (mSpeedActionGroup->checkedAction()->text() == "&Slow (1)") {
		m_viewer->setToSlow();
	} else if (mSpeedActionGroup->checkedAction()->text() == "&Medium (2)") {
		m_viewer->setToMedium();
	} else if (mSpeedActionGroup->checkedAction()->text() == "&Fast (3)") {
		m_viewer->setToFast();
	}
	m_viewer->startNewGame();
}
