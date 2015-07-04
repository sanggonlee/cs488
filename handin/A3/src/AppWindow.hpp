#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "Viewer.hpp"
#include "scene.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow();
    void setRootSceneNode(SceneNode* node);
    Q_SLOT void walkRoot();

private:
    void createActions();
    void createMenu();

    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_menu_mode;
    QMenu* m_menu_edit;
    QMenu* m_menu_options;

    std::vector<QAction*> m_menu_actions;
    std::vector<QAction*> m_menu_mode_actions;
    std::vector<QAction*> m_menu_edit_actions;
    std::vector<QAction*> m_menu_options_actions;
    Viewer* m_viewer;
    
    SceneNode* mRootSceneNode;
};

#endif
