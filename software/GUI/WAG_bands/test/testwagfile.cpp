#include "testwagfile.h"
#include "wagfile.h"

void TestWAGFile::testSetters() {
    QString newName = "nameFile";
    QString newFilepath = "/path/to/new/" + newName;
    QString newDesc = "this is a new desc";
    QString newAuth = "I am the new auth";
    QPointer<QHBoxLayout> newTags = new QHBoxLayout;
    newTags->addWidget(new ClosableLabel("new"));
    newTags->addWidget(new ClosableLabel("ya"));
    QVector<QString> newTagStrings;
    newTagStrings.push_back("new");
    newTagStrings.push_back("ya");
    QString newTagString = "new, ya";
    SAVE_LOCATION newSave = LOCALLY;

    QHBoxLayout *old = new QHBoxLayout;
    old->addWidget(new ClosableLabel("old"));
    WAGFile *w = new WAGFile("/old/file/path/filename", "old description, is so old",
                             "author me", old, LIBRARY);
    w->updateWAGFile(newFilepath, newDesc, newAuth, newTags, newSave);
    QCOMPARE(w->getName(), newName);
    QCOMPARE(w->getPathString(), newFilepath+".wagz");
    QCOMPARE(w->getDescription(), newDesc);
    QCOMPARE(w->getAuthor(), newAuth);
    QCOMPARE(w->getTagStrings(), newTagStrings);
    QCOMPARE(w->getTagString(), newTagString);
    QCOMPARE(w->getSaveLocation(), newSave);
    delete w;
}
