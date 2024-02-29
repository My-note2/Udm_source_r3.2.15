#include "FactoryGen.h"

#include "Utils.h"
#include <Uml.h>
#include <UmlExt.h>
#include <UdmUtil.h>

//! Constructor.
/*!
  Creates and initializes the Java API generation.
*/
FactoryGen::FactoryGen( const ::Uml::Diagram &diagram
                      , const ::Uml::Namespace &ns
                      , const string &  m_package_name 
                      , const string & m_root_name
                      , const map<string, string> & ns_map
                      , const string& inputfile
                      , ofstream & factory_output
                      , ofstream & example_output
                      , const string & type)
:   m_diagram( diagram ), m_ns( ns ), m_ns_map( ns_map ), m_inputfile( "" )
  , m_package_name(  m_package_name  ), m_root_name( m_root_name )
  , m_diag_name( m_diagram.name() ), m_output()
  , m_type( type ), m_factory_output( factory_output ), m_example_output( example_output )
{
  m_inputfile = inputfile;
  for (unsigned int i = 0; i < m_inputfile.size(); ++i)
  {
    if ((m_inputfile[i] == '\\') || (m_inputfile[i] == '/') || (m_inputfile[i] == ':'))
      m_inputfile[i] = '_';
  }
  //cout << "input file: " << m_inputfile << endl;
}

//! Destructor.
/*!
  .
*/
FactoryGen::~FactoryGen ( void )
{
}
//================================================

//! Entry point of Java API generation.
/*!
  .
*/
void FactoryGen::generate()
{
  addToFactoryRepository();
  generateExample();

  open( );
  header( );
  classJavadoc( );

  constructor( );
  initializeMetaClasses( );
  createDN( );
  openDN( );
  closeDN( );
  saveDN( );

  checkConstraints( );

  close( );
}

//! Adds this factory clss to the factory repository.
/*!
  Generates a getter function in the FactoryRepository Java class
  to access the factory class.
*/
void FactoryGen::addToFactoryRepository( )
{
  // add this factory to the factory repository
  m_factory_output << "\t/**" << endl;
  m_factory_output << "\t * Returns an instance of the factory <code>" << m_root_name << m_type << "Factory</code>" << endl;
  m_factory_output << containerDescriptionDoc( ) << endl;
  m_factory_output << "\t * @return An instance of the factory <code>" << m_root_name << m_type << "Factory</code>." << endl;
  m_factory_output << "\t * @throws UdmException If any Udm related exception occurred" << endl;
  m_factory_output << "\t * @see " << Utils::toPackageName( m_package_name ) << "." << m_root_name << m_type << "Factory " << endl;
  m_factory_output << "\t */ " << endl;
  m_factory_output << "\tpublic static " << Utils::toPackageName( m_package_name ) << "." << m_root_name << m_type << "Factory get" << namespaceOrDiagramName( )
    << m_root_name << m_type << "Factory() \n\t\t throws UdmException " << endl;
  m_factory_output << "\t{" << endl;
  m_factory_output << "\t\treturn new " << Utils::toPackageName( m_package_name ) << "." << m_root_name << m_type << "Factory();" << endl;
  m_factory_output << "\t}" << endl;
  m_factory_output << endl;
}

//! Generates example code that uses this factory.
/*!
  Generates example code in the TestExample.java source
  file that uses this factory.
*/
void FactoryGen::generateExample( )
{
  if (m_type == "File")
    generateExampleFile( );
  else
    generateExampleString( );
}

