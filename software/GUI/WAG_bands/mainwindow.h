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
#include "wagfile.h"
#include <set>

#include "communications/wifimanager.h"
#include "band/suit.h"

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
    void resizeEvent(QResizeEvent* r);
    void addTab(USER u, WAGFile *w, ACTION_TYPE a);

    QWidget *applicationWidget;
    QTabWidget *tabs;
    Overlay *overlay;

    // fonts & styles
    QFont titleFont;
    QString titleStyleSheet;
    QString textInputStyleWhite;
    QString textInputStyleRed;
    int buttonHeight;
    // menubar
    smartPushButton *newBtn;
    smartPushButton *openBtn;
    smartPushButton *settingsBtn;
    smartPushButton *helpBtn;
    smartPushButton *saveBtn;
    QWidget *menu;
    void createMenuButtons();
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
    OverlayWidget *settingsWidget;
    void createSettings();
    // save as overlay
    OverlayWidget *saveAsWidget;
    QLineEdit *saveAsFilenameTextEdit;
    QLabel *saveAsTagsLabel;
    QLineEdit *saveAsTagsTextEdit;
    QTextEdit *saveAsDescription;
    // open
    OverlayWidget *openFromLibWidget;
    OverlayWidget *openWidget;
    void createOpenFromLib(USER u);
    void createOpen(USER u);
    // create new file
    OverlayWidget *newFileWidget;
    QLineEdit *newFilenameTextEdit;
    QTextEdit *newFileDescription;
    QLineEdit *newFileTagsTextEdit;
    QLabel *newFileTagsLabel;
    QPushButton *addTagBtn;
    QPushButton *createNewFileBtn;
    void createNewFile(USER u);
    // user options
    OverlayWidget *userOptionsWidget;
    QWidget* createUserSelectionWindow(std::vector<USER> u);
    // move to wherever we initialize the suit object
    WifiManager *wifiMan;
    Suit *fullSuit;

private slots:
    // open
    void launchOpenFromComputer(USER u);
    void launchOpenFromLibrary(USER u);
    void launchOpen();
    void closeOpenFromLibrary();
    void closeOpen();
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
    void handleNewFileRequiredInput();
    void handleNewFileRequiredInput(QString);
    // on launch
    void launchUserOptions(USER);
    void closeUserOptions();
    void handleUserOptions(USER);

signals:
    void resizedWindow();
};
