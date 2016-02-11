#include "testreflection.h"

void runtests(int argc, char *argv[]){

    TestReflection test1;
    QTest::qExec(&test1, argc, argv);

    // TestSomething test2;
    // QTest::qExec(&test2, argc, argv);

    // TestSomethingElse test3;
    // QTest::qExec(&test3, argc, argv);



}
