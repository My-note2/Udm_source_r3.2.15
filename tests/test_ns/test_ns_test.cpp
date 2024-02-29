// test_ns.cpp : Defines the entry point for the console application.
//

#include "test_ns.h"
#include <Uml.h>

#include <string>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class TestNSTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestNSTest );
	CPPUNIT_TEST(test_ns);
	CPPUNIT_TEST_SUITE_END();
	void writeTest(const std::string& fname);
	void readTest(const std::string& fname);
public:
	void test_ns();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestNSTest);


void TestNSTest::writeTest(const std::string& fname)
{

	Udm::SmartDataNetwork out(test_ns::diagram);
	out.CreateNew(fname.c_str(), "test_ns_AB.xsd", test_ns::AB::AB::meta, Udm::CHANGES_PERSIST_ALWAYS);

	test_ns::AB::AB ab = test_ns::AB::AB::Cast(out.GetRootObject());

	test_ns::A::A1 a1 = test_ns::A::A1::Create(ab);
	test_ns::A::AA::A1 a_aa1 = test_ns::A::AA::A1::Create(ab);
	test_ns::B::B1 b1 = test_ns::B::B1::Create(ab);
	test_ns::AB::AB1 ab1 = test_ns::AB::AB1::Create(ab);
	test_ns::A::AA::A2 a2 = test_ns::A::AA::A2::Create(ab);
	test_ns::AB::ABAB::AB2 ab2 = test_ns::AB::ABAB::AB2::Create(ab, test_ns::AB::AB::meta_ab2);

	ab2.dst() += a2;

	out.CloseWithUpdate();
}

//==========================================
void TestNSTest::readTest(const std::string& fname)
{

	Udm::SmartDataNetwork in(test_ns::diagram);
	in.OpenExisting(fname.c_str(), "test_ns_AB.xsd", Udm::CHANGES_PERSIST_ALWAYS);

	test_ns::AB::AB ab = test_ns::AB::AB::Cast(in.GetRootObject());

	in.CloseNoUpdate();
}
//=========================================

void TestNSTest::test_ns()
{
		std::string fn ="test.xml";
		writeTest(fn);
		readTest(fn);

		// test initialization from another diagram
		Udm::SmartDataNetwork udmDN(Uml::diagram);
		udmDN.OpenExisting("test_ns_udm.xml", "", Udm::CHANGES_LOST_DEFAULT);
		
		Uml::Diagram theUmlDiagram(Uml::Diagram::Cast(udmDN.GetRootObject()));
		test_ns::Initialize(theUmlDiagram);
}

