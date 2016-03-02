#include "wagfile.h"
#include <QDebug>
#include <QFile>

#include <fstream>

QDataStream & operator>>(QDataStream & str, SAVE_LOCATION & v) {
    unsigned int loc = 0;
    str >> loc;
    if (loc)
        v = static_cast<SAVE_LOCATION>(loc);
    return str;
}


WAGFile::WAGFile(QString filename, QString in_description, QString author,
                 QVector<QString> in_tags, SAVE_LOCATION saveLoc) : description(in_description),
    author(author), tags(in_tags), loc(saveLoc), QObject() {
    setFilenameAndPath(filename);
    motionData = QHash<qint32, PositionSnapshot>();
    keys = QList<qint32>();
}

WAGFile::WAGFile(QString filename, QString in_description, QString author,
                 QHBoxLayout* container, SAVE_LOCATION saveLoc) : description(in_description),
    author(author), loc(saveLoc) {
    tags = QVector<QString>();
    replaceTags(container);
    setFilenameAndPath(filename);
    motionData = QHash<qint32, PositionSnapshot>();
    keys = QList<qint32>();
}

// If peek is true, this loads only the metadata, not the whole file.
WAGFile::WAGFile(QString filename, bool peek) {
    setFilenameAndPath(filename);
    motionData = QHash<qint32, PositionSnapshot>();
    keys = QList<qint32>();
    tags = QVector<QString>();
    loc = LOCALLY;

    if (peek)
        loadMetadataFromFile(filename);
    else
        loadFromFile(filename);
}

void WAGFile::replaceTags(QHBoxLayout* c) {
    tags.clear();
    int i;
    for (i=0; i < c->count(); i++) {
        tags.push_back(static_cast<ClosableLabel*>((c->itemAt(i)->widget()))->getText());
    }
}

QVector<ClosableLabel*> WAGFile::getTagLabels() {
    QVector<ClosableLabel*> c = QVector<ClosableLabel*>();
    for (int i=0; i < tags.size(); i++) {
        c.push_back(new ClosableLabel(tags[i]));
    }
    return c;
}

void WAGFile::setFilenameAndPath(QString filename) {
    path = boost::filesystem::path(filename.toStdString());
    if (!path.has_extension())
        path += ".wagz";

    if (loc == LOCALLY) {
        // if filename does not include the full path, add the current working directory
        if (!path.has_parent_path()) {
            qDebug() << "creating parent path";
            char *buffer = (char *) malloc (2048);
            if (getcwd (buffer, 2048) != buffer) {
                // TODO: throw error
            }
            boost::filesystem::path newPath = boost::filesystem::path(buffer);
            newPath += "/";
            newPath += path;
            free (buffer);
            path = newPath;
        }
    } else {
        if (!path.has_parent_path()) {
            std::ifstream myfile;
            myfile.open ("../WAG_bands/.WAGConfig");
            std::string library;
            std::getline(myfile, library);
            myfile.close();

            boost::filesystem::path newPath = boost::filesystem::path(library.c_str());
            newPath += "/";
            newPath += path;
            path = newPath;
        }
    }
    name = path.filename().replace_extension("").c_str();
}

qint32 WAGFile::getFrameNums() {
    // depends on if we're storing time or frame number
    // if frame number, just get max key
    // if time, need to convert to frames and then return

    QList<qint32> keys = motionData.keys();
    if (keys.size() == 0) {
        return 0;
    }
    QList<qint32>::iterator it = std::max_element(keys.begin(), keys.end());
    qint32 maxKey = *it;

    return maxKey; // TODO fix this
}

