// cintstringTest.cpp


#include <UdmBase.h>
#include <cint_string.h>
#include "genericTest.h"
#include "LampDiagram.h"

#ifdef WIN32
#include <io.h>
#include <stdio.h>
#else	//WIN32
#include <unistd.h>
#endif	//WIN32

#include <cstdlib>


// Registers the fixture into the 'registry'

CPPUNIT_TEST_SUITE_REGISTRATION( UdmTests::genericTest );

#define WIN_TEMP_ENV "TEMP"
#define WIN_TMP_ENV "TMP"


UDM_USE_DOM

#ifdef _WIN32
UDM_USE_MGA
#endif

using namespace LampDiagram;

char * UdmTests::genericTest::getRndFileName()
{
#ifdef WIN32
		char * def_tmp_dir = "c:\\";
#else
		char * def_tmp_dir = "/tmp/";
#endif
		
		char * tmp_dir = NULL;
		tmp_dir = getenv(WIN_TEMP_ENV);
		if (!tmp_dir)
			tmp_dir = getenv(WIN_TMP_ENV);
		
		if (!tmp_dir) tmp_dir = def_tmp_dir;
		
		//create a temporary folder and set the path
		//char t_dir_name[L_tmpnam +1];
		char * t_dir_name;
#ifdef WIN32
		t_dir_name = _tempnam(tmp_dir, "UDM");
#else	//WIN32
		t_dir_name = tempnam(tmp_dir, "UDM");
#endif

		return t_dir_name;
};

void UdmTests::genericTest::removeFile(const string &pathname)
{
#ifdef WIN32
	_unlink(pathname.c_str());
#else
	unlink(pathname.c_str());
#endif
}

