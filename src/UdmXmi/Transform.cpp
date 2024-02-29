// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>




#include <string>

#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#include <fstream.h>
#else
#include <iostream>
#include <fstream>
#endif

XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
//XALAN_USING_STD(endl)

#include <xercesc/util/PlatformUtils.hpp>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

XERCES_CPP_NAMESPACE_USE


#include <UdmXmi.h>

extern std::string __xmi_to_udm_str;
extern std::string __udm_to_xmi_str;



int do_xslt_trans( const char * file_in, const char * file_out, const char * file_xsl)
{


	int	theResult = -1;

	try
	{
	//	XALAN_USING_XERCES(XMLPlatformUtils)

		XALAN_USING_XALAN(XalanTransformer)

		// Call the static initializer for Xerces.
		XMLPlatformUtils::Initialize();

		// Initialize Xalan.
		XalanTransformer::initialize();

		{
			// Create a XalanTransformer.
			XalanTransformer theXalanTransformer;
			
			
			
			theResult = theXalanTransformer.transform(file_in, file_xsl, file_out);
			

			if(theResult != 0)
			{
				cerr << "XSLT Transform Error: \n" << theXalanTransformer.getLastError()
					<< std::endl  << std::endl; 
			}
		}

		// Terminate Xalan...
		XalanTransformer::terminate();

		// Terminate Xerces...
		XMLPlatformUtils::Terminate();

		// Clean up the ICU, if it's integrated...
		XalanTransformer::ICUCleanUp();
	}
	catch(...)
	{
		return -1;
	}

	return theResult;
};

int XmiToUdm(const char* in, const char * out)
{
	std::string xslt_name = std::string(in) + ".x2u.xslt";
	initXSLTstr();
	std::ofstream ff;
	ff.open(xslt_name.c_str());
	
	if(ff.good()) 
		ff << __xmi_to_udm_str;
	else 
		cerr << "Temporary file " + xslt_name +" could not be opened for writing!" << std::endl ;

	ff.close();


	return do_xslt_trans( in, out, xslt_name.c_str());
};

int UdmToXmi(const char * in, const char * out)
{
	std::string xslt_name = std::string(in) + ".u2x.xslt";

	initXSLTstr();
	std::ofstream ff;
	ff.open(xslt_name.c_str());
	
	if(ff.good()) 
		ff << __udm_to_xmi_str;
	else 
	{
		cerr << "Temporary file " + xslt_name +" could not be opened for writing!" << std::endl;
		return -1;
	}

	ff.close();

	
	return do_xslt_trans( in, out, xslt_name.c_str());

};




