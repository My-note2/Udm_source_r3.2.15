#ifndef CINTUDMPSEUDOTEST_H
#define CINTUDMPSEUDOTEST_H

#include <cppunit/extensions/HelperMacros.h>

namespace UdmTests
{
	class cintUdmPseudoTest : public CPPUNIT_NS::TestFixture
	{
		CPPUNIT_TEST_SUITE( cintUdmPseudoTest );
		CPPUNIT_TEST( testDOM );
		CPPUNIT_TEST( testMEM );
		CPPUNIT_TEST_SUITE_END();

		
	private:
		char * getRndFileName();
		bool cintudmpseudotest(const char *);	//returns false if failed
		public:
		void testDOM();
		void testMEM();
		//void testMGA();
	};
};
#endif  // CINTUDMPSEUDOTEST_H	
