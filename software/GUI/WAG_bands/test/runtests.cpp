#include "testreflection.h"
#include "testquatpose.h"
#include "testquatstate.h"
#include "testquaterror.h"
#include "testwagfile.h"

void runtests(int argc, char *argv[]){

    TestReflection test1;
    QTest::qExec(&test1, argc, argv);

    TestQuatPose test2;
    QTest::qExec(&test2, argc, argv);

    TestQuatState test3;
    QTest::qExec(&test3, argc, argv);

    TestQuatError test4;
    QTest::qExec(&test4, argc, argv);

    QApplication a(argc, argv);

    TestWAGFile test5;
    QTest::qExec(&test5, argc, argv);

}
