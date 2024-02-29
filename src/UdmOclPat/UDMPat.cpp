#include <iostream>
#include <fstream>
#include <cstdio>
#include <Uml.h>
#include <UmlExt.h>
#include "OCLConfig.h"
#include "UdmOcl.h"

UDM_USE_DOM

#ifdef _WIN32
UDM_USE_MGA
#endif

using namespace std;

void dummy(void) {; }

int main(int argc, char **argv) {

	if(argc != 4) {
		cerr << "Usage: UDMOCLPat <datalocator> <umldiagram> <patternfile>\n";
		cerr <<	"\tdatalocator:\t\tDOM:<domfilename> : open as DOM data\n";
		cerr << "\t\t\t\tGME:<projectname> : open as GME data \n";
		cerr << "\t\t\t\t<name_without_prefix> : open based on extension\n";
		cerr <<	"\tumldiagram: \t the UML-XML file that was used to create the data\n";
		cerr <<	"\tpatternfile: \t the pattern file\n";
		return -1;
	}
	try
	{		
		Udm::SmartDataNetwork  nmeta(Uml::diagram);
		nmeta.OpenExisting(argv[2],"Uml.dtd", Udm::CHANGES_LOST_DEFAULT);

		::Uml::Diagram thedgr = ::Uml::Diagram::Cast(nmeta.GetRootObject());
		Udm::UdmDiagram datadiagram;
		datadiagram.dgr = &thedgr;
		datadiagram.init = dummy;

		Udm::SmartDataNetwork nw(datadiagram);

		try {
			nw.OpenExisting(argv[1],"", Udm::CHANGES_LOST_DEFAULT);

			ifstream infile;
			infile.open(argv[3]);
			std::string s("");
			std::string line;
			while(!infile.eof()) {
				getline(infile, line);
				s = s + line + "\n";
			}

			UdmPat::ProcessPat(nw.GetRootMeta(), nw.GetRootObject(), GOCL_STL_NS()string( s ));
		}
		catch(const udm_exception e)
		{
			cerr << "Inner Exception: " << e.what();
			return(-1);
		} 
	}
	catch(const udm_exception e)
	{
		cerr << "Exception: " << e.what();
		return(-1);

	} 
	return 0;
}
