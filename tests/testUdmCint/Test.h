
#ifndef UNITTEST_H
#define UNITTEST_H

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

namespace UdmTests
{
	class Test : public CPPUNIT_NS::TestFixture
	{
		CPPUNIT_TEST_SUITE( Test );
		CPPUNIT_TEST( test );
		CPPUNIT_TEST_SUITE_END();

    protected:
      void readFromFile(std::istream& in, std::string& str);
      void testRead(const std::string& result);
      void testReadSBML2Ex(const std::string& result);
      void testWrite(std::string& result);

		public:
		void test();
	};
};
#endif  //UNITTEST_H
