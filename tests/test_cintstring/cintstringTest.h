
#ifndef CINTSTRINGTEST_H
#define CINTSTRINGTEST_H

#include <cppunit/extensions/HelperMacros.h>

namespace UdmTests
{
	class cintstringTest : public CPPUNIT_NS::TestFixture
	{
		CPPUNIT_TEST_SUITE( cintstringTest );
		CPPUNIT_TEST( test );
		CPPUNIT_TEST_SUITE_END();

		public:
		void test();
	};
};
#endif  // CINTSTRINGTEST_H
