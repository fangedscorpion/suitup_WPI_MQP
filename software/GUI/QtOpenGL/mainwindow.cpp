#include "mainwindow.h"
#include "window.h"

MainWindow::MainWindow() {
    setFixedSize(600,500);
    setCentralWidget(new Window());
}
