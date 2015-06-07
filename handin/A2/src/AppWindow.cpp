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
    // m_menubar = new QMenuBar;
    m_viewer = new Viewer(glFormat, this);
    layout->addWidget(m_viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    createActions();
    createMenu();
    
    m_status_bar = this->statusBar();
    connect(m_viewer, SIGNAL(statusChanged(QString)), this, SLOT(updateStatusBar(QString)));
}

void AppWindow::updateStatusBar(QString msg) {
	m_status_bar->showMessage(msg);
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
    
    QAction* resetAct = new QAction(tr("Reset"), this);
    m_menu_actions.push_back(resetAct);
    resetAct->setShortcut(QKeySequence(Qt::Key_A));
    connect(resetAct, SIGNAL(triggered()), m_viewer, SLOT(reset_view()));
    
    
    // Actions for mode menu
    QAction* viewRotateAct = new QAction(tr("View R&otate"), this);
    m_menu_mode_actions.push_back(viewRotateAct);
    viewRotateAct->setShortcut(QKeySequence(Qt::Key_O));
    viewRotateAct->setCheckable(true);
    connect(viewRotateAct, SIGNAL(triggered()), m_viewer, SLOT(setToViewRotate()));
    
    QAction* viewTranslateAct = new QAction(tr("View Tra&nslate"), this);
    m_menu_mode_actions.push_back(viewTranslateAct);
    viewTranslateAct->setShortcut(QKeySequence(Qt::Key_N));
    viewTranslateAct->setCheckable(true);
    connect(viewTranslateAct, SIGNAL(triggered()), m_viewer, SLOT(setToViewTranslate()));
    
    QAction* viewPerspectiveAct = new QAction(tr("View &Perspective"), this);
    m_menu_mode_actions.push_back(viewPerspectiveAct);
    viewPerspectiveAct->setShortcut(QKeySequence(Qt::Key_P));
    viewPerspectiveAct->setCheckable(true);
    connect(viewPerspectiveAct, SIGNAL(triggered()), m_viewer, SLOT(setToViewPerspective()));
    
    QAction* modelRotateAct = new QAction(tr("Model &Rotate"), this);
    m_menu_mode_actions.push_back(modelRotateAct);
    modelRotateAct->setShortcut(QKeySequence(Qt::Key_R));
    modelRotateAct->setCheckable(true);
    connect(modelRotateAct, SIGNAL(triggered()), m_viewer, SLOT(setToModelRotate()));
    
    QAction* modelTranslateAct = new QAction(tr("Model &Translate"), this);
    m_menu_mode_actions.push_back(modelTranslateAct);
    modelTranslateAct->setShortcut(QKeySequence(Qt::Key_T));
    modelTranslateAct->setCheckable(true);
    connect(modelTranslateAct, SIGNAL(triggered()), m_viewer, SLOT(setToModelTranslate()));
    
    QAction* modelScaleAct = new QAction(tr("Model &Scale"), this);
    m_menu_mode_actions.push_back(modelScaleAct);
    modelScaleAct->setShortcut(QKeySequence(Qt::Key_S));
    modelScaleAct->setCheckable(true);
    connect(modelScaleAct, SIGNAL(triggered()), m_viewer, SLOT(setToModelScale()));
    
    QAction* viewportAct = new QAction(tr("&Viewport Mode"), this);
    m_menu_mode_actions.push_back(viewportAct);
    viewportAct->setShortcut(QKeySequence(Qt::Key_V));
    viewportAct->setCheckable(true);
    connect(viewportAct, SIGNAL(triggered()), m_viewer, SLOT(setToViewport()));
    
    QActionGroup* modeActionGroup = new QActionGroup(this);
    modeActionGroup->addAction(viewRotateAct);
    modeActionGroup->addAction(viewTranslateAct);
    modeActionGroup->addAction(viewPerspectiveAct);
    modeActionGroup->addAction(modelRotateAct);
    modeActionGroup->addAction(modelTranslateAct);
    modeActionGroup->addAction(modelScaleAct);
    modeActionGroup->addAction(viewportAct);
    modeActionGroup->setExclusive(true);
    modelRotateAct->setChecked(true);
}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    m_menu_mode = menuBar()->addMenu(tr("&Mode"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }
    
    for (auto& action : m_menu_mode_actions) {
    	m_menu_mode->addAction(action);
    }
}