//! Generates example code that uses this factory.
void FactoryGen::generateExampleFile( )
{
  // generate example functions

  // CreateInFile
  m_example_output << endl;
  m_example_output << "\t/**" << endl;
  m_example_output << "\t * Creates a new data network in a file with root object <code>" << m_root_name << "</code> " << endl;
  m_example_output << containerDescriptionDoc( ) << endl;
  m_example_output << "\t * @throws UdmException" << endl;
  m_example_output << "\t */" << endl;
  m_example_output << "\tpublic void testCreateInFile_" << m_root_name << "_" << containerDescriptionFunc( ) << "() \n\t\tthrows UdmException " << endl;
  m_example_output << "\t{" << endl;
  m_example_output << "\t\t// open the data network" << endl;
  m_example_output << "\t\t" << Utils::toPackageName( m_package_name ) << "." << m_root_name << m_type << "Factory gtf \n\t\t\t= FactoryRepository.get" << namespaceOrDiagramName( ) << m_root_name << m_type << "Factory();" << endl;
  m_example_output << "\t\t" << Utils::toPackageName( m_package_name ) << "."  << m_root_name << " root \n\t\t\t= gtf.create(\"" << m_root_name << "-newmodel.xml\");" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t\t// manipulate the data network" << endl;
  m_example_output << "\t\t//...;" << endl;
  m_example_output << "\t\t"<< endl;
  m_example_output << "\t\t// check constraints" << endl;
  m_example_output << "\t\tgtf.checkConstraints();" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t\t// close the data network without saving the changes" << endl;
  m_example_output << "\t\tgtf.close();" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t}" << endl;

  // OpenFromFile
  m_example_output << endl;
  m_example_output << "\t/**" << endl;
  m_example_output << "\t * Opens an existing data network from a file with root object <code>" << m_root_name << "</code> " << endl;
  m_example_output << containerDescriptionDoc( ) << endl;
  m_example_output << "\t * @throws UdmException" << endl;
  m_example_output << "\t */" << endl;
  m_example_output << "\tpublic void testOpenFromFile_" << m_root_name << "_" << containerDescriptionFunc( ) << "() \n\t\tthrows UdmException " << endl;
  m_example_output << "\t{" << endl;
  m_example_output << "\t\t// open the data network" << endl;
  m_example_output << "\t\t" << Utils::toPackageName( m_package_name ) << "." << m_root_name << m_type << "Factory gtf \n\t\t\t= FactoryRepository.get" << namespaceOrDiagramName( ) << m_root_name << m_type << "Factory();" << endl;
  m_example_output << "\t\t" << Utils::toPackageName( m_package_name ) << "."  << m_root_name << " root \n\t\t\t= gtf.open(\"" << m_root_name << "-existingmodel.xml\");" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t\t// manipulate the data network" << endl;
  m_example_output << "\t\t//...;" << endl;
  m_example_output << "\t\t"<< endl;
  m_example_output << "\t\t// check constraints" << endl;
  m_example_output << "\t\tgtf.checkConstraints();" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t\t// close and save the data network" << endl;
  m_example_output << "\t\tgtf.save();" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t}" << endl;
}