bool UdmTests::genericTest::generictest(const char * src, const char * dst)
{

	Udm::SmartDataNetwork nw(LampDiagram::diagram);

		nw.CreateNew(src,"LampDiagram", RootFolder::meta);

		{
			RootFolder rrr = RootFolder::Cast(nw.GetRootObject());
			int count = 0;
			for (count = 0; count < 1; count++)
			{
				cout << "." ;
				Lamp doubleBulbLamp = Lamp::Create(rrr);

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

				w1.end<ElectricTerminal, Wire::ACE_End1>() = plt1;
				w1.end<ElectricTerminal, Wire::ACE_End2>() = mst1;


				//#2, #3: connect main switch output to sw1, sw2 inputs
				w2.end<ElectricTerminal, Wire::ACE_End1>() = mst2;
				w2.end<ElectricTerminal, Wire::ACE_End2>() = s1t1;

				w3.end<ElectricTerminal, Wire::ACE_End1>() = mst2;
				w3.end<ElectricTerminal, Wire::ACE_End2>() = s2t1;

				w10.end<ElectricTerminal, Wire::ACE_End1>() = mst2;
				w10.end<ElectricTerminal, Wire::ACE_End2>() = s3t1;
			
			
				//#4: connect sw1 output to bulb 1
				w4.end<ElectricTerminal, Wire::ACE_End1>() = s1t2;
				w4.end<ElectricTerminal, Wire::ACE_End2>() = b1t1;

				//#5: connect sw2 output to bulb 2
				w5.end<ElectricTerminal, Wire::ACE_End1>() = s2t2;
				w5.end<ElectricTerminal, Wire::ACE_End2>() = b2t1;

				//#8: connect sw3 output to halogenbulb
				w8.end<ElectricTerminal, Wire::ACE_End1>() = s3t2;
				w8.end<ElectricTerminal, Wire::ACE_End2>() = b3t1;


				//ground connections:
				//#6, #7 connect plug 2 to bulbs
				w6.end<ElectricTerminal, Wire::ACE_End1>() = plt2;
				w6.end<ElectricTerminal, Wire::ACE_End2>() = b1t2;

				w7.end<ElectricTerminal, Wire::ACE_End1>() = plt2;
				w7.end<ElectricTerminal, Wire::ACE_End2>() = b2t2;

				w9.end<ElectricTerminal, Wire::ACE_End1>() = plt2;
				w9.end<ElectricTerminal, Wire::ACE_End2>() = b3t2;

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
				//cl1.src_end() = bulb1;
				//cl1.dst_end() = switch1;
			
	
			
				//the other way to set associations would be
				set<ControlLink> s_cl;
				s_cl.insert(cl1);
				bulb1.connectors<ControlLink, Switch, Bulb::AR_dst>() = s_cl;
				switch1.connectors<ControlLink, Bulb, Switch::AR_src>() = s_cl;
				// this should fail to compile if enabled
#if CHECK_COMPILE
				switch1.connectors<ControlLink, HalogenBulb, Switch::AR_src>() = s_cl;
#endif
	

				ControlLink cl2 = ControlLink::Create(doubleBulbLamp);
				cl2.end<Bulb, ControlLink::ACE_src>() = bulb2;
				cl2.end<Switch, ControlLink::ACE_dst>() = switch2;
				cl2.name() = "Bulb 2 switcher";

				ControlLink cl3 = ControlLink::Create(doubleBulbLamp);
				cl3.end<Bulb, ControlLink::ACE_src>() = bulb3;
				cl3.end<Switch, ControlLink::ACE_dst>() = switch3;
				cl3.name() = "Halogen Bulb  switcher";

				// this should fail to compile if enabled
#if CHECK_COMPILE
				CPPUNIT_ASSERT( (bulb3 == cl3.end<HalogenBulb, ControlLink::ACE_src>()) );
#endif


			
				set<ControlLink> lks = doubleBulbLamp.children_kind<ControlLink>();
				set<ControlLink>::iterator l1;

				for(l1 = lks.begin(); l1 != lks.end(); l1++) 
				{
					bool bvar11 = ((*l1 == cl1) && (Bulb(l1->end<Bulb, ControlLink::ACE_src>()) == bulb1));
					bool bvar12 = ((*l1 == cl1) && (Switch(l1->end<Switch, ControlLink::ACE_dst>()) == switch1));

					bool bvar21 = ((*l1 == cl2) && (Bulb(l1->end<Bulb, ControlLink::ACE_src>()) == bulb2));
					bool bvar22 = ((*l1 == cl2) && (Switch(l1->end<Switch, ControlLink::ACE_dst>()) == switch2));

					bool bvar31 = ((*l1 == cl3) && (Bulb(l1->end<Bulb, ControlLink::ACE_src>()) == bulb3));
					bool bvar32 = ((*l1 == cl3) && (Switch(l1->end<Switch, ControlLink::ACE_dst>()) == switch3));

					bool bvar1 = bvar11 && bvar12;
					bool bvar2 = bvar21 && bvar22;
					bool bvar3 = bvar31 && bvar32;

					if (bvar1)	CPPUNIT_ASSERT((!bvar2 && !bvar3));
					if (bvar2)	CPPUNIT_ASSERT((!bvar3 && !bvar1));
					if (bvar3)	CPPUNIT_ASSERT((!bvar1 && !bvar2));

				}
				

				set<Bulb> s_bulb = doubleBulbLamp.children_kind<Bulb>();
				CPPUNIT_ASSERT (s_bulb.size() == 3);

				// can also search children by derived type
				set<HalogenBulb> s_hbulb = doubleBulbLamp.children_kind<HalogenBulb>();
				CPPUNIT_ASSERT(s_hbulb.size() == 1);

				for(set<Bulb>::iterator s_bulb_i = s_bulb.begin(); s_bulb_i != s_bulb.end(); s_bulb_i++)
				{
					set<ControlLink> s_cls = s_bulb_i->connectors<ControlLink, Switch, Bulb::AR_dst>();
					ControlLink cll = *s_cls.begin();
					vector<Udm::ObjectImpl*> sw_ois = s_bulb_i->__impl()->getAssociation(Bulb::meta_dst, Udm::TARGETFROMPEER);
					Switch peer_switch(*sw_ois.begin());
					
					CPPUNIT_ASSERT(s_cls.size() == 1);
					CPPUNIT_ASSERT(sw_ois.size() == 1);
					CPPUNIT_ASSERT(peer_switch);
					CPPUNIT_ASSERT(cl1);

					CPPUNIT_ASSERT (peer_switch == Switch(cll.end<Switch, ControlLink::ACE_dst>()));
			
				}

				//deleting by omission from the new set of children
				set<ControlLink> cl_test_set = doubleBulbLamp.children_kind<ControlLink>();
				CPPUNIT_ASSERT( cl_test_set.size() == 3);				
				cl_test_set.erase(cl_test_set.begin(), cl_test_set.end());
				cl_test_set.insert(cl1);
				try
				{
					doubleBulbLamp.children<ControlLink, Lamp::CR_ControlLink_children>() = cl_test_set;
				}
				catch (udm_exception &e)
				{
					cout << e.what() << endl;
				};
				cl_test_set = doubleBulbLamp.children_kind<ControlLink>();
				CPPUNIT_ASSERT( cl_test_set.size() == 1);
				CPPUNIT_ASSERT(*(cl_test_set.begin()) == cl1);




				//deleting by setting the parent to NULL
				//this is  way to delete cl1
				cl1.parent() = NULL;
				cl_test_set = doubleBulbLamp.children_kind<ControlLink>();
				CPPUNIT_ASSERT( cl_test_set.size() == 0);
				

				//recreate the deleted controlinks and reestablishthe connections
				cl1 = ControlLink::Create(doubleBulbLamp);
				cl1.end<Bulb, ControlLink::ACE_src>() = bulb1;
				cl1.end<Switch, ControlLink::ACE_dst>() = switch1;
				cl1.name() = "Bulb 1 switcher";

				cl2 = ControlLink::Create(doubleBulbLamp);
				cl2.end<Bulb, ControlLink::ACE_src>() = bulb2;
				cl2.end<Switch, ControlLink::ACE_dst>() = switch2;
				cl2.name() = "Bulb 2 switcher";

				cl3 = ControlLink::Create(doubleBulbLamp);
				cl3.end<Bulb, ControlLink::ACE_src>() = bulb3;
				cl3.end<Switch, ControlLink::ACE_dst>() = switch3;
				cl3.name() = "Halogen Bulb  switcher";

				cl_test_set = doubleBulbLamp.children_kind<ControlLink>();
				CPPUNIT_ASSERT( cl_test_set.size() == 3);
				CPPUNIT_ASSERT( Bulb(cl2.end<Bulb, ControlLink::ACE_src>()) == bulb2);
				CPPUNIT_ASSERT( Switch(cl2.end<Switch, ControlLink::ACE_dst>()) == switch2);
				CPPUNIT_ASSERT( Bulb(cl3.end<Bulb, ControlLink::ACE_src>()) == bulb3);
				CPPUNIT_ASSERT( Switch(cl3.end<Switch, ControlLink::ACE_dst>()) == switch3);
				CPPUNIT_ASSERT( Bulb(cl1.end<Bulb, ControlLink::ACE_src>()) == bulb1);
				CPPUNIT_ASSERT( Switch(cl1.end<Switch, ControlLink::ACE_dst>()) == switch1);

			

				
				//testing now associations

				Bulb b11 = cl1.end<Bulb, ControlLink::ACE_src>();
				Bulb b12 = cl2.end<Bulb, ControlLink::ACE_src>();

				cl1.end<Bulb, ControlLink::ACE_src>() = b12;
				cl2.end<Bulb, ControlLink::ACE_src>() = b11;

			
				//cl1 points to bulb2 and cl2 points to bulb1
				lks = doubleBulbLamp.children_kind<ControlLink>();
				for(l1 = lks.begin(); l1 != lks.end(); l1++) 
				{
					bool bvar11 = ((*l1 == cl1) && (Bulb(l1->end<Bulb, ControlLink::ACE_src>()) == bulb2));
					bool bvar12 = ((*l1 == cl1) && (Switch(l1->end<Switch, ControlLink::ACE_dst>()) == switch1));

					
					bool bvar21 = ((*l1 == cl2) && (Bulb(l1->end<Bulb, ControlLink::ACE_src>()) == bulb1));
					bool bvar22 = ((*l1 == cl2) && (Switch(l1->end<Switch, ControlLink::ACE_dst>()) == switch2));

					bool bvar31 = ((*l1 == cl3) && (Bulb(l1->end<Bulb, ControlLink::ACE_src>()) == bulb3));
					bool bvar32 = ((*l1 == cl3) && (Switch(l1->end<Switch, ControlLink::ACE_dst>()) == switch3));

					bool bvar1 = bvar11 && bvar12;
					bool bvar2 = bvar21 && bvar22;
					bool bvar3 = bvar31 && bvar32;

					if (bvar1)	CPPUNIT_ASSERT((!bvar2 && !bvar3));
					if (bvar2)	CPPUNIT_ASSERT((!bvar3 && !bvar1));
					if (bvar3)	CPPUNIT_ASSERT((!bvar1 && !bvar2));

				}


			//testing parentroles
			


			Lamp lamp1 = switch1.parent<Lamp, Switch::PR_MainSwitch_Lamp_parent>();
			Lamp lamp2 = switch1.parent<Lamp, Switch::PR_FunctionSwitch_Lamp_parent>();
			RootFolder r1 = ((ElectricDevice) switch1).parent<RootFolder, ElectricDevice::PR_RootFolder_parent>();
			

			CPPUNIT_ASSERT(!lamp1);		//this should be false, switch1 is not a mainswitch
			CPPUNIT_ASSERT(lamp2);		//this should be true
			CPPUNIT_ASSERT(!r1);		//this should be false again


			
			Udm::Object o1 = switch1.parent();
			CPPUNIT_ASSERT(o1);
			CPPUNIT_ASSERT(o1 == lamp2);

			long id = o1.uniqueId();
			CPPUNIT_ASSERT(id);
			Udm::Object o2 = nw.ObjectById(id);
			CPPUNIT_ASSERT(o2 == o1);

			

			//testing child roles
			set<ElectricDevice> ed_s = doubleBulbLamp.children_kind<ElectricDevice>();
			CPPUNIT_ASSERT(ed_s.size() == 8); //all the devices: 3 Bulb, 3 switch, 1 plug, 1 lamp

			ElectricDevice ed = *(ed_s.begin());
			set<Wire> ed_wires = ed.children<Wire, ElectricDevice::CR_Wire_children>();

			Switch dbl_mainSwitch = doubleBulbLamp.child<Switch, Lamp::CR_MainSwitch>();
			ElectricDevice dbl_mainSwitch_as_ED = doubleBulbLamp.child<ElectricDevice, Lamp::CR_MainSwitch>();

			set<Switch> dbl_switches = doubleBulbLamp.children<Switch, Lamp::CR_FunctionSwitch>();
			CPPUNIT_ASSERT(dbl_switches.size() == 3);
			set<ElectricDevice> dbl_switches_as_ED = doubleBulbLamp.children<ElectricDevice, Lamp::CR_FunctionSwitch>();
			CPPUNIT_ASSERT(dbl_switches_as_ED.size() == 3);

			set<Bulb> dbl_bulbs = doubleBulbLamp.children<Bulb, Lamp::CR_Bulb_children>();
			CPPUNIT_ASSERT(dbl_bulbs.size() == 3);

			set<HalogenBulb> dbl_hbulbs = doubleBulbLamp.children<HalogenBulb, Lamp::CR_Bulb_children>();
			CPPUNIT_ASSERT(dbl_hbulbs.size() == 1);

			//testing assignment
						
			Udm::SmartDataNetwork snw(LampDiagram::diagram);
			snw.CreateNew(dst,"LampDiagram",RootFolder::meta);
			snw = nw;						//Datanetwork copy operator
			//snw = nw;						//this should be also OK, but it does not work yet in case of MEM-MEM copy


			snw.CloseWithUpdate();
			
		}
		nw.CloseWithUpdate();
			
		}
	return false;
};

