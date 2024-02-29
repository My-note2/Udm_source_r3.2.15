// UdmToXmi.cpp : Defines the entry point for the console application.
//


#include <string>
#include <iostream>
#include <UdmXmi.h>


int main (int argc, char **argv)
{
	if (argc < 3 || argc > 3)
	{
		std::cout << "Usage: " << std::endl << 
			"UdmToXmi UDM_UML.xml XMI_UML.xml"<< std::endl;
		return -1;
	}	
	UdmToXmi(argv[1], argv[2]);
	return 0;
};

