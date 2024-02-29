// CreateLampModel.cpp : Create and test UDM data
//

#include "LampDiagram.h"


#ifdef _WIN32
UDM_USE_MGA
#endif

//UDM_USE_DOM


#include <Uml.h>
#include <UdmStatic.h>
#include <UmlExt.h>
#include <UdmBase.h>
#include <cint_string.h>
#include <UdmDom.h>

using namespace LampDiagram;
using namespace std;

int main_dom_string (int argc, char * argv[])
{
	string xml_xsd = 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<?udm interface=\"LampDiagram\" version=\"1.00\"?>\
<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
<!-- generated on Thu Apr 01 12:28:35 2004 -->\
 <xsd:complexType name=\"BulbType\">\
  <xsd:sequence>\
   <xsd:element name=\"ElectricTerminal\" type=\"ElectricTerminalType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"Wire\" type=\"WireType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
  </xsd:sequence>\
  <xsd:attribute name=\"_id\" type=\"xsd:ID\"/>\
  <xsd:attribute name=\"Wattage\" type=\"xsd:double\" use=\"required\"/>\
  <xsd:attribute name=\"Voltage\" type=\"xsd:double\"/>\
  <xsd:attribute name=\"MaxTempRating\" type=\"xsd:long\"/>\
  <xsd:attribute name=\"MaxVoltageRating\" type=\"xsd:double\"/>\
  <xsd:attribute name=\"position\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"name\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"dst\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>\
  <xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>\
 </xsd:complexType>\
 <xsd:complexType name=\"WireType\">\
  <xsd:attribute name=\"_id\" type=\"xsd:ID\"/>\
  <xsd:attribute name=\"Amps\" type=\"xsd:double\"/>\
  <xsd:attribute name=\"End2_end_\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"End1_end_\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>\
  <xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>\
 </xsd:complexType>\
 <xsd:complexType name=\"PlugType\">\
  <xsd:sequence>\
   <xsd:element name=\"ElectricTerminal\" type=\"ElectricTerminalType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"Wire\" type=\"WireType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
  </xsd:sequence>";

xml_xsd+= " <xsd:attribute name=\"_id\" type=\"xsd:ID\"/>\
  <xsd:attribute name=\"MaxTempRating\" type=\"xsd:long\"/>\
  <xsd:attribute name=\"MaxVoltageRating\" type=\"xsd:double\"/>\
  <xsd:attribute name=\"position\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"name\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"Format\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>\
  <xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>\
 </xsd:complexType>\
 <xsd:complexType name=\"LampType\">\
  <xsd:sequence>\
   <xsd:element name=\"Bulb\" type=\"BulbType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"ControlLink\" type=\"ControlLinkType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"ElectricTerminal\" type=\"ElectricTerminalType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"HalogenBulb\" type=\"HalogenBulbType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"Plug\" type=\"PlugType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"Switch\" type=\"SwitchType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"Wire\" type=\"WireType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
  </xsd:sequence>\
  <xsd:attribute name=\"_id\" type=\"xsd:ID\"/>\
  <xsd:attribute name=\"MaxTempRating\" type=\"xsd:long\"/>\
  <xsd:attribute name=\"MaxVoltageRating\" type=\"xsd:double\"/>\
  <xsd:attribute name=\"position\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"name\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"ArrayStr\" type=\"xsd:string\" default=\"second;first;\"/>\
  <xsd:attribute name=\"ModelName\" type=\"xsd:string\" default=\"Default Lamp Name\"/>\
  <xsd:attribute name=\"ArrayInt\" type=\"xsd:string\" default=\"5;4;3;2;\"/>\
  <xsd:attribute name=\"ArrayBool\" type=\"xsd:string\" default=\"false;true;false;true;\"/>\
  <xsd:attribute name=\"ArrayReal\" type=\"xsd:string\" default=\"9;8;7;6;\"/>\
  <xsd:attribute name=\"RegAttr\" type=\"xsd:string\"/>";

xml_xsd += "<xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>\
  <xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>\
 </xsd:complexType>\
 <xsd:complexType name=\"HalogenBulbType\">\
  <xsd:sequence>\
   <xsd:element name=\"ElectricTerminal\" type=\"ElectricTerminalType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"Wire\" type=\"WireType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
  </xsd:sequence>\
  <xsd:attribute name=\"_id\" type=\"xsd:ID\"/>\
  <xsd:attribute name=\"Wattage\" type=\"xsd:double\" use=\"required\"/>\
  <xsd:attribute name=\"Voltage\" type=\"xsd:double\"/>\
  <xsd:attribute name=\"MaxTempRating\" type=\"xsd:long\"/>\
  <xsd:attribute name=\"MaxVoltageRating\" type=\"xsd:double\"/>\
  <xsd:attribute name=\"position\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"name\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"dst\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>\
  <xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>\
 </xsd:complexType>\
 <xsd:complexType name=\"SwitchType\">\
  <xsd:sequence>\
   <xsd:element name=\"ElectricTerminal\" type=\"ElectricTerminalType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"Wire\" type=\"WireType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
  </xsd:sequence>\
  <xsd:attribute name=\"_id\" type=\"xsd:ID\"/>\
  <xsd:attribute name=\"__child_as\">\
   <xsd:simpleType>\
    <xsd:restriction base=\"xsd:string\">\
     <xsd:enumeration value=\"FunctionSwitch\"/>\
     <xsd:enumeration value=\"MainSwitch\"/>\
     <xsd:enumeration value=\"ElectricDevice\"/>\
    </xsd:restriction>\
   </xsd:simpleType>\
  </xsd:attribute>";

  xml_xsd += "<xsd:attribute name=\"MaxTempRating\" type=\"xsd:long\"/>\
  <xsd:attribute name=\"MaxVoltageRating\" type=\"xsd:double\"/>\
  <xsd:attribute name=\"position\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"name\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"Safe\" type=\"xsd:boolean\"/>\
  <xsd:attribute name=\"Amps\" type=\"xsd:double\"/>\
  <xsd:attribute name=\"src\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>\
  <xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>\
 </xsd:complexType>\
 <xsd:complexType name=\"ControlLinkType\">\
  <xsd:attribute name=\"_id\" type=\"xsd:ID\"/>\
  <xsd:attribute name=\"name\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"dst_end_\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"src_end_\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>\
  <xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>\
 </xsd:complexType>\
 <xsd:complexType name=\"RootFolderType\">\
  <xsd:sequence>\
   <xsd:element name=\"Bulb\" type=\"BulbType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"HalogenBulb\" type=\"HalogenBulbType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"Lamp\" type=\"LampType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"Plug\" type=\"PlugType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
   <xsd:element name=\"Switch\" type=\"SwitchType\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\
  </xsd:sequence>\
  <xsd:attribute name=\"_id\" type=\"xsd:ID\"/>\
  <xsd:attribute name=\"RegAttr\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>";

  xml_xsd += " <xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>\
  <xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>\
 </xsd:complexType>\
 <xsd:complexType name=\"ElectricTerminalType\">\
  <xsd:attribute name=\"_id\" type=\"xsd:ID\"/>\
  <xsd:attribute name=\"position\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"Type\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"End2\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"End1\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_archetype\" type=\"xsd:IDREF\"/>\
  <xsd:attribute name=\"_derived\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_instances\" type=\"xsd:IDREFS\"/>\
  <xsd:attribute name=\"_desynched_atts\" type=\"xsd:string\"/>\
  <xsd:attribute name=\"_real_archetype\" type=\"xsd:boolean\"/>\
  <xsd:attribute name=\"_subtype\" type=\"xsd:boolean\"/>\
 </xsd:complexType>\
 <xsd:element name=\"RootFolder\" type=\"RootFolderType\"/>\
</xsd:schema>";

	

  string xml_str = "<RootFolder xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"LampDiagram.xsd\"><Lamp name=\"Host Lamp \" ArrayInt=\"2;3;4;5\" ArrayStr=\"first;second;\" ArrayBool=\"false;false;true;true\" ArrayReal=\"6.000000000000000;7.000000000000000;8.000000000000000;9.000000000000000\" ModelName=\"Sequence tester lamp\"><Bulb name=\"Bulb 1\" Voltage=\"10.000000\" Wattage=\"100.000000\"/><Bulb name=\"Bulb 2\" Voltage=\"20.000000\" Wattage=\"200.000000\"/><Bulb name=\"Bulb 3\" Voltage=\"30.000000\" Wattage=\"300.000000\"/></Lamp></RootFolder>";


	UdmDom::str_xsd_storage::StoreXsd("LampDiagram.xsd", xml_xsd);
	
	try
	{


		UdmDom::DomDataNetwork ddn(LampDiagram::diagram);
		ddn.OpenExistingFromString(xml_str,"LampDiagram.xsd", Udm::CHANGES_PERSIST_ALWAYS);
		RootFolder rf = RootFolder::Cast(ddn.GetRootObject());
		Lamp l = *( (set<Lamp>(rf.Lamp_kind_children())).begin());
		l.name() = " name changed!";
		ddn.CloseWithUpdate();

		const string& outstr = ddn.Str();
		cout << outstr << endl;
		

		ddn.CreateNewToString("LampDiagram.xsd", RootFolder::meta, Udm::CHANGES_PERSIST_ALWAYS);
		RootFolder rf1 = RootFolder::Cast(ddn.GetRootObject());
		Lamp l1 = Lamp::Create(rf1);
		l1.name() = "cool!";
		ddn.CloseWithUpdate();

		const string & outstr1 = ddn.Str();
		cout << outstr1 << endl;



	}
	catch (udm_exception &e)
	{
		cout << e.what() << endl;
	};

	return 1;
};

