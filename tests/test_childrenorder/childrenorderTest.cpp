// cintstringTest.cpp


#include <UdmBase.h>
#include <cint_string.h>
#include "childrenorderTest.h"
#include "LampDiagram.h"

#ifdef WIN32
#include <io.h>
#include <stdio.h>
#else	//WIN32
#include <unistd.h>
#endif	//WIN32

#include <cstdlib>


// Registers the fixture into the 'registry'

CPPUNIT_TEST_SUITE_REGISTRATION( UdmTests::childrenorderTest );

#define WIN_TEMP_ENV "TEMP"
#define WIN_TMP_ENV "TMP"

using namespace LampDiagram;

std::string UdmTests::childrenorderTest::getRndFileName()
{
#ifdef WIN32
		const char * def_tmp_dir = "c:\\";
#else
		const char * def_tmp_dir = "/tmp/";
#endif
		
		const char * tmp_dir = NULL;
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
		std::string ret = t_dir_name != NULL ? t_dir_name : "";
		free(t_dir_name);

		return ret;
};

void UdmTests::childrenorderTest::removeFile(const string &pathname)
{
#ifdef WIN32
	_unlink(pathname.c_str());
#else
	unlink(pathname.c_str());
#endif
}

bool UdmTests::childrenorderTest::ordertest(const char * dgr_name)
{

	Udm::SmartDataNetwork nw(LampDiagram::diagram);

	nw.CreateNew(dgr_name,"LampDiagram", RootFolder::meta, Udm::CHANGES_PERSIST_ALWAYS);

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

		Plug pl = Plug::Create(lamp);


	}
	nw.CloseWithUpdate();
	nw.OpenExisting(dgr_name, "LampDiagram", Udm::CHANGES_PERSIST_ALWAYS);
	{
		RootFolder rrr = RootFolder::Cast(nw.GetRootObject());
		set<Lamp> lamps = rrr.Lamp_kind_children();
		Lamp lamp = *(lamps.begin());

		//checking the order of the Bulbs
		vector<Bulb> bulbs = lamp.Bulb_children();
		CPPUNIT_ASSERT(bulbs[0].Voltage() == 10);
		CPPUNIT_ASSERT(bulbs[1].Voltage() == 20);
		CPPUNIT_ASSERT(bulbs[2].Voltage() == 30);

		//inverting order
		
		vector<Bulb> new_vector;
		vector<Bulb>::reverse_iterator bulb_ri = bulbs.rbegin();
		while (bulb_ri != bulbs.rend())
		{
			new_vector.push_back(*bulb_ri);
			++bulb_ri;
		}
		lamp.Bulb_children() = new_vector;

		//checking the order of the Bulbs
		bulbs = lamp.Bulb_children();
		
		CPPUNIT_ASSERT(bulbs[0].Voltage() == 30);
		CPPUNIT_ASSERT(bulbs[1].Voltage() == 20);
		CPPUNIT_ASSERT(bulbs[2].Voltage() == 10);

		
		//taking out from the middle - that's index 1
		Bulb middle_b = bulbs[1];
		bulbs = lamp.Bulb_children() -= middle_b;
		
		
		//checking the order of the Bulbs
		bulbs = lamp.Bulb_children();
		CPPUNIT_ASSERT(bulbs[0].Voltage() == 30);
		CPPUNIT_ASSERT(bulbs[1].Voltage() == 10);
	
		
		//adding back to the  end 
		middle_b = Bulb::Create(lamp);
		middle_b.name() = "Bulb 2 recreated";
		middle_b.Wattage() =2000;
		middle_b.Voltage() = 200;

		
		//checking the order of the bulbs
		bulbs = lamp.Bulb_children();
		
		
		CPPUNIT_ASSERT(bulbs[0].Voltage() == 30);
		CPPUNIT_ASSERT(bulbs[1].Voltage() == 10);
		CPPUNIT_ASSERT(bulbs[2].Voltage() == 200);
		
	}
	nw.CloseWithUpdate();

	return false;
};

void UdmTests::childrenorderTest::testDOM()
{
	std::string fname_std = getRndFileName() + ".xml";
	ordertest(fname_std.c_str());
	removeFile(fname_std);
};

void UdmTests::childrenorderTest::testMEM()
{
	std::string fname_std = getRndFileName() + ".mem";
	ordertest(fname_std.c_str());
	removeFile(fname_std);
};
