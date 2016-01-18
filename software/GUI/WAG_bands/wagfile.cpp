#include "wagfile.h"

WAGFile::WAGFile(QString filename, QString in_description, QString author,
        QVector<QString> in_tags) : name(filename), description(in_description), author(author), tags(in_tags) {
    //    path = boost::filesystem::path(in_filename);
    //    path.remove_filename();

}