int main1(int argc, char * argv[])
{

	/*
	*
	*	UDM test code, which demonstrates how to preserve the order of children
	*	in the backend. It works only with MEM and DOM backends. 
	*
	*
	*/
	if(argc < 2) 
	{
			cout << "Usage: CreateLampModel <UdmBackEnd file(lamp to be created in)>\n";
			return -1;
	}
	try 
	{	
		
		Udm::SmartDataNetwork nw(diagram);

		nw.CreateNew(argv[1],"LampDiagram", RootFolder::meta, Udm::CHANGES_PERSIST_ALWAYS);

		{
			RootFolder rrr = RootFolder::Cast(nw.GetRootObject());
			Lamp lamp = Lamp::Create(rrr);
			lamp.name() = "Host Lamp ";
			lamp.ModelName() = "Sequence tester lamp";


			Bulb bulb1  = Bulb::Create(lamp);
			bulb1.name() = "Bulb 1";
			bulb1.Wattage() = 100;
			bulb1.Voltage() = 10;


			Bulb bulb2  = Bulb::Create(lamp);
			bulb2.name() = "Bulb 2";
			bulb2.Wattage() = 200;
			bulb2.Voltage() = 20;


			Bulb bulb3  = Bulb::Create(lamp);
			bulb3.name() = "Bulb 3";
			bulb3.Wattage() = 300;
			bulb3.Voltage() = 30;


		}
		nw.CloseWithUpdate();
		nw.OpenExisting(argv[1], "LampDiagram", Udm::CHANGES_PERSIST_ALWAYS);
		{
			RootFolder rrr = RootFolder::Cast(nw.GetRootObject());
			set<Lamp> lamps = rrr.Lamp_kind_children();
			Lamp lamp = *(lamps.begin());

			vector<Bulb> bulbs = lamp.Bulb_children();
			vector<Bulb> new_vector;

			vector<Bulb>::iterator bulb_i = bulbs.begin();
			cout << " The order of bulbs should be 1, 2, 3" << endl;
			cout << " The order of bulbs: " ;
			while (bulb_i != bulbs.end())
			{
				Bulb bulb = *bulb_i;
				cout << (string)bulb.name() << " ,";
				bulb_i++;
			}


			//inverting order
			vector<Bulb>::reverse_iterator bulb_ri = bulbs.rbegin();
			while (bulb_ri != bulbs.rend())
			{


				new_vector.push_back(*bulb_ri);
				++bulb_ri;
				

			}

			lamp.Bulb_children() = new_vector;

			bulbs = lamp.Bulb_children();
			bulb_i = bulbs.begin();
			cout << endl << " The order of bulbs should be 3, 2, 1" << endl;
			cout << " The order of bulbs: " ;
			while (bulb_i != bulbs.end())
			{
				Bulb bulb = *bulb_i;
				cout << (string)bulb.name() << " ,";
				bulb_i++;
			}


			//taking out from the middle - that's index 1
			Bulb middle_b = bulbs[1];
			bulbs = lamp.Bulb_children() -= middle_b;
			bulb_i = bulbs.begin();
			cout << endl << " Bulb 2 removed. The order of bulbs should be 3,1" << endl;
			cout << " The order of bulbs: " ;
			while (bulb_i != bulbs.end())
			{
				Bulb bulb = *bulb_i;
				cout << (string)bulb.name() << " ,";
				bulb_i++;
			}

			
			//adding back to the  end 
			middle_b = Bulb::Create(lamp);
			middle_b.name() = "Bulb 2 recreated";
			middle_b.Wattage() =2000;
			middle_b.Voltage() = 200;

			
			bulbs = lamp.Bulb_children();
		
			
			bulb_i = bulbs.begin();
		
			cout << endl << " Bulb 2 readded to the end. The order of bulbs should be 3,1,2" << endl;
			cout << " The order of bulbs: " ;
			while (bulb_i != bulbs.end())
			{
				Bulb bulb = *bulb_i;
				cout << (string)bulb.name() << " ,";
				bulb_i++;
			}
				

			cout << endl;



		}


	

	}
	catch (udm_exception e)
	{
		cout << "exception : " << e.what() << endl;
		
	}
	return 0;
	

}



