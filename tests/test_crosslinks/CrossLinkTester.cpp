#include "CrossLinkTester.h"
#include "CLPackage.h"
#include "CL.h"
#include "Package1.h"
#include "Package2.h"

CPPUNIT_TEST_SUITE_REGISTRATION( UdmTests::CrossLinkTester );

using namespace Package1;
using namespace Package2;
using namespace std;

void UdmTests::CrossLinkTester::test()
{
	try {
		Udm::UdmProject prj;
		//specifying instance data networks which will be present in the project
		Udm::DataNetworkSpecifier dnsP1("Package1_inst.xml", "Package1", ClassA::meta);
		Udm::DataNetworkSpecifier dnsP2("Package2_inst.xml", "Package2", Class1::meta);
		vector<Udm::DataNetworkSpecifier> dnsvec;
		dnsvec.push_back(dnsP1);
		dnsvec.push_back(dnsP2);
		//creating a Udm Project
		prj.CreateNew("test.udm", dnsvec, CL::diagram, Udm::CHANGES_LOST_DEFAULT);
		//obtaining handles to the datanetworks which were created
		Udm::DataNetwork& p1 = prj.GetDataNetwork("Package1_inst.xml");
		Udm::DataNetwork& p2 = prj.GetDataNetwork("Package2_inst.xml");
		//obtaining handles to the root objects (this is normal - existing - Udm operation)
		ClassA rootP1 = ClassA::Cast( p1.GetRootObject());
		Class1 rootP2 = Class1::Cast( p2.GetRootObject());
		// create Class2
		Class2 class2= Class2::Create( rootP2);
		// create Class3
		NSp::NSc::Class3 class3 = NSp::NSc::Class3::Create(rootP2);
		// create cross links
		rootP1.dst1()+= rootP2;
		rootP1.dst1()+= class2;
		rootP1.dst2()= class2;
		rootP1.dst3()+= class3;
		// verification
		std::set< Class1> dst1s= rootP1.dst1();
		CPPUNIT_ASSERT( dst1s.size() == 2);
		CPPUNIT_ASSERT( dst1s.find( rootP2) != dst1s.end());
		CPPUNIT_ASSERT( dst1s.find( class2) != dst1s.end());
		const Class2& dst2= rootP1.dst2();
		CPPUNIT_ASSERT( dst2 == class2);
		std::set< NSp::NSc::Class3> dst3s = rootP1.dst3();
		CPPUNIT_ASSERT( dst3s.size() == 1);
		CPPUNIT_ASSERT( dst3s.find( class3) != dst3s.end());
	} catch( udm_exception &e)
	{
		std::cout << e.what() << std::endl;
		CPPUNIT_ASSERT( false);
	}
}


