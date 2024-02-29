#include "TestGetAdjacentObjects.h"

CPPUNIT_TEST_SUITE_REGISTRATION( UdmTests::TestGetAdjacentObjects );

void UdmTests::TestGetAdjacentObjects::init() {
	//specifying instance data networks which will be present in the project
	Udm::DataNetworkSpecifier pa_dn("PackageA_inst.xml", "PackageA", PackageA::ClassA1Base::meta);
	Udm::DataNetworkSpecifier pb_dn("PackageB_inst.xml", "PackageB", PackageB::BCommonBase::meta);
	//putting them in a vector
	vector<Udm::DataNetworkSpecifier> dnsvec;
	dnsvec.push_back(pa_dn);
	dnsvec.push_back(pb_dn);
	//creating a Udm Project
	_pr.CreateNew("testmodel.udm", dnsvec,testAssociationScenarios::diagram);
	//obtaining handles to the datanetworks which were created
	Udm::DataNetwork& pa = _pr.GetDataNetwork("PackageA_inst.xml");
	Udm::DataNetwork& pb = _pr.GetDataNetwork("PackageB_inst.xml");
	//obtaining handles to the root objects (this is normal - existing - Udm operation)
	PackageA::ClassA1Base rootA = PackageA::ClassA1Base::Cast( pa.GetRootObject());
	PackageB::BCommonBase rootB = PackageB::BCommonBase::Cast( pb.GetRootObject());
	// populate models with objects
	// BCommonBase-BConn1-BCommonBase
	_childB1= PackageB::BCommonBase::Create( rootB);
	_childB2= PackageB::BCommonBase::Create( rootB);
	_bConn1= PackageB::BConn1::Create( rootB);
	_bConn1.dst_end()= _childB2;
	_bConn1.src_end()= _childB1;
	// BCommonBase-BConnBase-ClassB2
	_clB2= PackageB::ClassB2::Create( rootB);
	_connBase= PackageB::BConnBase::Create( rootB);
	_connBase.src_end()= _childB1;
	_connBase.dst_end()= _clB2;
	// BCommonBase-BConnBase-BCommonBase;
	_connBase_2nd= PackageB::BConnBase::Create( rootB);
	_connBase_2nd.src_end()= _childB2;
	_connBase_2nd.dst_end()= _childB2;
	// ClassA1-ClassA2Base
	_clA1= PackageA::ClassA1::Create( rootA);
	_clA2Base= PackageA::ClassA2Base::Create( rootA);
	_clA1.dst()+= _clA2Base;
	// ClassA1-AConn1-ClassA2
	_aConn1= PackageA::AConn1::Create( rootA);
	_clA2= PackageA::ClassA2::Create( rootA);
	_aConn1.srcConn_end()= _clA1;
	_aConn1.dstConn_end()= _clA2;
	// ClassA1-AConn2-ClassA2
	_aConn2= PackageA::AConn2::Create( rootA);
	_aConn2.srcConn_end()= _clA1;
	_aConn2.dstConn_end()= _clA2;
	// ClassB2-ClassA1
	_clB2.dstX()+= _clA1;
	// ClassB2-ClassA1Base
	_clB2.srcX()+= _clA1;
	// ClassB1-CrossAssocClass-ClassA1Base
	_clB1= PackageB::ClassB1::Create( rootB);
	_xAC= PackageB::CrossAssocClass::Create( rootB);
	_xAC.srcACX_end()= _clB1;
	_xAC.dstACX_end()= _clA1;
	// BCommonBase-BConn2-ClassB1
	_bConn2= PackageB::BConn2::Create( rootB);
	_bConn2.src_end()= _childB2;
	_bConn2.dst_end()= _clB1;
}

