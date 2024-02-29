/*
Copyright (c) Vanderbilt University, 2000-2001
ALL RIGHTS RESERVED
Vanderbilt University disclaims all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall Vanderbilt University be liable for
any special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.

  18/07/06 - endre
   - fix DomObject::isInstance()
   - allow setAssociation to change links of instance objects when direct is false
     (called from UdmUtil::reqCopyLinks for example)

  11/23/05 - endre
   - make DOM3GetElementAncestor non-recursive
   - optimize DOM3LookupNamespaceURI to fetch attribute prefix and value only
     when needed
   - fix memory leaks in DomObject::DSFind and MyEntityResolver::CheckForVersionInfo

  04/22/05 - kalmar

   - added setDomParserExternalSchemaLocation, which allows the parser to use 
        other xsd files to parse using different xsd's from 
        those being defined in the xml instance
   - Text attribute xml representation changed, it hides the attribute name in
      xml and the contained string is stored as a TEXT nodein the xml, the mixed ="true" is generated in schema sequence.
      This feature allows parsing and writting mixed containment like
      <A> Mixed <B size = "1"> containmnet </B> feature <A>
      The meta has class A, which contains class B, which has attribute size:String 
      and attribute value:Text
      only getTextValue and setTextValue need to be changed
    - added getNSURI in order to ensure URI mapping in the generated xml, too
      see AddURIToUMLNamespaceMapping, -u switch

  11/22/04	-	endre

			-	VC7++ .NET porting issues
  10/24/04	-	endre

			-		Added support for attributes persisted as text-nodes, like: 
					<cn>
						<value>
							1.64
						</value>
					</cn>
					
					Now cn has an attribute named "value" with value: 1.64

					Changed the function getStringAttr, setStringAttr and added three new functions:
					getElementNamed, getTextValue and setTextValue.



  02/10/04	-	endre

			-		DTDPath was not exported in DLL

  01/10/04	-	endre

			-		Fixed small bug in CreateNew(), even if the XSD location is provided with full path,
					in the XML file only the XSD filename should appear.

  07/09/04	-	endre

			-		Changed setChildren(). The function can be used to delete objects by omitting them
					from the new set of children which is passed to the function.
					If such deleted object had links, those links were not removed correctly..

  17/08/04	-	endre

			-		Changed setChildren(), ordering of XML elements (DOM_Node-s) was not taken care.
  23/07/04	-	endre
			
			-		Changed the usage of reqcopyLinks. The function has to be called twice, with the
					first with simpleLinks = true, then with simpleLinks = false, in order to copy 
					both types of links

  05/13/04	-	endre
			
				- fixed small bug in CreateNew: CreateNew() now sets the behaviour of the EntityResolver he uses
				if the DomDataNetwork is str based, it will set the entityresolver to UseStrXsd(), otherwise DontUseStrXSd()

  05/10/04	-	endre

				- added string based UdmDom::DomDataNetwork functionallity
  04/18/04	-	endre
			
				- when an object is detached, and the host datanetwork of the object is being part of a Udm project, 
				  the corresponding placeholder object (if any) should be detached as well.

  03/15/04	-	endre
				- Changed the syntax of createchild() according to Udm::ObjectImpl
  03/15/04	-	endre
			
			changed the internal dom attributes from using literal strings to constants
			defined in UdmDom namespace

  03/15/04	-	endre
	
			changed the uniqueId() function, There was a minor bug which caused a crash when uniqueId()
			was invoked on DomObject's without a data network

  24/02/04	-	endre

			Versioning of XML interfaces and checking the version upon loading XML documents
				- changed the class MyEntityResolver
				- DomDataNetwork::OpenExisting
  20/01/04	-	endre
  
			DomDataNetwork::CreateNew() will create XML-s using DOCTYPE Defintion (DTD),
			if a dtd file is provided as metalocator. 


  01/01/04 -    endre

			a lot of changelog info missing.
			the main change is that XSD is used instead of DTD

  11/26/02	-	endre

		-	now using Xerces 2.1.0. However, we stil using the deprecated API.
			We will change to IDOM, the apache recomended C++ binding later.

  06/05/02	-	endre
	
		-	Xerces 1_7_0 is crashing, linking against 1.6.0
		-	DomDataNetwork() builds a type map keyed by classnames, and findClass() works with that.
			This speeds up constructors looking for their type
		-	DomObject() is constructed with it's datanetwork pointer passed. __getdn is optimizied...it
			just returns a pointer.
		
		

  06/02/02	-	endre

		Upgrading xerces iface to Xerces 1.7.0
		Removed the entitysolver, which obviously had a bug. A DTD is looked up in the current directory and the path. 
		If not found, it's passed as an URI to xerces to resolve it. Providing the DTD as a windows resource no longer works. 
		(it had bugs, and it's obviously not platform independent!)

	
  05/10/02	-	endre
		
		 If a DomDataNetwork was closed by either CloseWithUpdate() or CloseWithNoUpdate(), the rootobject is set to NULL;
		 the ~DomDataNetwork() destructor simply returned when rootobject was NULL, thus did not remove itself from the map,
		 causing DomObject::__getdn() functions to crash when polling each networks RootObject.

		 The fix is that ~DomDataNetwork() removes itself from the map even if it has NULL RootObject.


  04/26/02	-	endre

		 getChildren() problem. Similar to getParent() problems.  Fixed

  04/14/02	-	endre
		
		  fixed the fix;) getParent() did not work correctly, if the parentrole's target was
		  an ancestor of the real parent. In that case the correct behaviour is to return the parent.

  04/10/02	-	endre
	
		fixed DomObject::getParent(). in certain situations this function
		returned it's parent even when the parentRole did not match.

*/

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#include <cstring>

// these redefinitions are for linking with the single-threaded runtime libs
#define strdup _strdup
#define strnicmp _strnicmp
#define access _access

#endif

// Use POSIX strncasecamp on other platforms
#ifndef WIN32
#include <strings.h>
#include <unistd.h>

#define strnicmp(a, b, n) strncasecmp(a,b,n)
#endif

#include <cstdio>
#include <cstdlib>
#include <fstream>
#if (defined(__MACH__) || defined(__APPLE__) )
// Since Apple LLVM version 5.0 this has to be included explicitly
// it is needed for access().
#include <unistd.h>
#endif


#include "Uml.h"
#include "UmlExt.h"
#include "UdmUtil.h"
//#include <ctime>


#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <xercesc/sax/EntityResolver.hpp>

#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "UdmDom.h"

#ifdef max
#undef max
#endif

XERCES_CPP_NAMESPACE_USE

#if _XERCES_VERSION < 30000
#define SET_ID_ATTR(dom_element, attr) dom_element->setIdAttribute(attr)
#else
#define SET_ID_ATTR(dom_element, attr) dom_element->setIdAttribute(attr, true)
#endif

#define UDM_DOM_URI_PREFIX "http://www.isis.vanderbilt.edu/2004/schemas/"

static const XMLCh gXML__id[] =
{
	chUnderscore, chLatin_i, chLatin_d, chNull
};

static const XMLCh gXML_udm_udmdom_nsmap[] =
{
	chLatin_u, chLatin_d, chLatin_m,
	chUnderscore, chLatin_u, chLatin_d, chLatin_m, chLatin_d, chLatin_o, chLatin_m,
	chUnderscore, chLatin_n, chLatin_s, chLatin_m, chLatin_a, chLatin_p,
	chNull
};

static const XMLCh gXML_udm[] =
{
	chLatin_u, chLatin_d, chLatin_m, chNull
};

static const XMLCh gXML_MBIS[] =
{
	chLatin_M, chLatin_B, chLatin_I, chLatin_S, chNull
};

static const XMLCh gXML_XMI[] =
{
	chLatin_X, chLatin_M, chLatin_I, chNull
};

static const XMLCh gXML_null[] =
{
	chNull
};

static const XMLCh gXML_space[] =
{
	chSpace, chNull
};

static const XMLCh gXML__subtype[] =
{
	chUnderscore, chLatin_s, chLatin_u, chLatin_b, chLatin_t, chLatin_y, chLatin_p, chLatin_e, chNull
};

static const XMLCh gXML__archetype[] =
{
	chUnderscore, chLatin_a, chLatin_r, chLatin_c, chLatin_h, chLatin_e, chLatin_t, chLatin_y, chLatin_p, chLatin_e, chNull
};

static const XMLCh gXML__derived[] =
{
	chUnderscore, chLatin_d, chLatin_e, chLatin_r, chLatin_i, chLatin_v, chLatin_e, chLatin_d, chNull
};

static const XMLCh gXML__instances[] =
{
	chUnderscore, chLatin_i, chLatin_n, chLatin_s, chLatin_t, chLatin_a, chLatin_n, chLatin_c, chLatin_e, chLatin_s, chNull
};

static const XMLCh gXML_t[] =
{
	chLatin_t, chNull
};

static const XMLCh gXML__desynched_atts[] =
{
	chUnderscore, chLatin_d, chLatin_e, chLatin_s, chLatin_y, chLatin_n, chLatin_c, chLatin_h, chLatin_e, chLatin_d,
	chUnderscore, chLatin_a, chLatin_t, chLatin_t, chLatin_s,
	chNull
};

static const XMLCh gXML___child_as[] =
{
	chUnderscore, chUnderscore, chLatin_c, chLatin_h, chLatin_i, chLatin_l, chLatin_d, chUnderscore, chLatin_a, chLatin_s, chNull
};

static const XMLCh gXML__real_archetype[] =
{
	chUnderscore, chLatin_r, chLatin_e, chLatin_a, chLatin_l,
	chUnderscore, chLatin_a, chLatin_r, chLatin_c, chLatin_h, chLatin_e, chLatin_t, chLatin_y, chLatin_p, chLatin_e, chNull
};

static const XMLCh gXML_false[] =
{
	chLatin_f, chLatin_a, chLatin_l, chLatin_s, chLatin_e, chNull
};

static const XMLCh gXML_true[] =
{
	chLatin_t, chLatin_r, chLatin_u, chLatin_e, chNull
};

static const XMLCh gXML__libname[] =
{
	chUnderscore, chLatin_l, chLatin_i, chLatin_b, chLatin_n, chLatin_a, chLatin_m, chLatin_e, chNull
};

static const XMLCh gXML_UTF_8[] =
{
	chLatin_U, chLatin_T, chLatin_F, chDash, chDigit_8, chNull
};


#ifdef _WIN32
char* transcode(const XMLCh* const toTranscode)
{
	MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager;
    if (!toTranscode)
        return 0;

    char* retVal = 0;
    if (*toTranscode)
    {
        // Calc the needed size
        const unsigned int neededLen = ::WideCharToMultiByte(CP_UTF8, 0, toTranscode, -1, NULL, 0, NULL, NULL);

        // Allocate a buffer of that size plus one for the null and transcode
        retVal = (char*) manager->allocate((neededLen + 1) * sizeof(char)); //new char[neededLen + 1];
        ::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)toTranscode, -1, retVal, neededLen+1, NULL, NULL);

        // And cap it off anyway just to make sure
        retVal[neededLen] = 0;
    }
     else
    {
        retVal = (char*) manager->allocate(sizeof(char)); //new char[1];
        retVal[0] = 0;
    }
    return retVal;
}


XMLCh* transcode(const std::string& toTranscode)
{
	MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager;
    XMLCh* retVal = 0;
	if (!toTranscode.length())
	{
        retVal = (XMLCh*) manager->allocate(sizeof(XMLCh));
        retVal[0] = 0;
		return retVal;
	}

    // Calculate the buffer size required
    const unsigned int neededLen = ::MultiByteToWideChar(CP_UTF8, 0, toTranscode.c_str(), toTranscode.length(), NULL, 0);
    if (neededLen == 0)
    {
        retVal = (XMLCh*) manager->allocate(sizeof(XMLCh));
        retVal[0] = 0;
        return retVal;
    }

    // Allocate a buffer of that size plus one for the null and transcode
    retVal = (XMLCh*) manager->allocate((neededLen + 1) * sizeof(XMLCh));
    ::MultiByteToWideChar(CP_UTF8, 0, toTranscode.c_str(), toTranscode.length(), (LPWSTR)retVal, neededLen + 1);

	retVal[neededLen] = 0;
    return retVal;
}

#elif UDM_LOCALE_IS_UTF8

char* transcode(const XMLCh* const toTranscode)
{
	return XMLString::transcode(toTranscode);
}

XMLCh* transcode(const std::string& toTranscode)
{
	return XMLString::transcode(toTranscode.c_str());
}

#else
#include "utf8/unchecked.h"
char* transcode(const XMLCh* const toTranscode)
{
	MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager;
	std::string transcoded;
	size_t length = 0;
	for (const XMLCh* c = toTranscode; *c != 0; c++)
	{
		length++;
	}
	utf8::unchecked::utf16to8(toTranscode, toTranscode + length, std::back_inserter(transcoded));
	char* const retVal = (char*) manager->allocate(transcoded.length() + 1);
	strcpy(retVal, transcoded.c_str());
	return retVal;
}

XMLCh* transcode(const std::string& toTranscode)
{
	MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager;
	std::vector<XMLCh> transcoded;
	utf8::unchecked::utf8to16(toTranscode.begin(), toTranscode.end(), std::back_inserter(transcoded));
	XMLCh* const retVal = (XMLCh*) manager->allocate((transcoded.size() + 1) * sizeof(XMLCh));
	XMLCh* copy = retVal;
	for (std::vector<XMLCh>::iterator it = transcoded.begin(); it != transcoded.end(); it++)
	{
		*copy = *it;
		copy++;
	}
	*copy = 0;
	return retVal;
}

#endif


namespace UdmDom
{	
	/*
		hack: To be GME-backend conformant,
		this tells whether a link is a GME reference
	*/
	bool isRefLink(const ::Uml::AssociationRole &meta, const int mode, const vector<Udm::ObjectImpl*>& nvect)
	{
		return ( ((string)(meta.name()) == "ref") && (mode == Udm::TARGETFROMPEER) && nvect.size()==1);
	};
	
// --------------------------- DomObject

	//function which returns the long id from the string id
	//used in Search(), and release()
	//the map of DOM_Element has this unsigned long id as key
	//to make it faster

	unsigned long GetLongID(const XMLCh *const a) 
	{
		//if DOMString compares to 0 than it refers to no string at all!
		if (!a || !XMLString::stringLen(a)) return 0;
		//this assertation failes
		//we should take a look why is this invoked with empty string
		//UDM_ASSERT (a != 0);
		
		char *buf = XMLString::transcode(a);

		char *endptr;
		// start parsing after the "id" prefix
		unsigned long retval = strtoul(buf + 2, &endptr, 16);
		UDM_ASSERT(*endptr == '\0');

		XMLString::release(&buf);

		return retval;

	};//eo unsigned long GetLongID(const XMLCh *const a)


#define EmptyVal(a) (XMLString::stringLen(a) == 0)

	class StrX
	{
	private :
		char* fLocalForm;
	
	public :
		StrX(const XMLCh* const toTranscode)
		{
			// Call the private transcoding method
			fLocalForm = transcode(toTranscode);
		}

		~StrX()
		{
			XMLString::release(&fLocalForm);
		}

		const char* localForm() const { return fLocalForm; }
	};

#define StrX(XMLstr) StrX(XMLstr).localForm()

	class XStr
	{
	private :
		XMLCh* fUnicodeForm;
	
	public :
		XStr(const std::string& toTranscode)
		{
			fUnicodeForm = transcode(toTranscode);
		}

		~XStr()
		{
			XMLString::release(&fUnicodeForm);
		}

		const XMLCh* unicodeForm() const { return fUnicodeForm; }
	};

#define X(str) XStr(str).unicodeForm()

	string getNSForNode(const DOMNode &node)
	{
		const XMLCh *namespaceURI = node.lookupNamespaceURI(node.getPrefix());
		if (namespaceURI == NULL) {
			return "";
			string e_description = "Empty namespace URIs are not allowed for node '";
			e_description += (string)StrX(node.getNodeName()) + "'";
			throw udm_exception(e_description);
		}

		char *ns_uri_p = XMLString::transcode(namespaceURI);

		xsd_ns_mapping_storage::str_str_map::iterator it_ns_map = xsd_ns_mapping_storage::static_xsd_ns_mapping_container.find(ns_uri_p);
		if (it_ns_map != xsd_ns_mapping_storage::static_xsd_ns_mapping_container.end()) {
			XMLString::release(&ns_uri_p);
			return it_ns_map->second;
		} else {
			string uri_prefix(UDM_DOM_URI_PREFIX);
			string ns_uri(ns_uri_p);
			XMLString::release(&ns_uri_p);

			// UDM_DOM_URI_PREFIX + namespace_path(delim = "/")
			if (ns_uri.compare(0, uri_prefix.length(), uri_prefix)) {
				string e_description = "Namespace URI does not begin with '" + uri_prefix + "' for node '";
				e_description += (string)StrX(node.getNodeName());
				e_description += "'. Did you create the mapping from UML to URI?";
				throw udm_exception(e_description);
			}

			ns_uri = ns_uri.substr(uri_prefix.length());
			if (ns_uri.find("__dgr_", 0) == 0)
				return "";
			else
				return ns_uri;
		}
	}



