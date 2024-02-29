#include "UtilsGen.h"

#include "Utils.h"
#include <Uml.h>

//! Constructor.
/*!
  Creates and initializes the Java API generation.
*/
UtilsGen::UtilsGen( const ::Uml::Diagram &diagram
      , const ::Uml::Namespace &ns
      , const string & package_name
      , const map<string, string> & ns_map)
  :   m_diagram ( diagram ), m_ns( ns ), m_ns_map( ns_map )
  , m_package_name(  package_name  )
  , m_output()
{
}

//! Destructor.
/*!
  .
*/
UtilsGen::~UtilsGen ( void )
{
}
//================================================

//! Entry point of Java API generation.
/*!
  .
*/
void UtilsGen::generate()
{
  open( );
  header( );
  wrapper( );

  close( );
}

//! Creates and opens the JAVA source file.
/*!
  Generates the Java source file for the utils class.
  Throws udm_exception in case of I/O error.
*/
void UtilsGen::open()
{
  m_output.open( (m_package_name + "//Utils.java").c_str() );

  if ( !m_output.good() )
  {
    throw udm_exception("Error opening file for writing " + m_package_name + ".Utils.java");
  }
}

//! Closes the JAVA source file.
void UtilsGen::close()
{
  // close the class
  m_output << "}" << endl;

  // close the file
  m_output.close();
  m_output.clear();
}

//! Generates the header of the source file.
void UtilsGen::header( )
{
  // generate header with time stamp
  m_output << "/* Generated on " << Utils::getTime() << " */" << endl << endl;
  m_output << Utils::getCopyrightText();

  //////
  // generate package name
  m_output << "package " << Utils::toPackageName( m_package_name ) << ";" << endl;
  m_output << endl;

  //////
  // generate imports
  m_output << "import edu.vanderbilt.isis.udm.*;" << endl;
  //m_output << "import java.lang.reflect.Array;" << endl;
  m_output << endl;

  // generate class documentation
  m_output << "/**" << endl;
  if ( m_ns != ::Uml::Namespace( NULL ) )
    m_output << " * Utility class for namespace <code>" << m_ns.getPath2("::", false) << "</code>." << endl;
  else
    m_output << " * Utility class for diagram <code>" << m_diagram.name() << "</code>." << endl;
  m_output << " */ " << endl;

  // generate the signature of the class
  m_output << "public class Utils" << endl;
  m_output << "{" << endl;
}

//! Generate function that wraps a pseudoobject in a domain specific object.
void UtilsGen::wrapper( )
{
  set< ::Uml::Class> classes = m_ns != ::Uml::Namespace( NULL ) ? m_ns.classes() : m_diagram.classes();
  set< ::Uml::Class>::iterator classes_i;

  // generate a static function that wraps a pseudoobject in a domain specific object
  m_output << "\t/**" << endl;
  m_output << "\t * Generates a domain specific object from the object specified by the parameter." << endl;
  m_output << "\t * @param  obj The generic udm object" << endl;
  m_output << "\t * @param  diagram The diagram of the data network that contains <code>obj</code>" << endl;
  m_output << "\t * @return  The domain specific object" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic static UdmPseudoObject wrapWithSubclass(UdmPseudoObject obj, Diagram diagram)";
  m_output <<  "\n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\tjava.lang.String type = obj.getType();" << endl;

  for ( classes_i = classes.begin(); classes_i != classes.end(); classes_i++ )
  {
    string c_name = classes_i->name();
    if ( classes_i->isAbstract() )
        continue;

    m_output << "\t\tif (" << c_name << ".META_TYPE.equals(type))" << endl;
    m_output << "\t\t{" << endl;
    m_output << "\t\t\treturn new " << c_name << "(obj, diagram);" << endl;
    m_output << "\t\t}" << endl;
    m_output << "\t\telse" << endl;
  }

  m_output << "\t\t{" << endl;
  m_output << "\t\t\tthrow new UdmException(\"Unexpected type: \'\" + type + \"\'\", true);" << endl;
  m_output << "\t\t}" << endl;
  m_output << "\t}" << endl;
  m_output << endl;

  // generate a static function that wraps an array of pseudoobjects in an array of domain specific objects
  m_output << "\t/**" << endl;
  m_output << "\t * Generates an array of domain specific objects from the objects specified by the parameter." << endl;
  m_output << "\t * @param  objs The generic udm objects" << endl;
  m_output << "\t * @param  arrayClass The type of the array class" << endl;
  m_output << "\t * @param  diagram The diagram of the data network that contains <code>obj</code>" << endl;
  m_output << "\t * @return  The array of domain specific objects" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic static UdmPseudoObject[] wrapWithSubclass(UdmPseudoObjectContainer objs, Class arrayClass, Diagram diagram) \n\t\t throws UdmException " << endl;
  m_output << "\t{" << endl;
  m_output << "\t\tif (!UdmPseudoObject.class.isAssignableFrom(arrayClass)) " << endl;
  m_output << "\t\t{" << endl;
  m_output << "\t\t\tthrow new IllegalArgumentException(\"\" + arrayClass + \" is not a subtype of \"+ UdmPseudoObject.class.getName());" << endl;
  m_output << "\t\t}" << endl;
  m_output << "\t\tUdmPseudoObject[] res = (UdmPseudoObject[]) java.lang.reflect.Array.newInstance(arrayClass, objs.getLength());" << endl;
  m_output << "\t\tfor (int i = 0; i < res.length; i++) " << endl;
  m_output << "\t\t{" << endl;
  m_output << "\t\t\tUdmPseudoObject obj = objs.getAt(i);" << endl;
  m_output << "\t\t\tif (obj != null) " << endl;
  m_output << "\t\t\t{" << endl;
  m_output << "\t\t\t\tres[i] = wrapWithSubclass(obj, diagram);" << endl;
  m_output << "\t\t\t}" << endl;
  m_output << "\t\t}" << endl;
  m_output << "\t\treturn res;" << endl;
  m_output << "\t}" << endl;
}