void UdmTests::TestGetAdjacentObjects::testAssocsFrom_childB1() {
	//		GetAdjacentObjects(const Class& clsDstType), when clsDstType= BCommonBase,  should return a set of {childB2, clB2}
	multiset< Udm::Object> res1= _childB1.GetAdjacentObjects( PackageB::BCommonBase::meta);
	multiset< Udm::Object> comp1;
	comp1.insert( _childB2);
	comp1.insert( _clB2);
	CPPUNIT_ASSERT( res1 == comp1);
	//		GetAdjacentObjects(const Class& clsDstType, const AssociationInfo& ascType), when
	//			ascType= BConn1, should return a set of {childB2}
	Udm::Object::AssociationInfo assocInfo2( PackageB::BConn1::meta);
	multiset< Udm::Object> res2= _childB1.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo2);
	multiset< Udm::Object> comp2;
	comp2.insert( _childB2);
	CPPUNIT_ASSERT( res2 == comp2);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(childB2, bConn1)}
	multiset< pair<Udm::Object, Udm::Object> > res2b = _childB1.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo2);
	multiset< pair<Udm::Object, Udm::Object> > comp2b;
	comp2b.insert( make_pair(_childB2, _bConn1));
	CPPUNIT_ASSERT( res2b == comp2b);

	//			ascType= BConn2, should return an empty set
	Udm::Object::AssociationInfo assocInfo3( PackageB::BConn2::meta);
	multiset< Udm::Object> res3= _childB1.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo3);
	multiset< Udm::Object> comp3;
	CPPUNIT_ASSERT( res3 == comp3);

	// and GetAdjacentObjectsWithAssocClass should return
	// an empty set
	multiset< pair<Udm::Object, Udm::Object> > res3b = _childB1.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo3);
	multiset< pair<Udm::Object, Udm::Object> > comp3b;
	CPPUNIT_ASSERT( res3b == comp3b);

	//			ascType= BConnBase, should return a set of {childB2, clB2}
	Udm::Object::AssociationInfo assocInfo4( PackageB::BConnBase::meta);
	multiset< Udm::Object> res4= _childB1.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo4);
	multiset< Udm::Object> comp4;
	comp4.insert( _childB2);
	comp4.insert( _clB2);
	CPPUNIT_ASSERT( res4 == comp4);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(childB2, bConn1), pair(clB2, connBase)}
	multiset< pair<Udm::Object, Udm::Object> > res4b = _childB1.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo4);
	multiset< pair<Udm::Object, Udm::Object> > comp4b;
	comp4b.insert( make_pair(_childB2, _bConn1));
	comp4b.insert( make_pair(_clB2, _connBase));
	CPPUNIT_ASSERT( res4b == comp4b);
}

void UdmTests::TestGetAdjacentObjects::testAssocsFrom_childB2() {
	// starting from childB2
	//		GetAdjacentObjects(const Class& clsDstType), when clsDstType= BCommonBase,  should return a set of {childB1, childB2, childB2, clB1}
	multiset< Udm::Object> res1= _childB2.GetAdjacentObjects( PackageB::BCommonBase::meta);
	multiset< Udm::Object> comp1;
	comp1.insert( _childB1);
	comp1.insert( _childB2);
	comp1.insert( _childB2);
	comp1.insert( _clB1);
	CPPUNIT_ASSERT( res1 == comp1);
	//		GetAdjacentObjects(const Class& clsDstType, const AssociationInfo& ascType), when
	//			ascType= BConn1, should return a set of {childB1}
	Udm::Object::AssociationInfo assocInfo2( PackageB::BConn1::meta);
	multiset< Udm::Object> res2= _childB2.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo2);
	multiset< Udm::Object> comp2;
	comp2.insert( _childB1);
	CPPUNIT_ASSERT( res2 == comp2);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(childB1, bConn1)}
	multiset< pair<Udm::Object, Udm::Object> > res2b = _childB2.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo2);
	multiset< pair<Udm::Object, Udm::Object> > comp2b;
	comp2b.insert( make_pair(_childB1, _bConn1));
	CPPUNIT_ASSERT( res2b == comp2b);

	//			actType= BConn2, should return a set of {clB1}
	Udm::Object::AssociationInfo assocInfo3( PackageB::BConn2::meta);
	multiset< Udm::Object> res3= _childB2.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo3);
	multiset< Udm::Object> comp3;
	comp3.insert( _clB1);
	CPPUNIT_ASSERT( res3 == comp3);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(clB1, bConn2)}
	multiset< pair<Udm::Object, Udm::Object> > res3b = _childB2.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo3);
	multiset< pair<Udm::Object, Udm::Object> > comp3b;
	comp3b.insert( make_pair(_clB1, _bConn2));
	CPPUNIT_ASSERT( res3b == comp3b);

	//			ascType= BConnBase, should return a set of {childB1, childB2, childB2, clB1}
	Udm::Object::AssociationInfo assocInfo4( PackageB::BConnBase::meta);
	multiset< Udm::Object> res4= _childB2.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo4);
	multiset< Udm::Object> comp4;
	comp4.insert( _childB1);
	comp4.insert( _childB2);
	comp4.insert( _childB2);
	comp4.insert( _clB1);
	CPPUNIT_ASSERT( res4 == comp4);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(childB1, bConn1), pair(childB2, connBase_2nd), pair(childB2, connBase_2nd), pair(clB1, bConn2)}
	multiset< pair<Udm::Object, Udm::Object> > res4b = _childB2.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo4);
	multiset< pair<Udm::Object, Udm::Object> > comp4b;
	comp4b.insert( make_pair(_childB1, _bConn1));
	comp4b.insert( make_pair(_childB2, _connBase_2nd));
	comp4b.insert( make_pair(_childB2, _connBase_2nd));
	comp4b.insert( make_pair(_clB1, _bConn2));
	CPPUNIT_ASSERT( res4b == comp4b);
}

