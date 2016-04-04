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

#define MOTION_LIBRARY "../WAG_bands/.WAGConfig"


enum SAVE_LOCATION {LIBRARY, LOCALLY};
enum SNAP_CLOSENESS {
    BEFORE,
    AFTER,
    CLOSEST
};

class WAGFile: public QObject {
    Q_OBJECT
public:

    WAGFile(QString filename, QString description, QString author,
            QVector<QString> in_tags, SAVE_LOCATION saveLoc);
    WAGFile(QString filename, QString description, QString author,
            QHBoxLayout* container, SAVE_LOCATION saveLoc);
    WAGFile(QString filename, SAVE_LOCATION saveLoc, bool peek = false); // Loads WAGFile content from the given filename.
    ~WAGFile();
    // getters
    // returns filename without extension
    QString getName() {return name;}
    QString getDescription() {return description;}
    QString getAuthor() {return author;}
    QString getPathWithoutFile() {return QString(path.parent_path().c_str()); }
    // returns entire filepath including filename and extension
    QString getPathString() {return QString(path.c_str());}
    QVector<QString> getTagStrings() {return tags;}
    QString getTagString() {return getTagStrings().toList().join(", ");}
    QVector<ClosableLabel*> getTagLabels();
    SAVE_LOCATION getSaveLocation() {return saveLoc;}
    qint32 getFrameNums();
    QHash<qint32, PositionSnapshot*> getMotionData();
    PositionSnapshot *getSnapshot(float approxPercentThroughFile, qint32 snapTime, SNAP_CLOSENESS retrieveType);
    QHash<qint32, PositionSnapshot*> getChunkInRange(qint32 startTime, qint32 endTime);
    void serializeHashmap(QDataStream *ds);
    QHash<qint32, PositionSnapshot *> deserialize(QDataStream *ds);
    // setters
    void updateFilename(QString newName);
    void updateDescription(QString desc) {description = desc;}
    void updateAuthor(QString auth) {author = auth;}
    void updateTags(QHBoxLayout* container);
    void updateSaveLocation(SAVE_LOCATION l) {saveLoc = l;}
    void updateMotionData(QHash<qint32, PositionSnapshot*> newMotionData);
    void updateWAGFile(QString filename, QString description, QString author,
                       QHBoxLayout* container, SAVE_LOCATION saveLoc);
    void saveToFile();

signals:
    void framesChanged(qint32);

private:
    // the path to the file (includes the file)
    boost::filesystem::path path;
    // the file's actual name x.wagz
    QString name;
    QString description;
    QString author;
    QVector<QString> tags;
    SAVE_LOCATION saveLoc;
    QHash<qint32, PositionSnapshot*> motionData;
    QList<qint32> keys;

    void clearHashmapData(QHash<qint32, PositionSnapshot *> data);
    qint32 pickValue(qint32 target, SNAP_CLOSENESS retrType, qint32 beforeTarget, qint32 afterTarget);

    void setFilenameAndPath(QString filename);
    void loadFromFile(QString f);
    void loadMetadataFromFile(QString f);

    static const qint32 fileEndInt = -1;
};
