// XmiToUdm.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include <UdmXmi.h>


int main (int argc, char **argv)
{
	if (argc < 3 || argc > 3)
	{
		std::cout << "Usage: " << std::endl <<
			"XmiToUdm XMI_UML.xml UDM_UML.xml"<< std::endl;
		return -1;
	}	
	XmiToUdm(argv[1], argv[2]);
	return 0;
};