	UDM_DLL void AddURIToUMLNamespaceMapping(const string & namespaceURI, 
                              const string& namespaceUML,
                              const string& xsdName)
	{
		xsd_ns_mapping_storage::str_str_map::value_type item(namespaceURI, namespaceUML);
		pair<xsd_ns_mapping_storage::str_str_map::const_iterator, bool> ins_res = xsd_ns_mapping_storage::static_xsd_ns_mapping_container.insert(item);
		
		if (!ins_res.second && namespaceUML.compare(ins_res.first->second))
			throw udm_exception(string("A mapping from namespace URI '" + namespaceURI + "' exists already: " + namespaceUML));

		xsd_ns_mapping_storage::str_str_map::value_type item1(namespaceUML, namespaceURI);
		pair<xsd_ns_mapping_storage::str_str_map::const_iterator, bool> ins_res1 = xsd_ns_mapping_storage::static_xsd_ns_back_mapping_container.insert(item1);
		
		if (!ins_res1.second && namespaceURI.compare(ins_res1.first->second))
			throw udm_exception(string("A mapping from URI namespace '"  + namespaceURI + "' exists already: " + namespaceUML));

 		xsd_ns_mapping_storage::str_str_map::value_type item2(namespaceURI, xsdName);
		pair<xsd_ns_mapping_storage::str_str_map::const_iterator, bool> ins_res2 = xsd_ns_mapping_storage::uri2xsdname.insert(item2);
		
		if (!ins_res2.second && xsdName.compare(ins_res2.first->second))
			throw udm_exception(string("A mapping from URI namespace '"  + namespaceURI + "' exists already: " + xsdName));

	}

	UDM_DLL void RemoveURIToUMLNamespaceMapping(const string & namespaceURI)
	{
		xsd_ns_mapping_storage::static_xsd_ns_mapping_container.erase(namespaceURI);
		xsd_ns_mapping_storage::static_xsd_ns_back_mapping_container.erase(namespaceURI);
		xsd_ns_mapping_storage::uri2xsdname.erase(namespaceURI);
    
	}

	UDM_DLL void ClearURIToUMLNamespaceMappings()
	{
		xsd_ns_mapping_storage::static_xsd_ns_mapping_container.clear();
		xsd_ns_mapping_storage::static_xsd_ns_back_mapping_container.clear();
		xsd_ns_mapping_storage::uri2xsdname.clear();
	}

	//=================================
	const string getNSURI(const string& ns_path )
	{
		string ns_uri;

		//when searching in the map replace '/' with "::"
		string ns_path_mapped = UdmUtil::replace_delimiter(ns_path, "/", "::");

		xsd_ns_mapping_storage::str_str_map::const_iterator it_ns_map = xsd_ns_mapping_storage::static_xsd_ns_back_mapping_container.find(ns_path_mapped);
		if (it_ns_map != xsd_ns_mapping_storage::static_xsd_ns_back_mapping_container.end()) 
			ns_uri = it_ns_map->second.c_str();
		else
		{
			ns_uri = string(UDM_DOM_URI_PREFIX);
			//ns_uri += "/";
			ns_uri += ns_path;
		}
		return ns_uri;
	}



	static inline void setAttrValue(DOMElement &element, const string &name, const string &value)
	{
		XMLCh *name_buf = transcode(name);
		XMLCh *value_buf = transcode(value);
		element.setAttribute(name_buf, value_buf);
		XMLString::release(&name_buf);
		XMLString::release(&value_buf);
	}

	static inline void setAttrValue(DOMElement &element, const XMLCh *name, const string &value)
	{
		XMLCh *value_buf = transcode(value);
		element.setAttribute(name, value_buf);
		XMLString::release(&value_buf);
	}

	static inline const XMLCh *getAttrValue(DOMElement &element, const string &name)
	{
		XMLCh *name_buf = transcode(name);
		const XMLCh *value = element.getAttribute(name_buf);
		XMLString::release(&name_buf);

		return value;
	}

	void setTextValue(DOMElement &parent_element, const string &value)
	{
		
		XMLCh *value_buf = transcode(value);
		DOMText *tn = parent_element.getOwnerDocument()->createTextNode(value_buf);
		parent_element.appendChild(tn);
		tn->setNodeValue(value_buf);
		XMLString::release(&value_buf);
	};

	void setTextValues(DOMElement &parent_element, const vector<string> &values)
	{
		string str;
		for(vector<string>::const_iterator v_i = values.begin(); v_i != values.end(); ++v_i)
			str += *v_i;

		setTextValue(parent_element, str);
	}


 	void getTextValues(const DOMElement &parent_element, vector<string> &values)
	{
    
		for(DOMNode *n = parent_element.getFirstChild(); n != NULL; n = n->getNextSibling())	
		{
			if ( (n->getNodeType() == DOMNode::TEXT_NODE))
			{
				DOMText *tn = (DOMText*)n;
				char *a = transcode(tn->getNodeValue());
        
				values.push_back(a);

				XMLString::release(&a);
			}
		}
	}

	string getTextValue(const DOMElement &parent_element)
	{
		string retval;

		for (DOMNode *n = parent_element.getFirstChild(); n != NULL; n = n->getNextSibling())
		{
			if ( (n->getNodeType() == DOMNode::TEXT_NODE))
			{
				DOMText *tn = (DOMText*)n;
				char *a = transcode(tn->getNodeValue());
				retval = a;
				XMLString::release(&a);
				break;
			}
		}

		return retval;
	}


// --------------------------- DomObject

	typedef Udm::ObjectImpl ObjectImpl;
	typedef Udm::DataNetwork DataNetwork;

	class DomObject : public ObjectImpl
	{
	
		
	//friends	
	friend void		SaveAs(Object &rootelement, const char *filename);
	
	
	//we need to set static_order data
	//when OpenExisting() a Data Network which has 
	//ID assignments
	friend void		DomDataNetwork::MapExistingIDs(const DOMNode &d);
	

	private:
		static uniqueId_type static_orderdata;
		const DataNetwork * mydn;
	
		
	public:
#if _MSC_VER >= 1800
        DomObject(const DomObject&) = delete;
        DomObject& operator=(const DomObject&) = delete;
#endif

		DOMElement *dom_element;
		
		
		DomObject(const ::Uml::Class &meta, DOMElement *n, const DataNetwork * dn = NULL) :
		dom_element(n), m_type(meta)
		{
		
			mydn = dn;
			if (!mydn) throw udm_exception("DomObject without a data network ?");
		}

		
		DomObject(/*const ::Uml::Diagram &diagram,*/ DOMElement *element,  const DataNetwork * dn = NULL) : dom_element(element)
		{
			mydn = dn;
			if (!mydn) throw udm_exception("DomObject without a data network ?");

			m_type = findClass(*element);
			
		}

		// --- reference counting
		// DOM does the reference counting, DOM_Element is just another pointer
		

		ObjectImpl *clone()
		{
			TRY_XML_EXCEPTION
		
			return new DomObject(m_type, dom_element, mydn);

			CATCH_XML_EXCEPTION("DomObject::clone()");
		}//eo clone()

		void release()
		{
			//delete from map (do we need to ?)No! Only when parent is set to NULL!

			delete this;
		}//eo release()

		bool isSubtype() const
		{
			//DOMString a = dom_element.getAttribute("subtype");
			const XMLCh *a = dom_element->getAttribute(gXML__subtype);

			if(EmptyVal(a)) return false;		 // default is false!
			return XMLString::startsWith(a, gXML_t);	 // true			
		}

		bool isInstance() const
		{
			const XMLCh *a = dom_element->getAttribute(gXML__archetype);

			if(EmptyVal(a)) return false;		 // default is false!
		
			return !isSubtype();
		}

		Udm::DataNetwork *__getdn() 
		{
			return const_cast<Udm::DataNetwork*>(mydn);
		}//eo __getdn();


	// --- lookup

		::Uml::Class findClass(const DOMElement &element) const
		{
			//This could be faster also, diagram are relatively small
			//so I don't waste my time here

			string ns_path = getNSForNode(element);		//namespace_path(delim = "/")
			string key;			//namespace_path:class
			if (ns_path.size()) {
				ns_path = UdmUtil::replace_delimiter(ns_path, "/", ":");
				key += ns_path + ":";
			}

			char *cl_name = transcode(element.getLocalName());	//class name
			key += cl_name;
			XMLString::release(&cl_name);

			map<string, ::Uml::Class>::iterator mcc_i = ((DomDataNetwork *)mydn)->meta_class_cache.find(key);

			if (mcc_i != ((DomDataNetwork *)mydn)->meta_class_cache.end())
				return mcc_i->second;

			string e_description = "Couldn't find in cache the class of element '" + key + "'";
			throw udm_exception(e_description);
		}; //eo findClass()


		//next set of functions are invoked a lot of times
		//and as such they must be fast

		

		// returns index (>= 0), -1 if not found
		static int DSFind(const XMLCh *where, const string &what) {
			if(where == NULL) return -1;
			
			XMLCh *what_buf = transcode(what);
			int index = XMLString::patternMatch(where, what_buf);

			XMLString::release(&what_buf);

			return index;
		};

		static int DSFind(const XMLCh *where, const XMLCh *what) {
			if(what == NULL) throw udm_exception("Search for null string");
			if(where == NULL) return -1;
			
			return XMLString::patternMatch(where, what);
		};

		static XMLCh* DSAppend(const XMLCh *src, const string &data)
		{
			UDM_ASSERT(src != NULL);

			size_t ret_length = XMLString::stringLen(src) + 1 + data.length() + 1;
			XMLCh *retval = (XMLCh*) XMLPlatformUtils::fgMemoryManager->allocate(ret_length * sizeof(XMLCh));
			XMLString::copyString(retval, src);
			XMLString::catString(retval, gXML_space);

			XMLCh *data_buf = transcode(data);
			XMLString::catString(retval, data_buf);

			XMLString::release(&data_buf);

			return retval;
		}

		static XMLCh* DSAppend(const XMLCh *src, const XMLCh *data)
		{
			UDM_ASSERT(src != NULL);

			int ret_length = XMLString::stringLen(src) + 1 + XMLString::stringLen(data) + 1;
			XMLCh *retval = (XMLCh*) XMLPlatformUtils::fgMemoryManager->allocate(ret_length * sizeof(XMLCh));
			XMLString::copyString(retval, src);
			XMLString::catString(retval, gXML_space);
			XMLString::catString(retval, data);

			return retval;
		}

		static XMLCh *DSAppendTo(XMLCh **target, const XMLCh *data, bool releaseTarget = true)
		{
			UDM_ASSERT(target != NULL);

			if (data == NULL)
				return *target;

			XMLCh *new_target;

			if (*target == NULL)
				new_target = XMLString::replicate(data);
			else {
				int new_length = XMLString::stringLen(*target) + XMLString::stringLen(data) + 1;
				new_target = (XMLCh*) XMLPlatformUtils::fgMemoryManager->allocate(new_length * sizeof(XMLCh));
				XMLString::copyString(new_target, *target);
				XMLString::catString(new_target, data);
			}

			if (releaseTarget)
				XMLString::release(target);

			*target = new_target;
			return new_target;
		}

		// remove from target the substring given by the parameters
		static void DSRemoveSubstr(XMLCh *target, int startIndex, int count)
		{
			UDM_ASSERT(target != NULL);

			int length = XMLString::stringLen(target);
			UDM_ASSERT(startIndex >= 0 && startIndex < length);
			UDM_ASSERT(count >= 0);

			if (startIndex + count >= length)
				*(target + startIndex) = chNull;
			else
				// +1 for the end of string
				XMLString::moveChars(target + startIndex, target + startIndex + count, length - startIndex - count + 1);
		}

		const XMLCh* GetID() 
		{
			//static unsigned long idcount = time(NULL);

			uniqueId_type id;
			const XMLCh *a = dom_element->getAttribute(gXML__id);
			if(EmptyVal(a)) 
			{
				XMLCh buf[25] = { chLatin_i, chLatin_d, chNull };
				//we have a unique id for ordering
				//so why maintain two of them ?
				id  = uniqueId();	

				//build the string attribute
				//begins with 'id'		
#if defined(_WIN32) && defined(_M_AMD64)
				swprintf_s(buf, L"id%Ix", id);
#else
				XMLString::binToText(id, buf + 2, 18, 16);
#endif

				//assign it
				dom_element->setAttribute(gXML__id, buf);
				SET_ID_ATTR(dom_element, gXML__id);

				a = dom_element->getAttribute(gXML__id);
				
				//store in the map, so we can search.
				//the insert is expected to be very fast,
				//since the keys are inserted in the right order
				
			}
			return a;
		};

		


	// --- type

		::Uml::Class m_type;

		//const ::Uml::Class &m_type;

		const ::Uml::Class &type() const { return m_type; }

	// --- order

		

		uniqueId_type uniqueId() const 
		{
			void *p = dom_element->getUserData(gXML__id);
			

			if( p == NULL )
			{
				p = (void*)++static_orderdata;
				dom_element->setUserData(gXML__id, p, NULL);
				((DomDataNetwork*)mydn)->DoMapping(dom_element, reinterpret_cast<uniqueId_type>(p), true);		
			
			
				if (const_cast<DomObject*>(this)->__getdn()->GetProject() != NULL)
				{
					//this is part of the project
					//we will record every id in the backend

					Udm::ObjectImpl::uniqueId_type id;
					const XMLCh *a = dom_element->getAttribute(gXML__id);
					if(EmptyVal(a)) 
					{
						XMLCh buf[25] = { chLatin_i, chLatin_d, chNull };
						id  = reinterpret_cast<uniqueId_type>(p);	
#if defined(_WIN32) && defined(_M_AMD64)
						swprintf_s(buf, L"id%Ix", id);
#else
						XMLString::binToText(id, buf + 2, 18, 16);
#endif
						dom_element->setAttribute(gXML__id, buf);
						SET_ID_ATTR(dom_element, gXML__id);
					}
				};
			};
			return reinterpret_cast<uniqueId_type>(p);
		}

		void desynch_attribute(const string &myname)
		{
			
			//DOMString pa = dom_element.getAttribute(DOMString("desynched_atts"));
			const XMLCh *pa = dom_element->getAttribute(gXML__desynched_atts);
			XMLCh *myname_buf = transcode(myname);
			if(EmptyVal(pa)) 
			{
				//dom_element.setAttribute( DOMString("desynched_atts"), DOMString(myname.c_str()));
				dom_element->setAttribute( gXML__desynched_atts, myname_buf);
			}
			else
			{
				//the attribute desynched_atts exists
				int k = DSFind(pa, myname);
				if(k < 0) 
				{
					//meta is not yet desynched
					XMLCh *pa_new = DSAppend(pa, myname_buf);
					//dom_element.setAttribute(DOMString("desynched_atts"), pa);
					dom_element->setAttribute(gXML__desynched_atts, pa_new);
					XMLString::release(&pa_new);
				}
			}

			XMLString::release(&myname_buf);
		}

		bool is_attribute_desynched(const string &myname) const
		{
			
			//DOMString pa = dom_element.getAttribute(DOMString("desynched_atts"));
			const XMLCh *pa = dom_element->getAttribute(gXML__desynched_atts);
			if(EmptyVal(pa)) return false;
			int k = DSFind(pa, myname);
			return (k >= 0);
			
		}
	/*

		Text attributes are supposed to generate multiple elements containing a single XML text node
	*/

		void setStringAttrArr(const ::Uml::Attribute &meta, const vector<string> &a, const bool direct)
		{
			//for String attibutes we have the same behaviour
			if ((string)meta.type() !=  "Text") return ObjectImpl::setStringAttrArr(meta, a, direct);
			
			string name = meta.name();

			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				if(direct)
				{
					setTextValues(*dom_element, a);
					//desynch the attribute
					desynch_attribute(name);
				}
				else
				{
					//check if desynched
					if (!is_attribute_desynched(name))
					{
						setTextValues(*dom_element, a);		
					}
				}
			}
			else
			{
				setTextValues(*dom_element, a);
				//go through all derived and instances
				vector<ObjectImpl*>::iterator i;
				vector<ObjectImpl*> deriveds = getDerived();
				for (i = deriveds.begin(); i != deriveds.end(); i++)
				{
					ObjectImpl* derived = *i;
					derived->setStringAttrArr(meta, a, false);
					derived->release();
				}

				vector<ObjectImpl*> instances = getInstances();
				for (i = instances.begin(); i != instances.end(); i++)
				{
					ObjectImpl* instance = *i;
					instance->setStringAttrArr(meta, a, false);
					instance->release();
				}
			}
		
		};
		vector<string> getStringAttrArr(const ::Uml::Attribute &meta) const
		{
			//for String attibutes we have the same behaviour
			if ((string)meta.type() !=  "Text") return ObjectImpl::getStringAttrArr(meta);
			vector<string> ret;
			getTextValues(*dom_element, ret);

			return ret;
		};
		
