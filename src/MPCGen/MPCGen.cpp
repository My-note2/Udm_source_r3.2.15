// MPCGen.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <fstream>
#include <string>

const char tmpl[] = "\
   staticname = * \n\
   includes += $(UDM_PATH)/include \n\
\n\
  Define_Custom(XME2UDM) {  \n\
      command           = &quot;$(UDM_PATH)&quot;/etc/xme2udmxml1 \n\
      inputext          = .xme \n\
      \n\
   }\n\
\n\
  Define_Custom(UDM) {\n\
      command           = &quot;$(UDM_PATH)&quot;/bin/Udm \n\
      inputext          = .xml\n\
   }\n";
//===============================
void usage()
{
  std::cout << "UDM project MPC generator. (c) 2005 ISIS Vanderbilt University" << std::endl;
  std::cout << "usage: MPCGen base_name xme_file [udm.exe command line options]" << std::endl;
}
//================================
int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    usage();
    exit(1);
  }

  std::string bn(argv[1]);
  std::string xmefn(argv[2]);

  std::string argstr;
  for (int i = 3; i < argc ; ++i)
    argstr += argv[i];


  std::string fname(bn);
  fname += ".mpc";
  std::ofstream out(fname.c_str());
  out << "project (" << bn <<") {" << std::endl;

  out << tmpl << std::endl;
  out << "XME2UDM_Files {"<< std::endl;
  out << xmefn << " >> " << bn << "_udm.xml"  << std::endl;
  out << "}"  << std::endl;


  out << "UDM_Files {"  << std::endl;
	out << "commandflags = " << argstr << std::endl;
  out << bn << "_udm.xml"  << " >> " << bn << ".h " << bn << ".cpp"  << std::endl;
  out << "}"  << std::endl;

  out << "Source_Files {"  << std::endl;
  out << bn << ".cpp"   << std::endl;
  out << "}"  << std::endl;

  out << "Header_Files {"  << std::endl;
  out << bn << ".h"  << std::endl;
  out << "}"  << std::endl;
  out << "}"  << std::endl;
	return 0;
}

