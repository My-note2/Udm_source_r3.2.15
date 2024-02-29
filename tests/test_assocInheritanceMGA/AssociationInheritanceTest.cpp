// AssociationInheritanceTest.cpp: implementation of the AssociationInheritanceTest class.
//
//////////////////////////////////////////////////////////////////////

#include "AssociationInheritanceTest.h"

#ifdef _WIN32
UDM_USE_MGA
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace SignalFlow;

CPPUNIT_TEST_SUITE_REGISTRATION(CAssociationInheritanceTest);

CAssociationInheritanceTest::CAssociationInheritanceTest()
{

}

CAssociationInheritanceTest::~CAssociationInheritanceTest()
{

}

void CAssociationInheritanceTest::TestAssociationInheritance()
{
	try {
	Udm::SmartDataNetwork nw1(diagram);
	nw1.OpenExisting("sf1.mga");

	RootFolder rf = RootFolder::Cast(nw1.GetRootObject());
	set<CompoundComponent> cps = rf.CompoundComponent_children();
	CompoundComponent cp = *(cps.begin());
	
	set<LocalPort> lps = cp.LocalPort_children();
	LocalPort lp = *(lps.begin());

	set<BaseSignal> bsfs = lp.dst();
	BaseSignal bsf = *(bsfs.begin());
	//cout<<(string)bsf.type().name()<<endl;    //It is supposed to be Signal
	Signal sf = Signal::Cast(bsf);	//It failed, before bug was fixed would throw invalid cast

	nw1.CloseNoUpdate();
	}
	catch(const udm_exception &e)	{
		CPPUNIT_ASSERT_MESSAGE(e.what(), 0);
	}
}

void CAssociationInheritanceTest::setUp()
{
}

void CAssociationInheritanceTest::tearDown()
{
}
