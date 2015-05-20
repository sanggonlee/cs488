#ifndef PAINTWINDOW_HPP
#define PAINTWINDOW_HPP

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include "paintcanvas.hpp"

// class PaintCanvas;

class PaintWindow : public QMainWindow
{
    Q_OBJECT

public:
    PaintWindow();
    virtual ~PaintWindow() {}


protected:

private slots:
    // Display a message box providing assistance with the line drawing
    // function.
    void help_line();
    void help_rectangle();
    void help_oval();

    void set_line();
    void set_rect();
    void set_oval();
    
    void setColourBlack();
	void setColourRed();
	void setColourGreen();
	void setColourBlue();

private:

    void createMenu();

    // Note: QMainWindow has its own QMenuBar where as QWidget does not
    // Each drop down menus
    QMenu* m_menu_app;
    QMenu* m_menu_tools;
    QMenu* m_menu_colour;
    QMenu* m_menu_help;

    // The canvas onto which the user draws.
    PaintCanvas* m_canvas;

	// The push buttons at the bottom of main window
	QPushButton* m_quitButton;
};

#endif