		string getStringAttr(const ::Uml::Attribute &meta) const
		{
			string a;

			if ((string)(meta.type()) == "Text")
			{
				a = getTextValue(*dom_element);
			}	
			else
			{
				XMLCh *name_buf = transcode( ((string) meta.name()) );
				DOMAttr *as = dom_element->getAttributeNode(name_buf);
				if(as != NULL && as->getSpecified()) 
				{
					char *a_buf = transcode(as->getValue());
					a = a_buf;
					XMLString::release(&a_buf);
				}
				XMLString::release(&name_buf);
	//			if(!a.length()) return meta.defvalue();
			}
			return a;
		}


		void setStringAttr(const ::Uml::Attribute &meta, const string &a, const bool direct = true)
		{
			string name = meta.name();

			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				if(direct)
				{
					//set the attribute
					if ((string)(meta.type()) == "Text")
						setTextValue(*dom_element, a);
					else
						setAttrValue(*dom_element, name, a);

					//desynch the attribute
					desynch_attribute(name);
				}
				else
				{
					//check if desynched
					if (!is_attribute_desynched(name))
					{
						//if not, set the attribute
						if ((string)(meta.type()) == "Text")
							setTextValue(*dom_element, a);
						else
							setAttrValue(*dom_element, name, a);
					}
				}
				archetype->release();
			}
			else
			{
				if ((string)(meta.type()) == "Text")
					setTextValue(*dom_element, a);
				else		
					setAttrValue(*dom_element, name, a);
			}
			
			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setStringAttr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setStringAttr(meta, a, false);
				instance->release();
			}

		}

		bool getBooleanAttr(const ::Uml::Attribute &meta) const
		{
			const XMLCh *a = getAttrValue(*dom_element, meta.name());
//			if(!a.length()) {
//				a = string(meta.defvalue()).c_str();
//			}
			if(EmptyVal(a)) return false;
			return XMLString::startsWith(a, gXML_t);	 // true
		}

		void setBooleanAttr(const ::Uml::Attribute &meta, bool a, const bool direct = true)
		{
			string name = meta.name();
			string value = a ? "true" : "false";

			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				if(direct)
				{
					//set the attribute
					setAttrValue(*dom_element, name, value);
					//desynch the attribute
					desynch_attribute(name);
				}
				else
				{
					//check if desynched
					if (!is_attribute_desynched(name))
						//if not, set the attribute
						setAttrValue(*dom_element, name, value);
				}
				archetype->release();
			}
			else
			{
				setAttrValue(*dom_element, name, value);
			}
			
			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setBooleanAttr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setBooleanAttr(meta, a, false);
				instance->release();
			}

		}

		__int64 getIntegerAttr(const ::Uml::Attribute &meta) const
		{
			const XMLCh *a = getAttrValue(*dom_element, meta.name());
//			if(!a.length()) {
//				a = string(meta.defvalue()).c_str();
//			}
			if(EmptyVal(a)) return 0;

			char *a_buf = XMLString::transcode(a);

			char *endptr;
			__int64 retval;
#ifdef _WIN32
			retval = _strtoi64(a_buf, &endptr, 10);
#else
			retval = strtoll(a_buf, &endptr, 10);
#endif

			if (*endptr != '\0')
			{
				XMLString::release(&a_buf);
				throw udm_exception("Attr is of non-integer format");
			}

			XMLString::release(&a_buf);

			return retval;
		}

		void setIntegerAttr(const ::Uml::Attribute &meta, __int64 a, const bool direct = true)
		{
			// TODO: avoid the name conversion
			string name = meta.name();
			char astr[50];
#ifdef _WIN32
			sprintf(astr, "%I64d",a);
#else
			sprintf(astr, "%lld", a);
#endif

			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				if(direct)
				{
					//set the attribute
					setAttrValue(*dom_element, name, astr);
					//desynch the attribute
					desynch_attribute(name);
				}
				else
				{
					//check if desynched
					if (!is_attribute_desynched(name))
						//if not, set the attribute
						setAttrValue(*dom_element, name, astr);
				}
				archetype->release();
			}
			else
			{
				setAttrValue(*dom_element, name, astr);
			}
				
			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setIntegerAttr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setIntegerAttr(meta, a, false);
				instance->release();
			}

/*			
			char astr[50];
			sprintf(astr, "%d",a);
			dom_element.setAttribute(DOMString(name.c_str()), DOMString(astr));
*/

		}

		double getRealAttr(const ::Uml::Attribute &meta) const
		{
			const XMLCh *a = getAttrValue(*dom_element, meta.name());
//			if(!a.length()) {
//				a = string(meta.defvalue()).c_str();
//			}
			if(EmptyVal(a)) return 0.0;

			char *a_buf = XMLString::transcode(a);
			double d;

			if (!UdmUtil::stringToDouble(a_buf, d))
			{
				XMLString::release(&a_buf);
				throw udm_exception("Attr is of non-float format");

			}
			XMLString::release(&a_buf);
			return d;
		}

		void setRealAttr(const ::Uml::Attribute &meta, double a, const bool direct = true)
		{
			string name = meta.name();
			string value = UdmUtil::doubleToString(a);

			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				if(direct)
				{
					//set the attribute
					setAttrValue(*dom_element, name, value);

					//desynch the attribute
					desynch_attribute(name);
				}
				else
				{
					//check if desynched
					if (!is_attribute_desynched(name))
						//if not, set the attribute
						setAttrValue(*dom_element, name, value);

				}
				archetype->release();
			}
			else
			{
				setAttrValue(*dom_element, name, value);
			}
				
			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setRealAttr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setRealAttr(meta, a, false);
				instance->release();
			}

		}

		long getAttrStatus(const ::Uml::Attribute &meta) const
		{
			long status = Udm::ATTSTATUS_HERE;

			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				// for now just set a value telling that the attribute value
				// is not changed here
				if (!is_attribute_desynched(meta.name()))
					status = Udm::ATTSTATUS_IN_ARCHETYPE1;

				archetype->release();
			}

			return status;
		}

	


		string GetANameFor(const ::Uml::Composition &comp) const {

			::Uml::CompositionChildRole chr = comp.childRole();
			string ret = chr.name();
			if(ret.empty())
			{
				if (::Uml::IsCrossNSComposition(comp))
					ret = ((::Uml::Class) chr.target()).getPath2("_", false);
				else
					ret = ((::Uml::Class) chr.target()).name();
			}
			return ret;
		}

	// --- containment
		ObjectImpl *getParent(const ::Uml::CompositionParentRole &role) const 
		{
// TODO: check role if not NULLROLE

			TRY_XML_EXCEPTION
			
			DOMNode *parent = dom_element->getParentNode();

			if ((parent == NULL) || (parent->getNodeType() != DOMNode::ELEMENT_NODE)) return &Udm::_null;
			
			DomObject * do_parent = new DomObject(static_cast<DOMElement*>(parent), mydn);
			
			if(role) {
				::Uml::Composition comp = ::Uml::matchChildToParent(m_type, role.target()); 

				//this will return the null object when there are multiple 
				//possible compositions and the current composition doesn't match
				//the requested compostion
				if(!comp && DSFind(dom_element->getAttribute(gXML___child_as), GetANameFor(role.parent()).c_str()) < 0)
				{
					delete do_parent;
					return &Udm::_null;
				}

				//still need to check the type
				//even if there is only on possible composition between me as child
				//and role.target() as parent,
				//might happen that my parent is not the requested one
				
				set< ::Uml::Class> cl_s = Uml::DescendantClasses(role.target());

				if (cl_s.find(do_parent->m_type)  != cl_s.end())
					return do_parent;
				else
				{
					delete do_parent;
					return &Udm::_null;
				}

			}
			
			return do_parent;

			CATCH_XML_EXCEPTION("DomObject::getParent()");
			
			
		}


		void removeAssociation(const string &tname, const string &oname, 
								const string &myid) 
		{
			XMLCh *tname_buf = transcode(tname);
			XMLCh *origattr = XMLString::replicate( dom_element->getAttribute(tname_buf) );
			dom_element->removeAttribute(tname_buf);
			XMLString::release(&tname_buf);

			XMLCh *oname_buf = transcode(oname);
			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = dom_element->getOwnerDocument();

			const BaseRefVectorOf<XMLCh> *v = XMLString::tokenizeString(origattr);
			for (unsigned int i = 0; i < v->size(); i++)
			{
				const XMLCh *p = v->elementAt(i);

				DOMElement *e = doc->getElementById(p);
				if(e == NULL) e = ((DomDataNetwork*)mydn)->Search(p);

				// the ID must exists
				UDM_ASSERT( e != NULL );

				const XMLCh *cpa = e->getAttribute(oname_buf);
				if (EmptyVal(cpa))
					e->removeAttribute(oname_buf);
				else
				{
					int k = DSFind(cpa, myid);
					if (k >= 0)
					{
						XMLCh *cpa_new = XMLString::replicate(cpa);
						DSRemoveSubstr(cpa_new, k, myid.length() + 1);
						if (XMLString::stringLen(cpa_new))
							e->setAttribute(oname_buf, cpa_new);
						else
							e->removeAttribute(oname_buf);
						XMLString::release(&cpa_new);
					}
					else
						e->removeAttribute(oname_buf);
				}
			}

			delete v;
			XMLString::release(&oname_buf);
			XMLString::release(&origattr);
		}

		void recursiveRemoveAssocs() 
		{

			TRY_XML_EXCEPTION


			//if this datanetwork is part of a project,
			//I might have a placeholder object,
			//which also should be deleted
			Udm::UdmProject * pr = NULL;
			if (__getdn())
				pr = __getdn()->GetProject();
			if (pr)
			{
				Object po = pr->GetPlaceHolder(clone(), false);
				if (po)
					po.DeleteObject();
			}

			// delete associations the element is involved in

			const XMLCh *myid = dom_element->getAttribute(gXML__id);
			
			if(!EmptyVal(myid)) 
			{
				string myid_str = StrX(myid);
				set< ::Uml::AssociationRole> rs = Uml::AncestorAssociationTargetRoles(m_type);
				for(set< ::Uml::AssociationRole>::iterator role = rs.begin(); role != rs.end(); role++) 
				{
					string tname = Uml::MakeRoleName(*role);
					string oname = Uml::MakeRoleName(Uml::theOther(*role));

					::Uml::Association aa = role->parent();
					if(::Uml::Class(aa.assocClass())) 
					{
						oname += "_end_";
					}
					removeAssociation(tname, oname, myid_str);		
				}
				::Uml::Association assoc = m_type.association();
				if(assoc) 
				{
					set< ::Uml::AssociationRole> rs = assoc.roles();
					for(set< ::Uml::AssociationRole>::iterator role = rs.begin(); role != rs.end(); role++) 
					{
						string tname = Uml::MakeRoleName(*role);
						string oname = Uml::MakeRoleName(Uml::theOther(*role));
						removeAssociation(tname + "_end_", oname, myid_str);		

					}
				}
				//if an object with archetype is deleted
				//it should be removed from archetype's list of
				//derived/instances
				//because of the logic of removeAssociation()
				//we cannot simply attempt to return both of them
				//an object is exclusively instance or subtype
				//we remove only the corresponding association
				ObjectImpl * at = getArchetype();
				if ( at && ( at != &Udm::_null) )
				{
					vector<ObjectImpl*> at_deriveds = at->getDerived();
					bool found = false;
					vector<ObjectImpl*>::iterator atd_i = at_deriveds.begin();

					while ((atd_i != at_deriveds.end()))
					{
						ObjectImpl * derived = *atd_i++;
						if (derived->uniqueId() == uniqueId()) found = true;
						derived->release();
					}
					
					if (found)
						//removeAssociation("archetype","derived",myid);
						removeAssociation(_udm_dom_ia_archetype,_udm_dom_ia_derived,myid_str);
					else
						//removeAssociation("archetype","instances",myid);
						removeAssociation(_udm_dom_ia_archetype,_udm_dom_ia_instances,myid_str);
				}
			}

			vector<ObjectImpl*> chds = getChildren(NULL,  NULL);
			for(vector<ObjectImpl*>::iterator i = chds.begin(); i != chds.end(); i++) 
			{
				static_cast<DomObject*>(*i)->recursiveRemoveAssocs();
				(*i)->release();
			}

			CATCH_XML_EXCEPTION("DomObject::recursiveRemoveAssocs()");

		}