#ifdef _WIN32
void UdmTests::genericTest::testDOMMGA()
{
	cout << endl << "generic tests and copy DOM to MGA";
	char * fname = getRndFileName();
	std::string fname_std = std::string(fname) + ".xml";
	std::string fname_std_1 = std::string(fname) + ".mga";
	generictest(fname_std.c_str(),fname_std_1.c_str());
	removeFile(fname_std);
	removeFile(fname_std_1);
	free(fname);
};

void UdmTests::genericTest::testMEMMGA()
{
	cout << endl << "generic tests and copy MEM to MGA:";
	char * fname = getRndFileName();
	std::string fname_std = std::string(fname) + ".mem";
	std::string fname_std_1 = std::string(fname) + ".mga";
	generictest(fname_std.c_str(),fname_std_1.c_str());
	removeFile(fname_std);
	removeFile(fname_std_1);
	free(fname);
};

void UdmTests::genericTest::testMGAMGA()
{
	cout << endl << "generic tests and copy MGA to MGA:";
	char * fname = getRndFileName();
	std::string fname_std = std::string(fname) + ".mga";
	std::string fname_std_1 = std::string(fname) + "1.mga";
	generictest(fname_std.c_str(),fname_std_1.c_str());
	removeFile(fname_std);
	removeFile(fname_std_1);
	free(fname);
};