//! Generates example code that uses this factory.
void FactoryGen::generateExampleString( )
{
  // generate example functions

  // CreateInString
  m_example_output << endl;
  m_example_output << "\t/**" << endl;
  m_example_output << "\t * Creates a new data network in a string with root object <code>" << m_root_name << "</code> " << endl;
  m_example_output << containerDescriptionDoc( ) << endl;
  m_example_output << "\t * @throws UdmException" << endl;
  m_example_output << "\t */" << endl;
  m_example_output << "\tpublic void testCreateInString_" << m_root_name << "_" << containerDescriptionFunc( ) << "() \n\t\tthrows UdmException " << endl;
  m_example_output << "\t{" << endl;
  m_example_output << "\t\t// open the data network" << endl;
  m_example_output << "\t\t" << Utils::toPackageName( m_package_name ) << "." << m_root_name << "StringFactory gtf \n\t\t\t= FactoryRepository.get" << namespaceOrDiagramName( ) << m_root_name << "StringFactory();" << endl;
  m_example_output << "\t\t" << Utils::toPackageName( m_package_name ) << "."  << m_root_name << " root \n\t\t\t= gtf.create();" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t\t// manipulate the data network" << endl;
  m_example_output << "\t\t//...;" << endl;
  m_example_output << "\t\t"<< endl;
  m_example_output << "\t\t// check constraints" << endl;
  m_example_output << "\t\tgtf.checkConstraints();" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t\t// close the data network without saving the changes" << endl;
  m_example_output << "\t\tgtf.close();" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t}" << endl;

  // OpenFromString
  m_example_output << endl;
  m_example_output << "\t/**" << endl;
  m_example_output << "\t * Opens an existing data network from a string with root object <code>" << m_root_name << "</code> " << endl;
  m_example_output << containerDescriptionDoc( ) << endl;
  m_example_output << "\t * @throws UdmException" << endl;
  m_example_output << "\t */" << endl;
  m_example_output << "\tpublic void testOpenFromString_" << m_root_name << "_" << containerDescriptionFunc( ) << "() \n\t\tthrows UdmException " << endl;
  m_example_output << "\t{" << endl; 
  m_example_output << "\t\tjava.lang.String xmlString = \"...\";" << endl;
  m_example_output << "\t\t// open the data network" << endl;
  m_example_output << "\t\t" << Utils::toPackageName( m_package_name ) << "." << m_root_name << "StringFactory gtf \n\t\t\t= FactoryRepository.get" << namespaceOrDiagramName( ) << m_root_name << "StringFactory();" << endl;
  m_example_output << "\t\t" << Utils::toPackageName( m_package_name ) << "."  << m_root_name << " root \n\t\t\t= gtf.open(xmlString);" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t\t// manipulate the data network" << endl;
  m_example_output << "\t\t//...;" << endl;
  m_example_output << "\t\t"<< endl;
  m_example_output << "\t\t// check constraints" << endl;
  m_example_output << "\t\tgtf.checkConstraints();" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t\t// close and save the data network" << endl;
  m_example_output << "\t\tjava.lang.String result = gtf.save();" << endl;
  m_example_output << "\t\tSystem.out.println(result);" << endl;
  m_example_output << "\t}" << endl;

  // OpenFromStream
  m_example_output << endl;
  m_example_output << "\t/**" << endl;
  m_example_output << "\t * Opens an existing data network from a stream with root object <code>" << m_root_name << "</code> " << endl;
  m_example_output << containerDescriptionDoc( ) << endl;
  m_example_output << "\t * @throws UdmException" << endl;
  m_example_output << "\t */" << endl;
  m_example_output << "\tpublic void testOpenFromStream_" << m_root_name << "_" << containerDescriptionFunc( ) << "(java.io.InputStream in) \n\t\tthrows UdmException " << endl;
  m_example_output << "\t{" << endl; 
  m_example_output << "\t\t// open the data network" << endl;
  m_example_output << "\t\t" << Utils::toPackageName( m_package_name ) << "." << m_root_name << "StringFactory gtf \n\t\t\t= FactoryRepository.get" << namespaceOrDiagramName( ) << m_root_name << "StringFactory();" << endl;
  m_example_output << "\t\t" << Utils::toPackageName( m_package_name ) << "."  << m_root_name << " root \n\t\t\t= gtf.open(in);" << endl;
  m_example_output << "\t\t//" << Utils::toPackageName( m_package_name ) << "." << m_root_name << "FileFactory gtf \n\t\t\t//= FactoryRepository.get" << namespaceOrDiagramName( ) << m_root_name << "FileFactory();" << endl;
  m_example_output << "\t\t//" << Utils::toPackageName( m_package_name ) << "."  << m_root_name << " root \n\t\t\t//= gtf.open(in);" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t\t// manipulate the data network" << endl;
  m_example_output << "\t\t//...;" << endl;
  m_example_output << "\t\t"<< endl;
  m_example_output << "\t\t// check constraints" << endl;
  m_example_output << "\t\tgtf.checkConstraints();" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t\t// close and save the data network" << endl;
  m_example_output << "\t\tjava.io.InputStream result = gtf.saveAsStream();" << endl;
  m_example_output << "\t\t" << endl;
  m_example_output << "\t}" << endl;
}