// role must be specified unless a is null, or child role in the new parent is unambigious
// if a was already the parent, existing other childroles are not deleted
// XML: if multiple roles are enabled, object will keep its existing role(s) as well if not moved to a new parent
		void setParent(ObjectImpl *a, const ::Uml::CompositionParentRole &role, const bool direct = true) 
		{

			TRY_XML_EXCEPTION 

			DOMNode *currentparent = dom_element->getParentNode();
			Udm::Object this_o = clone();
			
			
			if (direct && ( currentparent != NULL) && 	(currentparent->getNodeType() == DOMNode::ELEMENT_NODE))
			{
				DomObject * cp_do  = new DomObject(static_cast<DOMElement*>(currentparent), mydn);
				if (cp_do->isInstance())
					throw udm_exception("setParent: Instantiated objects can not be modified!");
				delete cp_do;
			}
			if(a != NULL && a != &Udm::_null) 
			{
				DomObject &aa = *static_cast<DomObject *>(a);
				
				XMLCh *chas = NULL;

				/*
				archetype/derived/subtype support
				we need to form three Udm::Objects here: this_o, curr_par_o, new_par_o
				we will use TOMI functions here

				*/

				Udm::Object new_par_o = aa.clone();
				
				if (direct)
				{
					if (aa.isInstance())
						throw udm_exception("setParent: Instantiated objects can not be modified!");

				}


				set<Udm::Object> derived_parent_set; 
				set<Udm::Object> derived = this_o.derived();
				set<Udm::Object>::iterator der_i = derived.begin();
				while (der_i != derived.end())
				{
					Udm::Object der_o = *der_i++;
					if (!der_o.hasRealArchetype())	//only matters if it is not a primarily inherited children
					{
						
						Udm::Object derived_subtree =der_o.FindClosestPrimarilyDerivedParent();
						Udm::Object new_par_coresp_o = new_par_o.FindCorrespondingObjectInSubtypesTree(derived_subtree, false);
						if (new_par_coresp_o)
						{
							der_o.__impl()->setParent(new_par_coresp_o.__impl(), role, false);
							derived_parent_set.insert(new_par_coresp_o);
						}
						else
						{
							//it was moved out from the inherited block
							//so what to do ?
							//simply detach it
							der_o.__impl()->setParent(NULL, NULL, false);
						}
					}
				}
	
				set<Udm::Object> instantiated_parent_set;
				set<Udm::Object> instances = this_o.instances();
				set<Udm::Object>::iterator inst_i = instances.begin();
				while (inst_i != instances.end())
				{
					Udm::Object inst_o = *inst_i++;
					if (!inst_o.hasRealArchetype())	//only matters if it is not a primarily inherited children
					{
						
						Udm::Object instance_subtree =inst_o.FindClosestPrimarilyDerivedParent();
						Udm::Object new_par_coresp_o = new_par_o.FindCorrespondingObjectInInstancesTree(instance_subtree, false);
						if (new_par_coresp_o)
						{
							inst_o.__impl()->setParent(new_par_coresp_o.__impl(), role, false);
							instantiated_parent_set.insert(new_par_coresp_o);
						}
						else
						{
							inst_o.__impl()->setParent(NULL, NULL, false);
						}
					}
				}

				

			   /*
				* order the children nodes in the alphabetical order of their types,
				* and as a 2nd level ordering, keep the order of creation of children,
				* within the same type

				currentparent:  DOMNode,   the current parent of this->dom_element
				aa:				DomObject , the new parent
				m_type:			::Uml::Class, the type of this
			   */
			   
				if(!aa.dom_element->isSameNode(currentparent)) 
				{
					string my_type_name = Uml::GetClassPath(m_type);

					bool inserted = false;
					DOMElement *insert_point = NULL;
					for(DOMNode *n = aa.dom_element->getFirstChild(); n != NULL;n = n->getNextSibling())	
					{
						if( n->getNodeType() == DOMNode::ELEMENT_NODE )
						{
							DOMElement *e = static_cast<DOMElement*>(n);

							// skip library roots
							DomObject *eo = new DomObject(e, mydn);
							bool is_libroot = eo->isLibRoot();
							delete eo;
							if (is_libroot) {
								insert_point = e;
								break;
							}

							string curr_child_name = Uml::GetClassPath(findClass(*e));

							if (my_type_name.compare(curr_child_name) < 0)
							{
								aa.dom_element->insertBefore(dom_element, e);
								inserted = true;
								break;
							}

						}
					}

					if (!inserted)
					{
						if (insert_point != NULL)
							aa.dom_element->insertBefore(dom_element, insert_point);
						else
							aa.dom_element->appendChild(dom_element);
					}


			   }

			   else if(DomDataNetwork::MultiRolesEnabled()) 
			   	chas = (XMLCh*) dom_element->getAttribute(gXML___child_as);
				
			   ::Uml::CompositionChildRole c_role;

			   ::Uml::Composition comp = Uml::matchChildToParent(m_type, aa.m_type);
				if(!role) 
				{
					if(!comp) throw udm_exception("Childrole has to be specified");
					// if role == 0 and comp != 0, no test or __child_as is necessary
					c_role = comp.childRole();
				}
				else if(comp != role.parent()) 
				{  // either 0 or different kinds
					comp = role.parent();
					// if different, or comp is 0 because of 0 roles, one of the followings will catch it:
					if(!Uml::IsDerivedFrom(m_type,::Uml::CompositionChildRole(comp.childRole()).target())) 
					{
						throw udm_exception("Invalid parentrole specified");
					}
					if(!Uml::IsDerivedFrom(aa.m_type, ::Uml::CompositionParentRole(comp.parentRole()).target())) 
					{
						throw udm_exception("Invalid parent specified");
					}

					// we only get here, if multiple roles match, so they have to be recorded
					string ncomp = GetANameFor(comp);
					XMLCh *ncomp_buf = transcode(ncomp);
					if(!EmptyVal(chas)) 
					{
						if(DSFind(chas,ncomp) < 0) 
						{
							XMLCh *chas_new = DSAppend(chas, ncomp_buf);
							dom_element->setAttribute(gXML___child_as, chas_new);
							XMLString::release(&chas_new);
						}
					}
					else dom_element->setAttribute(gXML___child_as, ncomp_buf);
					XMLString::release(&ncomp_buf);

					c_role = Uml::theOther(role);
				}

				//what if new parent has derived/instantiated objects?
				//and the new object just added(this) is coming from outside
				if (direct)
				{
					derived = new_par_o.derived();
					der_i = derived.begin();
					while (der_i != derived.end())
					{
						if (derived_parent_set.find(*der_i) == derived_parent_set.end())
							Object newObj = der_i->__impl()->createChild(c_role, m_type, this, true, false);
						der_i++;
					}

					instances = new_par_o.instances();
					inst_i = instances.begin();
					while (inst_i != instances.end())
					{
						if (instantiated_parent_set.find(*inst_i) == instantiated_parent_set.end())
							Object newObj = inst_i->__impl()->createChild(c_role, m_type, this, false, false);
						inst_i++;
					}
				}

			}
			else 
			{   // new parent is null, unlink from the object tree
				ObjectImpl *parent = getParent(Udm::NULLPARENTROLE);
				if(parent && parent != &Udm::_null) 
				{  //delete parent


					{
						//unlink all derived objects
						//unlink all instances
						vector<ObjectImpl*>deriveds = getDerived();
						vector<ObjectImpl*>instances = getInstances();
						vector<ObjectImpl*>::iterator i;

						//the detach() call will result in setParent(NULL, NULL)
						//and that will result in recursion here
						//and that will take care of removing all the objects
						//along an archetype/derived or archetype/instances 
						//tree
						for(i = deriveds.begin(); i != deriveds.end(); i++)
							(*i)->setParent(NULL, NULL, false);

						for(i = instances.begin(); i != instances.end(); i++)
							(*i)->setParent(NULL, NULL, false);

					}

					recursiveRemoveAssocs();
					DomObject &aa = *static_cast<DomObject *>(parent);
					if(role) 
					{
						if(!Uml::IsDerivedFrom(m_type, Uml::theOther(role).target())) 
						{
							throw udm_exception("Invalid parentrole specified");
						}
						if(!Uml::IsDerivedFrom(aa.m_type, role.target())) 
						{			// irrelevant role, no change
							parent->release();
							return;
						}
//#ifdef RECORD_ROLES
						::Uml::Composition comp = Uml::matchChildToParent(m_type, aa.m_type);
						if(!comp) 
						{
							comp = role.parent();
							string ncomp(GetANameFor(comp));
							const XMLCh *chas = dom_element->getAttribute(gXML___child_as);
							int p = DSFind(chas , ncomp);
							if(p < 0) {
								parent->release();
								return; // not found, no change
							}
							if(p != 0 || XMLString::stringLen(chas) != ncomp.length()) {
								XMLCh *chas_new = XMLString::replicate(chas);
								DSRemoveSubstr(chas_new, (p ? p - 1 : 0), ncomp.length() + 1);  // make sure we delete a separator as well
								dom_element->setAttribute(gXML___child_as, chas_new);
								XMLString::release(&chas_new);
								parent->release();
								return;												// do not delete child if multiroles were in effect
							}

							XMLCh *ncomp_buf = transcode(ncomp);
							UDM_ASSERT( XMLString::compareString(chas, ncomp_buf) == 0);
							XMLString::release(&ncomp_buf);
						}
						else 
						{ 
							// Only a single compositionrole matches these two kinds
							UDM_ASSERT(comp == role.parent());
						}
//#endif
					}
					//we are class DOMObject
					//dom_element is our private var of type DOM_Element
					//in the map there *might* be a mapping from unique id to dom_element
					//we remove it, if there is
					//also note that it's not a must
					uniqueId_type id = uniqueId();
					//Minor bugfix here
					//element should be removed after removing from the mapping.
					//since __getdn() calls need the DOM tree to be consistent
					//endre, 02/22/02
					
					IdToDomElementMap::iterator i = ((DomDataNetwork*)__getdn())->DomElements.find(id);
					//there might be dom elements that unique id is not set!
					if (i != ((DomDataNetwork*)__getdn())->DomElements.end()) ((DomDataNetwork*)__getdn())->DomElements.erase(i);
			
//#ifdef RECORD_ROLES
				
					dom_element->removeAttribute(gXML___child_as);
					aa.dom_element->removeChild(dom_element);
//#endif
					parent->release();
				}
			}

			CATCH_XML_EXCEPTION("DomObject::setParent");
		}

		void detach() 
		{
				setParent(NULL, NULL);
		}

		vector<ObjectImpl*> getChildren(const ::Uml::CompositionChildRole &role, const ::Uml::Class &kind) const {
			vector<ObjectImpl*> ret;
			string compname;
			::Uml::Class target;

			//::Uml::Diagram diagram = m_type.parent();

			//checking if role is valid for this object
			// getting the name for __child_as attribute, when composition is ambigous 
			if(role) 
			{
				target = role.target();

				if(!Uml::IsDerivedFrom(m_type, Uml::theOther(role).target())) throw udm_exception("Invalid role specified");
				compname = GetANameFor(::Uml::Composition(role.parent())).c_str();
			}

			
			for(DOMNode *n = dom_element->getFirstChild(); n != NULL; n = n->getNextSibling())	
			{
				if( n->getNodeType() == DOMNode::ELEMENT_NODE )
				{
					DOMElement *e = static_cast<DOMElement*>(n);
					::Uml::Class m = findClass(*e);

					if(role &&  Uml::IsDerivedFrom(m, target) ) 
					{
						//both , role was provided AND element e of type m is derived from childrole's target
						::Uml::Composition comp = Uml::matchChildToParent(m, m_type);
						
						//this returns NULL if composition was ambigous
						
						if(!comp && DSFind(e->getAttribute(gXML___child_as), compname) < 0) 
							//composition was ambigous and the element e of type m is not via childrole role.(so it won't be in the vector)
							continue;
						
						//composition is not ambigous, it's OK to add to vector
						//if kind was provided, check the type .. 
						if(kind && !Uml::IsDerivedFrom(m, kind)) continue;
						ret.push_back(new DomObject(m, e, mydn));
					}
					else
					{
						//either role was not provided OR type m of element e is unrelated with target's role
						if (!role)
						{
							//role was not provided, but type is ok.
							if(kind && !Uml::IsDerivedFrom(m, kind)) continue;
							ret.push_back(new DomObject(m, e, mydn));
						}
					}
					
				}

			}
			return ret;
		}

		vector<ObjectImpl*> sortchildren(const vector<ObjectImpl*> &a)
		{
			typedef vector<ObjectImpl*> implvec;
			map<const string, implvec> map_seq;
			vector<ObjectImpl*>::const_iterator a_i = a.begin();
			while (a_i != a.end())
			{
				vector<ObjectImpl*> & vec = map_seq[(*a_i)->type().getPath2("::", false)];
				vec.push_back(*a_i++);

			}

			vector<ObjectImpl*> ret;

			map<const string, implvec>::iterator map_seq_i = map_seq.begin();
			while (map_seq_i != map_seq.end())
			{

				vector<ObjectImpl*>  vec = map_seq_i->second;
				
				vector<ObjectImpl*>::iterator vec_i = vec.begin();

				while (vec_i != vec.end())
					ret.push_back(*vec_i++);

				map_seq_i++;
			};
			
			return ret;
		};

		void setChildren(const ::Uml::CompositionChildRole &role, const vector<ObjectImpl*> &a, const bool direct = true) 
		{
			
			Udm::Object this_o = clone();

			Udm::Object archetype = this_o.archetype();

			
			set<Udm::Object> erased;
			
			vector<ObjectImpl*> existing_children = getChildren(role, NULL);
			vector<ObjectImpl*>::iterator ec_i = existing_children.begin();
			while (ec_i != existing_children.end())
				erased.insert(*ec_i++);	//this is safe, getChildren() cloned the ObjectImpls
			

			set<Udm::Object> a_set;
			for(vector<ObjectImpl *>::const_iterator a_set_i = a.begin(); a_set_i != a.end(); a_set_i++) 
			{
				ObjectImpl * a_i = *a_set_i;
				a_set.insert(a_i->clone());		//Object is consturcted, will be released there
			}

			
			if (direct && archetype)
			{
				set<Udm::Object> instances = archetype.instances();

				if (instances.find(clone()) != instances.end()) //an Object is constructed, thus will be released
					throw udm_exception("Instance objects can not be modified!");

				//check whether instance children are going to be deleted
				//if so, that is an error.
				
			

				set<Object>::iterator ec_i = erased.begin();

				while (ec_i != erased.end())
				{
					if (a_set.find(*ec_i++) == a_set.end())
					{
						//an existing object is going to be deleted....
						if (ec_i->archetype() && !ec_i->hasRealArchetype())	//if it's an inherited children
							throw udm_exception("Inherited children objects can not be deleted!");
					}
				}
			}

			//we unlink all objects in erased, they would be deleted anyway.
			//but this way we make sure the links are removed OK.

			for( set<Object>::iterator erased_i = erased.begin();  erased_i != erased.end(); erased_i++)
			{
				erased_i->__impl()->detach();
			};


			DOMNode *n = dom_element->getFirstChild();
			while( n != NULL)	
			{
				if( n->getNodeType() == DOMNode::ELEMENT_NODE )
				{
			
			
					DOMElement *nn = static_cast<DOMElement*>(n);

					//const ::Uml::Diagram & diag = m_type.parent();
					
					DomObject * existing_child = new DomObject(/*diag,*/ nn, mydn);
					if (!(role && !Uml::IsDerivedFrom(existing_child->m_type, role.target())))
					{

						dom_element->removeChild(nn);
//#ifdef RECORD_ROLES
						//this doesn't make sense to me
						//dom_element.removeAttribute(DOMString("__child_as"));
						nn->removeAttribute(gXML___child_as);

						n = dom_element->getFirstChild();

					}
//#endif

					else n = n->getNextSibling();

					delete existing_child;
				}
			}

			//sort the children by their type!
			vector<ObjectImpl*> a_sorted = sortchildren(a);


			for(vector<ObjectImpl *>::const_iterator i = a_sorted.begin(); i != a_sorted.end(); i++) 
			{
				DomObject &de = *static_cast<DomObject *>(*i);
				string child_type_name = (*i)->type().getPath2("::", false);
				
				
//#ifdef RECORD_ROLES
				::Uml::Composition comp = Uml::matchChildToParent(de.m_type, m_type);
				if(!role) 
				{
					if(!comp) throw udm_exception("Childrole has to be specified");
					// if role == 0 and comp != 0, no test or __child_as is necessary
					
				}
				else if(comp != role.parent()) 
				{  // either 0 or different
				
					comp = role.parent();
					// if different, or comp is 0 because of 0 roles, one of the followings will catch it:
					if(!Uml::IsDerivedFrom(m_type, ::Uml::CompositionParentRole(comp.parentRole()).target())) {
						throw udm_exception("Invalid childrole specified");
					}
					if(!Uml::IsDerivedFrom(de.m_type,::Uml::CompositionChildRole(comp.childRole()).target())) {
						throw udm_exception("Invalid child specified");
					}
					// only gets here if comp was 0 beacause of multiple valid roles
					setAttrValue(*dom_element, gXML___child_as, GetANameFor(comp));

				}
//#endif

				//we need to take care about ordering

					bool inserted = false;
					for(DOMNode *n = dom_element->getFirstChild(); n != NULL; n = n->getNextSibling())	
					{
						if( n->getNodeType() == DOMNode::ELEMENT_NODE )
						{
							DOMElement *e = static_cast<DOMElement*>(n);

							string curr_child_name = findClass(*e).getPath2("::", false);

							if (child_type_name.compare(curr_child_name) < 0)
							{
								dom_element->insertBefore(de.dom_element, e);
								inserted = true;
								break;
							}
						}
					}

					if (!inserted)
						dom_element->appendChild(de.dom_element);

			}

			//derived & instantiated objects
			set<Object> derived = this_o.derived();
			set<Object>::iterator d_i = derived.begin();
			while (d_i != derived.end())
			{
				//this is a derived object of this_o
				Object d_o = *d_i++;

				//inside will contain the corresponding(!!) derived children
				//found to be in the same derived tree with this_o. 

				//outside will contain the set of our(!!!) children which are outside
				//the derived tree that contains this_o, and they need to be added
				//as inherited children with archetype to d_o
				vector<ObjectImpl*> inside;

				set<Object>::iterator a_set_i = a_set.begin();
				while (a_set_i != a_set.end())
				{
					Object a_set_o = *a_set_i++;
					Object a_set_o_der = a_set_o.getDerivedObjectInPrimarilyDerivedBlock(d_o);
					if (a_set_o_der) inside.push_back(a_set_o_der.__impl()->clone());
					else 
					{
						//for objects in outside, we create corresponding objects
						inside.push_back(d_o.__impl()->createChild(role, a_set_o.type(), a_set_o.__impl(), true, false));

					}
				}

				vector<ObjectImpl*> existing;	//subtype may have existing extension children
				
				existing = d_o.__impl()->getChildren(role, NULL);
				inside.insert(inside.end(), existing.begin(), existing.end());




				//for objects in inside, we invoke a setChildren on der_o
				d_o.__impl()->setChildren(role, inside, false);

				//we have to release the previously cloned impls
				vector<ObjectImpl*>::iterator inside_i = inside.begin();
				while (inside_i != inside.end())
				{
					ObjectImpl* in_inside = *inside_i++;
					in_inside->release();
				}

				
			}

			set<Object> instances = this_o.instances();
			set<Object>::iterator i_i = instances.begin();
			while (i_i != instances.end())
			{
				//this is a derived object of this_o
				Object i_o = *i_i++;

				//inside will contain the corresponding(!!) derived children
				//found to be in the same derived tree with this_o. 

				//outside will contain the set of our(!!!) children which are outside
				//the derived tree that contains this_o, and they need to be added
				//as inherited children with archetype to d_o
				vector<ObjectImpl*> inside;

				set<Object>::iterator a_set_i = a_set.begin();
				while (a_set_i != a_set.end())
				{
					Object a_set_o = *a_set_i++;
					Object a_set_o_der = a_set_o.getInstantiatedObjectInPrimarilyDerivedBlock(i_o);
					if (a_set_o_der) inside.push_back(a_set_o_der.__impl()->clone());
					else 
					{
						//for objects in outside, we create corresponding objects
						inside.push_back(i_o.__impl()->createChild(role, a_set_o.type(), a_set_o.__impl(), true, false));

					}
				}

				//for objects in inside, we invoke a setChildren on der_o
				i_o.__impl()->setChildren(role, inside, false);

				//we have to release the previously cloned impls
				vector<ObjectImpl*>::iterator inside_i = inside.begin();
				while (inside_i != inside.end())
				{
					ObjectImpl* in_inside = *inside_i++;
					in_inside->release();
				}

				
			}
		}


		ObjectImpl *createChild(
			const ::Uml::CompositionChildRole &childrole,	
			const ::Uml::Class &meta, 
			const Udm::ObjectImpl* archetype = NULL, 
			const bool subtype = false,
			const bool real_archetype = true,
			const bool need_safetype = false) 
		{
				if (!archetype || (archetype && real_archetype))
				{
					//normal child or primarily derived object is going to be created
					//if I'm an instantiated object, this operation should be denied.
					if (isInstance())
						throw udm_exception("createChild: Instantiated objects can not be modified!");
				}

				string casestr = meta.getPath2("::", false) + "-s in " + m_type.getPath2("::", false) + "-s";
				::Uml::CompositionChildRole role = childrole;
			    if(!role) {
					::Uml::Composition comp = Uml::matchChildToParent(meta, m_type);
					if(!comp) throw  udm_exception("Role must be specified for " + casestr); // OR this cannot contain an object of type meta
					role = comp.childRole();
				}
				if(!Uml::IsDerivedFrom(m_type, Uml::theOther(role).target())) {
					throw udm_exception("Invalid childrole specified for creating " + casestr);
				}
				if(!Uml::IsDerivedFrom(meta, role.target())) {
					throw udm_exception("Invalid child specified: " + casestr);
				}

				::Uml::Namespace parent_ns = meta.parent_ns();

				string node_name, node_uri;
				if (parent_ns)
				{
					node_name = parent_ns.getPath2("_", false) + ":";
					node_uri = getNSURI(parent_ns.getPath2("/", false));
				}
				node_name += (string)meta.name();

				//DOMString ns_uri = DOMString((string(UDM_DOM_URI_PREFIX) + '/' +  string(::Uml::Namespace(meta.parent()).name())).c_str());

				DomObject *dep = 
					new DomObject(meta, dom_element->getOwnerDocument()->createElementNS(X(node_uri), X(node_name)), mydn);

				//dep does not have at this moment an archetype!!!
				dep->setParent(this, Uml::theOther(role), false);
				// Set _id attribute. Users expect every element to have an id (for use with other xml tools)
				dep->GetID();

				
				
				if (archetype && archetype != &Udm::_null)
				{

					if (!real_archetype)
					{
						//record that it is not a real archetype
						//dep->dom_element.setAttribute(DOMString("real_archetype"), DOMString("false") );
						dep->dom_element->setAttribute(gXML__real_archetype, gXML_false);
					}

					if (subtype)
					{
						//record that it is a subtype
						//dep->dom_element.setAttribute(DOMString("subtype"), DOMString("true") );
						dep->dom_element->setAttribute(gXML__subtype, gXML_true);
					}

					//set the archetype/derived/instances relationship
					const XMLCh *myid = dep->GetID();
					//DOMString tname = "archetype";
					const XMLCh *tname = gXML__archetype;
					
					//DOMString oname = subtype ? "derived" : "instances";
					const XMLCh *oname = subtype ? gXML__derived : gXML__instances;

					DomObject& peer = *const_cast<DomObject *>((const DomObject*)archetype);
					const XMLCh *peerid = peer.GetID();


					dep->dom_element->setAttribute(tname,peerid);

					const XMLCh *pa = peer.dom_element->getAttribute(oname);
					if(DSFind(pa, myid) >= 0) 
						throw udm_exception("DOM error: Previous archetype/derived/instances relationship not removed completly!");

					if(EmptyVal(pa)) 
						peer.dom_element->setAttribute(oname, myid);
					else 
					{
						XMLCh *pa_new = DSAppend(pa, myid);
						peer.dom_element->setAttribute(oname, pa_new);
						XMLString::release(&pa_new);
					}	
					dep->CopyAttributesFromArchetype();

					//set up archetype relations for children, recursively

					static UdmUtil::copy_assoc_map cam; //this need to be the same object
														//through recursive calls

					//if real_archetype is true, it means that this call is not coming
					//recursively, so we clear this map
					if (real_archetype)
						cam.erase(cam.begin(), cam.end());
						



					set< ::Uml::Class> ancestorClasses=Uml::AncestorClasses(archetype->type());

					for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin(); 
					p_currClass!=ancestorClasses.end(); p_currClass++)
					{
						set< ::Uml::CompositionParentRole> compParentRoles=p_currClass->parentRoles();
						for(set< ::Uml::CompositionParentRole>::iterator p_currRole=compParentRoles.begin();
							p_currRole!=compParentRoles.end(); p_currRole++)
							{
								::Uml::CompositionChildRole oRole = Uml::theOther(*p_currRole);
								::Uml::Class childClass = oRole.target();
								vector<ObjectImpl*>children= archetype->getChildren(oRole, childClass);
								for(vector<ObjectImpl*>::iterator p_currImpl=children.begin();
									p_currImpl!=children.end();p_currImpl++)
									{
										ObjectImpl* p_srcChild=*p_currImpl;
										Udm::Object srcChild = p_srcChild;	//it will be released OK

										Object newchild = dep->createChild(oRole, p_srcChild->type(), p_srcChild, subtype, false);

										UdmUtil::copy_assoc_map::value_type map_item(srcChild, newchild);
										cam.insert(map_item);


										Object srcChildrefersto = srcChild.getReferencedObject();
										if (srcChildrefersto)
										{
											UdmUtil::copy_assoc_map::value_type map_item1(srcChildrefersto, srcChildrefersto);
											cam.insert(map_item1);
										}
									}
							}
					}

					if(real_archetype)
					{
						//copy links from archetype
						vector<ObjectImpl*>children= archetype->getChildren(NULL, NULL);
						for(vector<ObjectImpl*>::iterator p_currImpl=children.begin();p_currImpl!=children.end();p_currImpl++)
						{
								ObjectImpl* p_srcChild=*p_currImpl;
								Udm::Object srcChild = p_srcChild;	//it will be released OK
								
								UdmUtil::copy_assoc_map::iterator cam_i = cam.find(srcChild);
								if (cam_i == cam.end()) 
									throw udm_exception("Internal UdmCopy error");
									
								Udm::Object dstChild = cam_i->second;		//this is a safe Object to Object copy
								
								//copy all links recursively for this child
								UdmUtil::reqCopyLinks(p_srcChild, dstChild.__impl(),cam, false);			//copy simple links first
								UdmUtil::reqCopyLinks(p_srcChild, dstChild.__impl(),cam, false, false);		//then assoc. class based ones

						}
					}
				}
				else
					//set the default attributes
					dep->setDefaultAttributes();

				//if this has subtypes/instances, 
				//a new child has to be added to those objects as well
				vector<ObjectImpl*> deriveds = getDerived();
				vector<ObjectImpl*>::iterator deriveds_i  = deriveds.begin();

				while ( deriveds_i != deriveds.end())
				{
					Object newObject = (*deriveds_i)->createChild(childrole, meta, dep, true, false);
					(*deriveds_i)->release(); //getDerived() clones it.
					deriveds_i ++;
				}

				vector<ObjectImpl*> instances = getInstances();
				vector<ObjectImpl*>::iterator instances_i  = instances.begin();
				while ( instances_i != instances.end())
				{
					Object newObject = (*instances_i)->createChild(childrole, meta, dep, false, false);
					(*instances_i)->release(); //getInstances() clones it.
					instances_i ++;
				}

				return dep;
		}

		void getChildRole(ObjectImpl *c, ::Uml::CompositionChildRole &ret) const
		{
			ret = NULL;
			if (c == NULL || c == &Udm::_null) return;

			DomObject &cc = *static_cast<DomObject *>(c);

			// check that c is our child
			bool is_child = false;
			for (DOMNode *n = dom_element->getFirstChild(); n != NULL; n = n->getNextSibling())
				if (n->getNodeType() == DOMNode::ELEMENT_NODE && n->isEqualNode(cc.dom_element))
				{
					is_child = true;
					break;
				}

			if (!is_child) return;

			::Uml::Composition comp = ::Uml::matchChildToParent(cc.m_type, m_type);
			if (comp)
			{
				ret = comp.childRole();
				return;
			}

			XMLCh *chas = (XMLCh*) cc.dom_element->getAttribute(gXML___child_as);

			set< ::Uml::Class> pancs = ::Uml::AncestorClasses(m_type);
			set< ::Uml::Class> cancs = ::Uml::AncestorClasses(cc.m_type);
			for (set< ::Uml::Class>::const_iterator j = cancs.begin(); j != cancs.end(); j++) {
				set< ::Uml::CompositionChildRole> cr = j->childRoles();
				for (set< ::Uml::CompositionChildRole>::const_iterator i = cr.begin(); i != cr.end(); i++)
					if (pancs.find(::Uml::theOther(*i).target()) != pancs.end())
						if (DSFind(chas, GetANameFor(i->parent())) >= 0) {
							ret = *i;
							return;
						}
			}
		}


	// --- associations

	public:
		vector<ObjectImpl*> getAssociation(const ::Uml::AssociationRole &meta, int mode = Udm::TARGETFROMPEER) const 
		{
			vector<ObjectImpl*> ret;

			string name = Uml::MakeRoleName(meta);
			string followattr;

			if((::Uml::Class)((::Uml::Association)meta.parent()).assocClass()) {
				if(mode == Udm::TARGETFROMPEER) {
					followattr = (name + "_end_").c_str();
				}
				else if(mode == Udm::TARGETFROMCLASS) {
					name = name + "_end_";
				}
			}
			else {
				if(mode != Udm::TARGETFROMPEER) 
						throw udm_exception("Invalid access for non-class association role "+name);
			}

			// this must be an IDREF or IDREFS attribute
			const XMLCh *a = getAttrValue(*dom_element, name);

			const BaseRefVectorOf<XMLCh> *v = XMLString::tokenizeString(a);
			for (unsigned int i = 0; i < v->size(); i++)
			{
				const XMLCh *p = v->elementAt(i);

				//DOM_Element e = doc.getElementById(DOMString(i, p-i));
				//if(e == NULL) 	
				DOMElement *e = ((DomDataNetwork*)mydn)->Search(p);

				// the ID must exists
				UDM_ASSERT( e != NULL );

				if (followattr.length() > 0) {
					const XMLCh *a = getAttrValue(*e, followattr);
					if(EmptyVal(a)) throw udm_exception("Invalid assocClass, (missing _end_ attribute)");
					//DOM_Element ee = doc.getElementById(a);
					//if(ee == NULL) 	
					DOMElement *ee = ((DomDataNetwork*)mydn)->Search(a);
					e = ee;
				}
				// the ID must exists
				UDM_ASSERT( e != NULL );

				// create the object
				ret.push_back( new DomObject(/*m_type.parent(), */ e, mydn) );
			}

			delete v;
			
			return ret;
		}


		
		ObjectImpl * findInstanceAt(const DomObject * where)
		{
			vector<ObjectImpl*> instances = getInstances();
			vector<ObjectImpl*>::iterator i = instances.begin();
			ObjectImpl * found = NULL;

			while (i != instances.end())
			{
				DomObject * instance = (DomObject*)(*i);
				if (!(instance->hasRealArchetype()))
				{
					if (where->dom_element->isSameNode(instance->dom_element->getParentNode()))
						found = instance->clone();
				}
				(*i)->release();
				i++;
			};
			if (!found) throw udm_exception("Could not find the corresponding subtype or instance in one of the parent's subtype/instance object.");
			return found;//it is cloned!
		}


		ObjectImpl * findSubtypeAt(const DomObject * where)
		{
			vector<ObjectImpl*> deriveds = getDerived();
			vector<ObjectImpl*>::iterator i = deriveds.begin();
			ObjectImpl * found = NULL;

			while (i != deriveds.end())
			{
				DomObject * derived = (DomObject*)(*i);
				if (!(derived->hasRealArchetype()))
				{
					if (where->dom_element->isSameNode(derived->dom_element->getParentNode()))
						found = derived->clone();
				}
				(*i)->release();
				i++;
			};
			if (!found) throw udm_exception("Could not find the corresponding subtype or instance in one of the parent's subtype/instance object.");
			return found;//it is cloned!
		}

		ObjectImpl * findInstanceIn(const DomObject * where)
		{
			vector<ObjectImpl*> instances = getInstances();
			vector<ObjectImpl*>::iterator i = instances.begin();
			ObjectImpl * found = NULL;

			while (i != instances.end())
			{
				DomObject * instance = (DomObject*)(*i);
				if (!(instance->hasRealArchetype()))
				{
					DOMNode *parent = instance->dom_element->getParentNode();
			
					while ((!found) && (parent != NULL) && (parent->getNodeType() == DOMNode::ELEMENT_NODE))
					{
			
						if (parent->isSameNode(where->dom_element))
							found = instance->clone();

						parent = parent->getParentNode();
					}

				}
				(*i)->release();
				i++;
			};
			//if (!found) throw udm_exception("Could not find the corresponding subtype or instance in one of the parent's subtype/instance object.");
			return found;//it is cloned or NULL!
		}

		ObjectImpl * findSubtypeIn(const DomObject * where)
		{
			vector<ObjectImpl*> deriveds = getDerived();
			vector<ObjectImpl*>::iterator i = deriveds.begin();
			ObjectImpl * found = NULL;

			while (i != deriveds.end())
			{
				DomObject * derived = (DomObject*)(*i);
				if (!(derived->hasRealArchetype()))
				{
					DOMNode *parent = derived->dom_element->getParentNode();
			
					while ((!found) && (parent != NULL) && (parent->getNodeType() == DOMNode::ELEMENT_NODE))
					{
			
						if (parent->isSameNode(where->dom_element))
							found = derived->clone();

						parent = parent->getParentNode();
					}

				}
				(*i)->release();
				i++;
			};
			//if (!found) throw udm_exception("Could not find the corresponding subtype or instance in one of the parent's subtype/instance object.");
			return found;//it is cloned or NULL!
		}

		



