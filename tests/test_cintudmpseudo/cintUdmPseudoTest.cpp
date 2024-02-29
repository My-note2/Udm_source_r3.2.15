// cintUdmPseudoTest.cpp


#include <UdmBase.h>
#include <cint_string.h>
#include <UdmCintSwig.h>
#include "cintUdmPseudoTest.h"
#include "LampDiagram.h"

// Registers the fixture into the 'registry'

CPPUNIT_TEST_SUITE_REGISTRATION( UdmTests::cintUdmPseudoTest );

#define WIN_TEMP_ENV "TEMP"
#define WIN_TMP_ENV "TMP"

using namespace LampDiagram;

void myDisplay(char *msg)
{
    cout << "cint: " << msg << endl;
}


char * UdmTests::cintUdmPseudoTest::getRndFileName()
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

bool UdmTests::cintUdmPseudoTest::cintudmpseudotest(const char * dgr_name)
{

	Udm::SmartDataNetwork nw(LampDiagram::diagram);

	nw.CreateNew(dgr_name,"LampDiagram", RootFolder::meta);

	{
		RootFolder rrr = RootFolder::Cast(nw.GetRootObject());
		Lamp lamp = Lamp::Create(rrr);
		lamp.name() = "Host Lamp ";
		lamp.ModelName() = "Sequence tester lamp";


		Bulb bulb  = Bulb::Create(lamp);
		bulb.name() = "Bulb 1";
		bulb.Wattage() = 100;
		bulb.Voltage() = 10;

		Plug pl = Plug::Create(lamp);


		unsigned long dn_id = bulb.__impl()->__getdn()->uniqueId();
		unsigned long o_id = bulb.uniqueId();


		//Cint testcode with a UdmPseudoObject created in interpreted context
		cint_init(NULL, myDisplay);
		cint_add_object(bulb,"cint_bulb");
		
		string code;
		code = "cint_bulb.SetStrVal(\"name\",\"cint test 1\");";
		cint_int_calc(code);
		code = "cint_bulb.SetRealVal(\"Voltage\",101);";
		cint_int_calc(code);
		code = "cint_bulb.SetRealVal(\"Wattage\",102);";
		cint_int_calc(code);


		
			
		CPPUNIT_ASSERT( (string)bulb.name() == "cint test 1");
		CPPUNIT_ASSERT( bulb.Voltage() == 101);
		CPPUNIT_ASSERT( bulb.Wattage() == 102);

		//Cint testcode with a UdmPseudoObject created in compiled context
			
		cint_init(NULL, myDisplay);
		UdmPseudoObject pseudo_bulb(dn_id, o_id);
		char code_1[200];
		sprintf(code_1, "UdmPseudoObject * p_pseudo_bulb  = (UdmPseudoObject *) 0x%p;\n", &pseudo_bulb);
		cint_int_eval(code_1);
		sprintf(code_1,"p_pseudo_bulb->SetStrVal(\"name\",\"cint test 2\");");
		cint_int_eval(code_1);
		sprintf(code_1,"p_pseudo_bulb->SetRealVal(\"Voltage\",201);");
		cint_int_eval(code_1);
		sprintf(code_1,"p_pseudo_bulb->SetRealVal(\"Wattage\",202);");
		cint_int_eval(code_1);
		
		CPPUNIT_ASSERT( (string)bulb.name() == "cint test 2");
		CPPUNIT_ASSERT( bulb.Voltage() == 201);
		CPPUNIT_ASSERT( bulb.Wattage() == 202);




	}
	nw.CloseWithUpdate();

	return false;
};

void UdmTests::cintUdmPseudoTest::testDOM()
{
	char * fname = getRndFileName();
	std::string fname_std = std::string(fname) + ".xml";
	cintudmpseudotest(fname_std.c_str());
	free(fname);
};

void UdmTests::cintUdmPseudoTest::testMEM()
{
	char * fname = getRndFileName();
	std::string fname_std = std::string(fname) + ".mem";
	cintudmpseudotest(fname_std.c_str());
	free(fname);
};