void UdmTests::TestGetAdjacentObjects::testAssocsFrom_clB1() {
	// starting from clB1
	//		GetAdjacentObjects(const Class& clsDstType), when clsDstType= BCommonBase,  should return a set of {childB2}
	multiset< Udm::Object> res1= _clB1.GetAdjacentObjects( PackageB::BCommonBase::meta);
	multiset< Udm::Object> comp1;
	comp1.insert( _childB2);
	CPPUNIT_ASSERT( res1 == comp1);
	//		GetAdjacentObjects(const Class& clsDstType), when clsDstType= PackageA::ClassA1, should return a set of {clA1}
	multiset< Udm::Object> res2= _clB1.GetAdjacentObjects( PackageA::ClassA1::meta);
	multiset< Udm::Object> comp2;
	comp2.insert( _clA1);
	CPPUNIT_ASSERT( res2 == comp2);
	//		GetAdjacentObjects(const Class& clsDstType, const AssociationInfo& ascType), when
	//			ascType= BConn1, should return an empty set
	Udm::Object::AssociationInfo assocInfo3( PackageB::BConn1::meta);
	multiset< Udm::Object> res3= _clB1.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo3);
	multiset< Udm::Object> comp3;
	CPPUNIT_ASSERT( res3 == comp3);

	// and GetAdjacentObjectsWithAssocClass should return
	// an empty set
	multiset< pair<Udm::Object, Udm::Object> > res3b = _clB1.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo3);
	multiset< pair<Udm::Object, Udm::Object> > comp3b;
	CPPUNIT_ASSERT( res3b == comp3b);

	//			actType= BConn2, should return a set of {childB2}
	Udm::Object::AssociationInfo assocInfo4( PackageB::BConn2::meta);
	multiset< Udm::Object> res4= _clB1.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo4);
	multiset< Udm::Object> comp4;
	comp4.insert( _childB2);
	CPPUNIT_ASSERT( res4 == comp4);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(childB2, bConn2)}
	multiset< pair<Udm::Object, Udm::Object> > res4b = _clB1.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo4);
	multiset< pair<Udm::Object, Udm::Object> > comp4b;
	comp4b.insert( make_pair(_childB2, _bConn2));
	CPPUNIT_ASSERT( res4b == comp4b);

	//			ascType= BConnBase, should return a set of {childB2}
	Udm::Object::AssociationInfo assocInfo5( PackageB::BConnBase::meta);
	multiset< Udm::Object> res5= _clB1.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo5);
	multiset< Udm::Object> comp5;
	comp5.insert( _childB2);
	CPPUNIT_ASSERT( res5 == comp5);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(childB2, bConn2)}
	multiset< pair<Udm::Object, Udm::Object> > res5b = _clB1.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo5);
	multiset< pair<Udm::Object, Udm::Object> > comp5b;
	comp5b.insert( make_pair(_childB2, _bConn2));
	CPPUNIT_ASSERT( res5b == comp5b);

	//			ascType= CrossAssocClass, should return a set of {clA1}
	Udm::Object::AssociationInfo assocInfo6( PackageB::CrossAssocClass::meta);
	multiset< Udm::Object> res6= _clB1.GetAdjacentObjects( PackageA::ClassA1::meta, assocInfo6);
	multiset< Udm::Object> comp6;
	comp6.insert( _clA1);
	CPPUNIT_ASSERT( res6 == comp6);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(clA1, xAC)}
	multiset< pair<Udm::Object, Udm::Object> > res6b = _clB1.GetAdjacentObjectsWithAssocClasses( PackageA::ClassA1::meta, assocInfo6);
	multiset< pair<Udm::Object, Udm::Object> > comp6b;
	comp6b.insert( make_pair(_clA1, _xAC));
	CPPUNIT_ASSERT( res6b == comp6b);
}


