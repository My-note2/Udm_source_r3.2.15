// UDMTest.cpp: implementation of the CUDMTest class.
//
//////////////////////////////////////////////////////////////////////


#include "UDMTest.h"
#include <cppunit/Message.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPUNIT_TEST_SUITE_REGISTRATION( CUDMTest );

CUDMTest::CUDMTest():m_xmlfile(LampDiagram::diagram)
{

}

CUDMTest::~CUDMTest()
{

}

void CUDMTest::setUp()
{
	
}

void CUDMTest::tearDown()
{
	//close the network, so later tests can open it
	if(m_xmlfile.isOpen()) m_xmlfile.CloseWithUpdate(); 
}

void CUDMTest::TestFileCreation()
{
	m_xmlfile.CreateNew( "abc.xml", "LampDiagram.xsd", LampDiagram::RootFolder::meta, Udm::CHANGES_LOST_DEFAULT);
		
	CPPUNIT_ASSERT((m_root = LampDiagram::RootFolder::Cast(m_xmlfile.GetRootObject())));
}

void CUDMTest::TestObjectCreation()
{
	m_xmlfile.OpenExisting( "abc.xml", "LampDiagram.xsd", Udm::CHANGES_LOST_DEFAULT);
	m_root = LampDiagram::RootFolder::Cast(m_xmlfile.GetRootObject());

	//test that each item is successfully created
	CPPUNIT_ASSERT((m_lamp = LampDiagram::Lamp::Create(m_root)));
	CPPUNIT_ASSERT((m_bulb = LampDiagram::Bulb::Create(m_lamp)));
	CPPUNIT_ASSERT((m_switch = LampDiagram::Switch::Create(m_lamp,LampDiagram::Lamp::meta_MainSwitch)));
	CPPUNIT_ASSERT((m_plug = LampDiagram::Plug::Create(m_lamp)));
	CPPUNIT_ASSERT((m_term1 = LampDiagram::ElectricTerminal::Create(m_plug)));
	CPPUNIT_ASSERT((m_term2 = LampDiagram::ElectricTerminal::Create(m_bulb)));
}

void CUDMTest::TestExistingNetwork()
{
	GetNetwork();
	//test that GetNetwork got all the objects created in TestObjectCreation
	CPPUNIT_ASSERT(m_root);
	CPPUNIT_ASSERT(m_lamp);
	CPPUNIT_ASSERT(m_bulb);
	CPPUNIT_ASSERT(m_switch);
	CPPUNIT_ASSERT(m_plug);
	CPPUNIT_ASSERT(m_term1);
	CPPUNIT_ASSERT(m_term2);
}

void CUDMTest::TestAttributes()
{
	
	GetNetwork();

	//test attribute setting
	CPPUNIT_ASSERT(m_bulb.Voltage() = 115);
	CPPUNIT_ASSERT(m_bulb.Wattage() = 60);

	//test attribute checking
	CPPUNIT_ASSERT( m_bulb.Voltage() == 115);
	CPPUNIT_ASSERT( m_bulb.Wattage() == 60 );
	CPPUNIT_ASSERT( m_bulb.Voltage() != 100);
	CPPUNIT_ASSERT( m_bulb.Wattage() != 75 );
}

void CUDMTest::TestDirectAttributesAssignment()
{
	GetNetwork();

	m_term1.Type() = "terminal";
	m_term2.Type() = m_term1.Type();

	CPPUNIT_ASSERT( (string)m_term1.Type() == "terminal" );
	CPPUNIT_ASSERT( (string)m_term2.Type() == "terminal" );
}

void CUDMTest::TestConnection()
{
	GetNetwork();

	//create connections
	CPPUNIT_ASSERT(m_wire = LampDiagram::Wire::Create(m_lamp));
	CPPUNIT_ASSERT(m_link = LampDiagram::ControlLink::Create(m_lamp));

	//connect the connection ends
	CPPUNIT_ASSERT(m_wire.End1_end() = m_term1);
	CPPUNIT_ASSERT(m_wire.End2_end() = m_term2);
	CPPUNIT_ASSERT(m_link.src_end() = m_bulb);
	CPPUNIT_ASSERT(m_link.dst_end() = m_switch);

	//check the connection ends
	CPPUNIT_ASSERT( LampDiagram::Bulb::Cast(m_link.src_end()) == m_bulb);
	CPPUNIT_ASSERT( LampDiagram::Switch::Cast(m_link.dst_end()) == m_switch );
	CPPUNIT_ASSERT( LampDiagram::ElectricTerminal::Cast(m_wire.End1_end()) == m_term1);
	CPPUNIT_ASSERT( LampDiagram::ElectricTerminal::Cast(m_wire.End2_end()) == m_term2);
}

void CUDMTest::TestObjectDestruction()
{
	GetNetwork();

	m_bulb.DeleteObject();
	CPPUNIT_ASSERT( (Udm::Object::Cast(m_bulb.parent())==Udm::null));
	//CPPUNIT_ASSERT( !(m_bulb) );//== LampDiagram::Bulb(NULL));
	//CPPUNIT_ASSERT( !(m_term2)); //m_term2 was a child of m_bulb

	m_term2 = LampDiagram::ElectricTerminal::Create(Udm::null); //will throw udm_exception that cppunit expects

}

void CUDMTest::GetNetwork()
{
	//cppunit uses a separate instance of this class to run each test
	//rebuild the existing network
	m_xmlfile.OpenExisting( "abc.xml", "LampDiagram.xsd", Udm::CHANGES_LOST_DEFAULT);
	m_root = LampDiagram::RootFolder::Cast(m_xmlfile.GetRootObject());
	set<LampDiagram::Lamp> lamps = m_root.Lamp_kind_children();
	m_lamp = *(lamps.begin());
	set<LampDiagram::Bulb> bulbs = m_lamp.Bulb_kind_children();
	m_bulb = *(bulbs.begin());
	set<LampDiagram::Plug> plugs = m_lamp.Plug_kind_children();
	m_plug = *(plugs.begin());
	set<LampDiagram::Switch> switches = m_lamp.Switch_kind_children();
	m_switch = *(switches.begin());
	set<LampDiagram::ElectricTerminal> terms = m_plug.ElectricTerminal_kind_children();
	m_term1 = *(terms.begin());
	terms = m_bulb.ElectricTerminal_kind_children();
	m_term2 = *(terms.begin());
}
