#pragma once
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include <string>
#include <QString>
#include <QVector>
#include <QHash>
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
            QVector<QObject*> in_tags);
    // Loads WAGFile content from the given filename.
    WAGFile(QString filename);
    QString getName() {return name;}
    QString getDescription() {return description;}
    QString getAuthor() {return author;}
    QVector<ClosableLabel*> getTags() {return tags;}
    SAVE_LOCATION getSaveLocation() {return loc;}
    // doesn't return anything...
//    QString* getTagsString() { return tags.toList().join("; "); }
    void updateFilename(QString newName) { name = newName;}
    void updateDescription(QString desc) {description = desc;}
    void replaceTags(QVector<QObject*> t);
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
    QVector<ClosableLabel*> tags;
    SAVE_LOCATION loc;
    QHash<qint32, PositionSnapshot> motionData;

    void setFilenameAndPath(QString filename);
    void loadFromFile(QString f);
};
