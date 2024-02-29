#ifndef JavaUtils_H
#define JavaUtils_H

#include <Uml.h>

#include <string>
#include <fstream>

using namespace std;

class Utils
{
  public:
    //! Convert the std string to lower case.
    static string toLower ( const string & str );

    //! Creates a new directory with the specified dirname
    static void makeDir ( const std::string & dirname );

    //! The copyright text for the generated Java API.
    static const string & getCopyrightText ( );

    //! Creates a package signature from a directory structure.
    static string toPackageName ( const string & packagePath );

    //! Creates a directory structure from a package signature .
    static string toPackagePath ( const string & packageName );

    //! Generate the Java version of the input XML file in the destionation directory.
    static void  XML2Java( const string & xml_name, const string & input_xml, const string & dest_dir );

    //! The current date.
    static string getTime();

    //! The possible root objects of the given namespace.
    static vector< ::Uml::Class> getPossibleRootClasses( const set< ::Uml::Class> & classes );

    //! Returns the package signature for the specific class.
    static string getPackageSignature(const ::Uml::Class &cl
      , const string & current_ns
      , const string & pckg_hierarcy);

    //! Returns the first non-abstract ancestor of the class specified by the parameter
    static ::Uml::Class getFirstNonAbstractAncestor( const ::Uml::Class &cl );

    //! Returns the composition child role string for the given role.
    static string getCCRString( const ::Uml::CompositionChildRole& ccr );

  private:
    //! The copyright text for the generated Java API.
    static string m_copyright_text;
};
#endif //JavaUtils_H
