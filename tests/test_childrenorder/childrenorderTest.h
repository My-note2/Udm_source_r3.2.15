
#ifndef CHILDRENORDERTEST_H
#define CHILDRENORDERTEST_H

#include <cppunit/extensions/HelperMacros.h>

using namespace std;

namespace UdmTests
{
	class childrenorderTest : public CPPUNIT_NS::TestFixture
	{
		CPPUNIT_TEST_SUITE( childrenorderTest );
		CPPUNIT_TEST( testDOM );
		CPPUNIT_TEST( testMEM );
		CPPUNIT_TEST_SUITE_END();

		
	private:
		std::string getRndFileName();
		void removeFile(const string &pathname);
		bool ordertest(const char *);	//returns false if failed
		public:
		void testDOM();
		void testMEM();
		//void testMGA();
	};
};
#endif  // CHILDRENORDERTEST_H
