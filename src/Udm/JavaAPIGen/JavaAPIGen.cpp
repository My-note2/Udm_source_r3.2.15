#include "JavaAPIGen.h"

#include "Utils.h"
#include "FactoryGen.h"
#include "UtilsGen.h"
#include "ClassGen.h"

void GenerateJavaClass(const ::Uml::Class &cl, const string & pckg_hierarcy, const string& diagName);

//! Constructor.
/*!
  Creates and initializes the Java API generation.
*/
JavaAPIGen::JavaAPIGen( const ::Uml::Diagram &diagram
                       , const map<string, string>& ns_map
                       , const string& inputfile )
:   m_diagram( diagram ), m_ns_map( ns_map ), m_inputfile( inputfile )
  , m_diag_name( Utils::toLower( m_diagram.name() ))
  , m_example_dir( ( "test" ) )
  , m_edu_dir( string( "edu" ) )
  , m_vu_dir( m_edu_dir +  "/vanderbilt" )
  , m_isis_dir( m_vu_dir + "/isis" )
  , m_meta_dir( m_isis_dir + "/meta" )
  , m_diag_dir( m_isis_dir + "/" + m_diag_name)
  , m_example_output(), m_factory_output()
{
}

//! Destructor.
/*!
  .
*/
JavaAPIGen::~JavaAPIGen ( void )
{
}
//================================================

//! Entry point of Java API generation.
/*!
  .
*/
void JavaAPIGen::generate()
{
  // Create dirs
  createDirs();

  // Generate the Java version of the input UDM XML file
  Utils::XML2Java( string( m_diagram.name() ) + "_udm_xml", m_inputfile, m_meta_dir );

  // Generate package.html info
  generatePackageInfo();

  // Generate and open example file
  openExample(); 

  // Generate and open factory repository file
  openFactoryRepository(); 

  // Generate java code
  generateJava( m_diagram, m_ns_map, m_inputfile );

  // Close facotry repository file
  closeFactoryRepository();

  // Close example file
  closeExample();
}

//! Create directory structure for Java API generation.
/*!
  The directory structure reflects the java package structure.
*/
void JavaAPIGen::createDirs() const
{
   Utils::makeDir ( m_example_dir );
   Utils::makeDir ( m_edu_dir );
   Utils::makeDir ( m_vu_dir );
   Utils::makeDir ( m_isis_dir );
   Utils::makeDir ( m_meta_dir );
   Utils::makeDir ( m_diag_dir );
}

//! Generate package.html information file for javadoc.
/*!
  The content of package.html file is necessary for javadoc generation.
*/
void JavaAPIGen::generatePackageInfo( ) const
{
   // package info file
   ofstream pkg_output;
   pkg_output.open( (m_diag_dir + "/package.html" ).c_str( ) );
   if ( !pkg_output.good() )
   {
      throw udm_exception( "Error opening file for writing package.html" );
   }

   pkg_output << "<html>" << endl;
   pkg_output << "<body bgcolor=\"white\">" << endl;
   pkg_output << "Provides entry point to manipulate the data network. " << endl;
   pkg_output << "For overviews, tutorials, examples, guides, and tool documentation, please check " << endl;
   pkg_output << "the Factory classes." << endl;
   pkg_output << "</body>" << endl;
   pkg_output << "</html>" << endl;

}

//! Generate example test file.
/*!
  Opens and creates  the header of the example java file.
*/
void  JavaAPIGen::openExample( )
{
   m_example_output.open( ( m_example_dir + "/" + "TestExample.java" ).c_str() );
   if ( !m_example_output.good() )
   {
      throw udm_exception("Error opening file for writing package.html");
   }

   m_example_output << "" << endl;
   // generate header with time stamp
   m_example_output << "/* Generated on " << Utils::getTime( ) << " */" << endl << endl;
   m_example_output << Utils::getCopyrightText();
   // generate package name
   m_example_output << "package test;" << endl;
   m_example_output << endl; 
   m_example_output << "import edu.vanderbilt.isis." << m_diag_name << ".FactoryRepository;" << endl;
   m_example_output << "import java.io.InputStream;" << endl;
   m_example_output << "import edu.vanderbilt.isis.udm.UdmException;" << endl;
   m_example_output << endl;
   // generate class documentation
   m_example_output << "/**" << endl;
   m_example_output << " * Test." << endl;
   m_example_output << " */ " << endl;
   // generate the signature of the class
   m_example_output << "public class TestExample " << endl;
   m_example_output << "{" << endl;
}

/*!
  Closes the example java file.
*/
void  JavaAPIGen::closeExample( )
{
   m_example_output << "}" << endl;
}

