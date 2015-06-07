#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "Viewer.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow();

private:
    void createActions();
    void createMenu();
    
    Q_SLOT void updateStatusBar(QString msg);

    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_menu_mode;

    std::vector<QAction*> m_menu_actions;
    std::vector<QAction*> m_menu_mode_actions;
    Viewer* m_viewer;
    
    QStatusBar* m_status_bar;
};

#endif