int main_arch_der_sub_test(int argc, char * argv[])
{

	/*
	*
	*	UDM test code, which demonstrates how to use archetype/derived/subtypes
	*	with UDM, specially regarding attributes
	*
	*
	*/
	if(argc < 2) 
	{
			cout << "Usage: CreateLampModel <UdmBackEnd file(lamp to be created in)>\n";
			return -1;
	}
	try 
	{	
		
		Udm::SmartDataNetwork nw(diagram);

		nw.CreateNew(argv[1],"LampDiagram", RootFolder::meta, Udm::CHANGES_PERSIST_ALWAYS);

		{
			RootFolder rrr = RootFolder::Cast(nw.GetRootObject());
			Lamp lamp = Lamp::Create(rrr);
			lamp.name() = "Host Lamp ";
			lamp.ModelName() = "Sequence tester lamp";


			Bulb bulb1  = Bulb::Create(lamp);
			bulb1.name() = "Bulb 1";
			bulb1.Wattage() = 100;
			bulb1.Voltage() = 10;


			Bulb Bulb1_instance_1 = bulb1.CreateInstance(lamp);

			//Bulb1_instance_1.name() = "Bulb Instance 1";

			Bulb Bulb1_instance_2 = bulb1.CreateInstance(lamp);
			Bulb1_instance_2.name() = "Bulb Instance 2";

			Bulb Bulb1_derived_1 = bulb1.CreateDerived(lamp);
			//Bulb1_derived_1.name() = "Bulb derived 1";

			Bulb Bulb1_derived_2 = bulb1.CreateDerived(lamp);

			Bulb1_derived_2.name() = "Bulb derived 2";

	
			set<Udm::Object> ders = bulb1.derived();
			set<Udm::Object>::iterator d_i = ders.begin();
			while (d_i != ders.end())
			{
				Bulb derived_bulb = Bulb::Cast(*d_i++);
				cout << "Derived bulb name: " << (string)derived_bulb.name() << endl;

				Bulb archetype = Bulb::Cast(derived_bulb.archetype());
				cout << "Archetype bulb name: " << (string)archetype.name() << endl;

			}

			set<Udm::Object> insts = bulb1.instances();
			set<Udm::Object>::iterator i_i = insts.begin();
			while (i_i != insts.end())
			{
				Bulb instance_bulb = Bulb::Cast(*i_i++);
				cout << "Instance bulb name: " << (string)instance_bulb.name() << endl;

				Bulb archetype = Bulb::Cast(instance_bulb.archetype());
				cout << "Archetype bulb name: " << (string)archetype.name() << endl;

			}


			//bulb kind childrens
			set<Bulb> bulbs = lamp.Bulb_kind_children();
			int i = bulbs.size();

			
			//Bulb1_instance_1.parent() = NULL; 
			//Bulb1_derived_2.parent() = NULL;
			//bulb1.parent() = NULL;

			//bulb kind childrens
			bulbs = lamp.Bulb_kind_children();
			i = bulbs.size();

			bulb1.name() = "New Archetype name for Bulb1!";


			//Plug is required...
		
			Plug::Create(lamp);



		}
		nw.CloseWithUpdate();
		
		nw.OpenExisting(argv[1],"LampDiagram", Udm::CHANGES_PERSIST_ALWAYS);
		{
			RootFolder rf = RootFolder::Cast(nw.GetRootObject());
			vector<Lamp> rf_lamps = rf.Lamp_kind_children();
			vector<Bulb> lamp_bulbs = (*rf_lamps.begin()).Bulb_kind_children();
			vector<Bulb>::iterator lamp_bulbs_i = lamp_bulbs.begin();
			
			while (lamp_bulbs_i != lamp_bulbs.end())
			{
				Bulb this_bulb = *lamp_bulbs_i++;
				cout << " Bulb Name:  " << (string) this_bulb.name() << endl;
				cout << " -------------deriveds ----------- " << endl;

				set<Udm::Object> ders = this_bulb.derived();
				set<Udm::Object>::iterator d_i = ders.begin();
				while (d_i != ders.end())
				{
					Bulb derived_bulb = Bulb::Cast(*d_i++);
					cout << "\tDerived bulb name: " << (string)derived_bulb.name() << endl;

					Bulb archetype = Bulb::Cast(derived_bulb.archetype());
					cout << "\tArchetype bulb name: " << (string)archetype.name() << endl;

				}
				cout << " -------------instances ----------- " << endl;

				set<Udm::Object> insts = this_bulb.instances();
				set<Udm::Object>::iterator i_i = insts.begin();
				while (i_i != insts.end())
				{
					Bulb instance_bulb = Bulb::Cast(*i_i++);
					cout << "\tInstance bulb name: " << (string)instance_bulb.name() << endl;

					Bulb archetype = Bulb::Cast(instance_bulb.archetype());
					cout << "\tArchetype bulb name: " << (string)archetype.name() << endl;

				}

				cout << " -------------archetype ----------- " << endl;
				if (this_bulb.archetype())
				{
					Bulb archetype = Bulb::Cast(this_bulb.archetype());
					cout << "Archetype bulb name: " << (string)archetype.name() << endl;
				}
			}
		}
		

	}
	catch (udm_exception e)
	{
		cout << "exception : " << e.what() << endl;
		
	}
	return 0;
	

}

