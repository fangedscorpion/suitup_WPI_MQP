#include "wagfile.h"

WAGFile::WAGFile(QString filename, QString in_description, QString author,
        QVector<QString> in_tags) : description(in_description), author(author), tags(in_tags) {
    //    path = boost::filesystem::path(in_filename);
    //    path.remove_filename();
        name = filename + ".wagz";
}

WAGFile::WAGFile(QString filename, QString in_description, QString author,
        QString in_tags) : description(in_description), author(author) {
    //    path = boost::filesystem::path(in_filename);
    //    path.remove_filename();
    tags = in_tags.split("; ").toVector();
    name = filename + ".wagz";
}

void WAGFile::updateTags(QString t) {
    tags = t.split("; ").toVector();
}
