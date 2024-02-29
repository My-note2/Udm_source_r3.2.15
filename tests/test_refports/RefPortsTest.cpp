#include "RefPortsTest.h"
#include "RefPort.h"
#include <iostream>

#include <UdmUtil.h>

// Registers the fixture into the 'registry'

CPPUNIT_TEST_SUITE_REGISTRATION( UdmTests::refPortsTest );

using namespace RefPort;
using namespace Udm;

void UdmTests::refPortsTest::testDOM()
{
	cout << endl << "Reference ports on DOM" << endl;
	testRefPorts("RefPortInst.xml");
}

void UdmTests::refPortsTest::testMEM()
{
	cout << endl << "Reference ports on MEM" << endl;
	testRefPorts("RefPortInst.mem");
}

#ifdef _WIN32
void UdmTests::refPortsTest::testMGA()
{
	cout << endl << "Reference ports on MGA" << endl;
	testRefPorts("RefPortInst.mga");
}
#endif

void UdmTests::refPortsTest::testRefPorts(const string &file)
{
	Udm::SmartDataNetwork dn(RefPort::diagram);
	dn.CreateNew(file, "RefPort", RootFolder::meta, Udm::CHANGES_LOST_DEFAULT);

	//creating root folder
	RootFolder rf = RootFolder::Cast(dn.GetRootObject());
	rf.name() = "RootFolder";

	//creating root models
	Root root = Root::Create(rf);
	root.name() = "root";
	Root root_refs = Root::Create(rf);
	root_refs.name() = "root_refs";
	Root root_refs2 = Root::Create(rf);
	root_refs2.name() = "root_refs2";

	// create reference port parents
	APar apar = APar::Create(root);
	apar.name() = "apar";

	BPar bpar = BPar::Create(root);
	bpar.name() = "bpar";

	BPar bpar2 = BPar::Create(root);
	bpar2.name() = "bpar2";


	ARef1::Create(root_refs).ref() = apar;
	// create references
	ARef1 aref1 = ARef1::Create(root_refs);
	aref1.name() = "aref1";

	ARef1::Create(root_refs).ref() = apar;

	BRef::Create(root_refs).ref() = bpar;
	BRef::Create(root_refs).ref() = bpar2;
	
	BRef bref = BRef::Create(root_refs);
	bref.name() = "bref";

	BRef::Create(root_refs).ref() = bpar;
	BRef::Create(root_refs).ref() = bpar2;

	BRef::Create(root_refs2).ref() = bpar;
	BRef::Create(root_refs2).ref() = bpar2;

	BRef bref2 = BRef::Create(root_refs2);
	bref2.name() = "bref2";

	BRef::Create(root_refs2).ref() = bpar;
	BRef::Create(root_refs2).ref() = bpar2;
	BRef::Create(root_refs2).ref() = bref2;

	BRef::Create(root_refs).ref() = bpar;
	BRef::Create(root_refs).ref() = bpar2;
	BRef::Create(root_refs).ref() = bref2;


	BRef bref3 = BRef::Create(root_refs);
	bref3.name() = "bref3";

	BRef::Create(root_refs).ref() = bpar;
	BRef::Create(root_refs).ref() = bpar2;
	BRef::Create(root_refs).ref() = bref2;

	aref1.ref() = apar;
	bref.ref() = bpar;
	bref2.ref() = bpar2;
	bref3.ref() = bref2;

	// create ports
	A a = A::Create(apar);
	a.name() = "a";

	B b = B::Create(bpar);
	b.name() = "b";

	B b2 = B::Create(bpar2);
	b2.name() = "b2";

	C c = C::Create(root_refs);
	c.name() = "c";


	// set the refport container preferences
	c.a_end__rp_container() = aref1;
	c.b_end__rp_container() = bref;

	CPPUNIT_ASSERT(ARef1::Cast(c.a_end__rp_container()) == aref1);
	CPPUNIT_ASSERT(BRef::Cast(c.b_end__rp_container()) == bref);

	c.a_end__rp_container() = ARef1::Cast(Udm::null);
	c.b_end__rp_container() = BRef::Cast(Udm::null);

	CPPUNIT_ASSERT(ARef1::Cast(c.a_end__rp_container()) == Udm::null);
	CPPUNIT_ASSERT(BRef::Cast(c.b_end__rp_container()) == Udm::null);

	c.a_end__rp_container() = aref1;
	c.b_end__rp_container() = bref2;
	c.b_end__rp_container() = bref;

	CPPUNIT_ASSERT(ARef1::Cast(c.a_end__rp_container()) == aref1);
	CPPUNIT_ASSERT(BRef::Cast(c.b_end__rp_container()) == bref);

	// we allow aref and bref.__rp_container_rev().size() == 0 before a_ and b_end_end are set

	// connect src and dst
	c.a_end_end() = a;
	c.b_end_end() = b;

	std::set<C> conns;
	ARef1 aref1_ = ARef1::Cast(c.a_end__rp_container());
	conns = aref1_.a_end__rp_container_rev();
	CPPUNIT_ASSERT_EQUAL((size_t)1, conns.size());
	CPPUNIT_ASSERT_EQUAL(c, *conns.begin());

	std::set<C2> conns2;
	BRef bref1_ = BRef::Cast(c.b_end__rp_container());
	conns = bref1_.b_end__rp_container_rev();
	CPPUNIT_ASSERT_EQUAL((size_t)1, conns.size());
	CPPUNIT_ASSERT_EQUAL(c, *conns.begin());
	conns2 = bref1_.dstC2_refport_parent_rev();
	CPPUNIT_ASSERT_EQUAL((size_t)0, conns2.size());


#if 0
	// what should happen when the reference port is connected and the
	// user changes the reference port container preference?
	// - reconnect with the new container?
	// - throw an exception?

	// the problem is caused by the dual nature of reference port container
	// in MGA: either a registry preference or a real thing existing in the model

	// if the original support for refports will be dropped, then we
	// could probably reconnect with the new container...

	c.b_end__rp_container() = bref3;
	CPPUNIT_ASSERT(BRef::Cast(c.b_end__rp_container()) == bref);
	// after port change, the preference becomes visible
	c.b_end_end() = b2;
	CPPUNIT_ASSERT(BRef::Cast(c.b_end__rp_container()) == bref3);
#endif

	set<B> bs;
	conns.clear();
	a.b_end() = conns;

	// connect to another refport using another refport container
	// c.b_end__rp_container() = bref3;
	conns.clear();
	conns.insert(c);
	bref3.b_end__rp_container_rev() = conns;
	c.b_end_end() = b2;
	CPPUNIT_ASSERT_EQUAL(BRef::Cast(c.b_end__rp_container()), bref3);

	// show connecting chains
	{
		C::b_end_chain_t r = c.b_end_chain();
		C::b_end_chain_t::second_type v = r.second;
		CPPUNIT_ASSERT(B::Cast(r.first) == b2);
		cout << "Connecting chain between c and b2: " << v.size() << endl;
		for (vector<Udm::Object>::const_iterator i = v.begin(); i != v.end(); i++) {
			cout << "chain item: " << UdmUtil::ExtractName(*i) << "[" << i->uniqueId() << "]" << endl;
		}
	}

	{
		vector<Udm::Object> r = b2.getConnectingChain(B::meta_a_end_rev, c);
		cout << "Connecting chain between b2 and c: " << r.size() << endl;
		for (vector<Udm::Object>::const_iterator i = r.begin(); i != r.end(); i++) {
			cout << "chain item: " << UdmUtil::ExtractName(*i) << "[" << i->uniqueId() << "]" << endl;
		}
	}

	{
		C::a_end_chain_t r = c.a_end_chain();
		C::a_end_chain_t::second_type v = r.second;
		cout << "Connecting chain between c and a: " << v.size() << endl;
		for (vector<Udm::Object>::const_iterator i = v.begin(); i != v.end(); i++) {
			cout << "chain item: " << UdmUtil::ExtractName(*i) << "[" << i->uniqueId() << "]" << endl;
		}
	}

	{
		vector<Udm::Object> r = a.getConnectingChain(A::meta_b_end_rev, c);
		cout << "Connecting chain between a and c: " << r.size() << endl;
		for (vector<Udm::Object>::const_iterator i = r.begin(); i != r.end(); i++) {
			cout << "chain item: " << UdmUtil::ExtractName(*i) << "[" << i->uniqueId() << "]" << endl;
		}
	}

	// disconnect src
	c.a_end_chain().disconnect();
	CPPUNIT_ASSERT(A::Cast(c.a_end_end()) == Udm::null);
	CPPUNIT_ASSERT(APar::Cast(aref1.ref()) == apar);
	// We don't guarantee either assertions:
	//CPPUNIT_ASSERT(ARef1::Cast(c.a_end__rp_container()) == aref1);
	//CPPUNIT_ASSERT(ARef1::Cast(c.a_end__rp_container()) == Udm::null);
	CPPUNIT_ASSERT(B::Cast(c.b_end_end()) == b2);

	// disconnect dst
	b2.disconnectFrom(B::meta_a_end_rev, c);
	CPPUNIT_ASSERT(B::Cast(c.b_end_end()) == Udm::null);
	CPPUNIT_ASSERT(BPar::Cast(bref2.ref()) == bpar2);
	CPPUNIT_ASSERT(BRef::Cast(bref3.ref()) == bref2);
	// We don't guarantee either assertions:
	//CPPUNIT_ASSERT(BRef::Cast(c.b_end__rp_container()) == bref3);
	//CPPUNIT_ASSERT(BRef::Cast(c.b_end__rp_container()) == Udm::null);


	c.a_end_end() = a;

	// connect dst using references chain
	vector<Udm::Object> v1_refs;
	v1_refs.push_back(bref3);
	v1_refs.push_back(bref2);
	c.b_end_chain() = make_pair(b2, v1_refs);
	CPPUNIT_ASSERT(B::Cast(c.b_end_end()) == b2);
	CPPUNIT_ASSERT(BRef::Cast(c.b_end__rp_container()) == bref3);

	// disconnect dst
	c.b_end_chain().disconnect();
	CPPUNIT_ASSERT(B::Cast(c.b_end_end()) == Udm::null);

	// connect another dst using references chain
	v1_refs.clear();
	v1_refs.push_back(bref);
	c.b_end_chain() = make_pair(b, v1_refs);
	CPPUNIT_ASSERT(B::Cast(c.b_end_end()) == b);
	CPPUNIT_ASSERT(BRef::Cast(c.b_end__rp_container()) == bref);

	// disconnect dst
	c.b_end_chain().disconnect();
	CPPUNIT_ASSERT(B::Cast(c.b_end_end()) == Udm::null);


	// connect dst using reference port helper
	c.b_end__rp_container() = bref;
	c.b_end_chain() = b;
	CPPUNIT_ASSERT(B::Cast(c.b_end_end()) == b);

	// disconnect dst
	c.b_end_chain().disconnect();
	CPPUNIT_ASSERT(B::Cast(c.b_end_end()) == Udm::null);

	// connect another dst using reference port helper
	c.b_end__rp_container() = bref3;
	c.b_end_chain() = b2;
	CPPUNIT_ASSERT(B::Cast(c.b_end_end()) == b2);

	// disconnect dst
	c.b_end_chain().disconnect();
	CPPUNIT_ASSERT(B::Cast(c.b_end_end()) == Udm::null);

	// connect without a reference port helper set
	c.b_end__rp_container() = NULL;
	c.b_end_chain() = b2;
	CPPUNIT_ASSERT(B::Cast(c.b_end_end()) == b2);
	CPPUNIT_ASSERT(BRef::Cast(c.b_end__rp_container()) != Udm::null);

	dn.CloseNoUpdate();
}
