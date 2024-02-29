// cintstringTest.cpp


#include <UdmBase.h>
#include <UdmDom.h>
#include "LampDiagram.h"
#include "domstringdnTest.h"
#include <cstring>

#include <fstream>

// Registers the fixture into the 'registry'

CPPUNIT_TEST_SUITE_REGISTRATION( UdmTests::domstringdnTest );


using namespace LampDiagram;


void UdmTests::domstringdnTest::test()
{

	string xml_xsd;
xml_xsd += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
xml_xsd += "<?udm interface=\"LampDiagram\" version=\"1.00\"?>";
xml_xsd += "<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" elementFormDefault=\"qualified\" >";
xml_xsd += "<!-- generated on Sat Apr 30 13:47:03 2005 -->";
xml_xsd += "	<xsd:complexType name=\"BulbType\">";
xml_xsd += "		<xsd:sequence>";
xml_xsd += "			<xsd:element name=\"ElectricTerminal\" type=\"ElectricTerminalType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"Wire\" type=\"WireType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "		</xsd:sequence>";
xml_xsd += "		<xsd:attribute name=\"Wattage\" type=\"xsd:double\" use=\"required\"/>";
xml_xsd += "		<xsd:attribute name=\"Voltage\" type=\"xsd:double\"/>";
xml_xsd += "		<xsd:attribute name=\"MaxTempRating\" type=\"xsd:long\"/>";
xml_xsd += "		<xsd:attribute name=\"MaxVoltageRating\" type=\"xsd:double\"/>";
xml_xsd += "		<xsd:attribute name=\"position\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"name\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"dst\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>";
xml_xsd += "		<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>";
xml_xsd += "		<xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>";
xml_xsd += "	</xsd:complexType>";
xml_xsd += "	<xsd:complexType name=\"WireType\">";
xml_xsd += "		<xsd:attribute name=\"Amps\" type=\"xsd:double\"/>";
xml_xsd += "		<xsd:attribute name=\"End2_end_\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"End1_end_\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>";
xml_xsd += "		<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>";
xml_xsd += "		<xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>";
xml_xsd += "	</xsd:complexType>";
xml_xsd += "	<xsd:complexType name=\"ControlLinkType\">";
xml_xsd += "		<xsd:attribute name=\"name\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"dst_end_\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"src_end_\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>";
xml_xsd += "		<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>";
xml_xsd += "		<xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>";
xml_xsd += "	</xsd:complexType>";
xml_xsd += "	<xsd:complexType name=\"LampType\">";
xml_xsd += "		<xsd:sequence>";
xml_xsd += "			<xsd:element name=\"Bulb\" type=\"BulbType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"ControlLink\" type=\"ControlLinkType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"ElectricTerminal\" type=\"ElectricTerminalType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"HalogenBulb\" type=\"HalogenBulbType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"Plug\" type=\"PlugType\"/>";
xml_xsd += "			<xsd:element name=\"Switch\" type=\"SwitchType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"Wire\" type=\"WireType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "		</xsd:sequence>";
xml_xsd += "		<xsd:attribute name=\"MaxTempRating\" type=\"xsd:long\"/>";
xml_xsd += "		<xsd:attribute name=\"MaxVoltageRating\" type=\"xsd:double\"/>";
xml_xsd += "		<xsd:attribute name=\"position\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"name\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"ArrayStr\" type=\"xsd:string\" default=\"second;first;\"/>";
xml_xsd += "		<xsd:attribute name=\"ModelName\" type=\"xsd:string\" default=\"Default Lamp Name\"/>";
xml_xsd += "		<xsd:attribute name=\"ArrayInt\" type=\"xsd:string\" default=\"5;4;3;2;\"/>";
xml_xsd += "		<xsd:attribute name=\"ArrayBool\" type=\"xsd:string\" default=\"false;true;false;true;\"/>";
xml_xsd += "		<xsd:attribute name=\"ArrayReal\" type=\"xsd:string\" default=\"9;8;7;6;\"/>";
xml_xsd += "		<xsd:attribute name=\"sample\" type=\"xsd:double\" default=\"3.14159\"/>";
xml_xsd += "		<xsd:attribute name=\"RegAttr\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>";
xml_xsd += "		<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>";
xml_xsd += "		<xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>";
xml_xsd += "	</xsd:complexType>";
xml_xsd += "	<xsd:complexType name=\"ElectricTerminalType\">";
xml_xsd += "		<xsd:attribute name=\"position\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"Type\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"End2\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"End1\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>";
xml_xsd += "		<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>";
xml_xsd += "		<xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>";
xml_xsd += "	</xsd:complexType>";
xml_xsd += "	<xsd:complexType name=\"HalogenBulbType\">";
xml_xsd += "		<xsd:sequence>";
xml_xsd += "			<xsd:element name=\"ElectricTerminal\" type=\"ElectricTerminalType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"Wire\" type=\"WireType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "		</xsd:sequence>";
xml_xsd += "		<xsd:attribute name=\"Wattage\" type=\"xsd:double\" use=\"required\"/>";
xml_xsd += "		<xsd:attribute name=\"Voltage\" type=\"xsd:double\"/>";
xml_xsd += "		<xsd:attribute name=\"MaxTempRating\" type=\"xsd:long\"/>";
xml_xsd += "		<xsd:attribute name=\"MaxVoltageRating\" type=\"xsd:double\"/>";
xml_xsd += "		<xsd:attribute name=\"position\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"name\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"dst\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>";
xml_xsd += "		<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>";
xml_xsd += "		<xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>";
xml_xsd += "	</xsd:complexType>";
xml_xsd += "	<xsd:complexType name=\"SwitchType\">";
xml_xsd += "		<xsd:sequence>";
xml_xsd += "			<xsd:element name=\"ElectricTerminal\" type=\"ElectricTerminalType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"Wire\" type=\"WireType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "		</xsd:sequence>";
xml_xsd += "		<xsd:attribute name=\"__child_as\">";
xml_xsd += "			<xsd:simpleType>";
xml_xsd += "				<xsd:restriction base=\"xsd:string\">";
xml_xsd += "					<xsd:enumeration value=\"FunctionSwitch\"/>";
xml_xsd += "					<xsd:enumeration value=\"MainSwitch\"/>";
xml_xsd += "				</xsd:restriction>";
xml_xsd += "			</xsd:simpleType>";
xml_xsd += "		</xsd:attribute>";
xml_xsd += "		<xsd:attribute name=\"MaxTempRating\" type=\"xsd:long\"/>";
xml_xsd += "		<xsd:attribute name=\"MaxVoltageRating\" type=\"xsd:double\"/>";
xml_xsd += "		<xsd:attribute name=\"position\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"name\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"Safe\" type=\"xsd:boolean\"/>";
xml_xsd += "		<xsd:attribute name=\"Amps\" type=\"xsd:double\"/>";
xml_xsd += "		<xsd:attribute name=\"src\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>";
xml_xsd += "		<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>";
xml_xsd += "		<xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>";
xml_xsd += "	</xsd:complexType>";
xml_xsd += "	<xsd:complexType name=\"RootFolderType\">";
xml_xsd += "		<xsd:sequence>";
xml_xsd += "			<xsd:element name=\"Bulb\" type=\"BulbType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"HalogenBulb\" type=\"HalogenBulbType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"Lamp\" type=\"LampType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"Plug\" type=\"PlugType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"Switch\" type=\"SwitchType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "		</xsd:sequence>";
xml_xsd += "		<xsd:attribute name=\"RegAttr\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>";
xml_xsd += "		<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>";
xml_xsd += "		<xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>";
xml_xsd += "	</xsd:complexType>";
xml_xsd += "	<xsd:complexType name=\"PlugType\">";
xml_xsd += "		<xsd:sequence>";
xml_xsd += "			<xsd:element name=\"ElectricTerminal\" type=\"ElectricTerminalType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "			<xsd:element name=\"Wire\" type=\"WireType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>";
xml_xsd += "		</xsd:sequence>";
xml_xsd += "		<xsd:attribute name=\"MaxTempRating\" type=\"xsd:long\"/>";
xml_xsd += "		<xsd:attribute name=\"MaxVoltageRating\" type=\"xsd:double\"/>";
xml_xsd += "		<xsd:attribute name=\"position\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"name\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"Format\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>";
xml_xsd += "		<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>";
xml_xsd += "		<xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>";
xml_xsd += "		<xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>";
xml_xsd += "		<xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>";
xml_xsd += "		<xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>";
xml_xsd += "	</xsd:complexType>";
xml_xsd += " <xsd:element name=\"RootFolder\" type=\"RootFolderType\"/>";
xml_xsd += "</xsd:schema>";


	
string xml_str = "<RootFolder xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"LampDiagram.xsd\"><Lamp _id=\"id0D\" name=\"Host Lamp \" ArrayInt=\"2;3;4;5\" ArrayStr=\"first;second;\" ArrayBool=\"false;false;true;true\" ArrayReal=\"6;7;8;9\" ModelName=\"Sequence tester lamp\"><Bulb name=\"Bulb 1\" Voltage=\"10\" Wattage=\"100\"/><Bulb name=\"Bulb 2\" Voltage=\"20\" Wattage=\"200\"/><Bulb name=\"Bulb 3\" Voltage=\"30\" Wattage=\"300\"/><Plug/></Lamp></RootFolder>";
string xml_verify_str = 

"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n\
<RootFolder xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"LampDiagram.xsd\">\n\
\n\
  <Lamp ArrayBool=\"false;false;true;true\" ArrayInt=\"2;3;4;5\" ArrayReal=\"6;7;8;9\" ArrayStr=\"first;second;\" ModelName=\"Sequence tester lamp\" _id=\"id0D\" name=\" name changed!\">\n\
    <Bulb Voltage=\"10\" Wattage=\"100\" name=\"Bulb 1\"/>\n\
    <Bulb Voltage=\"20\" Wattage=\"200\" name=\"Bulb 2\"/>\n\
    <Bulb Voltage=\"30\" Wattage=\"300\" name=\"Bulb 3\"/>\n\
    <Plug/>\n\
  </Lamp>\n\
\n\
</RootFolder>\n";

	string xml_verify_str1 = 

"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n\
<RootFolder xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"LampDiagram.xsd\">\n\
\n\
  <Lamp ArrayBool=\"false;false;true;true\" ArrayInt=\"2;3;4;5\" ArrayReal=\"6;7;8;9\" ArrayStr=\"first;second;\" ModelName=\"Default Lamp Name\" _id=\"id10\" name=\"cool!\" sample=\"3.141592\"/>\n\
\n\
</RootFolder>\n";

	LampDiagram::Initialize();
	// may already be there due to udm.exe -g
	UdmDom::str_xsd_storage::RemoveXsd("LampDiagram.xsd");
	UdmDom::str_xsd_storage::StoreXsd("LampDiagram.xsd", xml_xsd);
	

	UdmDom::DomDataNetwork ddn(LampDiagram::diagram);
	ddn.OpenExistingFromString(xml_str,"LampDiagram.xsd", Udm::CHANGES_PERSIST_ALWAYS);
	RootFolder rf = RootFolder::Cast(ddn.GetRootObject());
	Lamp l = *( (set<Lamp>(rf.Lamp_kind_children())).begin());
	CPPUNIT_ASSERT(l.uniqueId() == 13);

	l.name() = " name changed!";
	ddn.CloseWithUpdate();

	
	const string& outstr = ddn.Str();

	CPPUNIT_ASSERT_EQUAL(xml_verify_str, outstr);


	ddn.CreateNewToString("LampDiagram.xsd", RootFolder::meta, Udm::CHANGES_PERSIST_ALWAYS);
	RootFolder rf1 = RootFolder::Cast(ddn.GetRootObject());
	Lamp l1 = Lamp::Create(rf1);
	l1.name() = "cool!";
	ddn.CloseWithUpdate();

	const string & outstr1 = ddn.Str();

	CPPUNIT_ASSERT_EQUAL(xml_verify_str1, outstr1);

};

