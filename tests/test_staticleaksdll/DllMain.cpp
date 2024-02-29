#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include "../test_refports/RefPort.h"
#include "LampDiagram.h"

#include "UdmBase.h"
#include "UdmStatic.h"
#include "Uml.h"

int main(int argc, char* argv[]);

__declspec(dllexport) void init(void* data) {
	_CrtMemState* state = (_CrtMemState*) data;

	Uml::Initialize();
	//RefPort::Initialize();
	LampDiagram::Initialize();
	UdmUtil::stacktrace();
#if defined(_M_IX86) // fails with cppunit dll, since there is only one test registry
	main(0, NULL);
#endif
}