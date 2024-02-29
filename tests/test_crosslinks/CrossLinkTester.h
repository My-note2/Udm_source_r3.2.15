
#ifndef CROSSLINKTESTER_H
#define CROSSLINKTESTER_H

#include <cppunit/extensions/HelperMacros.h>

namespace UdmTests
{
	class CrossLinkTester : public CPPUNIT_NS::TestFixture
	{
		CPPUNIT_TEST_SUITE( CrossLinkTester );
		CPPUNIT_TEST( test );
		CPPUNIT_TEST_SUITE_END();

		public:
		void test();
	};
};
#endif  // CROSSLINKTESTER_H
