/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_4;
    QGroupBox *playbackGroup;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *playbackLayout;
    QLabel *playbackRecordingLbl;
    QCheckBox *suitPlayChk;
    QComboBox *modeCmb;
    QLabel *SpeedFrameLbl;
    QSlider *speedFrameSlider;
    QHBoxLayout *speedSliderLabels;
    QLabel *quarterSpeed;
    QLabel *normalSpeed;
    QLabel *fourSpeed;
    QHBoxLayout *holdFrameLayout;
    QLabel *holdFrame;
    QSpinBox *secondsSpn;
    QLabel *seconds;
    QLabel *positionLbl;
    QSlider *positionSlider;
    QHBoxLayout *positionSliderLabels;
    QLabel *close;
    QLabel *rough;
    QGroupBox *recordGroup;
    QWidget *verticalLayoutWidget_5;
    QVBoxLayout *editRecordingLayout;
    QLabel *editRecordingLbl;
    QSpacerItem *editRecordingSpacer;
    QPushButton *cropBtn;
    QPushButton *undoBtn;
    QPushButton *splitBtn;
    QVBoxLayout *visualizationLayout;
    QHBoxLayout *videoFileLayout;
    QLabel *filenameLbl;
    QLabel *fileLbl;
    QOpenGLWidget *videoPlayer;
    QHBoxLayout *videoBarLayout;
    QPushButton *videoControllerBtn;
    QSlider *videoSlider;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(715, 326);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayoutWidget_3 = new QWidget(centralwidget);
        horizontalLayoutWidget_3->setObjectName(QStringLiteral("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 10, 701, 284));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        playbackGroup = new QGroupBox(horizontalLayoutWidget_3);
        playbackGroup->setObjectName(QStringLiteral("playbackGroup"));
        playbackGroup->setMinimumSize(QSize(235, 0));
        playbackGroup->setLayoutDirection(Qt::LeftToRight);
        playbackGroup->setAlignment(Qt::AlignCenter);
        playbackGroup->setFlat(false);
        verticalLayoutWidget_4 = new QWidget(playbackGroup);
        verticalLayoutWidget_4->setObjectName(QStringLiteral("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(0, 0, 232, 265));
        playbackLayout = new QVBoxLayout(verticalLayoutWidget_4);
        playbackLayout->setObjectName(QStringLiteral("playbackLayout"));
        playbackLayout->setContentsMargins(0, 0, 0, 0);
        playbackRecordingLbl = new QLabel(verticalLayoutWidget_4);
        playbackRecordingLbl->setObjectName(QStringLiteral("playbackRecordingLbl"));
        playbackRecordingLbl->setAlignment(Qt::AlignCenter);

        playbackLayout->addWidget(playbackRecordingLbl);

        suitPlayChk = new QCheckBox(verticalLayoutWidget_4);
        suitPlayChk->setObjectName(QStringLiteral("suitPlayChk"));
        suitPlayChk->setLayoutDirection(Qt::LeftToRight);

        playbackLayout->addWidget(suitPlayChk);

        modeCmb = new QComboBox(verticalLayoutWidget_4);
        modeCmb->setObjectName(QStringLiteral("modeCmb"));

        playbackLayout->addWidget(modeCmb);

        SpeedFrameLbl = new QLabel(verticalLayoutWidget_4);
        SpeedFrameLbl->setObjectName(QStringLiteral("SpeedFrameLbl"));

        playbackLayout->addWidget(SpeedFrameLbl);

        speedFrameSlider = new QSlider(verticalLayoutWidget_4);
        speedFrameSlider->setObjectName(QStringLiteral("speedFrameSlider"));
        speedFrameSlider->setOrientation(Qt::Horizontal);

        playbackLayout->addWidget(speedFrameSlider);

        speedSliderLabels = new QHBoxLayout();
        speedSliderLabels->setObjectName(QStringLiteral("speedSliderLabels"));
        quarterSpeed = new QLabel(verticalLayoutWidget_4);
        quarterSpeed->setObjectName(QStringLiteral("quarterSpeed"));

        speedSliderLabels->addWidget(quarterSpeed);

        normalSpeed = new QLabel(verticalLayoutWidget_4);
        normalSpeed->setObjectName(QStringLiteral("normalSpeed"));
        normalSpeed->setAlignment(Qt::AlignCenter);

        speedSliderLabels->addWidget(normalSpeed);

        fourSpeed = new QLabel(verticalLayoutWidget_4);
        fourSpeed->setObjectName(QStringLiteral("fourSpeed"));
        fourSpeed->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        speedSliderLabels->addWidget(fourSpeed);


        playbackLayout->addLayout(speedSliderLabels);

        holdFrameLayout = new QHBoxLayout();
        holdFrameLayout->setObjectName(QStringLiteral("holdFrameLayout"));
        holdFrame = new QLabel(verticalLayoutWidget_4);
        holdFrame->setObjectName(QStringLiteral("holdFrame"));

        holdFrameLayout->addWidget(holdFrame);

        secondsSpn = new QSpinBox(verticalLayoutWidget_4);
        secondsSpn->setObjectName(QStringLiteral("secondsSpn"));

        holdFrameLayout->addWidget(secondsSpn);

        seconds = new QLabel(verticalLayoutWidget_4);
        seconds->setObjectName(QStringLiteral("seconds"));

        holdFrameLayout->addWidget(seconds);


        playbackLayout->addLayout(holdFrameLayout);

        positionLbl = new QLabel(verticalLayoutWidget_4);
        positionLbl->setObjectName(QStringLiteral("positionLbl"));
        positionLbl->setAlignment(Qt::AlignCenter);

        playbackLayout->addWidget(positionLbl);

        positionSlider = new QSlider(verticalLayoutWidget_4);
        positionSlider->setObjectName(QStringLiteral("positionSlider"));
        positionSlider->setOrientation(Qt::Horizontal);

        playbackLayout->addWidget(positionSlider);

        positionSliderLabels = new QHBoxLayout();
        positionSliderLabels->setObjectName(QStringLiteral("positionSliderLabels"));
        close = new QLabel(verticalLayoutWidget_4);
        close->setObjectName(QStringLiteral("close"));

        positionSliderLabels->addWidget(close);

        rough = new QLabel(verticalLayoutWidget_4);
        rough->setObjectName(QStringLiteral("rough"));
        rough->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        positionSliderLabels->addWidget(rough);


        playbackLayout->addLayout(positionSliderLabels);

        verticalLayoutWidget_4->raise();

        horizontalLayout_4->addWidget(playbackGroup);

        recordGroup = new QGroupBox(horizontalLayoutWidget_3);
        recordGroup->setObjectName(QStringLiteral("recordGroup"));
        recordGroup->setMinimumSize(QSize(100, 0));
        verticalLayoutWidget_5 = new QWidget(recordGroup);
        verticalLayoutWidget_5->setObjectName(QStringLiteral("verticalLayoutWidget_5"));
        verticalLayoutWidget_5->setGeometry(QRect(0, 0, 102, 282));
        editRecordingLayout = new QVBoxLayout(verticalLayoutWidget_5);
        editRecordingLayout->setObjectName(QStringLiteral("editRecordingLayout"));
        editRecordingLayout->setContentsMargins(0, 0, 0, 0);
        editRecordingLbl = new QLabel(verticalLayoutWidget_5);
        editRecordingLbl->setObjectName(QStringLiteral("editRecordingLbl"));
        editRecordingLbl->setAlignment(Qt::AlignCenter);

        editRecordingLayout->addWidget(editRecordingLbl);

        editRecordingSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        editRecordingLayout->addItem(editRecordingSpacer);

        cropBtn = new QPushButton(verticalLayoutWidget_5);
        cropBtn->setObjectName(QStringLiteral("cropBtn"));

        editRecordingLayout->addWidget(cropBtn);

        undoBtn = new QPushButton(verticalLayoutWidget_5);
        undoBtn->setObjectName(QStringLiteral("undoBtn"));

        editRecordingLayout->addWidget(undoBtn);

        splitBtn = new QPushButton(verticalLayoutWidget_5);
        splitBtn->setObjectName(QStringLiteral("splitBtn"));

        editRecordingLayout->addWidget(splitBtn);


        horizontalLayout_4->addWidget(recordGroup);

        visualizationLayout = new QVBoxLayout();
        visualizationLayout->setObjectName(QStringLiteral("visualizationLayout"));
        videoFileLayout = new QHBoxLayout();
        videoFileLayout->setObjectName(QStringLiteral("videoFileLayout"));
        filenameLbl = new QLabel(horizontalLayoutWidget_3);
        filenameLbl->setObjectName(QStringLiteral("filenameLbl"));
        filenameLbl->setMaximumSize(QSize(80, 16777215));

        videoFileLayout->addWidget(filenameLbl);

        fileLbl = new QLabel(horizontalLayoutWidget_3);
        fileLbl->setObjectName(QStringLiteral("fileLbl"));

        videoFileLayout->addWidget(fileLbl);


        visualizationLayout->addLayout(videoFileLayout);

        videoPlayer = new QOpenGLWidget(horizontalLayoutWidget_3);
        videoPlayer->setObjectName(QStringLiteral("videoPlayer"));
        videoPlayer->setMinimumSize(QSize(300, 220));

        visualizationLayout->addWidget(videoPlayer);

        videoBarLayout = new QHBoxLayout();
        videoBarLayout->setObjectName(QStringLiteral("videoBarLayout"));
        videoControllerBtn = new QPushButton(horizontalLayoutWidget_3);
        videoControllerBtn->setObjectName(QStringLiteral("videoControllerBtn"));
        videoControllerBtn->setMaximumSize(QSize(30, 30));

        videoBarLayout->addWidget(videoControllerBtn);

        videoSlider = new QSlider(horizontalLayoutWidget_3);
        videoSlider->setObjectName(QStringLiteral("videoSlider"));
        videoSlider->setOrientation(Qt::Horizontal);

        videoBarLayout->addWidget(videoSlider);


        visualizationLayout->addLayout(videoBarLayout);


        horizontalLayout_4->addLayout(visualizationLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 715, 23));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        playbackGroup->setTitle(QString());
        playbackRecordingLbl->setText(QApplication::translate("MainWindow", "Playback Recording", 0));
        suitPlayChk->setText(QApplication::translate("MainWindow", "Play on Suit", 0));
        modeCmb->clear();
        modeCmb->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Step-through mode", 0)
         << QApplication::translate("MainWindow", "Timed mode", 0)
        );
        SpeedFrameLbl->setText(QApplication::translate("MainWindow", "Speed / Frame interval", 0));
        quarterSpeed->setText(QApplication::translate("MainWindow", "1/4x", 0));
        normalSpeed->setText(QApplication::translate("MainWindow", "1x", 0));
        fourSpeed->setText(QApplication::translate("MainWindow", "4x", 0));
        holdFrame->setText(QApplication::translate("MainWindow", "Hold last frame for", 0));
        seconds->setText(QApplication::translate("MainWindow", "sec(s)", 0));
        positionLbl->setText(QApplication::translate("MainWindow", "Position matching tolerance", 0));
        close->setText(QApplication::translate("MainWindow", "close", 0));
        rough->setText(QApplication::translate("MainWindow", "rough", 0));
        recordGroup->setTitle(QString());
        editRecordingLbl->setText(QApplication::translate("MainWindow", "Edit Recording", 0));
        cropBtn->setText(QApplication::translate("MainWindow", "Crop", 0));
        undoBtn->setText(QApplication::translate("MainWindow", "Undo", 0));
        splitBtn->setText(QApplication::translate("MainWindow", "Split", 0));
        filenameLbl->setText(QApplication::translate("MainWindow", "Filename: ", 0));
        fileLbl->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        videoControllerBtn->setText(QApplication::translate("MainWindow", "Play", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
