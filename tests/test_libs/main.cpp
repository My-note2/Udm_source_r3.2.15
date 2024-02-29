#include <cppunit/CompilerOutputter.h>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include "LibsTest.h"

#ifdef _WIN32
#include <crtdbg.h>
#include <signal.h>
#include <windows.h>

int __cdecl CrtReportHook(int reportType, char *message, int *returnValue)
{
	if (reportType == _CRT_ASSERT) {
		DebugBreak();
		return TRUE;
	}
	return FALSE;
}

void __cdecl AbrtHandler(int)
{
	DebugBreak();
}
#endif

int main(int argc, char* argv[])
{
#ifdef _WIN32
	signal(SIGABRT, &AbrtHandler);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
	_set_abort_behavior(0, _WRITE_ABORT_MSG);
	_CrtSetReportHook(&CrtReportHook);
#endif
	if (argc > 1) {
		UdmTests::remove_test_files = false;
	}

	CPPUNIT_NS::Test *suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();

	// Adds the test to the list of test to run
	CPPUNIT_NS::TextUi::TestRunner runner;
	runner.addTest( suite );

	// Change the default outputter to a compiler error format outputter
	runner.setOutputter( new CPPUNIT_NS::CompilerOutputter( &runner.result(),
                                                       std::cerr ) );
	// Run the test.
	bool wasSucessful = runner.run();

	// Return error code 1 if the one of test failed.
	return wasSucessful ? 0 : 1;
	
}
