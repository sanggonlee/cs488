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

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void createActions();
    void createMenu();
    
    Q_SLOT void startNewGame();

    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_menu_drawMode;
    QMenu* m_menu_speed;
    
    QActionGroup *mSpeedActionGroup;

    std::vector<QAction*> m_menu_actions;
    std::vector<QAction*> m_menu_drawMode_actions;
    std::vector<QAction*> m_menu_speed_actions;
    Viewer* m_viewer;
};

#endif