void UdmTests::TestGetAdjacentObjects::testAssocsFrom_clB2() {
	// starting from clB2
	//		GetAdjacentObjects(const Class& clsDstType), when clsDstType= BCommonBase,  should return a set of {childB1}
	multiset< Udm::Object> res1= _clB2.GetAdjacentObjects( PackageB::BCommonBase::meta);
	multiset< Udm::Object> comp1;
	comp1.insert( _childB1);
	CPPUNIT_ASSERT( res1 == comp1);
	//		GetAdjacentObjects(const Class& clsDstType), when clsDstType= PackageA::ClassA1, should return a set of {clA1,clA1}
	multiset< Udm::Object> res2= _clB2.GetAdjacentObjects( PackageA::ClassA1::meta);
	multiset< Udm::Object> comp2;
	comp2.insert( _clA1);
	comp2.insert( _clA1);
	CPPUNIT_ASSERT( res2 == comp2);
	//		GetAdjacentObjects(const Class& clsDstType, const AssociationInfo& ascType), when
	//			ascType= BConn1, should return an empty set
	Udm::Object::AssociationInfo assocInfo3( PackageB::BConn1::meta);
	multiset< Udm::Object> res3= _clB2.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo3);
	multiset< Udm::Object> comp3;
	CPPUNIT_ASSERT( res3 == comp3);

	// and GetAdjacentObjectsWithAssocClass should return
	// an empty set
	multiset< pair<Udm::Object, Udm::Object> > res3b = _clB2.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo3);
	multiset< pair<Udm::Object, Udm::Object> > comp3b;
	CPPUNIT_ASSERT( res3b == comp3b);

	//			actType= BConn2, should return an empty set
	Udm::Object::AssociationInfo assocInfo4( PackageB::BConn2::meta);
	multiset< Udm::Object> res4= _clB2.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo4);
	multiset< Udm::Object> comp4;
	CPPUNIT_ASSERT( res4 == comp4);

	// and GetAdjacentObjectsWithAssocClass should return
	// an empty set
	multiset< pair<Udm::Object, Udm::Object> > res4b = _clB2.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo4);
	multiset< pair<Udm::Object, Udm::Object> > comp4b;
	CPPUNIT_ASSERT( res4b == comp4b);

	//			ascType= BConnBase, should return a set of {childB1}
	Udm::Object::AssociationInfo assocInfo5( PackageB::BConnBase::meta);
	multiset< Udm::Object> res5= _clB2.GetAdjacentObjects( PackageB::BCommonBase::meta, assocInfo5);
	multiset< Udm::Object> comp5;
	comp5.insert( _childB1);
	CPPUNIT_ASSERT( res5 == comp5);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(childB1, connBase)}
	multiset< pair<Udm::Object, Udm::Object> > res5b = _clB2.GetAdjacentObjectsWithAssocClasses( PackageB::BCommonBase::meta, assocInfo5);
	multiset< pair<Udm::Object, Udm::Object> > comp5b;
	comp5b.insert( make_pair(_childB1, _connBase));
	CPPUNIT_ASSERT( res5b == comp5b);
}

