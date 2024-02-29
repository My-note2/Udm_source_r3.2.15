#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include "../Udm/File2Code/File2Code.h"
//==================================
int main(int argc, char **argv)
{
 	if(argc < 3)
	{
		std::cerr << "Usage: " << argv[0] << " <text_filename> <namespace_name> <JAVA|CPP> <package_name>" << std::endl;
		return(-1);
	}

	try
	{

    std::string infname(argv[1]);

    std::string pn;

    File2Code::GenModeType mode(File2Code::CPP);
    if (argc >= 4)
    {
      std::string ar(argv[3]);
      if (ar == "JAVA")
      {
        if (argc != 5)
        {
          std::cerr << "Usage: " << argv[0] << " <text_filename> <namespace_name> <JAVA> <package_name>" << std::endl;
          return(-1);
        }
        pn = argv[4];
        mode = File2Code::JAVA;
      }
      else
      if (ar == "CPP")
      {
        mode = File2Code::CPP;
      }
    }
    File2Code f2c(".//", argv[2], infname, mode, pn);
    f2c.gen();

	}
 	catch(const std::exception &e)
	{
    std::cerr << "Exception: " << e.what();
		return 1;

   }

  return 0;
}
