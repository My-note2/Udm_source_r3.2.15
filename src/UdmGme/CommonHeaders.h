#define STRICT
#ifndef _WIN32_WINNT
#if _MSC_VER >= 1500
#define _WIN32_WINNT 0x0500		// Windows 2000 is the minimal version supported by VS 2008
#else
#define _WIN32_WINNT 0x0400
#endif
#endif
#include <windows.h>
#include <cstdio>
#include <tchar.h>
//#include <objbase.h>
//#include <atlbase.h>
//extern CComModule _Module;
//#include <atlcom.h>
//CComModule _Module;

/*
24/09/04	-	endre

			- removed  the UdmGme.h #include directive, and added the only function which is needed from that namespace
*/

// KMS: these renames are for GME r1365 (Work around "MIDL changes the case of identifier in generated type library" http://support.microsoft.com/kb/q220137/)
#if !defined(__INTELLISENSE__)
#import "file:Core.dll" version("1.0") no_implementation
#import "file:Meta.dll" version("1.0") no_implementation rename("Kind", "kind")
#import "file:Mga.dll" version("1.0") no_implementation rename("Dst", "dst") rename("Src", "src") rename("Kind", "kind")
#else
// IntelliSense has a known issue with the above lines.
//  http://connect.microsoft.com/VisualStudio/feedback/details/533526/vc-2010-intellisense-import-directive-using-libid-does-not-work
#ifdef _DEBUG
// If IntelliSense reports "Cannot open source file", compile then reopen the project
// KMS: VS intellisense has a hard time loading from typelib GUID. This fixes it:
#include "..\..\Build\Win32\v100\Debug\obj\src\UdmGme\Core.tlh"
#include "..\..\Build\Win32\v100\Debug\obj\src\UdmGme\Mga.tlh"
#include "..\..\Build\Win32\v100\Debug\obj\src\UdmGme\Meta.tlh"
#else
#include "..\..\Build\Win32\v100\Release\obj\src\UdmGme\Core.tlh"
#include "..\..\Build\Win32\v100\Release\obj\src\UdmGme\Mga.tlh"
#include "..\..\Build\Win32\v100\Release\obj\src\UdmGme\Meta.tlh"
#endif
#endif


//only this is needed from the UdmGme namespace.
namespace UdmGme
{
	void com_exception(HRESULT a, IUnknown *b, REFIID c);
	void com_exception(HRESULT a, IErrorInfo *errorinfo);
};



#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


#include "Uml.h"
#include "UmlExt.h"

#define _com_issue_errorex(a, b, c) UdmGme::com_exception(a, b, c)
