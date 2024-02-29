
#ifndef CHILDRENORDERTEST_H
#define CHILDRENORDERTEST_H

#include <cppunit/extensions/HelperMacros.h>

using namespace std;

namespace UdmTests
{
	class domstringdnTest : public CPPUNIT_NS::TestFixture
	{
		CPPUNIT_TEST_SUITE( domstringdnTest );
		CPPUNIT_TEST( test );
		CPPUNIT_TEST_SUITE_END();

		
	private:
		public:
		
		void test();	//returns false if failed
		//void testMGA();
	};
};
#endif  // CHILDRENORDERTEST_H
