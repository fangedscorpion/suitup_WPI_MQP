#pragma once
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include <string>
#include <QString>
#include <QVector>

class WAGFile {
public:

    WAGFile(QString filename, QString description, QString author,
            QVector<QString> in_tags);
    WAGFile(QString filename, QString description, QString author,
            QString in_tags);
    QString getName() {return name;}
    QString getDescription() {return description;}
    QString getAuthor() {return author;}
    QVector<QString> getTags() {return tags;}
    // doesn't return anything...
//    QString* getTagsString() { return tags.toList().join("; "); }
    void updateFilename(QString newName) { name = newName;}
    void updateDescription(QString desc) {description = desc;}
    void updateTags(QVector<QString> t) {tags = t;}
    void updateTags(QString t);

private:
    // the path to the file location
//    boost::filesystem::path path;
    // the file's actual name x.wagz
    QString name;
    QString description;
    QString author;
    QVector<QString> tags;
};
