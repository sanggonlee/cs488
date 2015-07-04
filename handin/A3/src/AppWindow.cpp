#include <QtWidgets>
#include <iostream>
#include "AppWindow.hpp"

AppWindow::AppWindow() {
    setWindowTitle("488 Assignment Two");

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    m_viewer = new Viewer(glFormat, this);
    layout->addWidget(m_viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    createActions();
    createMenu();
    
    connect(m_viewer, SIGNAL(requestRewalk()), this, SLOT(walkRoot()));
}

void AppWindow::createActions() {
    // Creates a new action for quiting and pushes it onto the menu actions vector 
    QAction* quitAct = new QAction(tr("&Quit"), this);
    m_menu_actions.push_back(quitAct);

    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P); 
    quitAct->setShortcuts(QKeySequence::Quit);

    // Set the tip
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
    
    
	// Application menu
    QAction* resetPositionAct = new QAction(tr("Reset Pos&ition"), this);
    m_menu_actions.push_back(resetPositionAct);
    resetPositionAct->setShortcut(QKeySequence(Qt::Key_I));
    connect(resetPositionAct, SIGNAL(triggered()), m_viewer, SLOT(resetPosition()));
    
    QAction* resetOrientationAct = new QAction(tr("Reset &Orientation"), this);
    m_menu_actions.push_back(resetOrientationAct);
    resetOrientationAct->setShortcut(QKeySequence(Qt::Key_O));
    connect(resetOrientationAct, SIGNAL(triggered()), m_viewer, SLOT(resetOrientation()));

    QAction* resetAllAct = new QAction(tr("Reset &All"), this);
    m_menu_actions.push_back(resetAllAct);
    resetAllAct->setShortcut(QKeySequence(Qt::Key_A));
    connect(resetAllAct, SIGNAL(triggered()), m_viewer, SLOT(resetAll()));
    
    // Mode menu
    QAction* posOriAct = new QAction(tr("&Position/Orientation"), this);
    m_menu_mode_actions.push_back(posOriAct);
    posOriAct->setShortcut(QKeySequence(Qt::Key_P));
    posOriAct->setCheckable(true);
    connect(posOriAct, SIGNAL(triggered()), m_viewer, SLOT(setToPositionOrientation()));
    
    QAction* jointsAct = new QAction(tr("&Joints"), this);
    m_menu_mode_actions.push_back(jointsAct);
    jointsAct->setShortcut(QKeySequence(Qt::Key_J));
    jointsAct->setCheckable(true);
    connect(jointsAct, SIGNAL(triggered()), m_viewer, SLOT(setToJoints()));
    
    QActionGroup* modeActionGroup = new QActionGroup(this);
    modeActionGroup->addAction(posOriAct);
    modeActionGroup->addAction(jointsAct);
    modeActionGroup->setExclusive(true);
    posOriAct->setChecked(true);
    
    // Edit menu
    QAction* undoAct = new QAction(tr("&Undo"), this);
    m_menu_edit_actions.push_back(undoAct);
    undoAct->setShortcut(QKeySequence(Qt::Key_U));
    connect(undoAct, SIGNAL(triggered()), m_viewer, SLOT(undo()));
    
    QAction* redoAct = new QAction(tr("&Redo"), this);
    m_menu_edit_actions.push_back(redoAct);
    redoAct->setShortcut(QKeySequence(Qt::Key_R));
    connect(redoAct, SIGNAL(triggered()), m_viewer, SLOT(redo()));
    
    // Options menu
    QAction* circleAct = new QAction(tr("&Circle"), this);
    m_menu_options_actions.push_back(circleAct);
    circleAct->setShortcut(QKeySequence(Qt::Key_C));
    circleAct->setCheckable(true);
    connect(circleAct, SIGNAL(triggered()), m_viewer, SLOT(setToCircle()));
    
    QAction* zBufferAct = new QAction(tr("&Z-buffer"), this);
    m_menu_options_actions.push_back(zBufferAct);
    zBufferAct->setShortcut(QKeySequence(Qt::Key_Z));
    zBufferAct->setCheckable(true);
    connect(zBufferAct, SIGNAL(triggered()), m_viewer, SLOT(setToZBuffer()));
    
    QAction* backfaceCullAct = new QAction(tr("&Backface cull"), this);
    m_menu_options_actions.push_back(backfaceCullAct);
    backfaceCullAct->setShortcut(QKeySequence(Qt::Key_B));
    connect(backfaceCullAct, SIGNAL(triggered()), m_viewer, SLOT(setToBackfaceCull()));
    backfaceCullAct->setCheckable(true);
    
    QAction* frontfaceCullAct = new QAction(tr("&Frontface cull"), this);
    m_menu_options_actions.push_back(frontfaceCullAct);
    frontfaceCullAct->setShortcut(QKeySequence(Qt::Key_F));
    connect(frontfaceCullAct, SIGNAL(triggered()), m_viewer, SLOT(setToFrontfaceCull()));
    frontfaceCullAct->setCheckable(true);
}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }
    
    m_menu_mode = menuBar()->addMenu(tr("&Mode"));
    for (auto& action: m_menu_mode_actions) {
    	m_menu_mode->addAction(action);
    }
    
    m_menu_edit = menuBar()->addMenu(tr("&Edit"));
    for (auto& action: m_menu_edit_actions) {
    	m_menu_edit->addAction(action);
    }
    
    m_menu_options = menuBar()->addMenu(tr("&Options"));
    for (auto& action: m_menu_options_actions) {
    	m_menu_options->addAction(action);
    }
}

void AppWindow::setRootSceneNode(SceneNode* node) {
	mRootSceneNode = node;
	mRootSceneNode->setViewer(m_viewer);
	mRootSceneNode->walk_gl(false);
}

void AppWindow::walkRoot() {
	mRootSceneNode->walk_gl(false);
}
