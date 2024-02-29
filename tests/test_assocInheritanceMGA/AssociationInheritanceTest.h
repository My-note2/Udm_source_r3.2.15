// AssociationInheritanceTest.h: interface for the AssociationInheritanceTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASSOCIATIONINHERITANCETEST_H__49DF46F4_E084_43D4_B7F2_83CBFB764CDA__INCLUDED_)
#define AFX_ASSOCIATIONINHERITANCETEST_H__49DF46F4_E084_43D4_B7F2_83CBFB764CDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <Uml.h>
#include <UdmStatic.h>
#include <UmlExt.h>
#include <UdmBase.h>

#include "SignalFlow.h"

using namespace std;

class CAssociationInheritanceTest  : public CppUnit::TestFixture
{
	
CPPUNIT_TEST_SUITE( CAssociationInheritanceTest );
CPPUNIT_TEST( TestAssociationInheritance );
CPPUNIT_TEST_SUITE_END();

public:
	CAssociationInheritanceTest();
	virtual ~CAssociationInheritanceTest();

	void setUp();
	void tearDown();

	void TestAssociationInheritance();

};

#endif // !defined(AFX_ASSOCIATIONINHERITANCETEST_H__49DF46F4_E084_43D4_B7F2_83CBFB764CDA__INCLUDED_)