//			A  ( ROLE = 'as' )  C      (ROLE = 'bs')     B
// attrs	bs		    as_end_/ \bs_end_                as

//          C.bs_end_ = bs = 'bs' in A and  'bs_end_' in C
//		    C.as_end_ = as = 'as' in B and  'as_end_' in C

		void setAssociation(const ::Uml::AssociationRole &role, const vector<ObjectImpl*> &nvect, int mode = Udm::TARGETFROMPEER, const bool direct = true)	
		{
			

			Udm::Object this_o = clone();	//most of the archetype/derived/instances related 
											//code will use TOMI Object level functions
											//so we use objects
			
			
			const XMLCh *myid = GetID();
			string tname = Uml::MakeRoleName(role);
			::Uml::AssociationRole orole = Uml::theOther(role);
			string oname = Uml::MakeRoleName(orole);

			

			//get the existing associations
			vector<ObjectImpl*> existing_associations = getAssociation(role, mode);
			set<Udm::Object> existing_ass(existing_associations.begin(), existing_associations.end());


			bool no_change = false;
			if(existing_ass.size() == nvect.size())
			{
				no_change = true;
				
				for(vector<ObjectImpl*>::const_iterator i = nvect.begin(); i != nvect.end(); i++) 
				{
					Object new_ass = (*i)->clone();
					if (existing_ass.find(new_ass) == existing_ass.end())
					{
						no_change = false;
						break;
					}
					
				};

			};

			if (no_change) return;//all OK!, nothing changed.
			
			
			DomObject * do_parent = (DomObject*)getParent(NULL);
			if (do_parent && ((ObjectImpl*)do_parent != &Udm::_null))
			{
				if (do_parent->isInstance() && direct && !isRefLink(role, mode, nvect))
				{
					delete do_parent;
					throw udm_exception("Links can not be added/removed to/from children of instance objects!");				
				}
			}





			if(mode == Udm::CLASSFROMTARGET) 
				oname += "_end_";
			else if(mode == Udm::TARGETFROMCLASS) 
				tname += "_end_";
			
			XMLCh *oname_buf = transcode(oname);
			XMLCh *tname_buf = transcode(tname);
			XMLCh *origattr = XMLString::replicate( dom_element->getAttribute(tname_buf) );

			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = dom_element->getOwnerDocument();

			XMLCh *aa = NULL;
			for(vector<ObjectImpl* >::const_iterator i = nvect.begin(); i != nvect.end(); i++) 
			{
				UDM_ASSERT(dynamic_cast<DomObject*>(*i));
				UDM_ASSERT(static_cast<DomObject *>(*i)->__getdn() == __getdn());
				DomObject& peer = *static_cast<DomObject *>(*i);

				const XMLCh *peerid = peer.GetID();
				if(aa != NULL) 
					DSAppendTo(&aa, gXML_space);
				DSAppendTo(&aa, peerid);

				const XMLCh *pa = peer.dom_element->getAttribute(oname_buf);
				if(DSFind(pa, myid) >= 0) 
				{
					int k = DSFind(origattr, peerid);
					if(k >= 0) DSRemoveSubstr(origattr, k, XMLString::stringLen(peerid) + 1);
					continue;  // no change on peer side
				}

				if(EmptyVal(pa)) peer.dom_element->setAttribute(oname_buf, myid);
				else if(orole.max() == 1) 
				{
					DOMElement *currentpeer = doc->getElementById(pa);
					if(currentpeer != NULL) 
					{
						const XMLCh *cpa = currentpeer->getAttribute(tname_buf);
						if (EmptyVal(cpa))
							currentpeer->removeAttribute(tname_buf);
						else
						{
							int k = DSFind(cpa, peerid);
							if (k >= 0)
							{
								XMLCh *cpa_new = XMLString::replicate(cpa);
								DSRemoveSubstr(cpa_new, k, XMLString::stringLen(peerid) + 1);
								if (XMLString::stringLen(cpa_new))
									currentpeer->setAttribute(tname_buf, cpa_new);
								else
									currentpeer->removeAttribute(tname_buf);
								XMLString::release(&cpa_new);
							}
							else
								currentpeer->removeAttribute(tname_buf);
						}
					}
					peer.dom_element->setAttribute(oname_buf, myid);
				}
				else 
				{
					XMLCh *pa_new = DSAppend(pa, myid);
					peer.dom_element->setAttribute(oname_buf, pa_new);
					XMLString::release(&pa_new);
				}
			}

			if(aa == NULL) dom_element->removeAttribute(tname_buf);
			else {
				dom_element->setAttribute(tname_buf, aa); 
				XMLString::release(&aa);
			}

			{
				const BaseRefVectorOf<XMLCh> *v = XMLString::tokenizeString(origattr);
				for (unsigned int i = 0; i < v->size(); i++)
				{
					const XMLCh *p = v->elementAt(i);

					//DOM_Element e = doc.getElementById(DOMString(i, p-i));
					//if(e == NULL) 	
					DOMElement *e = ((DomDataNetwork*)mydn)->Search(p);

					// the ID must exists
					UDM_ASSERT( e != NULL );

					const XMLCh *cpa = e->getAttribute(oname_buf);
					if (EmptyVal(cpa))
						e->removeAttribute(oname_buf);
					else
					{
						int k = DSFind(cpa, myid);
						if (k >= 0)
						{
							XMLCh *cpa_new = XMLString::replicate(cpa);
							DSRemoveSubstr(cpa_new, k, XMLString::stringLen(myid) + 1);
							if (XMLString::stringLen(cpa_new))
								e->setAttribute(oname_buf, cpa_new);
							else
								e->removeAttribute(oname_buf);
							XMLString::release(&cpa_new);
						}
						else
							e->removeAttribute(oname_buf);
					}
				}

				delete v;
			}

			XMLString::release(&origattr);
			XMLString::release(&tname_buf);
			XMLString::release(&oname_buf);



			//for all instance objects
			//here is the thing:
			// for each instantiated object of my parent(let it be 'a')
			//	1. find the corresponding (not primarily instantiated) object of me in a (as direct 1st level child)
			//  2. find the corresponding (not primarily instantiated) object for each peer (must be conatined somewhere in the archetype)
			//  3. create a new vector
			//  4. invoke recursively setAssociation() for the corresponding objects, direct will be false

			//check if we are setting up references
			if (isRefLink(role, mode, nvect))
			{
				vector<ObjectImpl*> instances = getInstances();
				vector<ObjectImpl*>::iterator instances_i = instances.begin();
				while (instances_i != instances.end())
				{
					(*instances_i)->setAssociation(role, nvect,mode, false);
					delete (DomObject*)*instances_i;
					instances_i++;
				}


				vector<ObjectImpl*> deriveds = getDerived();
				vector<ObjectImpl*>::iterator deriveds_i = deriveds.begin();
				while (deriveds_i  != deriveds.end())
				{
					(*deriveds_i )->setAssociation(role, nvect,mode, false);
					delete (DomObject*)*deriveds_i ;
					deriveds_i ++;
				}

				if (do_parent && ((ObjectImpl*)do_parent != &Udm::_null)) {
					delete do_parent;
				}
				return; // no need for the rest ;)
			}
		
			

			if (do_parent && ((ObjectImpl*)do_parent != &Udm::_null))
			{
				vector<ObjectImpl*>::iterator oset_i;

				vector<ObjectImpl*> instances = do_parent->getInstances();
				oset_i = instances.begin();
				while (oset_i != instances.end())
				{
					
					//Udm::Object this_o_inst = this_o.FindCorrespondingObjectInInstances((*oset_i)->clone(), false);
					Udm::Object this_o_inst = findInstanceAt((DomObject*)(*oset_i));
					
					set<Object> new_vect;		//this will contain the vector of corresponding objects
					for(vector<ObjectImpl* >::const_iterator k = nvect.begin(); k != nvect.end(); k++) 
					{
						DomObject& peer = *static_cast<DomObject *>(*k);
						Udm::Object peer_o_inst = peer.findInstanceIn((DomObject*)(*oset_i));
						if (peer_o_inst) new_vect.insert(peer_o_inst);
					}

					set<Object> existing_corresp;//this really needs to be a set
					for(set<Object>::iterator kk = existing_ass.begin(); kk != existing_ass.end(); kk++)
						existing_corresp.insert(kk->FindCorrespondingObjectInInstancesTree((*oset_i)->clone(), false));
					
					//get the current associations for this_o_inst
					vector<ObjectImpl*> this_o_inst_curr_ass = this_o_inst.__impl()->getAssociation(role, mode);

					//basically all objects corresponding to nvect objects should be set in the derived objects
					// plus, all current associations which are not in the set of corresponding objects of existing peers
					for(vector<ObjectImpl* >::const_iterator kkk = this_o_inst_curr_ass.begin(); kkk != this_o_inst_curr_ass.end(); kkk++) 
					{
						Udm::Object toi_ca_o(*kkk);			//get association clones it
						if (existing_corresp.find(toi_ca_o) == existing_corresp.end())
							new_vect.insert(toi_ca_o);		//no problem if it's already there
					}

					//create the new vector
					vector<ObjectImpl *> new_vector;
					for(set<Object>::iterator kkkk = new_vect.begin(); kkkk != new_vect.end(); kkkk++)
					{
						new_vector.push_back(kkkk->__impl());
					}

					//set the associations (recurisevly)
					//cout << "*";
					this_o_inst.__impl()->setAssociation(role, new_vector, mode, false);
							
					(*oset_i)->release();//getInstances() clones it
					oset_i++;
				
				}
			

				


				//for all derived objects
				//here is the thing:
				// for each derived object of my parent (a)
				//	1. find the corresponding (not primarily instantiated) object of me in a (as direct 1st level child)
				//  2. find the corresponding (not primarily instantiated) object for each peer (must be conatined somewhere in the archetype)
				//  3. create a new vector
				//  4. get the existing peers of the corresponding objects
				//  5. somehow merge the two vectors in a set
				//	6. invoke recursively setAssociation() for the corresponding objects, direct will be false
				
				vector<ObjectImpl*> deriveds = do_parent->getDerived();
				oset_i = deriveds.begin();
				while (oset_i != deriveds.end())
				{
					//Udm::Object this_o_inst = this_o.FindCorrespondingObjectInSubtypes((*oset_i)->clone(), false);
					Udm::Object this_o_inst = findSubtypeAt((DomObject*)(*oset_i));
					
					set<Object> new_vect;		//this will contain the vector of corresponding objects
					for(vector<ObjectImpl* >::const_iterator k = nvect.begin(); k != nvect.end(); k++) 
					{
						DomObject& peer = *static_cast<DomObject *>(*k);
						Udm::Object peer_o_inst = peer.findInstanceIn((DomObject*)(*oset_i));
						if (peer_o_inst)new_vect.insert(peer_o_inst);
					}

					set<Object> existing_corresp;
					for(set<Object>::iterator kk = existing_ass.begin(); kk != existing_ass.end(); kk++)
						existing_corresp.insert(kk->FindCorrespondingObjectInSubtypesTree((*oset_i)->clone(), false));
					
					//get the current associations for this_o_inst
					vector<ObjectImpl*> this_o_inst_curr_ass = this_o_inst.__impl()->getAssociation(role, mode);

					//basically all objects corresponding to nvect objects should be set in the derived objects
					// plus, all current associations which are not in the set of corresponding objects of existing peers
					for(vector<ObjectImpl*>::const_iterator kkk = this_o_inst_curr_ass.begin(); kkk != this_o_inst_curr_ass.end(); kkk++) 
					{
						Udm::Object toi_ca_o(*kkk);			//get association clones it
						if (existing_corresp.find(toi_ca_o) == existing_corresp.end())
							new_vect.insert(toi_ca_o);		//no problem if it's already there
					}

					//create the new vector
					vector<ObjectImpl *> new_vector;
					for(set<Object>::iterator kkkk = new_vect.begin(); kkkk != new_vect.end(); kkkk++)
					{
						new_vector.push_back(kkkk->__impl());
					}

					//set the associations (recurisevly)
					//cout << "=";
					this_o_inst.__impl()->setAssociation(role, new_vector, mode, false);
							
					(*oset_i)->release();//getDerived() clones it.
					oset_i++;
				
				}

				delete do_parent;
			}		
		}
		vector<ObjectImpl*> getDerived() const
		{
			vector<ObjectImpl*> ret;
			
			// this must be an IDREF or IDREFS attribute
			//DOMString a = dom_element.getAttribute("derived");
			const XMLCh *a = dom_element->getAttribute(gXML__derived);
			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = dom_element->getOwnerDocument();


			const BaseRefVectorOf<XMLCh> *v = XMLString::tokenizeString(a);
			for (unsigned int i = 0; i < v->size(); i++)
			{
				const XMLCh *p = v->elementAt(i);

				DOMElement *e = ((DomDataNetwork*)mydn)->Search(p);

				// the ID must exists
				UDM_ASSERT( e != NULL );

				// create the object
				ret.push_back( new DomObject(/*m_type.parent(), */e, mydn) );
			}

			delete v;

			return ret;
		};
		vector<ObjectImpl*> getInstances() const
		{
			vector<ObjectImpl*> ret;
			// this must be an IDREF or IDREFS attribute
			//DOMString a = dom_element.getAttribute("instances");
			const XMLCh *a = dom_element->getAttribute(gXML__instances);
			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = dom_element->getOwnerDocument();


			const BaseRefVectorOf<XMLCh> *v = XMLString::tokenizeString(a);
			for (unsigned int i = 0; i < v->size(); i++)
			{
				const XMLCh *p = v->elementAt(i);

				DOMElement *e = ((DomDataNetwork*)mydn)->Search(p);

				// the ID must exists
				UDM_ASSERT( e != NULL );

				// create the object
				ret.push_back( new DomObject(/*m_type.parent(), */e, mydn) );
			}

			delete v;
			
			return ret;

		};
		ObjectImpl* getArchetype() const
		{
	
			// this must be an IDREF or IDREFS attribute
			//DOMString a = dom_element.getAttribute("archetype");
			const XMLCh *a = dom_element->getAttribute(gXML__archetype);

			if(EmptyVal(a)) return &Udm::_null;

			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = dom_element->getOwnerDocument();

			//e = doc.getElementById(a);
			//if(e == NULL) 	
			DOMElement *e = ((DomDataNetwork*)mydn)->Search(a);

			// the ID must exists
			UDM_ASSERT( e != NULL );

			// create the object
			return new DomObject(/*m_type.parent(), */e, mydn);
	
		};

		bool hasRealArchetype() const
		{
			//DOMString atype = dom_element.getAttribute("archetype");
			const XMLCh *atype = dom_element->getAttribute(gXML__archetype);
			
			if (!EmptyVal(atype))
			{
				//DOMString a = dom_element.getAttribute("real_archetype");
				const XMLCh *a = dom_element->getAttribute(gXML__real_archetype);

				if(EmptyVal(a)) return true;		 // default is true!
				return XMLString::startsWith(a, gXML_t);	 // true
			}
			throw udm_exception("hasRealArchetype() can be evaluated only on instantiated  or derived objects, or (inherited)children of derived objects.");
			return false;							//it has no archetype
		};

		virtual ~DomObject(){};

		bool isLibObject() const
		{
			DOMNode *parent = dom_element->getParentNode();
			while (parent != NULL && parent->getNodeType() == DOMNode::ELEMENT_NODE) {
				DomObject * do_parent = new DomObject(static_cast<DOMElement*>(parent), mydn);
				bool parent_isLibRoot = do_parent->isLibRoot();
				delete do_parent;

				if (parent_isLibRoot)
					return true;

				parent = parent->getParentNode();
			}
			return false;
		}

		bool isLibRoot() const
		{
			const XMLCh *pa = dom_element->getAttribute(gXML__libname);
			if (!EmptyVal(pa))
				return true;

			return false;
		}

		bool getLibraryName(string &name) const
		{
			if (!isLibRoot())
				return false;

			const XMLCh *pa = dom_element->getAttribute(gXML__libname);
			if (EmptyVal(pa))
				name = "";
			else
				name = StrX(pa);

			return true;
		}

		void setLibraryName(const char *name)
		{
			// TODO: allow this only on root folders
			if (name != NULL)
				setAttrValue(*dom_element, gXML__libname, name);
			else {
				dom_element->removeAttribute(gXML__libname);
				vector<ObjectImpl*> chds = getChildren(NULL, type());
				for (vector<ObjectImpl*>::const_iterator i = chds.begin(); i != chds.end(); i++) {
					static_cast<DomObject*>(*i)->setLibraryName(name);
					(*i)->release();
				}
			}
		}

		ObjectImpl *createLibRootChild(const ::Uml::Class &meta, const bool need_safetype = false)
		{
				::Uml::Namespace parent_ns = meta.parent_ns();

				string node_name, node_uri;
				if (parent_ns)
				{
					node_name = parent_ns.getPath2("_", false) + ":";
					node_uri = getNSURI(parent_ns.getPath2("/", false));
				}
				node_name += (string)meta.name();

				DomObject *dep = 
					new DomObject(meta, dom_element->getOwnerDocument()->createElementNS(X(node_uri), X(node_name)), mydn);

				// always attach libraries to the end
				dom_element->appendChild(dep->dom_element);

				//set the default attributes
				dep->setDefaultAttributes();

				return dep;
		}

		vector<ObjectImpl*> getLibRoots() const
		{
			vector<ObjectImpl*> ret;
			
			for(DOMNode *n = dom_element->getFirstChild(); n != NULL; n = n->getNextSibling())	
			{
				if( n->getNodeType() == DOMNode::ELEMENT_NODE )
				{
					DOMElement *e = static_cast<DOMElement*>(n);
					const XMLCh *pa = e->getAttribute(gXML__libname);
					if (!EmptyVal(pa))
						ret.push_back(new DomObject(findClass(*e), e, mydn));
				}

			}
			return ret;
		}

	};

