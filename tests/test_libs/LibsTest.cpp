#include <UdmBase.h>

#ifdef _WIN32
UDM_USE_MGA
#endif
UDM_USE_DOM

#ifndef _WIN32
#include <unistd.h>
#endif

#include "LibsTest.h"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <iostream>

using namespace std;
using namespace LampDiagram;

CPPUNIT_TEST_SUITE_REGISTRATION( UdmTests::LibsTest );

bool UdmTests::remove_test_files = true;

void UdmTests::LibsTest::removeFile(const string &pathname)
{
#ifdef _WIN32
	_unlink(pathname.c_str());
#else
	unlink(pathname.c_str());
#endif
}

Lamp UdmTests::LibsTest::createLamp(RootFolder &root, const string &ns)
{
	Lamp lamp = Lamp::Create(root);
	lamp.name() = ns + " Lamp";

	Bulb bulb1	= Bulb::Create(lamp); 
	bulb1.name() = ns + " Bulb1";

	Bulb bulb2	= bulb1.CreateInstance(lamp); 
	bulb2.name() = ns + " Bulb2";

	Plug plug = Plug::Create(lamp);
	plug.name() = ns + " Plug";

	return lamp;
}

void UdmTests::LibsTest::testLibOperations(const string &backend_ext)
{
	Udm::SmartDataNetwork nw(LampDiagram::diagram), nw_lib1(LampDiagram::diagram), nw_lib2(LampDiagram::diagram);

	string lib1_name = "lib1." + backend_ext;
	string lib2_name = "lib2." + backend_ext;
	string test1_name = "test1." + backend_ext;
	string test2_name = "test2." + backend_ext;

	string metalocator = "LampDiagram";

	// create a datanetwork that will be attached later as a library
	{
		nw_lib1.CreateNew(lib1_name, metalocator, RootFolder::meta, Udm::CHANGES_PERSIST_ALWAYS);
		RootFolder lib1_root = RootFolder::Cast(nw_lib1.GetRootObject());
		createLamp(lib1_root, "Lib1");
		nw_lib1.CloseWithUpdate();
	}

	// create another datanetwork that will also be attached as a library;
	// attach to this the first library
	{
		nw_lib2.CreateNew(lib2_name, metalocator, RootFolder::meta, Udm::CHANGES_PERSIST_ALWAYS);
		RootFolder rrr = RootFolder::Cast(nw_lib2.GetRootObject());
		createLamp(rrr, "Lib2");

		nw_lib1.OpenExisting(lib1_name, metalocator, Udm::CHANGES_LOST_DEFAULT);
		RootFolder lib1_root = RootFolder::Cast(rrr.AttachLibrary(nw_lib1.GetRootObject(), lib1_name));
		nw_lib1.CloseNoUpdate();

		nw_lib2.CloseWithUpdate();
	}

	// create a new data-network and attach the previous lib
	{
		nw.CreateNew(test1_name, metalocator, RootFolder::meta, Udm::CHANGES_PERSIST_ALWAYS);
		RootFolder rrr = RootFolder::Cast(nw.GetRootObject());
		CPPUNIT_ASSERT(!rrr.isLibObject());
		CPPUNIT_ASSERT(!rrr.isLibRoot());

		Lamp lamp = Lamp::Create(rrr);
		lamp.name() = "My Lamp";

		nw_lib2.OpenExisting(lib2_name, metalocator, Udm::CHANGES_LOST_DEFAULT);
		//CBackendDump dumper(&cout);
		//dumper.VisitObjects(nw_lib2.GetRootObject());

		RootFolder lib2 = RootFolder::Cast(rrr.AttachLibrary(nw_lib2.GetRootObject(), lib2_name));
		// the root object of an attached library is not a library object because
		// it was created in the host data-network
		CPPUNIT_ASSERT(!lib2.isLibObject());

		string tmp_lib2_name;
		CPPUNIT_ASSERT(lib2.getLibraryName(tmp_lib2_name) && tmp_lib2_name == lib2_name);

		Lamp lib2_lamp = *((set<Lamp>) lib2.Lamp_kind_children()).begin();
		CPPUNIT_ASSERT(lib2_lamp.isLibObject());
		CPPUNIT_ASSERT(!lib2_lamp.isLibRoot());

		Plug lib2_plug = lib2_lamp.Plug_child();
		CPPUNIT_ASSERT(lib2_plug.isLibObject());

		Bulb lib2_bulb;
		set<Bulb> lib2_bulbs = lib2_lamp.Bulb_children();
		for (set<Bulb>::const_iterator i = lib2_bulbs.begin(); i != lib2_bulbs.end(); i++) {
			CPPUNIT_ASSERT(i->isLibObject());
			if (!i->isInstance())
				lib2_bulb = *i;
		}


		// lib1
		RootFolder lib1 = RootFolder::Cast( *lib2.GetChildObjects(RootFolder::meta).begin() );
		// nested lib, its root object is a library object because it is attached to
		// a library
		CPPUNIT_ASSERT(lib1.isLibObject());

		string tmp_lib1_name;
		CPPUNIT_ASSERT(lib1.getLibraryName(tmp_lib1_name) && tmp_lib1_name == lib1_name);

		Bulb bulb = lib2_bulb.CreateInstance(lamp);
		CPPUNIT_ASSERT(!bulb.isLibObject());

		Plug plug = Plug::Create(lamp);
		plug.name() = "My Plug";

		nw.CloseWithUpdate();
	}

	// reopen the project and repeat some tests
	{
		nw.OpenExisting(test1_name, metalocator, Udm::CHANGES_PERSIST_ALWAYS);
		RootFolder rrr = RootFolder::Cast(nw.GetRootObject());
		CPPUNIT_ASSERT(!rrr.isLibObject());
		Lamp lamp = *((set<Lamp>) rrr.Lamp_kind_children()).begin();
		CPPUNIT_ASSERT(!lamp.isLibObject());
		Bulb bulb = *((set<Bulb>) lamp.Bulb_children()).begin();
		CPPUNIT_ASSERT(!bulb.isLibObject());
		Plug plug = lamp.Plug_child();
		CPPUNIT_ASSERT(!plug.isLibObject());

		RootFolder lib2 = RootFolder::Cast( *rrr.GetChildObjects(RootFolder::meta).begin() );
		CPPUNIT_ASSERT(!lib2.isLibObject());
		CPPUNIT_ASSERT(lib2.isLibRoot());
		Lamp lib2_lamp = *((set<Lamp>) lib2.Lamp_kind_children()).begin();
		CPPUNIT_ASSERT(lib2_lamp.isLibObject());

		Plug lib2_plug = lib2_lamp.Plug_child();
		CPPUNIT_ASSERT(lib2_plug.isLibObject());

		Bulb lib2_bulb;
		set<Bulb> lib2_bulbs = lib2_lamp.Bulb_children();
		for (set<Bulb>::const_iterator i = lib2_bulbs.begin(); i != lib2_bulbs.end(); i++) {
			CPPUNIT_ASSERT(i->isLibObject());
			if (!i->isInstance())
				lib2_bulb = *i;
		}

		// lib1
		RootFolder lib1 = RootFolder::Cast( *lib2.GetChildObjects(RootFolder::meta).begin() );
		CPPUNIT_ASSERT(lib1.isLibObject());
		CPPUNIT_ASSERT(lib1.isLibRoot());

		// Instances() called on a library object sees the instances created inside the host
		set<Bulb> lib2_bulb_instances = lib2_bulb.Instances();
		CPPUNIT_ASSERT(lib2_bulb_instances.size() == 2);


		if (backend_ext != "xml") {
			// tests skipped on DOM because DOM has extra
			// code that says that RootFolder child of
			// RootFolder object is a library root

			// detach libraries
			lib2.setLibraryName(NULL);
			CPPUNIT_ASSERT(!lib2.isLibRoot());
			CPPUNIT_ASSERT(!lib2_lamp.isLibObject());

			CPPUNIT_ASSERT(!lib1.isLibObject());
			CPPUNIT_ASSERT(!lib1.isLibRoot());

			// re-attach
			lib1.setLibraryName(lib1_name.c_str());
			lib2.setLibraryName(lib2_name.c_str());

			CPPUNIT_ASSERT(!lib2.isLibObject());
			CPPUNIT_ASSERT(lib2.isLibRoot());
			CPPUNIT_ASSERT(lib2_lamp.isLibObject());

			CPPUNIT_ASSERT(lib1.isLibObject());
			CPPUNIT_ASSERT(lib1.isLibRoot());
		}

		nw.CloseNoUpdate();
	}

	if (remove_test_files) {
		removeFile(test1_name);
		removeFile(test2_name);
		removeFile(lib1_name);
		removeFile(lib2_name);
	}
}

#ifdef _WIN32
void UdmTests::LibsTest::testMGA()
{
	cout << endl << "library operations on MGA";
	testLibOperations("mga");
}
#endif

void UdmTests::LibsTest::testDOM()
{
	cout << endl << "library operations on DOM";
	testLibOperations("xml");
}

void UdmTests::LibsTest::testMEM()
{
	cout << endl << "library operations on MEM";
	testLibOperations("mem");
}

