// UdmBackendDump.cpp : Defines the entry point for the console application.
//

// UdmBackendDump	-	Tihamér Levendovszky March, 2002


#include <iostream>
#include <sstream>


#include "BackendDump.h"
#include "Uml.h"

UDM_USE_DOM
#ifdef _WIN32
UDM_USE_MGA
#endif

void dummy(void) {; } // Dummy function for initialization


int main(int argc, char* argv[])
{
	if(argc!=3) 
	{
		std::cout<<"UdmBackendDump <backend_file.mga>|<backend_file.xml> <backend_meta_file.xml>"<<std::endl;
		return -1;
	}

//	try
//	{		
		// Loading the meta for the host graph
		Udm::SmartDataNetwork  ddnMeta(::Uml::diagram);	
		::Uml::Diagram theUmlDiagram;

		// Opening the XML meta of the host graph
		ddnMeta.OpenExisting(argv[2],"uml.dtd", Udm::CHANGES_LOST_DEFAULT);
		
		// Casting the DataNetwork to diagram
		theUmlDiagram = ::Uml::Diagram::Cast(ddnMeta.GetRootObject());
		
		// Creating the UDM diagram
		Udm::UdmDiagram udmDataDiagram;
		udmDataDiagram.dgr = &theUmlDiagram;
		udmDataDiagram.init = dummy;

		Udm::SmartDataNetwork sdnBackend(udmDataDiagram);
		sdnBackend.OpenExisting(argv[1], "", Udm::CHANGES_PERSIST_ALWAYS);

		// Dump to stdout
		{
			CBackendDump dmp;
			dmp.Dump(&sdnBackend);
		}

		// Dump to string - for Windows GUI Applications
		{
			std::stringstream strout;
			CBackendDump dmp(&strout);
			dmp.Dump(&sdnBackend);
			//strout.rdbuf()->freeze(0);
		}
		
//	}
//	catch(udm_exception &exc) 
//	{
//		cout<<"Exception occurred: "<<exc.what()<<endl;
//	}

	return 0;
}

