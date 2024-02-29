#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#if _MSC_VER < 1800
#include <xdebug>
#endif

#include "Windows.h"

#include <cppunit/extensions/HelperMacros.h>

#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#define new DEBUG_NEW


namespace UdmTests
{
	class StaticLeaksTest : public CPPUNIT_NS::TestFixture
	{
		CPPUNIT_TEST_SUITE( StaticLeaksTest );
		CPPUNIT_TEST( testStaticLeaks );
		CPPUNIT_TEST_SUITE_END();

	public:
		void testStaticLeaks() {
	CoInitialize(0);

	typedef void (*init_t)(void*);
	init_t init;
	new int;
	_CrtMemState initState;
	new double; _CrtMemCheckpoint(&initState);
	_CrtMemState dllState;

	HMODULE test_staticleaksdll = LoadLibrary("test_staticleaksdll.dll");
	if (!test_staticleaksdll)
		DebugBreak();
#if defined(_M_AMD64)
	init = (init_t)GetProcAddress(test_staticleaksdll , "?init@@YAXPEAX@Z");
#elif defined(_M_IX86)
	init = (init_t)GetProcAddress(test_staticleaksdll , "?init@@YAXPAX@Z");
#endif
	if (!init)
		DebugBreak();
	init(&dllState);

	typedef void (*terminate_t)();
	HMODULE xerces = GetModuleHandle("XERCES-C_2_8D.DLL");
	if (xerces) {
		terminate_t terminate = (terminate_t)GetProcAddress(xerces, "?Terminate@XMLPlatformUtils@xercesc_2_8@@SAXXZ");
		if (terminate) {
			terminate();
		}
	}
	FreeLibrary(test_staticleaksdll);

	// To test leak detection:
	// new int;
	//_malloc_dbg(13, _CLIENT_BLOCK, __FILE__, __LINE__);
	//_malloc_dbg(14, _NORMAL_BLOCK, __FILE__, __LINE__);
	_CrtMemState finalState;
	_CrtMemCheckpoint(&finalState);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtMemDumpAllObjectsSince(&initState);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	_CrtMemState diff;
	if (_CrtMemDifference(&diff, &initState, &finalState)) {
		CPPUNIT_ASSERT_MESSAGE("Memory leak detected", 0);
	}
	int flag = _CrtSetDbgFlag(0x21);
	_CrtSetDbgFlag(flag | _CRTDBG_LEAK_CHECK_DF);

		}
	};
};

// Registers the fixture into the 'registry'

CPPUNIT_TEST_SUITE_REGISTRATION( UdmTests::StaticLeaksTest );