void UdmTests::TestGetAdjacentObjects::testAssocsFrom_clA1() {
	// starting from clA1
	//		GetAdjacentObjects(const Class& clsDstType), when clsDstType= ClassA2Base,  should return a set of {clA2Base, clA2, clA2}
	multiset< Udm::Object> res1= _clA1.GetAdjacentObjects( PackageA::ClassA2Base::meta);
	multiset< Udm::Object> comp1;
	comp1.insert( _clA2Base);
	comp1.insert( _clA2);
	comp1.insert( _clA2);
	CPPUNIT_ASSERT( res1 == comp1);
	//		GetAdjacentObjects(const Class& clsDstType), when clsDstType= PackageB::ClassB2, should return a set of {clB2,clB2}
	multiset< Udm::Object> res2= _clA1.GetAdjacentObjects( PackageB::ClassB2::meta);
	multiset< Udm::Object> comp2;
	comp2.insert( _clB2);
	comp2.insert( _clB2);
	CPPUNIT_ASSERT( res2 == comp2);
	//		GetAdjacentObjects(const Class& clsDstType, const AssociationInfo& ascType), when
	//			ascType= AConn1, should return a set of {clA2}
	Udm::Object::AssociationInfo assocInfo3( PackageA::AConn1::meta);
	multiset< Udm::Object> res3= _clA1.GetAdjacentObjects( PackageA::ClassA2Base::meta, assocInfo3);
	multiset< Udm::Object> comp3;
	comp3.insert( _clA2);
	CPPUNIT_ASSERT( res3 == comp3);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(clA2, aConn1)}
	multiset< pair<Udm::Object, Udm::Object> > res3b = _clA1.GetAdjacentObjectsWithAssocClasses( PackageA::ClassA2Base::meta, assocInfo3);
	multiset< pair<Udm::Object, Udm::Object> > comp3b;
	comp3b.insert( make_pair(_clA2, _aConn1));
	CPPUNIT_ASSERT( res3b == comp3b);

	//			actType= AConn2, should return a set of {clA2}
	Udm::Object::AssociationInfo assocInfo4( PackageA::AConn2::meta);
	multiset< Udm::Object> res4= _clA1.GetAdjacentObjects( PackageA::ClassA2Base::meta, assocInfo4);
	multiset< Udm::Object> comp4;
	comp4.insert( _clA2);
	CPPUNIT_ASSERT( res4 == comp4);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(clA2, aConn2)}
	multiset< pair<Udm::Object, Udm::Object> > res4b = _clA1.GetAdjacentObjectsWithAssocClasses( PackageA::ClassA2Base::meta, assocInfo4);
	multiset< pair<Udm::Object, Udm::Object> > comp4b;
	comp4b.insert( make_pair(_clA2, _aConn2));
	CPPUNIT_ASSERT( res4b == comp4b);

	//			ascType= AConnBase, should return a set of {clA2,clA2}
	Udm::Object::AssociationInfo assocInfo5( PackageA::AConnBase::meta);
	multiset< Udm::Object> res5= _clA1.GetAdjacentObjects( PackageA::ClassA2Base::meta, assocInfo5);
	multiset< Udm::Object> comp5;
	comp5.insert( _clA2);
	comp5.insert( _clA2);
	CPPUNIT_ASSERT( res5 == comp5);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(clA2, aConn1), pair(clA2, aConn2)}
	multiset< pair<Udm::Object, Udm::Object> > res5b = _clA1.GetAdjacentObjectsWithAssocClasses( PackageA::ClassA2Base::meta, assocInfo5);
	multiset< pair<Udm::Object, Udm::Object> > comp5b;
	comp5b.insert( make_pair(_clA2, _aConn1));
	comp5b.insert( make_pair(_clA2, _aConn2));
	CPPUNIT_ASSERT( res5b == comp5b);
}

void UdmTests::TestGetAdjacentObjects::testAssocsFrom_clA2Base() {
	// starting from clA2Base
	//		GetAdjacentObjects(const Class& clsDstType), when clsDstType= ClassA1Base,  should return a set of {clA1}
	multiset< Udm::Object> res1= _clA2Base.GetAdjacentObjects( PackageA::ClassA1Base::meta);
	multiset< Udm::Object> comp1;
	comp1.insert( _clA1);
	CPPUNIT_ASSERT( res1 == comp1);
	//		GetAdjacentObjects(const Class& clsDstType), when clsDstType= ClassA1, should return a set of {clA1}
	multiset< Udm::Object> res2= _clA2Base.GetAdjacentObjects( PackageA::ClassA1::meta);
	multiset< Udm::Object> comp2;
	comp2.insert( _clA1);
	CPPUNIT_ASSERT( res2 == comp2);
}

