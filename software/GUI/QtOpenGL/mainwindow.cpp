
#include "mainwindow.h"
#include "window.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>

MainWindow::MainWindow() {
    setFixedSize(600,500);
    setCentralWidget(new Window(this));
}
