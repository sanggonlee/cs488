#include <QApplication>
#include <QDesktopWidget>
#include <string>
#include <iostream>
#include "AppWindow.hpp"
#include "scene_lua.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    std::string filename = "puppet.lua";
    if (argc >= 2) {
        filename = argv[1];
    }

    // This is how you might import a scene
    SceneNode* root = import_lua(filename);
    if (!root) {
        std::cerr << "Could not open " << filename << std::endl;
        return 1;
    }
 
    AppWindow window;
    window.resize(window.sizeHint());
    int desktopArea = QApplication::desktop()->width() * 
                      QApplication::desktop()->height();

    int widgetArea = window.width() * window.height();

    if (((float)widgetArea / (float) desktopArea) < 0.75f) {
        window.show();
    } else {
        window.showMaximized();
    }

    return app.exec();
}