void UdmTests::TestGetAdjacentObjects::testAssocsFrom_clA2() {
	// starting from clA2
	//		GetAdjacentObjects(const Class& clsDstType), when clsDstType= ClassA1Base,  should return a set of {clA1, clA1}
	multiset< Udm::Object> res1= _clA2.GetAdjacentObjects( PackageA::ClassA1Base::meta);
	multiset< Udm::Object> comp1;
	comp1.insert( _clA1);
	comp1.insert( _clA1);
	CPPUNIT_ASSERT( res1 == comp1);
	//		GetAdjacentObjects(const Class& clsDstType, const AssociationInfo& ascType), when
	//			ascType= AConn1, should return a set of {clA1}
	Udm::Object::AssociationInfo assocInfo2( PackageA::AConn1::meta);
	multiset< Udm::Object> res2= _clA2.GetAdjacentObjects( PackageA::ClassA1Base::meta, assocInfo2);
	multiset< Udm::Object> comp2;
	comp2.insert( _clA1);
	CPPUNIT_ASSERT( res2 == comp2);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(clA1, aConn1)}
	multiset< pair<Udm::Object, Udm::Object> > res2b = _clA2.GetAdjacentObjectsWithAssocClasses( PackageA::ClassA1Base::meta, assocInfo2);
	multiset< pair<Udm::Object, Udm::Object> > comp2b;
	comp2b.insert( make_pair(_clA1, _aConn1));
	CPPUNIT_ASSERT( res2b == comp2b);

	//			actType= AConn2, should return a set of {clA1}
	Udm::Object::AssociationInfo assocInfo3( PackageA::AConn2::meta);
	multiset< Udm::Object> res3= _clA2.GetAdjacentObjects( PackageA::ClassA1Base::meta, assocInfo3);
	multiset< Udm::Object> comp3;
	comp3.insert( _clA1);
	CPPUNIT_ASSERT( res3 == comp3);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(clA1, aConn2)}
	multiset< pair<Udm::Object, Udm::Object> > res3b = _clA2.GetAdjacentObjectsWithAssocClasses( PackageA::ClassA1Base::meta, assocInfo3);
	multiset< pair<Udm::Object, Udm::Object> > comp3b;
	comp3b.insert( make_pair(_clA1, _aConn2));
	CPPUNIT_ASSERT( res3b == comp3b);

	//			ascType= AConnBase, should return a set of {clA1,clA1}
	Udm::Object::AssociationInfo assocInfo4( PackageA::AConnBase::meta);
	multiset< Udm::Object> res4= _clA2.GetAdjacentObjects( PackageA::ClassA1Base::meta, assocInfo4);
	multiset< Udm::Object> comp4;
	comp4.insert( _clA1);
	comp4.insert( _clA1);
	CPPUNIT_ASSERT( res4 == comp4);

	// and GetAdjacentObjectsWithAssocClass should return
	// a set of {pair(clA1, aConn1), pair(clA1, aConn2)}
	multiset< pair<Udm::Object, Udm::Object> > res4b = _clA2.GetAdjacentObjectsWithAssocClasses( PackageA::ClassA1Base::meta, assocInfo4);
	multiset< pair<Udm::Object, Udm::Object> > comp4b;
	comp4b.insert( make_pair(_clA1, _aConn1));
	comp4b.insert( make_pair(_clA1, _aConn2));
	CPPUNIT_ASSERT( res4b == comp4b);
}

void testLampGetSrcDstObject(const string &dst, bool has_direction_support)
{
	Udm::SmartDataNetwork nw(LampDiagram::diagram);
	nw.CreateNew(dst, "LampDiagram", LampDiagram::RootFolder::meta, Udm::CHANGES_LOST_DEFAULT);

	LampDiagram::RootFolder rf = LampDiagram::RootFolder::Cast(nw.GetRootObject());
	LampDiagram::Lamp lamp = LampDiagram::Lamp::Create(rf);

	LampDiagram::ControlLink cl = LampDiagram::ControlLink::Create(lamp);
	LampDiagram::Bulb bulb = LampDiagram::Bulb::Create(lamp);
	LampDiagram::Switch mswitch = LampDiagram::Switch::Create(lamp, LampDiagram::Lamp::meta_MainSwitch);
	
	cl.src_end() = bulb;
	cl.dst_end() = mswitch;

	if (has_direction_support) {
		CPPUNIT_ASSERT( cl.getSrcObject() == bulb );
		CPPUNIT_ASSERT( cl.getDstObject() == mswitch );
	} else {
		CPPUNIT_ASSERT( cl.getSrcObject() == &Udm::_null );
		CPPUNIT_ASSERT( cl.getDstObject() == &Udm::_null );
	}

	nw.CloseNoUpdate();
}

void UdmTests::TestGetAdjacentObjects::testGetSrcDstObject()
{

#ifdef _WIN32
	testLampGetSrcDstObject("lamptest.mga", true);
#endif

	testLampGetSrcDstObject("lamptest.mem", false);
}

void UdmTests::TestGetAdjacentObjects::test()
{
	try {
		init();
		testAssocsFrom_childB1();
		testAssocsFrom_childB2();
		testAssocsFrom_clB1();
		testAssocsFrom_clB2();
		testAssocsFrom_clA1();
		testAssocsFrom_clA2Base();
		testAssocsFrom_clA2();
		testGetSrcDstObject();
	} catch (const udm_exception& e) {
		std::cout << e.what() << std::endl;
		CPPUNIT_FAIL("Udm exception");
	} catch ( CPPUNIT_NS::Exception& e) {
		throw;
	} catch (...) {
		CPPUNIT_FAIL("Unknown exception");
	}
}