void UdmTests::genericTest::testMGADOM()
{
	cout << endl << "generic tests and copy MGA to DOM:";
	char * fname = getRndFileName();
	std::string fname_std = std::string(fname) + ".mga";
	std::string fname_std_1 = std::string(fname) + ".xml";
	generictest(fname_std.c_str(),fname_std_1.c_str());
	removeFile(fname_std);
	removeFile(fname_std_1);
	free(fname);
};

void UdmTests::genericTest::testMGAMEM()
{
	cout << endl << "generic tests and copy MGA to MEM:";
	char * fname = getRndFileName();
	std::string fname_std = std::string(fname) + ".mga";
	std::string fname_std_1 = std::string(fname) + ".mem";
	generictest(fname_std.c_str(),fname_std_1.c_str());
	removeFile(fname_std);
	removeFile(fname_std_1);
	free(fname);
};
#endif // _WIN32

void UdmTests::genericTest::testDOMDOM()
{
	cout << endl << "generic tests and copy DOM to DOM:";
	char * fname = getRndFileName();
	std::string fname_std = std::string(fname) + ".xml";
	std::string fname_std_1 = std::string(fname) + "1.xml";
	generictest(fname_std.c_str(),fname_std_1.c_str());
	removeFile(fname_std);
	removeFile(fname_std_1);
	free(fname);
};