//! Generate facotry repository file.
/*!
  Opens and creates  the header of the facotry repository java file.
*/
void  JavaAPIGen::openFactoryRepository( )
{
   m_factory_output.open( ( m_diag_dir + "/" + "FactoryRepository.java" ).c_str( ) );

   if ( !m_factory_output.good() )
   {
      throw udm_exception("Error opening file for writing FactoryRepository.java");
   }

  // generate header with time stamp
   m_factory_output << "/* Generated on " << Utils::getTime( ) << " */" << endl << endl;
   m_factory_output << Utils::getCopyrightText();
   // generate package name
   m_factory_output << "package edu.vanderbilt.isis." << m_diag_name << ";" << endl;
   m_factory_output << endl;
   m_factory_output << "import edu.vanderbilt.isis.udm.UdmException;" << endl;
   m_factory_output << endl;

   // generate class documentation
   m_factory_output << "/**" << endl;
   m_factory_output << " * Repository of domain specific factory classes." << endl;
   m_factory_output << " */ " << endl;

   // generate the signature of the class
   m_factory_output << "public class FactoryRepository " << endl;
   m_factory_output << "{" << endl;
}

/*!
  Closes the facotry repository java file.
*/
void  JavaAPIGen::closeFactoryRepository( )
{
   m_factory_output << "}" << endl;
}

//! Generate the JAVA API source code.
/*!
  Generate the factory classes to create UDM object models and the
  JAVA representation of the corresponding UML diagram.
*/
void JavaAPIGen::generateJava( const ::Uml::Diagram &diagram
  , const map<string, string> & ns_map
  , const string& inputfile)
{

  // the corresponding JAVA package name
  string package_name = m_diag_dir;

  ::Uml::Namespace nullNs;

  // generate JAVA from XSD
  Utils::XML2Java( (string) m_diagram.name() + "_xsd", (string) m_diagram.name() + ".xsd", m_meta_dir );

  // the possible root objects of this diagram
  vector< ::Uml::Class> roots = Utils::getPossibleRootClasses( m_diagram.classes() );
  vector< ::Uml::Class>::iterator r_i = roots.begin();

  // generate the specific factories
  for(; r_i != roots.end(); r_i++ )
  {
    // the type of the root object
    string rc_name = r_i->name();
    //std::cout << "\troot object: "<<rc_name<<std::endl;
    //GenerateFactories( *nses_i, package_name, rc_name, ns_map, m_inputfile, m_factory_output, m_example_output );
    FactoryGen file_fact_gen( m_diagram, nullNs, package_name, rc_name, ns_map, m_inputfile, m_factory_output, m_example_output, "File" );
    file_fact_gen.generate( );
    FactoryGen string_fact_gen( m_diagram, nullNs, package_name, rc_name, ns_map, m_inputfile, m_factory_output, m_example_output, "String" );
    string_fact_gen.generate( );
  }
  
  // generate a Utils.java for each namespaces that contains the wrappers
  UtilsGen utils_gen( m_diagram, nullNs, package_name, ns_map );
  utils_gen.generate( );

  // generate java classes in the diagram
  set< ::Uml::Class> uml_classes = m_diagram.classes();
  for( set< ::Uml::Class>::iterator uc_i = uml_classes.begin(); uc_i != uml_classes.end(); uc_i++ )
  {
    ClassGen class_gen( *uc_i, package_name, m_diagram.name(), "" );
    class_gen.generate( );
  }


  // the namespaces in the diagram
  ::Uml::DiagramNamespaces nses(diagram);

  for( ::Uml::DiagramNamespaces::iterator nses_i = nses.begin(); nses_i != nses.end(); nses_i++ )
  {
    // the corresponding JAVA package name
	string package_name = m_diag_dir + "/" + Utils::toLower(nses_i->getPath2("/", false));
    // create the directory corresponding to the JAVA package name
    Utils::makeDir(package_name);

    // generate JAVA from XSD
	Utils::XML2Java( nses_i->getPath2("_") + "_xsd", nses_i->getPath2("_") + ".xsd", m_meta_dir );

    // the possible root objects of this namespace
    vector< ::Uml::Class> roots = Utils::getPossibleRootClasses( nses_i->classes() );
    vector< ::Uml::Class>::iterator r_i = roots.begin();

    // generate the specific factories
    for(; r_i != roots.end(); r_i++ )
    {
        // the type of the root object
        string rc_name = r_i->name();
        //std::cout << "\troot object: "<<rc_name<<std::endl;
        //GenerateFactories( *nses_i, package_name, rc_name, ns_map, m_inputfile, m_factory_output, m_example_output );
        FactoryGen file_fact_gen( m_diagram, *nses_i, package_name, rc_name, ns_map, m_inputfile, m_factory_output, m_example_output, "File" );
        file_fact_gen.generate( );
        FactoryGen string_fact_gen( m_diagram, *nses_i, package_name, rc_name, ns_map, m_inputfile, m_factory_output, m_example_output, "String" );
        string_fact_gen.generate( );
    }
    
    
    // generate a Utils.java for each namespaces that contains the wrappers
    UtilsGen utils_gen( m_diagram, *nses_i, package_name, ns_map );
    utils_gen.generate( );

    // generate java classes in the namespace
    set< ::Uml::Class> uml_classes = nses_i->classes();
    for( set< ::Uml::Class>::iterator uc_i = uml_classes.begin(); uc_i != uml_classes.end(); uc_i++ )
    {
      //GenerateJavaClass(*uc_i, package_name, diagName);
      ClassGen class_gen( *uc_i, package_name, m_diagram.name(), nses_i->getPath2("::", false) );
      class_gen.generate( );
    }
  }
}
