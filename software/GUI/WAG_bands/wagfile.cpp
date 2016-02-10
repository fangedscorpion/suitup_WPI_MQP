#include "wagfile.h"
#include <QDebug>

WAGFile::WAGFile(QString filename, QString in_description, QString author,
                 QVector<QString> in_tags) : description(in_description), author(author), tags(in_tags) {
    setFilenameAndPath(filename);
    motionData = QHash<qint32, PositionSnapshot>();
}

WAGFile::WAGFile(QString filename, QString in_description, QString author,
                 QString in_tags) : description(in_description), author(author) {
    tags = in_tags.split("; ").toVector();
    setFilenameAndPath(filename);
    motionData = QHash<qint32, PositionSnapshot>();
    qDebug()<<motionData.isEmpty();
}

void WAGFile::updateTags(QString t) {
    tags = t.split("; ").toVector();
}

void WAGFile::setFilenameAndPath(QString filename) {
    path = boost::filesystem::path(filename.toStdString());
    if (!path.has_extension())
        path += ".wagz";

    if (!path.has_root_path()) {
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
    name = path.filename().c_str();
}

qint32 WAGFile::getFrameNums() {
    // depends on if we're storing time or frame number
    // if frame number, just get max key
    // if time, need to convert to frames and then return

    QList<qint32> keys = motionData.keys();
    qDebug("GETTIG KEYS\n\nasdfas\n\n");
    if (keys.size() == 0) {
        return 0;
    }
    QList<qint32>::iterator it = std::max_element(keys.begin(), keys.end());
    qint32 maxKey = *it;

    return maxKey; // TODO fix this
}

PositionSnapshot WAGFile::getSnapshot(qint32 snapTime, SNAP_CLOSENESS retrieveType) {
    qDebug("HERE82");
    qDebug()<<&motionData;
    motionData[1] = PositionSnapshot();
    qDebug()<<"HERE2312";
    motionData = QHash<qint32, PositionSnapshot>();
    qDebug()<<motionData.size();
    if (motionData.contains(snapTime)) {
        qDebug("HERE238");
        return motionData[snapTime];
    }
    qDebug("HERE@#!");
    QList<qint32> keys = motionData.keys();
    if (keys.size() == 0) {
        qDebug("HERE@!#");
        return PositionSnapshot();
        // not sure what to do here....
    }
    qDebug("HERE@#I@O");
    qSort(keys);

    qint32 closestKey = 0;

    qDebug("HERE24");
    for (int i = 0; i < keys.size(); i++) {
        if (keys[i] > snapTime) {
            if (retrieveType == AFTER) {
                closestKey = keys[i];
                break;
            }
            else if (retrieveType == BEFORE) {
                if (i == 0) {
                    closestKey = keys[i];
                } else {
                    closestKey = keys[i-1];
                }
                break;
            } else {
                if (i == 0) {
                    closestKey = keys[i];
                } else {
                    qint32 afterKey = keys[i];
                    qint32 beforeKey = keys[i - 1];
                    if ((afterKey - snapTime) < (snapTime - beforeKey)) {
                        closestKey = afterKey;
                    } else {
                        closestKey = beforeKey;
                    }
                    break;
                }
            }
        } else if (i == (keys.size() - 1)) {
            closestKey = keys[i];
        }
    }
    qDebug("HErew8293");
    return motionData[closestKey];
}

void WAGFile::setMotionData(QHash<qint32, PositionSnapshot> newMotionData) {
    motionData = newMotionData;
}

QHash<qint32, PositionSnapshot> WAGFile::getMotionData() {

}