void UdmTests::genericTest::testDOMMEM()
{
	cout << endl << "generic tests and copy DOM to MEM:";
	char * fname = getRndFileName();
	std::string fname_std = std::string(fname) + ".xml";
	std::string fname_std_1 = std::string(fname) + ".mem";
	generictest(fname_std.c_str(),fname_std_1.c_str());
	removeFile(fname_std);
	removeFile(fname_std_1);
	free(fname);
};

void UdmTests::genericTest::testMEMMEM()
{
	cout << endl << "generic tests and copy MEM to MEM:";
	char * fname = getRndFileName();
	std::string fname_std = std::string(fname) + ".mem";
	std::string fname_std_1 = std::string(fname) + "1.mem";
	generictest(fname_std.c_str(),fname_std_1.c_str());
	removeFile(fname_std);
	removeFile(fname_std_1);
	free(fname);
};

void UdmTests::genericTest::testMEMDOM()
{
	cout << endl << "generic tests and copy MEM to DOM:";
	char * fname = getRndFileName();
	std::string fname_std = std::string(fname) + ".mem";
	std::string fname_std_1 = std::string(fname) + ".xml";
	generictest(fname_std.c_str(),fname_std_1.c_str());
	removeFile(fname_std);
	removeFile(fname_std_1);
	free(fname);
};