//! Creates and opens the JAVA source file.
/*!
  Generates the Java source file for the factory.
  Throws udm_exception in case of I/O error.
*/
void FactoryGen::open()
{
  m_output.open( (m_package_name + "//" + m_root_name + m_type + "Factory.java").c_str() );

  if ( !m_output.good() )
  {
    throw udm_exception("Error opening file for writing " + m_root_name + m_type + "Factory.java");
  }
}

//! Closes the JAVA source file.
void FactoryGen::close()
{
  // close the class
  m_output << "}" << endl;

  // close the file
  m_output.close();
  m_output.clear();
}

//! Generates the header of the source file.
void FactoryGen::header( )
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
  m_output << "import java.io.InputStream;" << endl;
  m_output << endl;
  m_output << "import edu.vanderbilt.isis.udm.Diagram;" << endl;
  m_output << "import edu.vanderbilt.isis.udm.UdmException;" << endl;
  m_output << "import edu.vanderbilt.isis.udm.Udm" << m_type << "Factory;" << endl;
  m_output << "import edu.vanderbilt.isis.udm.UdmHelper;" << endl;
  m_output << endl;
}

//! Generates the class JAVADOC of the source file.
void FactoryGen::classJavadoc( )
{
  if (m_type == "File")
    classJavadocFile( );
  else
    classJavadocString( );
}

//! Generates the class JAVAODC of the source file.
void FactoryGen::classJavadocFile( )
{
  // generate class documentation
  m_output << "/**" << endl;
  m_output << " * Domain specific factory class for <code>" << m_root_name << "</code>." << endl;
  m_output << " * <p>This factory allows only file and stream operations on the data network." << endl;
  m_output << " * <p>This example opens an existing data network from a file" << endl;
  m_output << " * where the type of the root object is <code>"<< m_root_name << "</code>." << endl;
  m_output << " * <blockquote><pre>" << endl;
  m_output << " *         ....;" << endl;
  m_output << " *        // open the data network" << endl;
  m_output << " *        " << m_root_name << "FileFactory gtf = FactoryRepository.get" << namespaceOrDiagramName( ) << m_root_name << "FileFactory();" << endl;
  m_output << " *        " << m_root_name << " root = gtf.open(\"" << m_root_name << "-existingmodel.xml\");" << endl;
  m_output << " * " << endl;
  m_output << " *         // manipulate the data network" << endl;
  m_output << " *         ....;" << endl;
  m_output << " * " << endl;
  m_output << " *         // check constraints" << endl;
  m_output << " *         gtf.checkConstraints();" << endl;
  m_output << " * " << endl;
  m_output << " *         // close the data network without saving the changes" << endl;
  m_output << " *         gtf.close();" << endl;
  m_output << " *         ....;" << endl;
  m_output << " * </pre></blockquote>" << endl;
  m_output << " */ " << endl;
}

//! Generates the class JAVAODC of the source file.
void FactoryGen::classJavadocString( )
{
  // generate class documentation
  m_output << "/**" << endl;
  m_output << " * Domain specific factory class for <code>" << m_root_name << "</code>." << endl;
  m_output << " * <p>This factory allows only string and stream operations on the data network." << endl;
  m_output << " * <p>This example opens an existing data network from a string" << endl;
  m_output << " * where the type of the root object is <code>"<< m_root_name << "</code>." << endl;
  m_output << " * <blockquote><pre>" << endl;
  m_output << " *         ....;" << endl;
  m_output << " *         String xmlString = \"...\";" << endl;
  m_output << " *        // open the data network" << endl;
  m_output << " *        " << m_root_name << "StringFactory gtf = FactoryRepository.get" << namespaceOrDiagramName( ) << m_root_name << "StringFactory();" << endl;
  m_output << " *        " << m_root_name << " root = gtf.open(xmlString);" << endl;
  m_output << " * " << endl;
  m_output << " *         // manipulate the data network" << endl;
  m_output << " *         ....;" << endl;
  m_output << " * " << endl;
  m_output << " *         // check constraints" << endl;
  m_output << " *         gtf.checkConstraints();" << endl;
  m_output << " * " << endl;
  m_output << " *         // close  and save the data network in a string" << endl;
  m_output << " *         String resul = gtf.save();" << endl;
  m_output << " *         ....;" << endl;
  m_output << " * </pre></blockquote>" << endl;
  m_output << " */ " << endl;
}

