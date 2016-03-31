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
#include "customWidgets/superslider.h"
#include "customWidgets/overlay.h"
#include "customWidgets/overlaywidget.h"
#include "playbackcontroller.h"
#include "customWidgets/smartpushbutton.h"
#include "customWidgets/smartradiobutton.h"
#include "customWidgets/styledcheckbox.h"
#include "wagfile.h"
#include <set>
#include <QTableWidget>

#include "communications/wifimanager.h"
#include "band/suit.h"
#include "model/modelloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

    void setCurrentTabName(QString s);
    void lockOnPlayOrRecord(bool suitRecording);

private:
    DISPLAY_TYPE preSettingsMode;
    Ui::MainWindow* ui;
    void resizeEvent(QResizeEvent* r);
    void addTab(USER u, WAGFile* w, ACTION_TYPE a);
    QWidget* getCurrentTabcontent() { return tabs->currentWidget(); }

    QTabWidget* tabs;
    Overlay* overlay;

    // fonts & styles
    QString textInputStyleWhite;
    QString textInputStyleRed;
    // menubar
    SmartPushButton* newBtn;
    SmartPushButton* openBtn;
    SmartPushButton* settingsBtn;
    QHBoxLayout* createMenuButtons();
    // statusbar
    QLabel* connectionStatus;
    QLabel* batteryStatus;
    QHBoxLayout* createStatusBar();
    // settings overlay
    StyledCheckBox* leftShoulder;
    StyledCheckBox* leftUpperArm;
    StyledCheckBox* leftLowerArm;
    StyledCheckBox* rightShoulder;
    StyledCheckBox* rightUpperArm;
    StyledCheckBox* rightLowerArm;
    SmartPushButton* connectBands;
    OverlayWidget* settingsWidget;
    void createSettings();
    // open (from lib)
    OverlayWidget* openFromLibWidget;
    OverlayWidget* openWidget;
    QTableWidget* openFromLibTable;
    SmartPushButton* openFromLibBtn;
    QLineEdit* openFromLibFilterBar;
    QComboBox* openFromLibFilterOpts;
    void createOpenFromLib(USER u);
    void createOpenMotionOptions(USER u) ;
    // create new file
    OverlayWidget* newMotionWidget;
    QLineEdit* newMotionNameTextEdit;
    QTextEdit* newMotionDescription;
    QLineEdit* newMotionTagsTextEdit;
    QHBoxLayout* newMotionTagsLayout;
    SmartPushButton* addTagBtn;
    SmartRadioButton* newMotionCompRadio;
    QLabel* newMotionSaveLocation;
    SmartPushButton* createNewMotionBtn;
    SmartPushButton* newMotionBrowseBtn;
    void createNewMotion(USER u);
    // user options
    OverlayWidget* userOptionsWidget;
    QWidget* createUserSelectionWindow(std::vector<USER> u);
    // move to wherever we initialize the suit object
    WifiManager* wifiMan;
    Suit* fullSuit;
    ModelLoader *modelLoader;
    Model* calibModel;
    QSet<BandType> connectedBands;

private slots:
    // open
    void launchOpenFromComputer(USER u);
    void launchOpenFromLibrary(USER u);
    void launchOpen(USER u);
    void closeOpenFromLibrary();
    void closeOpenMotionOptions();
    void openFromLibrary(USER u);
    void handleOpenFromLibBtn(int, int);
    void handleOpenFromLibFilter(QString = "");
    // settings
    void launchSettings();
    void closeSettings();
    void handleConnectedBands();
    // new file
    void saveNewMotion(USER u);
    void closeNewMotion();
    void launchNewMotion(USER u);
    void handleNewMotionRequiredInput();
    void handleNewMotionRequiredInput(QString);
    void handleNewMotionRadios();
    void addTag();
    // on launch
    void launchUserOptions(USER);
    void closeUserOptions();
    void handleUserOptions(USER);
    // misc
    void closeTab(int i); // TODO: check for unsaved stuff
    void connectCheckedBands();
    void indicateConnectionStatusChange(BandType changedBand, ConnectionStatus updatedStatus);
    void catchTabChange(int);
    // statusbar
    void updateConnectionStatus(BandType b, ConnectionStatus c);
    void updateBatteryStatus();
    void catchLowBatterySignal(BandType lowBatteryBand, bool batteryIsLow);
signals:
    void modeChanged(DISPLAY_TYPE);
};