//	unsigned long DomObject::idcount = time(NULL);
	DomObject::uniqueId_type DomObject::static_orderdata = 1;

// --------------------------- parser

	static void setDomParserExternalSchemaLocation(XercesDOMParser &parser)
	{
		string all_ns;
		if (xsd_ns_mapping_storage::uri2xsdname.empty())
			return;
   
		xsd_ns_mapping_storage::str_str_map::iterator it_ns_map = xsd_ns_mapping_storage::uri2xsdname.begin();
		for(;it_ns_map != xsd_ns_mapping_storage::uri2xsdname.end();++it_ns_map)
		{
      
			const string& uri = it_ns_map->first;
			const string& xsdname = it_ns_map->second;
			all_ns += uri + " " + xsdname + " ";
		}
   
		all_ns.resize(all_ns.size()-2);//cut the space

		parser.setExternalSchemaLocation( X(all_ns) );
	}

	class MobiesErrorHandler : public ErrorHandler
	{
	public:
		virtual void  warning (const SAXParseException& exception) { }
		virtual void  error (const SAXParseException& exception) { fatalError(exception); }
 
		virtual void  fatalError (const SAXParseException& exception)
		{
			char buffer[34];

			string description = "Error at file '";
			description += StrX(exception.getSystemId());

			description += "', line ";
			//_itoa(exception.getLineNumber(), buffer, 10);
			sprintf(buffer,"%d", (unsigned int)exception.getLineNumber());
			description += buffer;

			description += ", column ";
			//_itoa(exception.getColumnNumber(), buffer, 10);
			sprintf(buffer,"%d", (unsigned int)exception.getColumnNumber());
			description += buffer;

			description += ". Message: ";
			description += StrX(exception.getMessage());

			throw udm_exception(description);
		}

		virtual void  resetErrors () { }
	};

	class MobiesDOMErrorHandler : public DOMErrorHandler
	{
	public:
		bool handleError(const DOMError& error)
		{
			char buffer[34];

			DOMLocator *location = error.getLocation();

			string description = "at '";
			description += StrX(location->getURI());
			description += "'";

			description += ", line ";
			sprintf(buffer, "%d", (unsigned int)location->getLineNumber());
			description += buffer;

			description += ", column ";
			sprintf(buffer, "%d", (unsigned int)location->getColumnNumber());
			description += buffer;

			description += ". Message: ";
			description += StrX(error.getMessage());


			if (error.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
				cerr << "Warning " << description << endl;
			else
				throw udm_exception("Error " + description);
			return true;
		}
	};

//========================================================

	class MyEntityResolver : public EntityResolver 
	{
		bool _resolved;
		bool vinfo;
		string name;
		string version;
		
		
		bool _use_str_xsd;

	

		bool CheckForVersionInfo(const InputSource* is, const string &systemid)
		{
			if (!is) return false;
			/*
				<?udm interface="UdmProject" version="1.00"?>
			*/

			XercesDOMParser parser;

			parser.setValidationScheme(XercesDOMParser::Val_Never);
			setDomParserExternalSchemaLocation(parser);

			MobiesErrorHandler errorHandler;
			parser.setErrorHandler(&errorHandler);

			try 
			{
				parser.parse(*is);
			}

			catch(const udm_exception& toCatch)
			{
				string description = "Error during parsing DTD/XSD: '";
				description += systemid;
				description += "'. Exception message is: ";
				description += toCatch.what();

				throw udm_exception(description);
			}

			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *pDoc = parser.getDocument();
			DOMNode *pNode = pDoc->getFirstChild();
			while(pNode != 0)
			{
				if(pNode->getNodeType()==DOMNode::PROCESSING_INSTRUCTION_NODE)
				{

					// do what you want
					DOMProcessingInstruction * DPI = static_cast<DOMProcessingInstruction*>(pNode);
					if (XMLString::equals(DPI->getTarget(), gXML_udm))
					{
						const char * interface_att_name="interface=\"";
						const char * version_att_name="version=\"";

						string version_string(StrX(DPI->getData()));

						string vs_1 = version_string.substr(version_string.find(interface_att_name) + strlen(interface_att_name), string::npos);
						name = vs_1.substr(0, vs_1.find('"'));
						string vs_2 = version_string.substr(version_string.find(version_att_name) + strlen(version_att_name), string::npos);
						version = vs_2.substr(0, vs_2.find('"'));
						
						if (name.size() && version.size()) vinfo = true;

						return true;

					}
				}
				pNode=pNode->getNextSibling();
			}

			return false;
		};
	public:
		void clear()
		{
			_resolved = false;
			vinfo = false;
		};
		
		bool resolved() const
		{
			return _resolved;
		};
		bool hasVersionInfo() const
		{
			return vinfo;
		};

		string getName() const
		{
			return name;
		};

		string getVersion() const
		{
			return version;
		};

		void UseStrXsd()
		{
			_use_str_xsd = true;
		};

		void DontUseStrXsd()
		{
			_use_str_xsd = false;

		};

	  virtual InputSource* resolveEntity(const   XMLCh* const    publicId, const XMLCh* const systemId) 
	  {
		  string sysid = StrX(systemId);

		  InputSource* is = NULL;

		  if (_use_str_xsd)
		  {
			  //look up in the string map.
			  str_xsd_storage::str_str_map::iterator it_sxc = str_xsd_storage::static_xsd_container.find(sysid);
			  if (it_sxc != str_xsd_storage::static_xsd_container.end())
			  {
				  const string &xsd_str = it_sxc->second;
				  is = new MemBufInputSource((const unsigned char * )xsd_str.c_str(), xsd_str.size(), X("MBIS_XSD_" + sysid));
			  }

			  else return HandlerBase().resolveEntity(publicId, systemId); 

		
		  }
		  else
		  {
			//removed the hacks from  here....
			//no automagic things: just current directory, PATH, and URI 

		
		
#ifdef _WIN32
			
			//search in resources
			//resource checking is done only when  DTDPath is not set
			//(not to confuse Udm.exe when the -d is provided
			//and probably XMI input is supplied)
			if (DomDataNetwork::DTDPath.empty())
			{
				HANDLE myProcId = GetCurrentProcess();
				HMODULE lpModules[200];
				HMODULE resFound = NULL;
				DWORD needed;

				//try to bring in psapi.dll
				HMODULE psmod = LoadLibrary("psapi.dll");

				HRSRC resource = NULL;

				if (psmod)
				{
					typedef BOOL (__stdcall *fp )(HANDLE, HMODULE*, DWORD, LPDWORD);

					fp fnctptr = (fp)GetProcAddress(psmod, "EnumProcessModules");
					if (fnctptr)
					{
						if ((*fnctptr)(myProcId, lpModules, sizeof(lpModules), &needed))
						{
							DWORD module_index = 0;
							while (module_index < std::min((size_t)needed/sizeof(HMODULE), sizeof(lpModules)/sizeof(lpModules[0])))
							{
								resource = FindResource(lpModules[module_index], sysid.c_str(), "XSD");
								if (resource) {
									resFound = lpModules[module_index];
									break;
								}
								module_index++;
							}
						}
					}
					FreeLibrary(psmod);
				}
				else
					resource = FindResource(NULL, sysid.c_str(), "XSD");
				if (resource)
				{

					DWORD size = SizeofResource(resFound, resource);
					HGLOBAL hResData = LoadResource(resFound, resource);
					if (hResData)
					{
						const unsigned char * p = (const unsigned char *)LockResource(hResData);
						// KMS n.b. if the dll this resource is in is unloaded, this memory becomes invalid
						is =  new MemBufInputSource(p, size, X("MBIS_RESOURCE_XSD_" + sysid));
					}
				}
			}
#endif

      //search order: string resource, file, windows resource
			string filename;

			if (!is)
			{
				//also look up in the string map.
				str_xsd_storage::str_str_map::iterator it_sxc = str_xsd_storage::static_xsd_container.find(sysid);
				if (it_sxc != str_xsd_storage::static_xsd_container.end())
				{
				  const string &xsd_str = it_sxc->second;
				  is = new MemBufInputSource((const unsigned char * )xsd_str.c_str(), xsd_str.size(), X("MBIS_XSD_" + sysid));
				}
			}
			if (!is)
			{
				filename = DomDataNetwork::FindFile(sysid);
				if(!filename.empty()) 
				{
					is = new LocalFileInputSource( X(filename) );
				}
			}

			if (!is) 
				return HandlerBase().resolveEntity(publicId, systemId);
		  }

		//check vor version Info
		vinfo = CheckForVersionInfo(is,sysid);
		return is;
	  }
	};

	static class strstorage {
		char *p;
	public:
		strstorage() { p = NULL; };
		~strstorage() { if(p) free(p); }
		typedef const char *charp;
		charp load(const char *str) {
			char *pt = str ? strdup(str) : NULL;
			if(p) free(p);
			return p = pt;
		}
		operator charp() { return p; }
	} pp;

//========================================================

	static XercesDOMParser* createDOMParser()
	{
		XercesDOMParser *parser = new XercesDOMParser();

		parser->setValidationScheme(XercesDOMParser::Val_Always);

		parser->setDoNamespaces(true);
		parser->setDoSchema(true);

		parser->setCreateEntityReferenceNodes(false);
		parser->setIncludeIgnorableWhitespace(false);

		MyEntityResolver *resolver = new MyEntityResolver();
		resolver->clear();
		parser->setEntityResolver(resolver);

		parser->setErrorHandler(new MobiesErrorHandler());

		setDomParserExternalSchemaLocation(*parser);

		return parser;
	}

	static void releaseDOMParser(XercesDOMParser **parser)
	{
		const EntityResolver *resolver = (*parser)->getEntityResolver();
		if (resolver != NULL)
			delete resolver;

		const ErrorHandler *errHandler = (*parser)->getErrorHandler();
		if (errHandler != NULL)
			delete errHandler;

		delete *parser;
	}

	class MySerializer {
#if _XERCES_VERSION < 30000
		DOMWriter *serializer;
#else
		DOMLSSerializer *serializer;
#endif
	public:
		MySerializer(const XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc);
		~MySerializer();

		void serializeTo(const XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc, XMLFormatTarget *target) const;
	};

#if _XERCES_VERSION < 30000

	MySerializer::MySerializer(const XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc)
	{
		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(X("LS"));
		serializer = ((DOMImplementationLS*)impl)->createDOMWriter();

		const XMLCh *doc_encoding = doc->getEncoding();
		if (doc_encoding)
			serializer->setEncoding(doc_encoding);
		else
			serializer->setEncoding(gXML_UTF_8);

		if (serializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
			serializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);

		serializer->setErrorHandler(new MobiesDOMErrorHandler());
	}

	MySerializer::~MySerializer()
	{
		DOMErrorHandler *err_handler = serializer->getErrorHandler();
		delete err_handler;

		delete serializer;
	}

	void MySerializer::serializeTo(const XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc, XMLFormatTarget *target) const
	{
		serializer->writeNode(target, *doc);
	}

#else //_XERCES_VERSION < 30000

	MySerializer::MySerializer(const XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc)
	{
		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(X("LS"));
		serializer = ((DOMImplementationLS*)impl)->createLSSerializer();

		DOMConfiguration *conf = serializer->getDomConfig();
		if (conf->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
			conf->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

		conf->setParameter(XMLUni::fgDOMErrorHandler, new MobiesDOMErrorHandler());
	}

	MySerializer::~MySerializer()
	{
		DOMErrorHandler *err_handler = (DOMErrorHandler*) serializer->getDomConfig()->getParameter(XMLUni::fgDOMErrorHandler);
		delete err_handler;

		serializer->release();
	}

	void MySerializer::serializeTo(const XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc, XMLFormatTarget *target) const
	{
		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(X("LS"));
		DOMLSOutput *output = ((DOMImplementationLS*)impl)->createLSOutput();
		output->setByteStream(target);
		output->setEncoding(gXML_UTF_8);

		serializer->write(doc, output);
		output->release();
	}

#endif //_XERCES_VERSION < 30000


	void DomDataNetwork::AddToMetaClassesCache(const set< ::Uml::Class> &meta_classes)
	{
		for (set< ::Uml::Class>::const_iterator mci = meta_classes.begin(); mci != meta_classes.end(); mci++)
		{
			pair<string, ::Uml::Class> mcc_item(mci->getPath2(":", false), *mci);
			pair<map<string,  ::Uml::Class>::iterator, bool> ins_res = meta_class_cache.insert(mcc_item);
			if (!ins_res.second)
				throw udm_exception("Insert failed when creating meta classes by name map!");

		}
	}

	UDM_DLL DomDataNetwork::DomDataNetwork(const Udm::UdmDiagram &metainfo, Udm::UdmProject* project) :
	Udm::DataNetwork(metainfo, project)  
	{

		str_based = false;
		str = "";
		
		saveondestroy = false;
		rootobject = NULL;
		/*if(!LoadLibrary("..\\lib\\xerces-c_1_2")) MessageBox(NULL, "Baj van", "Load lib", 0); */

		parser = createDOMParser();

		//create a cache for meta-classes 
		AddToMetaClassesCache(metaroot.dgr->classes());

		::Uml::DiagramNamespaces meta_namespaces(*metaroot.dgr);
		for (::Uml::DiagramNamespaces::iterator mni = meta_namespaces.begin(); mni != meta_namespaces.end(); mni++)
		{
			AddToMetaClassesCache(mni->classes());
		}
	}


	void DomDataNetwork::DoMapping(DOMElement *const e, DomObject::uniqueId_type id,  bool force)
	{
			IdToDomElementMapItem item_to_store(id, e);
			pair<IdToDomElementMap::iterator, bool> ins_res;
			ins_res = DomElements.insert(item_to_store);
			UDM_ASSERT( (!force) || (force && ins_res.second) );

	};


	DOMElement* DomDataNetwork::Search(const XMLCh *str) 
	{
		IdToDomElementMap::iterator i = DomElements.find(GetLongID(str));
		if (i != DomElements.end() ) return (*i).second;

		return NULL;
		
	};//eo Search()



	UDM_DLL DomDataNetwork::~DomDataNetwork() { 

		TRY_XML_EXCEPTION

		if(!rootobject)
		{
			//somebody might have already close it with either CloseUpdate or CloseNoUpdate ! 
			releaseDOMParser(&parser);
			return;
		}

		if(saveondestroy) CloseWithUpdate();
		else CloseNoUpdate();

		releaseDOMParser(&parser);

		CATCH_XML_EXCEPTION("DomDataNetwork::~DataNetwork()")
	}

	//ugly but necesarry
	UDM_DLL str_xsd_storage::str_str_map str_xsd_storage::static_xsd_container;
	UDM_DLL xsd_ns_mapping_storage::str_str_map xsd_ns_mapping_storage::uri2xsdname;
	UDM_DLL xsd_ns_mapping_storage::str_str_map xsd_ns_mapping_storage::static_xsd_ns_mapping_container;
	UDM_DLL xsd_ns_mapping_storage::str_str_map xsd_ns_mapping_storage::static_xsd_ns_back_mapping_container;

	

	UDM_DLL void DomDataNetwork::MapExistingIDs(const DOMNode &d) 
	{
		TRY_XML_EXCEPTION

		DOMNode *child = d.getFirstChild();
		while (child != 0) 
		{
			if(child->getNodeType() == DOMNode::ELEMENT_NODE) 
			{
				
				DOMElement *dd = static_cast<DOMElement *>(child);	
				const XMLCh *a = dd->getAttribute(gXML__id);
				if (!EmptyVal(a))
				{
					// attribute "id" is of type ID
					SET_ID_ATTR(dd, gXML__id);

					//convert to long
					long id = GetLongID(a);
					//check if a found id is higher then our id
					//so that newly created elements in this 
					//DOM would get a proper ID.
					if (id)
					{
						//id could be zero
						if (id > DomObject::static_orderdata)
							DomObject::static_orderdata = id;

						//insert in the map so we could find it
						//later on
						IdToDomElementMapItem item_to_store(id, dd);
						pair<IdToDomElementMap::iterator, bool> ins_res;
						ins_res = DomElements.insert(item_to_store);
						//check for uniqueness
						UDM_ASSERT(ins_res.second);

						//set uniqueId to id
						dd->setUserData(gXML__id, (void*) id, NULL);

					}//eo if (id)
				}//eo if (a != 0)
				MapExistingIDs(*child);
			 }//eo if(child->getNodeType() == DOMNode::ELEMENT_NODE) 
			 child = child->getNextSibling();
		}//eo while (child != 0)
			
		CATCH_XML_EXCEPTION("DomDataNetwork::MapExistingIDs()");

	}//eo void DomDataNetwork::MapExistingIDs(const DOMNode &d) 


	void DomDataNetwork::MapNamespaces(const DOMNode *d) 
	{
		TRY_XML_EXCEPTION

		while (d != 0) 
		{
			if(d->getNodeType() == DOMNode::PROCESSING_INSTRUCTION_NODE) 
			{
				const DOMProcessingInstruction *DPI = static_cast<const DOMProcessingInstruction *>(d);
				if (XMLString::equals(DPI->getTarget(), gXML_udm_udmdom_nsmap))
				{
					char *data = XMLString::transcode(DPI->getData());
					vector<string> custom_ns_map = UdmUtil::string_to_vector(data, ' ');
					XMLString::release(&data);

					for (vector<string>::const_iterator i = custom_ns_map.begin(); i != custom_ns_map.end(); i++)
					{
						string::size_type loc = i->find('=');
						if (loc == string::npos)
							throw udm_exception("Corrupt custom NS map element: " + *i);

						string ns_path = i->substr(0, loc);
						string ns_uri = i->substr(loc + 1);
						if (ns_uri.length() < 2)
							throw udm_exception("Corrupt custom NS map element: " + *i);

						ns_uri = ns_uri.substr(1, ns_uri.size() - 2);

						string xsd_file = UdmUtil::replace_delimiter(ns_path, "::", "_") + ".xsd";
						RemoveURIToUMLNamespaceMapping(ns_uri);
						AddURIToUMLNamespaceMapping(ns_uri, ns_path, xsd_file);
					}
				}
			 }//eo if(d->getNodeType() == DOMNode::PROCESSING_INSTRUCTION_NODE) 
			 d = d->getNextSibling();
		}//eo while (d != 0)
			
		CATCH_XML_EXCEPTION("DomDataNetwork::MapNamespaces()");

	}//eo void DomDataNetwork::MapNamespaces(const DOMNode *d) 


	UDM_DLL void DomDataNetwork::OpenExistingFromString(string &str, 
									const string &metalocator, 
									enum Udm::BackendSemantics sem)
	{
		TRY_XML_EXCEPTION

		str_based = true;

		UDM_ASSERT( str.length());

		parser->reset();

		MyEntityResolver *resolver = (MyEntityResolver *) parser->getEntityResolver();
		resolver->clear();
		resolver->UseStrXsd();

		::Uml::Diagram dgr = GetRootMeta();
		try
		{
			const InputSource * is = new MemBufInputSource( (const unsigned char *)str.c_str(), str.size(), gXML_MBIS);
			parser->parse(*is);
			delete is;
		}
		/*
			Our MobiesErrorHandler throws udm_exceptions...
		*/
		catch(const udm_exception& toCatch)
		{
			//parsing failed
			//maybe it's the wrong version ?
			//we check this first, we report version mismatch error, if there is
			//version checking
			if (resolver->hasVersionInfo())
			{
				if (resolver->getName() != (string)dgr.name()) 
				{
					string err = string("Parsing failed, meta name information mismatch! DTD/XSD: ") + resolver->getName() + ", meta: " + (string)dgr.name();
					throw udm_exception(err);
				}
				if (resolver->getVersion() != (string)dgr.version()) 
				{
					string err = string("Parsing failed, meta version information mismatch! DTD/XSD: ") + resolver->getVersion() + ", meta: " + (string)dgr.version();
					throw udm_exception(err);
				}
			}
			//end of version checking

			//if we got here, the version info seems to be OK, but however the document still can not be parsed
			
			string description = "Error during parsing input string. ";
			description += "'. Exception message is: ";
			description += toCatch.what();

			throw udm_exception(description);
		}

		XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = parser->getDocument();
		DOMElement *root = doc->getDocumentElement();
		UDM_ASSERT( root != NULL );

		//parser.getDocument().setUserData((void *)pp.load(systemname.c_str()));

		//map UML namespaces to URI
		DOMNode *first_child = doc->getFirstChild();
		MapNamespaces(first_child);

	
		rootobject = new DomObject(/*dgr, */root, this);
		//version checking
		if (resolver->hasVersionInfo())
		{
			if (resolver->getName() != (string)dgr.name()) 
			{
				string err = string("Meta name information mismatch! DTD/XSD: ") + resolver->getName() + ", meta: " + (string)dgr.name();
				throw udm_exception(err);
			}
			if (resolver->getVersion() != (string)dgr.version()) 
			{
				string err = string("Meta version information mismatch! DTD/XSD: ") + resolver->getVersion() + ", meta: " + (string)dgr.version();
				throw udm_exception(err);
			}
		
		
		}
		//end of version checking

		saveondestroy = (sem != Udm::CHANGES_LOST_DEFAULT);
		
		//empty the map of elements, one might open an existing
		//diagram on an already used diagram
		if (!DomElements.empty())
			DomElements.erase(DomElements.begin(), DomElements.end());
		//get all the id's
		//this must be recursive, but, at least, it's run only once/UDM session
		MapExistingIDs(*root);

		
		this->str.erase();
		this->str = str;

		CATCH_XML_EXCEPTION("DomDataNetwork::OpenExistingFromString()");
	};

	UDM_DLL void DomDataNetwork::OpenExisting(const string &systemname, 
									const string &metalocator, 
									enum Udm::BackendSemantics sem) 
	{


		TRY_XML_EXCEPTION

		UDM_ASSERT( systemname.length());

		parser->reset();

		MyEntityResolver *resolver = (MyEntityResolver *) parser->getEntityResolver();
		resolver->clear();
		resolver->DontUseStrXsd();

		savesystemname = systemname;
		if(!strnicmp(savesystemname.c_str(),"DOM:", static_cast<size_t>(4))) savesystemname.erase(0,4);

		::Uml::Diagram dgr = GetRootMeta();
		try
		{
			parser->parse(X(savesystemname));
		}
		/*
			Our MobiesErrorHandler throws udm_exceptions...
		*/
		catch(const udm_exception& toCatch)
		{
			//parsing failed
			//maybe it's the wrong version ?
			//we check this first, we report version mismatch error, if there is
			//version checking
			if (resolver->hasVersionInfo())
			{
				if (resolver->getName() != (string)dgr.name()) 
				{
					string err = string("Parsing failed, meta name information mismatch! DTD/XSD: ") + resolver->getName() + ", meta: " + (string)dgr.name();
					throw udm_exception(err);
				}
				if (resolver->getVersion() != (string)dgr.version()) 
				{
					string err = string("Parsing failed, meta version information mismatch! DTD/XSD: ") + resolver->getVersion() + ", meta: " + (string)dgr.version();
					throw udm_exception(err);
				}
			}
			//end of version checking

			//if we got here, the version info seems to be OK, but however the document still can not be parsed
			
			string description = "Error during parsing: '";
			description += systemname;
			description += "'. Exception message is: ";
			description += toCatch.what();

			throw udm_exception(description);
		}

		XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = parser->getDocument();
		DOMElement *root = doc->getDocumentElement();
		UDM_ASSERT( root != NULL );

		//doc->setUserData((void *)pp.load(systemname.c_str()));

		//map UML namespaces to URI
		DOMNode *first_child = doc->getFirstChild();
		MapNamespaces(first_child);

	
		rootobject = new DomObject(/*dgr,*/ root, this);
		//version checking
		if (resolver->hasVersionInfo())
		{
			if (resolver->getName() != (string)dgr.name()) 
			{
				string err = string("Meta name information mismatch! DTD/XSD: ") + resolver->getName() + ", meta: " + (string)dgr.name();
				throw udm_exception(err);
			}
			if (resolver->getVersion() != (string)dgr.version()) 
			{
				string err = string("Meta version information mismatch! DTD/XSD: ") + resolver->getVersion() + ", meta: " + (string)dgr.version();
				throw udm_exception(err);
			}
		
		
		}
		//end of version checking

		saveondestroy = (sem != Udm::CHANGES_LOST_DEFAULT);
		
		//empty the map of elements, one might open an existing
		//diagram on an already used diagram
		if (!DomElements.empty())
			DomElements.erase(DomElements.begin(), DomElements.end());
		//get all the id's
		//this must be recursive, but, at least, it's run only once/UDM session
		MapExistingIDs(*root);

		CATCH_XML_EXCEPTION("DomDataNetwork::OpenExisting()");

	}

	UDM_DLL void DomDataNetwork::CreateNewToString(const string &metalocator, const ::Uml::Class &rootclass, 
									enum Udm::BackendSemantics sem )
	{
		CreateNew("",metalocator, rootclass,sem);
		str_based = true;
		str.erase();
	};

	UDM_DLL const string& DomDataNetwork::Str()
	{
		return str;
	};


//================================================
	UDM_DLL void DomDataNetwork::CreateNew(const string &systemname, 
									const string &metalocator, const ::Uml::Class &rootclass, 
									enum Udm::BackendSemantics sem) 
	{

#ifdef _WIN32
		static const char path_dlim =';';
		static const char path_hier ='\\';
#else
		static const char path_dlim =':';
		static const char path_hier ='/';
#endif


		TRY_XML_EXCEPTION


			parser->reset();

			bool force_dtd = false; //default is xsd
			if (metalocator.size() > 4)
			{
				if (metalocator.find('.') != metalocator.npos)
				{
					string extension = metalocator.substr(metalocator.find('.')+1, metalocator.npos);
					force_dtd = (strncmp(extension.c_str(), "dtd", 3) == 0);
				}
			}
			

			UDM_ASSERT( metalocator.length());
			savesystemname = systemname;
			if(!strnicmp(savesystemname.c_str(),"DOM:", static_cast<size_t>(4))) savesystemname.erase(0,4);

			XStr ml(metalocator);
			MyEntityResolver mr;
			
			if (str_based) mr.UseStrXsd();
			else mr.DontUseStrXsd();

		

			/*
				This hack is unfortunatly needed because of GME & SmartDataNetwork.
				For ex., LampDiagram can be used as metalocator in SmartDataNetworks. 
				
				//XSD
				That *must* identify both the MGA paradigm and the XML dtd. So we look it up first
				and if it's not succesfull we  try again with .dtd appended. 

				That *must* identify both the MGA paradigm and the XML Schema or DTD.
				So we look it up first and if it's not succesfull we try again with
				.xsd or .dtd appended.


			*/
				
			InputSource *pp = mr.resolveEntity(ml.unicodeForm(),ml.unicodeForm());


			
	
			//XSD
			string xsd_file = metalocator;
			if(!pp)
			{
				string mlExtension = xsd_file.size() >= 4 ?	xsd_file.substr(xsd_file.size() - 4, 4) : "";
				if(strnicmp(mlExtension.c_str(), ".xsd", static_cast<size_t>(4)) != 0 )
				{
					xsd_file += ".xsd";
					XStr ml2(xsd_file);
					pp = mr.resolveEntity(ml2.unicodeForm(), ml2.unicodeForm());
		
				}
			}


			if(!pp) 
			{
				string err;
				err = "Metalocator could not be resolved: ";
				err += metalocator;
				err += ". Make sure that it's a valid FILE either in the current directory, or in PATH, or it's a valid URI, and it identifies a valid DTD.";

				throw udm_exception(err);
			}	
			
			delete pp;			//it surely does exist ;-)
			
			DOMImplementation *impl = DOMImplementation::getImplementation();

			string rootname = rootclass.name();

			
			DOMDocumentType *dtd = NULL;

			//XSD
			//dtd= impl.createDocumentType(DOMString(rootname.c_str()),DOMString(), ml);

			//XSD
			if(force_dtd)  // XXX: should be: if use DTD
			{
				dtd = impl->createDocumentType(X(rootname), gXML_null, ml.unicodeForm());
			}


			::Uml::Namespace rootclass_ns = rootclass.parent_ns();

			string root_qualified_name, root_uri;

			if (rootclass_ns != ::Uml::Namespace(NULL))
			{
				root_qualified_name = rootclass_ns.getPath2("_", false) + ":" + rootname;
				root_uri = getNSURI(rootclass_ns.getPath2("/", false));
			}
			else
			{
				root_qualified_name = rootname;
			}


			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = impl->createDocument(
					X(root_uri),                    // root element namespace URI.
					X(root_qualified_name),            // root element name
					dtd);  // document type object (DTD).
		
			

			DOMElement *rootElem = doc->getDocumentElement();
			UDM_ASSERT( rootElem != NULL );

			rootobject = new DomObject(/*GetRootMeta(),*/ rootElem, this);
			
			//set the default attributes
			rootobject.__impl()->setDefaultAttributes();

			//XSD
			if(!force_dtd)  // XXX: should be: if use XML Schema
			{
				std::string fname_pathless;
				if (xsd_file.rfind(path_hier) != std::string::npos)
					fname_pathless = xsd_file.substr(xsd_file.rfind(path_hier) + 1, std::string::npos);
				else 
					fname_pathless = xsd_file;

				// set XML Schema
				DOMElement *de = static_cast<DomObject *>(GetRootObject().__impl())->dom_element;
				
				de->setAttributeNS(X("http://www.w3.org/2000/xmlns/"),
						   X("xmlns:xsi"), X("http://www.w3.org/2001/XMLSchema-instance"));

				de->setAttributeNS(X("http://www.w3.org/2001/XMLSchema-instance"),
						   X("xsi:noNamespaceSchemaLocation"), X((string)metaroot.dgr->name() + ".xsd"));

				::Uml::DiagramNamespaces nses(*metaroot.dgr);
				::Uml::DiagramNamespaces::iterator nses_i = nses.begin();

				string schemalocations;
				vector<string> custom_ns_map;
				while (nses_i != nses.end())
				{
					string ns_uri = getNSURI(nses_i->getPath2("/", false));

					if (schemalocations.size())
						schemalocations += ' ';

					schemalocations += ns_uri;
					schemalocations += ' ';
					schemalocations += nses_i->getPath2("_");
					schemalocations += ".xsd";

					de->setAttributeNS(X("http://www.w3.org/2000/xmlns/"),
							   X(string("xmlns:") + nses_i->getPath2("_", false)), X(ns_uri));

					string ns_path = nses_i->getPath2("::", false);
					xsd_ns_mapping_storage::str_str_map::const_iterator it_ns_map = xsd_ns_mapping_storage::static_xsd_ns_back_mapping_container.find(ns_path);
					if (it_ns_map != xsd_ns_mapping_storage::static_xsd_ns_back_mapping_container.end())
						custom_ns_map.push_back(ns_path + "=\"" + UdmUtil::escape_chars(it_ns_map->second, '\\', "\"") + "\"");

					nses_i++;


				};
				if (schemalocations.size())
					de->setAttributeNS(X("http://www.w3.org/2001/XMLSchema-instance"),
						     X("xsi:schemaLocation"), X(schemalocations) );

				if (custom_ns_map.size())
				{
					DOMProcessingInstruction *DPI = doc->createProcessingInstruction(gXML_udm_udmdom_nsmap, X(UdmUtil::vector_to_string(custom_ns_map, ' ')));
					//DOM_ProcessingInstruction DPI2 = doc.createProcessingInstruction(DOMString("udm_udmdom_nsmap"), DOMString(UdmUtil::vector_to_string(custom_ns_map, ' ').c_str()));
					doc->insertBefore(DPI, doc->getFirstChild());
					//doc.insertBefore(DPI2, doc.getFirstChild());
				}
			}



			saveondestroy = (sem != Udm::CHANGES_LOST_DEFAULT);

		

		
		CATCH_XML_EXCEPTION("DomDataNetwork::CreateNew()") 
		
	}
//======================================================
	UDM_DLL void DomDataNetwork::CommitEditSequence() { ; }
	UDM_DLL void DomDataNetwork::AbortEditSequence() { ; }

	UDM_DLL void DomDataNetwork::CloseNoUpdate() 
	{
		TRY_XML_EXCEPTION

			// We must delete doc if the dn was created via CreateNew
			if (parser->getDocument() == NULL) {
				DOMElement *de = static_cast<DomObject *>(GetRootObject().__impl())->dom_element;
				XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = de->getOwnerDocument();
				doc->release();
			}
			rootobject = NULL;

		CATCH_XML_EXCEPTION("DomDataNetwork::CloseNoUpdate()");
	}


	UDM_DLL void DomDataNetwork::CloseWithUpdate() 
	{

		TRY_XML_EXCEPTION

			DOMElement *de = static_cast<DomObject *>(GetRootObject().__impl())->dom_element;
			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = de->getOwnerDocument();
			UDM_ASSERT( doc != NULL);

			MySerializer serializer(doc);

			if (str_based) 
			{
				str.erase();

				// XXX: this needs rethinking, we're putting bytes
				// into a string of characters
				MemBufFormatTarget *mem_target = new MemBufFormatTarget();
				serializer.serializeTo(doc, mem_target);
				str = string((const char *) mem_target->getRawBuffer(), mem_target->getLen());

				delete mem_target;
			}
			else
			{
				XMLFormatTarget *file_target = new LocalFileFormatTarget(X(savesystemname));
				serializer.serializeTo(doc, file_target);
				delete file_target;
			}


			if (parser->getDocument() == NULL) {
				doc->release();
			}
			rootobject = NULL;
		
		CATCH_XML_EXCEPTION("DomDataNetwork::CloseWithUpdate()") 

	}

	UDM_DLL void DomDataNetwork::SaveAs(string systemname)
	{
		TRY_XML_EXCEPTION

			DOMElement *de = static_cast<DomObject *>(GetRootObject().__impl())->dom_element;
			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = de->getOwnerDocument();
			UDM_ASSERT( doc != NULL);

			MySerializer serializer(doc);
			XMLFormatTarget *file_target = new LocalFileFormatTarget(X(systemname));
			serializer.serializeTo(doc, file_target);
			delete file_target;
		
		CATCH_XML_EXCEPTION("DomDataNetwork::SaveAs()") 
	}; 


	UDM_DLL Object DomDataNetwork::ObjectById(Object::uniqueId_type id) 
	{ 
		TRY_XML_EXCEPTION

		
			IdToDomElementMap::iterator i = DomElements.find(id);

			if (i != DomElements.end())
				return new DomObject(/*GetRootMeta(),*/ i->second, this);
			else return NULL;

		CATCH_XML_EXCEPTION("DomDataNetwork::ObjectById()") 
	}


	const string DomDataNetwork::FindFile(const string file_name)
	{


#ifdef _WIN32
		static const char path_dlim =';';
		static const char path_hier ='\\';
#else
		static const char path_dlim =':';
		static const char path_hier ='/';
#endif

		

		string ret;			// we return this;

		if (DTDPath.empty())
		{
			//look in curent folder;

			if (access(file_name.c_str(), 4) == 0) return file_name;

			//check PATH environment variable
			const char * _path = getenv("PATH");
			char * path = NULL;
			char * full_file_name = NULL;

			//
			//follows state-of-the-art file lookup in PATH
			//
			if (_path)
			{
				path = new char[strlen(_path) + 1]; strcpy(path, _path);			//safe copy of the env. var we can fool arround with
				char * curr_path = path;											//jump start
				bool found = false;													//we keep lookin'

				while (!found && curr_path && *curr_path != '\0')					
				{
					char * next_delim = strchr(curr_path, path_dlim);
					if (next_delim) *next_delim++ = '\0';

					full_file_name = new char[strlen(curr_path) + file_name.size() + 2];//hier. char+ ending '\0'
					sprintf(full_file_name, "%s%c%s", curr_path, path_hier, file_name.c_str());
					if (access(full_file_name, 4) == 0)
					{
						found = true;
						ret = full_file_name;
						delete [] full_file_name;
						continue;
					}
					delete [] full_file_name;
					curr_path = next_delim;
				}

				delete [] path;
				
			}

		}
		else
		{
			char * full_file_name = new char[DTDPath.size() + file_name.size() + 2];//hier. char+ ending '\0'
			sprintf(full_file_name, "%s%c%s", DTDPath.c_str(), path_hier, file_name.c_str());
			
			if (access(full_file_name, 4) == 0) ret = full_file_name;
			delete [] full_file_name;

		}
		return ret;
	};


	bool  DomDataNetwork::multiroles;
	UDM_DLL string  DomDataNetwork::DTDPath;

	static class reg {
		public:
		reg() {
			XMLPlatformUtils::Initialize();
			DomDataNetwork::RegisterBackend("DOM", "xml", &DomDataNetwork::factory);
			UdmDom::Dummy_For_Linker++;	//just to use the variable...
		}	
		~reg() {
			DomDataNetwork::UnRegisterBackends();
			//disabled due to crashes caused by destructors called after Terminate()
			//not calling Terminate causes appverif failures when UdmDll is unloaded, since XMLPlatformUtils::Initialize creates critical sections
			//XMLPlatformUtils::Terminate();
		}
	} _reg_unused;

	UDM_DLL int DummyFL;   // for linker
	
	
	bool isXmi(const string& systemname)
	{
		
		TRY_XML_EXCEPTION

		UDM_ASSERT( systemname.length());

/*		savesystemname = systemname;
		if(!strnicmp(savesystemname.c_str(),"DOM:", 4)) savesystemname.erase(0,4);*/

		XercesDOMParser *parser = createDOMParser();

		try
		{
			parser->parse( X(systemname) );
		}
		catch(const XMLException& toCatch)
		{
			string description = "Error during parsing: '";
			description += systemname;
			description += "'. Exception message is: ";
			description += StrX(toCatch.getMessage());

			releaseDOMParser(&parser);
			throw udm_exception(description);
		}

		DOMElement *root = parser->getDocument()->getDocumentElement();
		const XMLCh *root_name = root->getNodeName();

		bool retval = XMLString::stringLen(root_name) == 3 && XMLString::equals(root_name, gXML_XMI);

		releaseDOMParser(&parser);

		return retval;

		CATCH_XML_EXCEPTION("UdmDom::isXmi()");
	};


}

