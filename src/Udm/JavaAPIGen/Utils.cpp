#include "Utils.h"

#include "../File2Code/File2Code.h"
#include <ctime>
#include <UmlExt.h>
#include <UdmUtil.h>

#ifdef _WIN32

  #include <direct.h>
  void make_dir( const std::string& fname)
  {
    _mkdir(fname.c_str());
  }
#else	//WIN32
  #include <cerrno>
  #include <cstring>
  #include <unistd.h>
  #include <utime.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <dirent.h>

  void make_dir( const std::string& fname)
  {
    if (mkdir(fname.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH))
			throw udm_exception(string("Could not create temporary directory(")+ fname + string("): ") + strerror(errno));
	}
#endif	//WIN32


//! Convert the std string to lower case.
/*!
  Returns the string specified by the parameter in lower case.
*/
string Utils::toLower ( const string & str )
{
   string ret;

   for ( unsigned int i = 0; i < str.size(); i++ )
   {
     ret += tolower(str[i]);
   }

   return ret;
}

//! Creates a new directory with the specified directory name.
/*!
  Creates a new directory specified by the parameter.
*/
void Utils::makeDir (  const std::string& dirname )
{
  make_dir( dirname );
}

//! The copyright text for the generated Java API.
string Utils::m_copyright_text = 
    string("/* This is a generated file, do not modify its content.\n")
  + " * Copyright (c) Vanderbilt University, 2000-2005\n"
  + " * ALL RIGHTS RESERVED\n"
  + " * Vanderbilt University disclaims all warranties with regard to this \n"
  + " * software, including all implied warranties of merchantability and \n"
  + " * fitness.  In no event shall Vanderbilt University be liable for any \n"
  + " * special, indirect or consequential damages or any damages whatsoever\n"
  + " * resulting from loss of use, data or profits, whether in an action of\n"
  + " * contract, negligence or other tortious action, arising out of or in \n"
  + " * connection with the use or performance of this software.	\n */\n\n";

//! The copyright text for the generated Java API.
/*!
  Returns the copyright text for the generated Java API.
*/
const string & Utils::getCopyrightText ( )
{
  return m_copyright_text;
}

//! Creates a package signature from a directory structure.
/*!
  Returns the package signature of the directory structure
  specified by the parameter.

  E.g.: edu/isis/vanderbilt/meta -> edu.isis.vanderbilt.meta
*/
string Utils::toPackageName ( const string & packagePath )
{
	string ret(packagePath);
	std::replace(ret.begin(), ret.end(), '/', '.');
	return ret;
}

//! Creates a directory structure from a package signature .
/*!
  Returns the directory structure of the package signature 
  specified by the parameter.

  E.g.: edu.isis.vanderbilt.meta -> edu/isis/vanderbilt/meta 
*/
string Utils::toPackagePath ( const string & packageName )
{
   string ret ( packageName );

   for ( unsigned int i = 0; i < ret.size(); i++ )
   {
      if ( ret[i] == '.' )
         ret[i] = '/';
   }

   return ret;
}

//! Generate the Java version of the input XML file.
/*!
  Store the Java string representation of the input XML file in the destionation directory.
*/
void Utils::XML2Java( const string & java_file_name, const string & input_xml, const string & dest_dir )
{
  File2Code f2c_udm(dest_dir, java_file_name, input_xml, File2Code::JAVA, Utils::toPackageName(dest_dir));

  f2c_udm.gen();

}

//! The current date.
/*!
  Returns the current date in the form as follows: "Tue Jun 21 09:43:55 2005".
*/
string Utils::getTime()	
{
		time_t t;
		time(&t);
		char *c = ctime(&t);

		c[24] = '\0';
		return c;
}

//! The possible root objects of the given namespace.
/*!
  Returns the possible root objects of the namespace specified
  by the parameter.
*/
vector< ::Uml::Class> Utils::getPossibleRootClasses (const set< ::Uml::Class> & classes)
{
  vector< ::Uml::Class> globalElements;
  set< ::Uml::Class>::const_iterator i = classes.begin();

  while ( i != classes.end() )
  {
    if ( !(bool)i->isAbstract() )
    {
      set< ::Uml::Class> p_c = Uml::AncestorContainerClasses(*i); //all possible containers

      //the condition: It's not contained or it's only container is itself
      if ( (p_c.size() == 0) || ((p_c.size() == 1) && (*(p_c.begin()) == *i)) )
        globalElements.push_back(*i);
    }

    ++i;
  }

  return globalElements;
} 

//! Returns the package signature for the specific class.
/*!
  If the namespace path of cl differs from the namespace path specified by the 
  parameter current_ns_path, then it returns the the abolute path 
  (package signature) of cl.
*/
string Utils::getPackageSignature(const ::Uml::Class &cl
  , const string & current_ns_path
  , const string & pckg_hierarcy)

{
	ASSERT(pckg_hierarcy[pckg_hierarcy.length()-1] != '/');
	ASSERT(pckg_hierarcy[pckg_hierarcy.length()-1] != '.');
	ASSERT(current_ns_path[current_ns_path.length()-1] != '.');
	::Uml::Namespace cl_ns = cl.parent_ns();
	string cl_ns_path = cl_ns ? cl_ns.getPath2(".", false) : "";
	cl_ns_path = Utils::toLower(cl_ns_path);
	string ret;
	ret = Utils::toPackageName(pckg_hierarcy);
	if (current_ns_path.length())
	{
		ret.resize(ret.length() - (current_ns_path.length() + 1));
	}
	if (cl_ns_path.length())
	{
		ret += ".";
		ret += cl_ns_path;
	}
	ret += ".";
	ASSERT(ret[ret.length()-1] == '.');
	return ret;
}

//! Returns the first non-abstract ancestor of the class specified by the parameter
::Uml::Class Utils::getFirstNonAbstractAncestor( const ::Uml::Class &cl )
{
  set< ::Uml::Class> ancs = cl.baseTypes();
  if ( ancs.size() )
  {
    ::Uml::Class anc = *(ancs.begin());
    ::Uml::Class ret = getFirstNonAbstractAncestor(anc);

    if ( ret )
      return ret;

    if ( cl.isAbstract() )
      return NULL;

    return cl;
  }

  if ( cl.isAbstract() )
    return NULL;

  return cl;
} 

//! Returns the composition child role string for the given role.
string Utils::getCCRString(const ::Uml::CompositionChildRole& ccr)
{
  return ( (string)ccr.name() ) + "_ccr";
};
