#ifndef LIBSTEST_H
#define LIBSTEST_H

#include <string>
#include <cppunit/extensions/HelperMacros.h>

#include "LampDiagram.h"

using namespace std;

namespace UdmTests
{
	extern bool remove_test_files;

	class LibsTest : public CPPUNIT_NS::TestFixture
	{
		CPPUNIT_TEST_SUITE( LibsTest );
#if defined(_WIN32)
		CPPUNIT_TEST( testMGA );
#endif
		CPPUNIT_TEST( testDOM );
		CPPUNIT_TEST( testMEM );
		CPPUNIT_TEST_SUITE_END();
		
	private:
		void removeFile(const string &pathname);
		void testLibOperations(const string &backend_ext);

		LampDiagram::Lamp createLamp(LampDiagram::RootFolder &root, const string &ns);

	public:
#ifdef _WIN32
		void testMGA();
#endif
		void testDOM();
		void testMEM();
	};
};

#endif	// LIBSTEST_H
