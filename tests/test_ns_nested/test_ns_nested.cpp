#include "..\test_ns\main.cpp"


#include <Uml.h>

#include <string>
#include <cppunit/extensions/HelperMacros.h>

#include "MetaLevel3.h"

using namespace std;

class TestNSNestedTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestNSNestedTest );
	CPPUNIT_TEST(writeTest);
	CPPUNIT_TEST_SUITE_END();
public:
#ifdef _WIN32
	void writeTest();
#endif
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestNSNestedTest);

#ifdef _WIN32
void TestNSNestedTest::writeTest()
{

	Udm::SmartDataNetwork out(MetaLevel3::diagram);
	out.CreateNew("test_level3.mga", "MetaLevel3", MetaLevel3::RootFolder::meta, Udm::CHANGES_LOST_DEFAULT);

	auto model = MetaLevel3::NS_Level2::NS_Level1::Level1_Model::Create(out.GetRootObject());
	auto dst = MetaLevel3::NS_Level2::NS_Level1::Level1_Atom_dest::Create(model);
	auto src = MetaLevel3::NS_Level2::NS_Level1::Level1_Atom_src::Create(model);
	auto conn = MetaLevel3::NS_Level2::NS_Level1::Level1_Connection::Create(model);
	conn.dstLevel2_Connection_end() = dst;
    conn.srcLevel2_Connection_end() = src;

	CPPUNIT_ASSERT(static_cast< ::MetaLevel3::NS_Level2::NS_Level1::Level1_Atom_dest>(conn.dstLevel2_Connection_end()) == dst);
    CPPUNIT_ASSERT(static_cast< ::MetaLevel3::NS_Level2::NS_Level1::Level1_Atom_src>(conn.srcLevel2_Connection_end()) == src);

	out.CloseWithUpdate();
}
#endif
