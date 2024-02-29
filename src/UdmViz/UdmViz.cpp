// UdmViz.cpp : Defines the entry point for the console application.
//


#include "UdmVizDump.h"

void dummy(void) {; } // Dummy function for initialization


int main(int argc, char* argv[])
{
	if(argc!=3 && argc!=4) 
	{
		std::cout<<"UdmViz <backend_file> <backend_meta_file> [-<a>|<l><al>]"<<std::endl;
		return -1;
	}

	try
	{		
		// Loading the meta for the host graph
		Udm::SmartDataNetwork  ddnMeta(Uml::diagram);	
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

		UdmVizDump VizDump;
	

		if(argc==4)
		{
			if(argv[3][1]=='a')
			{
				VizDump.bAggregate=true;
			}
			else
			{
				VizDump.bAggregate=false;
			}

			VizDump.bLinks=false;

			if(argv[3][1]=='l')
			{
				VizDump.bLinks=true;
			}
			if(argv[3][2]=='l')
			{
				VizDump.bLinks=true;
			}
		}

		VizDump.Dump(&sdnBackend);

	}
	catch(udm_exception &exc) 
	{
		std::cerr<<"Exception occurred: "<<exc.what()<<std::endl;
	}

	return 0;
}




