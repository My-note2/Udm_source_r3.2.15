#ifndef UdmProject_xsd_H
#define UdmProject_xsd_H
#include <string>
#pragma warning( disable : 4010)

namespace UdmProject_xsd
{
const std::string& getString()
{
	static std::string str;
	if (str.empty())
	{
		str +="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		str +="<?udm interface=\"UdmProject\" version=\"1.00\"?>\n";
		str +="<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n";
		str +=" elementFormDefault=\"qualified\" \n";
		str +=">\n";
//		str +="<!-- generated on Thu Nov 05 17:19:40 2015 -->\n";
		str +="\n";
		str +="\n";
		str +="	<xsd:complexType name=\"DatanetworkType\">\n";
		str +="		<xsd:attribute name=\"__child_as\">\n";
		str +="			<xsd:simpleType>\n";
		str +="				<xsd:restriction base=\"xsd:string\">\n";
		str +="					<xsd:enumeration value=\"cross_associations\"/>\n";
		str +="					<xsd:enumeration value=\"instances\"/>\n";
		str +="					<xsd:enumeration value=\"crosslinks\"/>\n";
		str +="				</xsd:restriction>\n";
		str +="			</xsd:simpleType>\n";
		str +="		</xsd:attribute>\n";
		str +="		<xsd:attribute name=\"metaDgr\" type=\"xsd:string\" use=\"required\"/>\n";
		str +="		<xsd:attribute name=\"systemname\" type=\"xsd:string\" use=\"required\"/>\n";
		str +="		<xsd:attribute name=\"metalocator\" type=\"xsd:string\"/>\n";
		str +="		<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>\n";
		str +="		<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>\n";
		str +="		<xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>\n";
		str +="		<xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>\n";
		str +="		<xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>\n";
		str +="		<xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>\n";
		str +="		<xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>\n";
		str +="	</xsd:complexType>\n";
		str +="\n";
		str +="	<xsd:complexType name=\"ProjectType\">\n";
		str +="		<xsd:sequence>\n";
		str +="			<xsd:element name=\"Datanetwork\" type=\"DatanetworkType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n";
		str +="			<xsd:element name=\"Project\" type=\"ProjectType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n";
		str +="		</xsd:sequence>\n";
		str +="		<xsd:attribute name=\"name\" type=\"xsd:string\" use=\"required\"/>\n";
		str +="		<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>\n";
		str +="		<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>\n";
		str +="		<xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>\n";
		str +="		<xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>\n";
		str +="		<xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>\n";
		str +="		<xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>\n";
		str +="		<xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>\n";
		str +="		<xsd:attribute name=\"_libname\" type=\"xsd:string\"/>\n";
		str +="	</xsd:complexType>\n";
		str +="\n";
		str +=" <xsd:element name=\"Project\" type=\"ProjectType\"/>\n";
		str +="\n";
		str +="</xsd:schema>\n";
		str +="\n";
	}
		return str;
}
} //namespace
#endif
