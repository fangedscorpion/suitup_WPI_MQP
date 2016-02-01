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

    bool isVoiceControlOn() { return voiceControl->isChecked(); }


private:
    Ui::MainWindow *ui;
    void resizeEvent(QResizeEvent* r);
    void addTab(USER u, WAGFile *w, ACTION_TYPE a);
    QWidget *getCurrentTabcontent() { return tabs->currentWidget(); }

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
    QCheckBox *rightShoulder;
    QCheckBox *rightUpperArm;
    QCheckBox *rightLowerArm;
    QPushButton *connectBands;
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
    void createOpen(USER u) ;
    // create new file
    OverlayWidget *newMotionWidget;
    QLineEdit *newMotionNameTextEdit;
    QTextEdit *newMotionDescription;
    QLineEdit *newMotionTagsTextEdit;
    QLabel *newMotionTagsLabel;
    QPushButton *addTagBtn;
    QPushButton *createNewMotionBtn;
    void createNewMotion(USER u);
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
    void saveNewMotion(USER u);
    void closeNewMotion();
    void launchNewMotion(USER u);
    void handleNewMotionRequiredInput();
    void handleNewMotionRequiredInput(QString);
    // on launch
    void launchUserOptions(USER);
    void closeUserOptions();
    void handleUserOptions(USER);
    // misc
    void setTabContentVoiceControl(bool b);
    void setVoiceControl(bool b) { voiceControl->setChecked(b); }
    void closeTab(int i) { tabs->removeTab(i); } // TODO: check for unsaved stuff
    void connectCheckedBands();
    void indicateConnectionStatusChange(BandType changedBand, ConnectionStatus updatedStatus);

signals:
    void resizedWindow();
};
