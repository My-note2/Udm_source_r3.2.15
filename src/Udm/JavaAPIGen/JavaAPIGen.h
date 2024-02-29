#ifndef JavaAPIGen_H
#define JavaAPIGen_H

#include <Uml.h>
#include <UmlExt.h>
#include <fstream>

using namespace std;

class JavaAPIGen
{
  public:
    //! Constructor.
    JavaAPIGen( const ::Uml::Diagram &diagram
      , const map<string, string>& ns_map
      , const string& inputfile );

    //! Destructor.
    ~JavaAPIGen( void );
    
    //! Entry point of Java API generation.
    void  generate( );

  private:  
    //! Create directory structure for Java API generation.
    void  createDirs( ) const;

    //! Generate package.html information file for javadoc.
    void  generatePackageInfo( ) const;

    //! Generate example test file.
    void  openExample( );
    void  closeExample( );

    //! Generate facotry repository file.
    void  openFactoryRepository( );
    void  closeFactoryRepository( );

    //! Generate the JAVA API source code.
    void generateJava( const ::Uml::Diagram &diagram
      , const map<string, string> & ns_map
      , const string& inputfile) ;

  private:
    //! The UDM UML diagram.
    const ::Uml::Diagram & m_diagram;

    //! Namespace directory.
    const map<string, string>& m_ns_map;

    //! The input UDM XML file.
    const string & m_inputfile;

    // The examples test file.
   ofstream m_example_output;

    // The factory repository file.
   ofstream m_factory_output;

   //! The name of the diagram.
   const string m_diag_name;

   //! Directory structure.
   const string m_example_dir;
   const string m_edu_dir;
   const string m_vu_dir;
   const string m_isis_dir;
   const string m_meta_dir;
   const string m_diag_dir;

};
#endif //JavaAPIGen_H
