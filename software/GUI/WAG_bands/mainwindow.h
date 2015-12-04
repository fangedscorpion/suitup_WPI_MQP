#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QAction>
#include <QOpenGLWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QPainter>
#include <QStackedWidget>
#include <QGraphicsView>
#include <QLineEdit>
#include <QRadioButton>
#include <QFileDialog>
#include <QTextEdit>
#include <QGroupBox>
#include <QStyle>
#include <QTabWidget>
#include <visualization/glwidget.h>
#include "superslider.h"
#include "overlay.h"
#include "overlaywidget.h"
#include "playbackcontroller.h"

enum ACTION_TYPE { EDIT, PLAYBACK, RECORD };

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    void createMenuActions(QMenuBar *menu);
    void resizeEvent(QResizeEvent* r);
    void createSettings();
    void createPlaybackOptionsAndControls();
    void createEditRecordingOptionsAndControls();
    void createViewer();
    void createSaveAs();
    void createOpenFromLib();
    QWidget* createWelcomeWindow();
    void createNewFile();
    void addTab(ACTION_TYPE t, QString filename);

    Overlay *overlay;
    OverlayWidget *settingsWidget;
    QWidget *applicationWidget;
    QTabWidget *tabs;

    // fonts & styles
    QFont titleFont;
    QString titleStyleSheet;
    // menubar
    QAction *saveAct;
    QAction *saveAsAct;
    // settings overlay
    QGraphicsView *view;
    QCheckBox *voiceControl;
    QCheckBox *leftShoulder;
    QCheckBox *leftUpperArm;
    QCheckBox *leftLowerArm;
    QCheckBox *leftHand;
    QCheckBox *rightShoulder;
    QCheckBox *rightUpperArm;
    QCheckBox *rightLowerArm;
    QCheckBox *rightHand;
    // save as overlay
    OverlayWidget *saveAsWidget;
    QLineEdit *saveAsFilenameTextEdit;
    QLabel *saveAsTagsLabel;
    QLineEdit *saveAsTagsTextEdit;
    QTextEdit *saveAsDescription;
    // open from library overlay
    OverlayWidget *openFromLibWidget;
    // create new file
    OverlayWidget *newFileWidget;
    QLineEdit *newFilenameTextEdit;
    QTextEdit *newFileDescription;
    QLineEdit *newFileTagsTextEdit;
    QLabel *newFileTagsLabel;


private slots:
    // open
    void launchOpenFromComputer();
    void launchOpenFromLibrary();
    void closeOpenFromLibrary();
    void openFromLibrary();
    // settings
    void launchSettings();
    void closeSettings();
    void saveSettings();
    // save/saveAs
    void save();
    void launchSaveAs();
    void saveSaveAs();
    void closeSaveAs();
    void launchSaveToComputer();
    void addTag();
    // new file
    void saveNewFile();
    void closeNewFile();
    void launchNewFile();

signals:
    void resizedWindow();
};

#endif // MAINWINDOW_H
