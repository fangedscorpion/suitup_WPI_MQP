#pragma once

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
#include "smartpushbutton.h"
#include <set>

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
    void createMenuButtons();
    void resizeEvent(QResizeEvent* r);
    void createSettings();
    void createPlaybackOptionsAndControls();
    void createEditRecordingOptionsAndControls();
    void createViewer();
//    void createSaveAs();
    void createOpenFromLib(USER u);
    QWidget* createUserSelectionWindow(std::vector<USER> u);
    void createNewFile(USER u);
    void addTab(USER u, QString filename, ACTION_TYPE a);

    Overlay *overlay;
    OverlayWidget *settingsWidget;
    QWidget *applicationWidget;
    QTabWidget *tabs;

    // fonts & styles
    QFont titleFont;
    QString titleStyleSheet;
    // menubar
    smartPushButton *newBtn;
    smartPushButton *openBtn;
    smartPushButton *settingsBtn;
    smartPushButton *helpBtn;
    smartPushButton *saveBtn;
    QWidget *menu;
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
    // user options
    OverlayWidget *userOptionsWidget;


private slots:
    // open
    void launchOpenFromComputer(USER u);
    void launchOpenFromLibrary(USER u);
    void closeOpenFromLibrary();
    void openFromLibrary(USER u);
    // settings
    void launchSettings();
    void closeSettings();
    void saveSettings();
    // save/saveAs
    void save();
    void addTag();
    // new file
    void saveNewFile(USER u);
    void closeNewFile();
    void launchNewFile(USER u);
    // on launch
    void launchUserOptions(USER);
    void closeUserOptions();
    void handleUserOptions(USER);

signals:
    void resizedWindow();
};
