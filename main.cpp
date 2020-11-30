#include "notepad.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication EditorApp(argc, argv);
    Notepad Editor;
    Editor.show();
    return EditorApp.exec();
}

/*
The QApplication Object:
    * Manages application-wide resources
    * Necessary to run any Qt program that uses QtWidgets
    * Constructs an application object with argc command line arguments that run in argv
    * return EditorApp.excec(); - makes the QApplication enter its event loop, so that events are sent to the widgets of the application

The QGuiApplication Object:
    * Used for GUI applications that do not use QtWidgets

Types of Widgets:
    * Text edits
    * Scroll bars
    * Labels
    * Radio buttons
    * Widget container
    * Dialog
    * Main app window

Widgets:
    * Not visible by default, .show() function makes them visible
*/
