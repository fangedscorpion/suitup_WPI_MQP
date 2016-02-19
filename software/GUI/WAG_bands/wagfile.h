#pragma once
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include <string>
#include <QString>
#include <QVector>
#include <QHash>
#include <QHBoxLayout>
#include "positionsnapshot.h"
#include "customWidgets/closablelabel.h"

enum SAVE_LOCATION {LIBRARY, LOCALLY};
enum SNAP_CLOSENESS {
    BEFORE,
    AFTER,
    CLOSEST
};

class WAGFile {
public:

    WAGFile(QString filename, QString description, QString author,
            QVector<QString> in_tags);
    WAGFile(QString filename, QString description, QString author,
            QHBoxLayout* container);
    WAGFile(QString filename); // Loads WAGFile content from the given filename.
    QString getName() {return name;}
    QString getDescription() {return description;}
    QString getAuthor() {return author;}
    QVector<QString> getTagStrings() {return tags;}
    QVector<ClosableLabel*> getTagLabels();
    SAVE_LOCATION getSaveLocation() {return loc;}
    void updateFilename(QString newName) { name = newName;}
    void updateDescription(QString desc) {description = desc;}
    void replaceTags(QHBoxLayout* container);
    void updateSaveLocation(SAVE_LOCATION l) {loc = l;}
    qint32 getFrameNums();
    void setMotionData(QHash<qint32, PositionSnapshot> newMotionData);
    QHash<qint32, PositionSnapshot> getMotionData();

    PositionSnapshot getSnapshot(qint32 snapTime, SNAP_CLOSENESS retrieveType);
    QHash<qint32, PositionSnapshot> getChunkInRange(qint32 startTime, qint32 endTime);

    void saveToFile();

private:
    // the path to the file (includes the file)
    boost::filesystem::path path;
    // the file's actual name x.wagz
    QString name;
    QString description;
    QString author;
    QVector<QString> tags;
    SAVE_LOCATION loc;
    QHash<qint32, PositionSnapshot> motionData;

    void setFilenameAndPath(QString filename);
    void loadFromFile(QString f);
};