PositionSnapshot WAGFile::getSnapshot(float approxPercentThroughFile, qint32 snapTime, SNAP_CLOSENESS retrieveType) {
    if (motionData.contains(snapTime)) {
        return motionData[snapTime];
    }
    if (keys.size() == 0) {
        return PositionSnapshot();
        // not sure what to do here....
    } else if (keys.size() == 1) {
        return motionData[keys[0]];
    }

    int checkIndex = keys.size() * approxPercentThroughFile;
    int incVal;
    qint32 closestKey = keys[checkIndex];
    if (keys[checkIndex] < snapTime) {
        incVal = 1;
    }
    else {
        incVal = -1;
    }

    bool found = false;
    int keySize = keys.size();
    while (!found) {
        if ((checkIndex == 0) && (incVal < 0)) {
            closestKey = pickValue(snapTime, retrieveType, keys[checkIndex], keys[checkIndex + incVal]);
            found = true;
        } else if ((checkIndex == (keySize - 1)) && (incVal > 0)) {
            closestKey = pickValue(snapTime, retrieveType, keys[checkIndex + incVal], keys[checkIndex]);
            found = true;
        } else {
            // both greater than snaptime or both less than snaptime
            if (((keys[checkIndex] - snapTime) < 0) == ((keys[checkIndex + incVal] - snapTime) < 0)) {
                checkIndex += incVal;
            } else {
                if (incVal < 0) {
                    closestKey = pickValue(snapTime, retrieveType, keys[checkIndex + incVal], keys[checkIndex]);
                } else {
                    closestKey = pickValue(snapTime, retrieveType, keys[checkIndex], keys[checkIndex + incVal]);
                }
                found = true;
            }
        }
    }
    return motionData[closestKey];
}

qint32 WAGFile::pickValue(qint32 target, SNAP_CLOSENESS retrType, qint32 beforeTarget, qint32 afterTarget) {
    if (retrType == BEFORE)    {
        return beforeTarget;
    }
    if (retrType == AFTER) {
        return afterTarget;
    }
    if ((afterTarget - target) < (target - beforeTarget)) {
        return afterTarget;
    }
    return beforeTarget;
}




void WAGFile::setMotionData(QHash<qint32, PositionSnapshot> newMotionData) {
    motionData = newMotionData;
    keys = motionData.keys();
    qDebug()<<"WAGFile: Motion data size: "<<keys.size();
    qSort(keys);
    emit framesChanged(this->getFrameNums());
}

QHash<qint32, PositionSnapshot> WAGFile::getMotionData() {
    return motionData;
}

QHash<qint32, PositionSnapshot> WAGFile::getChunkInRange(qint32 startTime, qint32 endTime) {
    QHash<qint32, PositionSnapshot> motionDataCopy = QHash<qint32, PositionSnapshot>(motionData);
    for (int i = 0; i < keys.length(); i++) {
        if ((keys[i] < startTime) || (keys[i] > endTime)) {
            motionDataCopy.remove(keys[i]);
        }
    }

    return motionDataCopy;
}

void WAGFile::saveToFile() {
    QFile file(path.c_str());
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_5);
    out << description;
    out << author;
    out << tags;
    out << loc;
    //    out << motionData;

    qDebug() << "path: " << path.c_str();
    qDebug() << "name: " << name.toStdString().c_str();
    qDebug() << "desc: " << description.toStdString().c_str();
    qDebug() << "author: " << author.toStdString().c_str();
    qDebug() << "loc: " << loc;
    qDebug() << "tags: " << tags;


    file.flush();
    file.close();
}

void WAGFile::loadFromFile(QString f) {
    QFile file(f);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_5);
    in >> description;
    in >> author;
    in >> tags;
    in >> loc;
    //    in >> motionData;

    qDebug() << "path: " << path.c_str();
    qDebug() << "name: " << name.toStdString().c_str();
    qDebug() << "desc: " << description.toStdString().c_str();
    qDebug() << "author: " << author.toStdString().c_str();
    qDebug() << "loc: " << loc;
    qDebug() << "tags: " << tags;

    file.close();
}

void WAGFile::loadMetadataFromFile(QString f) {
    QFile file(f);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_5);
    in >> description;
    in >> author;
    in >> tags;
    in >> loc;

    file.close();
}