//! Generates the constructor.
void FactoryGen::constructor( )
{
  // generate the signature of the class
  m_output << "public class " << m_root_name << m_type << "Factory" << endl;
  m_output << "{" << endl;
  m_output << "\t// resource information" << endl;
  m_output << "\tprivate static final java.lang.String packagePath = \"/edu/vanderbilt/isis/meta/\";" << endl;
  m_output << "\tprivate static final java.lang.String xmlMetaFile =\"" << m_inputfile << "\";" << endl;
  m_output << "\tprivate static final java.lang.String xsdMetaFile =\"" << containerDescriptionFunc( ) << ".xsd\";" << endl;
  m_output << "\tprivate static final java.lang.String metaName =\"" << m_diag_name << "\";" << endl;
  m_output << "\t// the wrapped " << Utils::toLower( m_type ) << " factory instance" << endl;
  m_output << "\tprivate Udm" << m_type << "Factory factory;" << endl;
  m_output << endl;

  //  generate the constructor
  m_output << "\t/**" << endl;
  m_output << "\t * Constructor." << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  m_output << "\tpublic " << m_root_name << m_type << "Factory()\n\t\t throws UdmException " << endl;
  m_output << "\t{" << endl;
  m_output << "\t\tfactory = new Udm" << m_type << "Factory(xmlMetaFile, xsdMetaFile, metaName, packagePath){" << endl;
  m_output << "\t\t\tprotected void loadDiagram() throws UdmException {" << endl;
  m_output << "\t\t\t\tmetaDiagram =" << endl;
  m_output << "\t\t\t\t\tnew Diagram(" << endl;
  m_output << "\t\t\t\t\t\txmlMetaFile, edu.vanderbilt.isis.meta." << m_diag_name << "_udm_xml.getString());" << endl;
  m_output << "\t\t\t}" << endl;
  m_output << "\t\t};" << endl;
  m_output << "\t\tinitializeMetaClasses();" << endl;
  m_output << "\t}" << endl;
  m_output << endl;
}

//! Generates a function that initializes the meta classes of this factory.
void FactoryGen::initializeMetaClasses( )
{
  // generate a function to initialize the meta classes 
  m_output << "\t/**" << endl;
  m_output << "\t * Initializes the meta classes." << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  m_output << "\tprivate void initializeMetaClasses() \n\t\t throws UdmException " << endl;
  m_output << "\t{" << endl;

  set< ::Uml::Class>::iterator classes_i;
  set< ::Uml::Class> classes = m_ns != ::Uml::Namespace( NULL ) ? m_ns.classes() : m_diagram.classes();

  for( classes_i = classes.begin(); classes_i != classes.end(); classes_i++ )
  {
    ::Uml::Class cl = *classes_i;

    if ( cl.isAbstract() == false )
    {
      m_output << "\t\tfactory.getDiagram().getMetaClass(" << cl.name() << ".META_TYPE, ";
      m_output << cl.name() << ".META_TYPE_NS);" << endl;
    }
  }

  
  // map namespace to uri
  map<string, string>::const_iterator ns_iter = m_ns_map.begin();
  if ( ns_iter != m_ns_map.end() )
  {
    m_output << "\t\t// map namespace to uri" << endl;
    //m_output << "\t\tUdmHelper.ClearURIToUMLNamespaceMappings();" << std::endl;
    for(; ns_iter != m_ns_map.end(); ++ns_iter )
    {
      const std::string& ns = ns_iter->first;
      const std::string& uri = ns_iter->second;
      std::string xsdname = (string)m_diagram.name() + "_" + UdmUtil::replace_delimiter(ns, "::", "_") + ".xsd";

      m_output << "\t\tUdmHelper.AddURIToUMLNamespaceMapping(\""<< uri<<"\",\""<<ns<<"\",\""<<xsdname<<"\" );" << std::endl;
    }
  }

  // load XSDs
  m_output << "\t\t// load XSDs" << endl;
  m_output << "\t\tUdmHelper.StoreXsd(\"" << m_diag_name << ".xsd\", edu.vanderbilt.isis.meta." << m_diag_name << "_xsd.getString());" << endl;
  ::Uml::DiagramNamespaces nses(m_diagram);
  ::Uml::DiagramNamespaces::iterator nses_i = nses.begin();
  for ( ; nses_i != nses.end(); nses_i++)
  {
    string nsn = nses_i->getPath2("_");
    m_output << "\t\tUdmHelper.StoreXsd(\"" << nsn << ".xsd\", edu.vanderbilt.isis.meta." << nsn << "_xsd.getString());" << endl;
  }

  m_output << "\t}" << endl;
  m_output << endl;
}

