// UDMTest.h: interface for the CUDMTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDMTEST_H__57C5593D_915E_44A7_BC87_2760673C56F1__INCLUDED_)
#define AFX_UDMTEST_H__57C5593D_915E_44A7_BC87_2760673C56F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "LampDiagram.h"
#include "UmlExt.h"

#include "UdmStatic.h"

using namespace std;

class CUDMTest   : public CppUnit::TestFixture  
{
CPPUNIT_TEST_SUITE( CUDMTest );
CPPUNIT_TEST( TestFileCreation );
CPPUNIT_TEST( TestObjectCreation );
CPPUNIT_TEST( TestExistingNetwork );
CPPUNIT_TEST( TestAttributes );
CPPUNIT_TEST( TestDirectAttributesAssignment );
CPPUNIT_TEST( TestConnection );
CPPUNIT_TEST_EXCEPTION( TestObjectDestruction, udm_exception ); //this test expects an exception
CPPUNIT_TEST_SUITE_END();


public:
	CUDMTest();
	virtual ~CUDMTest();

	void setUp();
	void tearDown();

	void TestFileCreation();
	void TestObjectCreation();
	void TestExistingNetwork();
	void TestAttributes();
	void TestDirectAttributesAssignment();
	void TestConnection();
	void TestObjectDestruction();

private:
	
	//cppunit creates a new instance of this class
	//for running each individual Test Function
	void GetNetwork();

	Udm::SmartDataNetwork m_xmlfile;
	LampDiagram::RootFolder m_root;
	LampDiagram::Lamp m_lamp;
	LampDiagram::Bulb m_bulb;
	LampDiagram::Switch m_switch;
	LampDiagram::Plug m_plug;
	LampDiagram::ElectricTerminal m_term1;
	LampDiagram::ElectricTerminal m_term2;
	LampDiagram::Wire m_wire;
	LampDiagram::ControlLink m_link;

	

};

#endif // !defined(AFX_UDMTEST_H__57C5593D_915E_44A7_BC87_2760673C56F1__INCLUDED_)
