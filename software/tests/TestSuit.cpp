/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_TestSuit_init = false;
#include "/home/samee/MQP/suitup_WPI_MQP/software/tests/TestSuit.h"

static TestSuit suite_TestSuit;

static CxxTest::List Tests_TestSuit = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSuit( "tests/TestSuit.h", 6, "TestSuit", suite_TestSuit, Tests_TestSuit );

static class TestDescription_suite_TestSuit_testBands : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSuit_testBands() : CxxTest::RealTestDescription( Tests_TestSuit, suiteDescription_TestSuit, 9, "testBands" ) {}
 void runTest() { suite_TestSuit.testBands(); }
} testDescription_suite_TestSuit_testBands;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