//! Generates a function to create a new data network in file or string with the root object .
void FactoryGen::createDN( )
{
  if (m_type == "File")
    createDNInFile( );
  else
    createDNInString( );
}

//! Generates a function to create a new data network in file with the root object .
void FactoryGen::createDNInFile( )
{
  // generate a function to create a new data network in file
  // with the root object 
  m_output << "\t/**" << endl;
  m_output << "\t * Creates a new data network with root object of type <code>";
  m_output << m_root_name << "</code> in the file specified by the parameter." << endl;
  m_output << "\t * @param  instanceFileName The name of the instance file" << endl;
  m_output << "\t * @return  The root object of the data network" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic " << m_root_name << " create" << "(java.lang.String instanceFileName) \n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\t" << m_root_name << " root =" << endl;
  m_output << "\t\t\tnew " << m_root_name << "(" << endl;
  m_output << "\t\t\t\tfactory.createNewDataNetwork(instanceFileName, " << m_root_name << ".META_TYPE, " << m_root_name
      << ".META_TYPE_NS)," << endl;
  m_output << "\t\t\t\tfactory.getDiagram());" << endl;
  m_output << "\t\treturn root;" << endl;
  m_output << "\t}" << endl;
  m_output << endl;
}

//! Generates a function to create a new data network in string with the root object .
void FactoryGen::createDNInString( )
{
  // generate a function to create a new data network in a string
  // with the root object 
  m_output << "\t/**" << endl;
  m_output << "\t * Creates a new data network with root object of type <code>";
  m_output << m_root_name << "</code> in a string." << endl;
  m_output << "\t * @return  The root object of the data network" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic " << m_root_name << " create" << "()\n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\t" << m_root_name << " root =" << endl;
  m_output << "\t\t\tnew " << m_root_name << "(" << endl;
  m_output << "\t\t\t\tfactory.createNewDataNetwork(" << m_root_name << ".META_TYPE, " << m_root_name << ".META_TYPE_NS)" << endl;
  m_output << "\t\t\t\t, factory.getDiagram());" << endl;
  m_output << "\t\treturn root;" << endl;
  m_output << "\t}" << endl;
  m_output << endl;
}

//! Generate the function that opens an existing data network  with the root object.
void FactoryGen::openDN( )
{
  if (m_type == "File")
    openDNFromFile( );
  else
    openDNFromString( );
}

