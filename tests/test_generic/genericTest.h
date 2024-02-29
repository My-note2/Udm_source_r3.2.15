
#ifndef GENERICTEST_H
#define GENERICTEST_H

#include <cppunit/extensions/HelperMacros.h>

using namespace std;

namespace UdmTests
{
	class genericTest : public CPPUNIT_NS::TestFixture
	{
		CPPUNIT_TEST_SUITE( genericTest );
#if defined(_WIN32)
		CPPUNIT_TEST( testDOMMGA );
		CPPUNIT_TEST( testMGADOM );
		CPPUNIT_TEST( testMGAMGA );
		CPPUNIT_TEST( testMGAMEM );
		CPPUNIT_TEST( testMEMMGA );
#endif
		CPPUNIT_TEST( testDOMDOM );
		CPPUNIT_TEST( testDOMMEM );
		CPPUNIT_TEST( testMEMMEM );
		CPPUNIT_TEST( testMEMDOM );
		CPPUNIT_TEST_SUITE_END();

		
	private:
		char * getRndFileName();
		void removeFile(const string &pathname);
		bool generictest(const char *, const char *);	//returns false if failed
		public:
#ifdef _WIN32
		void testDOMMGA();
		void testMEMMGA();
		void testMGADOM();
		void testMGAMGA();
		void testMGAMEM();
#endif
		void testDOMDOM();
		void testDOMMEM();
		void testMEMDOM();
		void testMEMMEM();
	};
};
#endif  // GENERICTEST_H
