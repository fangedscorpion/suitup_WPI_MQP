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
    QString getName() {return name;}
    QString getDescription() {return description;}
    QString getAuthor() {return author;}
    QVector<QString> getTags() {return tags;}
    void updateFilename(QString newName) { name = newName;}

private:
    // the path to the file location
//    boost::filesystem::path path;
    // the file's actual name x.wagz
    QString name;
    QString description;
    QString author;
    QVector<QString> tags;
};