int main(int argc, char* argv[])
{

	/*
	*  This code creates a lamp, and then copies it to another Udm DataNetwork,
	*  using the Udm::DataNetwork assignment operator(=).
	*
	*  Two arguments are expected, two filenames. The first will be the datanetwork where the lamp
	*  is created, the second is a copy of the first. The filenames can have any extension of .MEM, .MGA,
	*  .DOM. In case of MGA, the Lamp paradigm must be registered first.
	*
	*	The example shows the basic UDM operations:
	*		- creating & removing objects, associations, manipulating children
	*		- association class based associations
	*		- setting & getting simple attributes, simple non-persistent attributes
	*		- setting & getting array attributes, array non-persistent attributes, accessing array attribute items by index
	*		- copying a datanetwork as a whole
	*		- different ways of walking the object tree
	*		- creating instances and subtypes
	*		
	*/
	if(argc < 3) {
			cout << "Usage: CreateLampModel <UdmBackEnd file(lamp to be created in)> <UdmBackEnd file(copy)>\n";
			return -1;
	}
	try {	
		
		
		Udm::SmartDataNetwork nw(diagram);

		nw.CreateNew(argv[1],"LampDiagram", RootFolder::meta, Udm::CHANGES_PERSIST_ALWAYS);

		{
			RootFolder rrr = RootFolder::Cast(nw.GetRootObject());
			int count = 0;
			//for (count = 0; count < 100; count++)
			//{
				Lamp doubleBulbLamp = Lamp::Create(rrr);

				
				vector<__int64> a;
				a.push_back(0x1234567890abcdefLL);	//64bit integer
				a.push_back(7);
				a.push_back(1);
				
				doubleBulbLamp.ArrayInt() = a;
				
				
				cout << " Array indexer test: integer..." << endl;
				cout << doubleBulbLamp.ArrayInt()[0] << endl;
				

				//doubleBulbLamp.ArrayInt()[4] = 3;
				//doubleBulbLamp.ArrayInt()[4] += 2;

				cout << (long) doubleBulbLamp.ArrayInt()[4] << endl;

				
				//testing non-persistent array attributes
				doubleBulbLamp.TempArrayInt() = a;			//assign
				a = doubleBulbLamp.TempArrayInt();			//retrieve
				//display
				cout << "Testing non-persistent array attribute(int) :" << endl;
				vector<__int64>::const_iterator a_i = a.begin();
				while (a_i != a.end()) cout << *a_i++ << ","; 
				cout << endl;

				cout << " Array indexer test: non-persistent integer..." << endl;
				cout << doubleBulbLamp.TempArrayInt()[0] << endl;
				//doubleBulbLamp.TempArrayInt()[4] = 3;
				//doubleBulbLamp.TempArrayInt()[4] += 2;

				cout << doubleBulbLamp.TempArrayInt()[4] << endl;

			
				vector<double> b;
				b.push_back(1.1);
				b.push_back(2.4);
				b.push_back(3.141592653589793234);
				doubleBulbLamp.ArrayReal() = b;

				//testing non-persistent array attributes
				doubleBulbLamp.TempArrayReal() = b;			//assign
				b = doubleBulbLamp.TempArrayReal();			//retrieve
				//display
				cout << "Testing non-persistent array attribute(double) :" << endl;
				vector<double>::const_iterator b_i =b.begin();
				while (b_i != b.end()) cout << *b_i++ << ","; 
				cout << endl;
			
			
				vector<bool> c;
				c.push_back(true);
				c.push_back(true);
				c.push_back(false);
				doubleBulbLamp.ArrayBool() = c;

				//testing non-persistent array attributes
				doubleBulbLamp.TempArrayBoolean() = c;			//assign
				c = doubleBulbLamp.TempArrayBoolean();			//retrieve
				//display
				cout << "Testing non-persistent array attribute(boolean) :" << endl;
				vector<bool>::const_iterator c_i = c.begin();
				while (c_i != c.end()) *c_i++ ? cout << "true," : cout <<"false,"; 
				cout << endl;

				vector<string> d;
				d.push_back("apple");
				d.push_back("orange");
				d.push_back("hello");
				d.push_back("semi;colon");
				d.push_back("doublesemi;;colon");
				d.push_back("triplesemi;;;colon");
				d.push_back("back\\slash");
				d.push_back("double back\\\\slash");
				d.push_back("triple back\\\\\\slash");
				d.push_back("triple semi-coloback\\;\\;\\;slash-1");
				d.push_back("triple semi-coloback\\\\\\;;;slash-2");
				doubleBulbLamp.ArrayStr() = d;


				cout << " Array indexer test: string..." << endl;
				cout << (string) doubleBulbLamp.ArrayStr()[4] << endl;
				//doubleBulbLamp.ArrayStr()[4] = " array indexer assignment";
				//doubleBulbLamp.ArrayStr()[4] += " += operator";

				cout << (string) doubleBulbLamp.ArrayStr()[4] << endl;

				//testing non-persistent array attributes
				doubleBulbLamp.TempArrayStr() = d;			//assign
				d = doubleBulbLamp.TempArrayStr();			//retrieve
				//display
				cout << "Testing non-persistent array attribute(string) :" << endl;
				vector<string>::const_iterator d_i = d.begin();
				while (d_i != d.end()) cout << *d_i++  << ","; 
				cout << endl;


				//annotations
				vector<string> annotations;
				annotations.push_back("Anno1=\\=my annotation on this Lamp\\=");
				annotations.push_back("Anno1/aspects=600,300");
				annotations.push_back("Anno1/aspects/*=");
				annotations.push_back("Anno1/hidden=0");
				annotations.push_back("Anno1/inheritable=0");
				doubleBulbLamp.annotations() = annotations;		//assign
				annotations = doubleBulbLamp.annotations();		//retrieve
				cout << "Annotations on doubleBulbLamp:" << endl;
				vector<string>::const_iterator annotations_i = annotations.begin();
				while (annotations_i != annotations.end())
					cout << "\t" << *annotations_i++ << endl;
				cout << endl;



				char lamp_name[70]; 
				sprintf(lamp_name, "HighLight XL150 instance no. %d, here follows end-of line:\n", count);
				doubleBulbLamp.name() = lamp_name;
				
				doubleBulbLamp.MaxVoltageRating() = 220.0; 
				doubleBulbLamp.MaxTempRating()    = 200; 
				ElectricTerminal termA = ElectricTerminal::Create(doubleBulbLamp);
				ElectricTerminal termB = ElectricTerminal::Create(doubleBulbLamp);
				termA.position() = "(500,20)";
				termB.position() = "(500,85)";

			
				Plug thePlug = Plug::Create(doubleBulbLamp);
				thePlug.position() = "(500,150)";
				thePlug.name() = "Plug";
				ElectricTerminal plt1 = ElectricTerminal::Create(thePlug);
				plt1.position() = "(20,100)";
				ElectricTerminal plt2 = ElectricTerminal::Create(thePlug);
				plt2.position() = "(20,200)";

		// create mainswitch and its terminals
				Switch mainSwitch = Switch::Create(doubleBulbLamp, Lamp::meta_MainSwitch);
				mainSwitch.position() = "(300,100)";
				mainSwitch.name() = "MainSW";
				ElectricTerminal mst1 = ElectricTerminal::Create(mainSwitch);
				mst1.position() = "(20,100)";
				ElectricTerminal mst2 = ElectricTerminal::Create(mainSwitch);
				mst2.position() = "(220,100)";

		//create switches and its terminals
				Switch switch1 = Switch::Create(doubleBulbLamp, Lamp::meta_FunctionSwitch);
				switch1.position() = "(300,250)";
				switch1.name() = "SW1";
				ElectricTerminal s1t1 = ElectricTerminal::Create(switch1);
				s1t1.position() = "(20,100)";
				ElectricTerminal s1t2 = ElectricTerminal::Create(switch1);
				s1t2.position() = "(220,100)";

				Switch switch2 = Switch::Create(doubleBulbLamp, Lamp::meta_FunctionSwitch);
				switch2.position() = "(300,250)";
				switch2.name() = "SW 2";
				ElectricTerminal s2t1 = ElectricTerminal::Create(switch2);
				s2t1.position() = "(20,100)";
				ElectricTerminal s2t2 = ElectricTerminal::Create(switch2);
				s2t2.position() = "(220,100)";


				Switch switch3 = Switch::Create(doubleBulbLamp, Lamp::meta_FunctionSwitch);
				switch3.position() = "(300,250)";
				switch3.name() = "Halogen switch";
				ElectricTerminal s3t1 = ElectricTerminal::Create(switch3);
				s3t1.position() = "(20,100)";
				ElectricTerminal s3t2 = ElectricTerminal::Create(switch3);
				s3t2.position() = "(220,100)";


			

	// Create bulbs with terminals
				Bulb bulb1	= Bulb::Create(doubleBulbLamp); 
				bulb1.name() = "Bulb1";
				bulb1.position() = "(100,150)";
				ElectricTerminal b1t1 = ElectricTerminal::Create(bulb1);
				b1t1.position() = "(220,100)";
				ElectricTerminal b1t2 = ElectricTerminal::Create(bulb1);
				b1t2.position() = "(220,200)";

				Bulb bulb2	= Bulb::Create(doubleBulbLamp); 
				bulb2.name() = "Bulb2";
				bulb2.position() = "(100,150)";
				ElectricTerminal b2t1 = ElectricTerminal::Create(bulb2);
				b2t1.position() = "(220,100)";
				ElectricTerminal b2t2 = ElectricTerminal::Create(bulb2);
				b2t2.position() = "(220,200)";
	
	//test inherited parentroles & childroles

				HalogenBulb bulb3 = HalogenBulb::Create(doubleBulbLamp, Lamp::meta_Bulb_children);
				bulb3.name() = "HalogenBulb3";
				bulb3.position() = "(100, 150)";
				ElectricTerminal b3t1 = ElectricTerminal::Create(bulb3, HalogenBulb::meta_ElectricTerminal_children);
				b3t1.position() = "(320,100)";
				ElectricTerminal b3t2 = ElectricTerminal::Create(bulb3);
				b3t2.position() = "(320,200)";
	







		// wire up the lamp
		//create wires in the lamp
				
				Wire w1 = Wire::Create(doubleBulbLamp);
				Wire w2 = Wire::Create(doubleBulbLamp);
				Wire w3 = Wire::Create(doubleBulbLamp);
				Wire w4 = Wire::Create(doubleBulbLamp);
				Wire w5 = Wire::Create(doubleBulbLamp);
				Wire w6 = Wire::Create(doubleBulbLamp);
				Wire w7 = Wire::Create(doubleBulbLamp);
				Wire w8 = Wire::Create(doubleBulbLamp);
				Wire w9 = Wire::Create(doubleBulbLamp);
				Wire w10 = Wire::Create(doubleBulbLamp);






		//connect them

				//live onnections
				//#1: connect plug 1 to main switch input
				//this is one way to connect it!

				w1.End1_end() = plt1;
				w1.End2_end() = mst1;


				//#2, #3: connect main switch output to sw1, sw2 inputs
				w2.End1_end() = mst2;
				w2.End2_end() = s1t1;

				w3.End1_end() = mst2;
				w3.End2_end() = s2t1;

				w10.End1_end() = mst2;
				w10.End2_end() = s3t1;
			
			
				//#4: connect sw1 output to bulb 1
				w4.End1_end() = s1t2;
				w4.End2_end() = b1t1;

				//#5: connect sw2 output to bulb 2
				w5.End1_end() = s2t2;
				w5.End2_end() = b2t1;

				//#8: connect sw3 output to halogenbulb
				w8.End1_end() = s3t2;
				w8.End2_end() = b3t1;


				//ground connections:
				//#6, #7 connect plug 2 to bulbs
				w6.End1_end() = plt2;
				w6.End2_end() = b1t2;

				w7.End1_end() = plt2;
				w7.End2_end() = b2t2;

				w9.End1_end() = plt2;
				w9.End2_end() = b3t2;






	// assign values to a few attrs		
			
				w1.Amps() = 3.5;
				w2.Amps() = 4;
				w3.Amps() = 5;
				w4.Amps() = 11;
				w5.Amps() = 12;
				w6.Amps() = 13;
				w7.Amps() = 15.0;
				w8.Amps() = 14.0;
				w9.Amps() = 16.0;
				w10.Amps() = 17.0;



				bulb1.Wattage() = 55;
				bulb2.Wattage() = 155;
				bulb3.Wattage() = 200;

				//this should got  a default value
				//doubleBulbLamp.ModelName() = "SuperDoubleLight 155";

				s1t1.Type() = string("Copper");
				s1t2.Type() = string("Aluminium");
				

				//creating control links

				ControlLink cl1 = ControlLink::Create(doubleBulbLamp);
				cl1.name() = "Bulb 1 switcher";

				//one way to set associations:
				cl1.src_end() = bulb1;
				cl1.dst_end() = switch1;
			
	/*
			
				//the other way to set associations would be
				set<ControlLink> s_cl;
				s_cl.insert(cl1);
				bulb1.dst() = s_cl;
				switch1.src() = s_cl;
	*/

				ControlLink cl2 = ControlLink::Create(doubleBulbLamp);
				cl2.src_end() = bulb2;
				cl2.dst_end() = switch2;
				cl2.name() = "Bulb 2 switcher";

				ControlLink cl3 = ControlLink::Create(doubleBulbLamp);
				cl3.src_end() = bulb3;
				cl3.dst_end() = switch3;
				cl3.name() = "Halogen Bulb  switcher";



			
				set<ControlLink> lks = doubleBulbLamp.ControlLink_kind_children();
				set<ControlLink>::iterator l1;

				for(l1 = lks.begin(); l1 != lks.end(); l1++) {
					cout << "Control: " << string(l1->name()) << " "; 
					cout << "Bulb: " << string(Bulb(l1->src_end()).name()) << " "; 
					cout << "Switch: " << string(Switch(l1->dst_end()).name()) << endl << endl; 
				}
				

				set<Bulb> s_bulb = doubleBulbLamp.Bulb_kind_children();
				for(set<Bulb>::iterator s_bulb_i = s_bulb.begin(); s_bulb_i != s_bulb.end(); s_bulb_i++)
				{
					cout << " Name of the bulbs: " << string(s_bulb_i->name()) << endl;
					set<ControlLink> s_cls = s_bulb_i->dst();
					cout << " number of associated controllinks: " << (long) (s_cls.size()) << endl;
					ControlLink cll = *s_cls.begin();
					cout << " associated control link: " << (string) cll.name() << endl;
					
					//this is safe, because the vector will contain only one pointer,
					//which is already clone()-ed,
					//but will be released as soon as peer_switch goes out of context
					vector<Udm::ObjectImpl*> sw_ois = s_bulb_i->__impl()->getAssociation(LampDiagram::Bulb::meta_dst, Udm::TARGETFROMPEER);
					cout << " number of peers(meta_dst) :" << (long) sw_ois.size() << endl;
					Switch peer_switch(*sw_ois.begin());
					cout << " name of peer: " << (string)peer_switch.name() << endl;
			
				}


			//testing object delete
			//this is  way to delete cl1
			//cl1.parent() = NULL;



			//testing new set of childs	
			//this is a way to delete cl2

			//set<ControlLink> cl_test_set;
			//cl_test_set.insert(cl1);
			//doubleBulbLamp.ControlLink_kind_children() = cl_test_set;


			//testing now associations

			Bulb b11 = cl1.src_end();
			Bulb b12 = cl2.src_end();

			cl1.src_end() = b12;
			cl2.src_end() = b11;

			lks = doubleBulbLamp.ControlLink_kind_children();


			for(l1 = lks.begin(); l1 != lks.end(); l1++) {
				cout << "Control: " << string(l1->name()) << " "; 
				cout << "Bulb: " << string(Bulb(l1->src_end()).name()) << " "; 
				cout << "Switch: " << string(Switch(l1->dst_end()).name()) << endl << endl; 
			}


			//testing parentroles


			Lamp lamp1 = switch1.MainSwitch_Lamp_parent();
			if (lamp1)
			{
				cout << "lamp1 is not NULL" << endl;
				long id = lamp1.uniqueId();
				cout << id << endl;
			}

			Lamp lamp2 = switch1.FunctionSwitch_Lamp_parent();
			if (lamp2)
			{
				cout << "lamp2 is not NULL" << endl;
				long id = lamp2.uniqueId();
				cout << id << endl;
			}

		
			RootFolder r1 = switch1.RootFolder_parent();
			if (r1)
			{
				cout << "r1 is not NULL" << endl;
				long id = r1.uniqueId();
				cout << id << endl;
			}

			Udm::Object o1 = switch1.parent();
			if (o1)
			{
				cout << "o1 is not NULL" << endl;
				long id = o1.uniqueId();
				cout << id << endl;

			}

			//testing child roles

			set<ElectricDevice> ed_s = doubleBulbLamp.ElectricDevice_kind_children();
			
			cout << ed_s.size() << " electric devices found in lamp ..." << endl;


			//create a switch directly in the rootfolder (testing the DTD features)
			//Switch rr_sw = Switch::Create(rrr);
			//rr_sw.name() = "rootfolder switch";

			//create derived object
		

			
			//testing assignment
						
			Udm::SmartDataNetwork snw(diagram);
			snw.CreateNew(argv[2],"LampDiagram",RootFolder::meta, Udm::CHANGES_PERSIST_ALWAYS);
			snw = nw;						//Datanetwork copy operator
			snw = nw;						//Datanetwork copy operator - again - should not duplicate

			snw.CloseWithUpdate();
		
			
			
			Udm::Object oo = nw.ObjectById(bulb1.uniqueId());
			Bulb uu = Bulb::Cast(oo);

			cout << long(uu.Wattage()) << endl;
			cout << long(oo.uniqueId()) << endl;


//		}
		nw.CloseWithUpdate();
			
		}
		
	
	}

	catch(const udm_exception &e)
	{
		cout << "Exception: " << e.what() << endl;
		return(11);

	} 

	try {				// reload the data to see if it is valid

		

		Udm::SmartDataNetwork nw(diagram);
		nw.OpenExisting(argv[1],"", Udm::CHANGES_LOST_DEFAULT);
		{
		
		
			RootFolder rr = RootFolder::Cast(nw.GetRootObject());
			set<LampDiagram::Lamp> l_s = rr.Lamp_kind_children();
			cout << " number of lamps: " << l_s.size() << endl;
			Lamp l = *l_s.begin();
			cout << "lamp: " << (string) l.name() ;

			cout << "ArrayInt: ";
			vector<__int64> vl = l.ArrayInt();
			vector<__int64>::const_iterator vl_i = vl.begin();
			
			while (vl_i != vl.end()) cout << *vl_i++ << ","; 
			
			cout << endl;
			
			cout << "ArrayFloat: ";
			vector<double> vd = l.ArrayReal();
			vector<double>::const_iterator vd_i = vd.begin();
			while (vd_i != vd.end())
				cout << *vd_i++ << ","; 
			cout << endl;
			
			cout << "ArrayBool: ";
			vector<bool> vb = l.ArrayBool();
			vector<bool>::const_iterator vb_i = vb.begin();
			while (vb_i != vb.end())
				cout << (*vb_i++ ? "true" : "false") << ", "; 
			cout << endl;

			cout << "ArrayStr: ";
			vector<string> vs = l.ArrayStr();
			vector<string>::const_iterator vs_i = vs.begin();
			while (vs_i != vs.end())
				cout << *vs_i++ << ","; 
			cout << endl;





			nw.CloseNoUpdate();
		}
		
		cout << "Basically, all is OK" << endl ;
	}

	catch(const udm_exception &e)	{
		cout << "Exception: " << e.what()  << endl;
		return(12);

	}

	cint_string a("hello");
	cint_string b("hell0");
	cint_string c("hello");

	if (a == b) cout << " a equals to b " << endl;
	if (a == c) cout << " a equals to c " << endl;

	cout << "Meta test: " << endl;
	cout << " Switch::meta_src:" << Switch::meta_src.name() << endl;
	cout << " Bulb::meta_dst:" << Bulb::meta_dst.name() << endl;
	
	return 0;
}


