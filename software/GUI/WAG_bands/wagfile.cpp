#include "wagfile.h"
#include <QDebug>

WAGFile::WAGFile(QString filename, QString in_description, QString author,
                 QVector<QString> in_tags) : description(in_description), author(author), tags(in_tags) {
    setFilenameAndPath(filename);
}

WAGFile::WAGFile(QString filename, QString in_description, QString author,
                 QString in_tags) : description(in_description), author(author) {
    tags = in_tags.split("; ").toVector();
    setFilenameAndPath(filename);
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
