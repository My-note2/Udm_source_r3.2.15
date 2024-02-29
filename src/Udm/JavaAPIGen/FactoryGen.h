#ifndef FactoryGen_H
#define FactoryGen_H

#include <Uml.h>
#include <fstream>

using namespace std;

class FactoryGen
{
  public:
    //! Constructor.
    FactoryGen(const ::Uml::Diagram &diagram
      , const ::Uml::Namespace &ns
      , const string & package_name
      , const string & root_name
      , const map<string, string> & ns_map
      , const string& inputfile
      , ofstream & factory_output
      , ofstream & example_output
      , const string & type);

    //! Destructor.
    ~FactoryGen( void );
    
    //! Entry point of domainspecific factory class generation.
    void  generate( );

  private:  
    //! Adds the factory clss to the factory repository.
    void addToFactoryRepository( );

    //! Generates example code that uses this factory.
    void generateExample( );
    void generateExampleFile( );
    void generateExampleString( );

    //! Creates and opens the JAVA source file.
    void open( );
    void close( );

    //! Generates the header of the source file.
    void header( );

    //! Generates the class JAVADOC of the source file.
    void classJavadoc( );
    void classJavadocFile( );
    void classJavadocString( );

    //! Generates the constructor of this factory.
    void constructor( );

    //! Generates a function that initializes the meta classes of this factory.
    void initializeMetaClasses( );

    //! Generates a function to create a new data network in file or string with the root object.
    void createDN( );
    void createDNInFile( );
    void createDNInString( );

    //! Generates the save function for the network.
    void saveDN( );
    void saveDNFile( );
    void saveDNString( );
    //! Generates the save function for the network that saves the network to the stream.
    void saveAsStream( );

    //! Generates the function that opens an existing data network with the root object.
    void openDN( );
    void openDNFromFile( );
    void openDNFromString( );

    //! Generate the close function that closes the opened data network without updating it.
    void closeDN( );

    //! Generates the ocl evaluation function.
    void checkConstraints( );

    //! Returns the namespace name if it is not null, otherwise the diagram name.
    string namespaceOrDiagramName( );

    //! Returns the Java code snippet that describes the container.
    string containerDescriptionDoc( );

    //! Returns the function name part that combines the diagram name with the namespace name.
    string containerDescriptionFunc( );

  private:
    //! The namespace of this factory.
    const ::Uml::Namespace & m_ns;

    //! The UDM UML diagram.
    const ::Uml::Diagram m_diagram;

    //! Namespace directory.
    const map<string, string>& m_ns_map;

    //! The input UDM XML file.
    string m_inputfile;

    //! The name of the package for the generated Java file.
    const string m_package_name;

    //! The name of the root object in the namespace.
    const string m_root_name;

   //! The name of the diagram.
   const string m_diag_name;

   //! Destination fatcory file.
   ofstream m_output;

   //! The type of the factory.
   const string m_type;

    // The examples test file.
   ofstream & m_example_output;

    // The factory repository file.
   ofstream & m_factory_output;

};
#endif //FactoryGen_H
