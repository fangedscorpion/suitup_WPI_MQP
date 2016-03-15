#define WAG_TEST

#ifdef WAG_TEST
    #include "test/runtests.h"

    int main(int argc, char *argv[]){
        runtests(argc, argv);
        return 0;
    }

#else
    #include "mainwindow.h"
    #include <QApplication>
    #include <QPushButton>
    #include <QOpenGLWidget>

    int main(int argc, char *argv[]){
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    }

#endif
