#ifndef PythonAPIGen_H
#define PythonAPIGen_H

#include <Uml.h>
#include <UmlExt.h>
#include <fstream>

using namespace std;

class PythonAPIGen;

class PythonInheritanceSolver : public ::UdmCPPGen::InheritanceSolver
{
    friend class PythonAPIGen;
public:
    PythonInheritanceSolver(const ::Uml::Diagram &diagram, const ::UdmCPPGen::UdmGen& gen, bool sort_by_namespace = false);
    virtual string getAncestorList(const ::Uml::Class &cl) const;
};
class PythonAPIGen
{
  public:
    //! Constructor.
    PythonAPIGen( const ::Uml::Diagram &diagram
      , const map<string, string>& ns_map
      , const string& inputfile );

    //! Destructor.
    ~PythonAPIGen( void );
    
    //! Entry point of Python API generation.
    void  generate( );

  private:  
    void open();
	void generateClass(::Uml::Class &cls, const PythonInheritanceSolver &is);
	void generateAttributes(::Uml::Class &cls);
	void generateChildrenAccess(::Uml::Class &cls);
	void generateAssociations(::Uml::Class &cls);
    void generateParentAccess(::Uml::Class &cls);
  private:
    //! The UDM UML diagram.
    const ::Uml::Diagram & m_diagram;

    //! Namespace directory.
    const map<string, string>& m_ns_map;

    //! The input UDM XML file.
    const string & m_inputfile;

    ofstream m_output;



};
#endif //PythonAPIGen_H
