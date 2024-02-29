#ifndef REFPORTSTEST_H
#define REFPORTSTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <string>

using namespace std;

namespace UdmTests
{
	class refPortsTest : public CPPUNIT_NS::TestFixture
	{
		CPPUNIT_TEST_SUITE( refPortsTest );
		CPPUNIT_TEST(testDOM);
		CPPUNIT_TEST(testMEM);
#if defined(_WIN32)
		CPPUNIT_TEST(testMGA);
#endif
		CPPUNIT_TEST_SUITE_END();

	public:
		void testRefPorts(const string &file);
		void testDOM();
		void testMEM();
#ifdef _WIN32
		void testMGA();
#endif
	};
};

#endif // REFPORTSTEST_H