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
    MainWindow(QPointer<QWidget> parent = 0);
    ~MainWindow();

    void setCurrentTabName(QString s);
    void lockOnPlayOrRecord(bool suitRecording);

private:
    QPointer<Ui::MainWindow> ui;
    void resizeEvent(QResizeEvent* r);
    void addTab(USER u, QPointer<WAGFile> w, ACTION_TYPE a);
    QPointer<QWidget> getCurrentTabcontent() { return tabs->currentWidget(); }

    QPointer<QTabWidget> tabs;
    QPointer<Overlay> overlay;

    // fonts & styles
    QString textInputStyleWhite;
    QString textInputStyleRed;
    // menubar
    QPointer<SmartPushButton> newBtn;
    QPointer<SmartPushButton> openBtn;
    QPointer<SmartPushButton> settingsBtn;
    QPointer<QHBoxLayout> createMenuButtons();
    // statusbar
    QPointer<QLabel> connectionStatus;
    QPointer<QLabel> batteryStatus;
    QPointer<QHBoxLayout> createStatusBar();
    // settings overlay
    QPointer<StyledCheckBox> leftShoulder;
    QPointer<StyledCheckBox> leftUpperArm;
    QPointer<StyledCheckBox> leftLowerArm;
    QPointer<StyledCheckBox> rightShoulder;
    QPointer<StyledCheckBox> rightUpperArm;
    QPointer<StyledCheckBox> rightLowerArm;
    QPointer<SmartPushButton> connectBands;
    QPointer<OverlayWidget> settingsWidget;
    void createSettings();
    // open (from lib)
    QPointer<OverlayWidget> openFromLibWidget;
    QPointer<OverlayWidget> openWidget;
    QPointer<QTableWidget> openFromLibTable;
    QPointer<SmartPushButton> openFromLibBtn;
    QPointer<QLineEdit> openFromLibFilterBar;
    QPointer<QComboBox> openFromLibFilterOpts;
    void createOpenFromLib(USER u);
    void createOpenMotionOptions(USER u) ;
    // create new file
    QPointer<OverlayWidget> newMotionWidget;
    QPointer<QLineEdit> newMotionNameTextEdit;
    QPointer<QTextEdit> newMotionDescription;
    QPointer<QLineEdit> newMotionTagsTextEdit;
    QPointer<QHBoxLayout> newMotionTagsLayout;
    QPointer<SmartPushButton> addTagBtn;
    QPointer<SmartRadioButton> newMotionCompRadio;
    QPointer<QLabel> newMotionSaveLocation;
    QPointer<SmartPushButton> createNewMotionBtn;
    QPointer<SmartPushButton> newMotionBrowseBtn;
    void createNewMotion(USER u);
    // user options
    QPointer<OverlayWidget> userOptionsWidget;
    QPointer<QWidget> createUserSelectionWindow(std::vector<USER> u);
    // move to wherever we initialize the suit object
    QPointer<WifiManager> wifiMan;
    QPointer<Suit> fullSuit;
    ModelLoader *modelLoader;
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
    void modeChanged(ACTION_TYPE);
};
