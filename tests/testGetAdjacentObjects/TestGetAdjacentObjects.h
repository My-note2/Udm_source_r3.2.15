#ifndef TESTGETADJACENTOBJECTS_H
#define TESTGETADJACENTOBJECTS_H

#include <cppunit/extensions/HelperMacros.h>
#include <UdmBase.h>
#include "testAssociationScenarios.h"
#include "PackageA.h"
#include "PackageB.h"
#include "LampDiagram.h"

using namespace std;

#ifdef _WIN32
UDM_USE_MGA
#endif
UDM_USE_DOM

namespace UdmTests
{
	class TestGetAdjacentObjects : public CPPUNIT_NS::TestFixture
	{
		CPPUNIT_TEST_SUITE( TestGetAdjacentObjects );
		CPPUNIT_TEST( test );
		CPPUNIT_TEST_SUITE_END();

	public:
		void test();

	protected:
		//
		void init();
		//
		void testAssocsFrom_childB1();
		void testAssocsFrom_childB2();
		void testAssocsFrom_clB1();
		void testAssocsFrom_clB2();
		//
		void testAssocsFrom_clA1();
		void testAssocsFrom_clA2Base();
		void testAssocsFrom_clA2();
		//
		void testGetSrcDstObject();

	private:
		Udm::UdmProject _pr;
		//
		PackageB::BCommonBase _childB1, _childB2;
		PackageB::ClassB1 _clB1;
		PackageB::ClassB2 _clB2;
		PackageB::BConnBase _connBase, _connBase_2nd;
		PackageB::BConn1 _bConn1;
		PackageB::BConn2 _bConn2;
		//
		PackageB::CrossAssocClass _xAC;
		//
		PackageA::ClassA1 _clA1;
		PackageA::ClassA2Base _clA2Base;
		PackageA::ClassA2 _clA2;
		PackageA::AConn1 _aConn1;
		PackageA::AConn2 _aConn2;
	};
};

#endif //TESTGETADJACENTOBJECTS_H