//! Generate the function that opens an existing data network from file with the root object..
void FactoryGen::openDNFromFile( )
{
  // generate the function that opens an existing data network from file
  // with the root object
  m_output << "\t/**" << endl;
  m_output << "\t * Opens an existing data network from the file specified by the parameter." << endl;
  m_output << "\t * @param  instanceFileName The name of the instance file" << endl;
  m_output << "\t * @return  The root object of the data network" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic " << m_root_name << " open(java.lang.String instanceFileName) \n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\t" << m_root_name << " root = \n\t\t\tnew " << m_root_name << "(" << endl;
  m_output << "\t\t\t\tfactory.openExistingDataNetwork(instanceFileName)" << endl;
  m_output << "\t\t\t\t, factory.getDiagram());" << endl;
  m_output << "\t\t\treturn root;" << endl;
  m_output << "\t}" << endl;
  m_output << endl;

  // generate the function that opens an existing data network from stream
  // with the root
  m_output << "\t/**" << endl;
  m_output << "\t * Opens an existing data network from the stream specified by the parameter." << endl;
  m_output << "\t * @param  xmlStream The stream that contains the xml" << endl;
  m_output << "\t * @return  The root object of the data network" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic " << m_root_name << " open(java.io.InputStream xmlStream) \n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\t" << m_root_name << " root = \n\t\t\tnew " << m_root_name << "(" << endl;
  m_output << "\t\t\t\tfactory.openExistingDataNetworkFromStream(xmlStream)" << endl;
  m_output << "\t\t\t\t, factory.getDiagram());" << endl;
  m_output << "\t\treturn root;" << endl;
  m_output << "\t}" << endl;
  m_output << endl;
}

//! Generate the function that opens an existing data network from a string with the root object.
void FactoryGen::openDNFromString( )
{
  // generate the function that opens an existing data network from a string
  // with the root object
  m_output << "\t/**" << endl;
  m_output << "\t * Opens an existing data network from a string specified by the parameter." << endl;
  m_output << "\t * @param  xmlString The string containing the xml." << endl;
  m_output << "\t * @return  The root object of the data network" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic " << m_root_name << " open(java.lang.String xmlString) \n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\t" << m_root_name << " root = \n\t\t\tnew " << m_root_name << "(" << endl;
  m_output << "\t\t\t\tfactory.openExistingDataNetwork(xmlString)" << endl;
  m_output << "\t\t\t\t, factory.getDiagram());" << endl;
  m_output << "\t\treturn root;" << endl;
  m_output << "\t}" << endl;
  m_output << endl;

  // generate the function that opens an existing data network from stream
  // with the root
  m_output << "\t/**" << endl;
  m_output << "\t * Opens an existing data network from the stream specified by the parameter." << endl;
  m_output << "\t * @param  xmlStream The stream that contains the xml" << endl;
  m_output << "\t * @return  The root object of the data network" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic " << m_root_name << " open(java.io.InputStream xmlStream) \n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\t" << m_root_name << " root = \n\t\t\tnew " << m_root_name << "(" << endl;
  m_output << "\t\t\t\tfactory.openExistingDataNetworkFromStream(xmlStream)" << endl;
  m_output << "\t\t\t\t, factory.getDiagram());" << endl;
  m_output << "\t\treturn root;" << endl;
  m_output << "\t}" << endl;
  m_output << endl;
}

//! Generates the save function for the network.
void FactoryGen::saveDN( )
{
  if (m_type == "File")
    saveDNFile( );
  else
    saveDNString( );

  saveAsStream( );
}

//! Generates the save function for the network that exists in a file.
void FactoryGen::saveDNFile( )
{
  //generate the save function 
  m_output << "\t/**" << endl;
  m_output << "\t * Closes and updates the data network by saving the changes in the data network." << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic void save() \n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\tfactory.closeWithUpdate();" << endl;
  m_output << "\t\tUdmHelper.ClearXsdStorage();" << endl;
  m_output << "\t\tfactory.unloadDiagram();" << endl;
  m_output << "\t}" << endl;
  m_output << endl;

  // generate the saveAs function 
  m_output << "\t/**" << endl;
  m_output << "\t * Closes the data network, saves the changes in a file" << endl;
  m_output << "\t * specified by the parameter, and unloads the diagram." << endl;
  m_output << "\t * @param  instanceFileName The name of the m_output file where the xml will be stored" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic void saveAs(java.lang.String instanceFileName) \n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\tfactory.saveAs(instanceFileName);" << endl;
  m_output << "\t\tUdmHelper.ClearXsdStorage();" << endl;
  m_output << "\t\tfactory.unloadDiagram();" << endl;
  m_output << "\t}" << endl;
  m_output << endl;
}

//! Generates the save function for the network that exists in a string.
void FactoryGen::saveDNString( )
{
  //generate the save function 
  m_output << "\t/**" << endl;
  m_output << "\t * Closes and updates the data network by saving the changes in the data network." << endl;
  m_output << "\t * @return  The data network in a string" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic java.lang.String save() \n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\tfactory.closeWithUpdate();" << endl;
  m_output << "\t\tjava.lang.String result = factory.saveAs();" << endl;
  m_output << "\t\tUdmHelper.ClearXsdStorage();" << endl;
  m_output << "\t\tfactory.unloadDiagram();" << endl;
  m_output << "\t\treturn result;" << endl;
  m_output << "\t}" << endl;
  m_output << endl;
}

//! Generates the save function for the network that saves the network to the stream.
void FactoryGen::saveAsStream( )
{
  // generate the saveAsStream function 
  m_output << "\t/**" << endl;
  m_output << "\t * Closes and updates the data network, saves the changes in a stream," << endl;
  m_output << "\t * and unloads the diagram." << endl;
  m_output << "\t * @return  The data network in a stream" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic java.io.InputStream saveAsStream() \n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\tfactory.closeWithUpdate();" << endl;
  m_output << "\t\tjava.io.InputStream result = factory.saveAsStream();" << endl;
  m_output << "\t\tUdmHelper.ClearXsdStorage();" << endl;
  m_output << "\t\tfactory.unloadDiagram();" << endl;
  m_output << "\t\treturn result;" << endl;
  m_output << "\t}" << endl;
  m_output << endl;
}

//! Generates the ocl evaluation function.
void FactoryGen::checkConstraints( )
{
  //generate the ocl evaluation function 
  m_output << "\t/**" << endl;
  m_output << "\t * Calls the constraint checker." << endl;
  m_output << "\t * @return  The result of the constriant evaluation" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic java.lang.String checkConstraints() \n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\treturn (factory.getDataNetwork().checkConstraints());" << endl;
  m_output << "\t}" << endl;
  m_output << endl;
}

 //! Generate the close function that closes the opened data network without updating it.
void FactoryGen::closeDN( )
{
  // generate the close function 
  m_output << "\t/**" << endl;
  m_output << "\t * Closes the data network without updating it and unloads the diagram." << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  // function signature
  m_output << "\tpublic void close() \n\t\t throws UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\tfactory.closeNoUpdate();" << endl;
  m_output << "\t\tUdmHelper.ClearXsdStorage();" << endl;
  m_output << "\t\tfactory.unloadDiagram();" << endl;
  m_output << "\t}" << endl;
}

//! Returns the namespace name if it is not null, otherwise the diagram name.
string FactoryGen::namespaceOrDiagramName( )
{
  if ( m_ns != ::Uml::Namespace( NULL ) )
    return m_ns.getPath2("_", false);
  return m_diag_name;
}

//! Returns the Java code snippet that describes the container.
string FactoryGen::containerDescriptionDoc( )
{
  if ( m_ns != ::Uml::Namespace( NULL ) )
    return "\t * in namespace <code>" + (string) m_ns.getPath2("::", false) + "</code>.";
  return "\t * in diagram <code>" + m_diag_name + "</code>.";
}

//! Returns the function name part that combines the diagram name with the namespace name.
string FactoryGen::containerDescriptionFunc( )
{
  if ( m_ns != ::Uml::Namespace( NULL ) )
    return m_ns.getPath2("_");
  return m_diag_name;
}
