
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
      void readSBML(const std::string& fname);
      void readTest();
      void selfTest();
      void evaluate(const ::Uml::Diagram& metaDiagram,
              const Udm::Object& objContext,
              std::ostream& out);
    
    void read(const std::string& fname,
              std::ostream& out);

		public:
		void test();
	};
};
#endif  //UNITTEST_H
