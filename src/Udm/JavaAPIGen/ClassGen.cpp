#include "ClassGen.h"

#include "Utils.h"
#include <Uml.h>
#include <UmlExt.h>

#include <cstring>

#include <sstream>
#include <algorithm>
#include <deque>
#include <iterator>

//! Constructor.
/*!
  Creates and initializes the Java API generation.
*/
ClassGen::ClassGen( const ::Uml::Class &cl
      , const string & pckg_name
      , const string & diag_name
      , const string & ns_path )
:   m_cl( cl ), m_package_name(  pckg_name  ), m_diag_name(  diag_name  )
  , m_output(), m_cl_name ( cl.name() )
  , m_ns_path( Utils::toLower( ns_path ) )
  , m_ns_path_orig( ns_path )
  , m_base_name( "UdmPseudoObject" )
{
}


//! Destructor.
/*!
  .
*/
ClassGen::~ClassGen ( void )
{
}
//================================================

//! Entry point of Java API generation.
/*!
  .
*/
void ClassGen::generate()
{
  open( );

  header( );
  constructor( );
  getMetaClass( );

  construction( );
  CG<ofstream> cg(m_cl, m_package_name, m_output, m_ioutput, m_cl_name, m_ns_path);
  cg.accessChildren( );
  cg.accessAttributes( );
  cg.associations( );

  set<Uml::Class> bases = m_cl.baseTypes();
  set<Uml::Class> inheritedBases;

  deque< ::Uml::Class> basesDeque;
  basesDeque.push_back(m_cl);
  while (!basesDeque.empty())
  {
	  Uml::Class base = basesDeque.front();
	  basesDeque.pop_front();
	  std::set<Uml::Class> basesBases = base.baseTypes();
	  std::copy(basesBases.begin(), basesBases.end(), std::back_inserter(basesDeque));
	  std::copy(basesBases.begin(), basesBases.end(), std::inserter(inheritedBases, inheritedBases.begin()));
  }

  if (bases.size())
  {
	  Uml::Class firstBase = *bases.begin();
	  set<Uml::Class> firstBaseAncestors = Uml::AncestorClasses(firstBase);
	  vector<Uml::Class> baseClassesToGenerate;
	  baseClassesToGenerate.resize(inheritedBases.size() + 1);
	  set_difference(inheritedBases.begin(), inheritedBases.end(), firstBaseAncestors.begin(), firstBaseAncestors.end(), baseClassesToGenerate.begin());

	  vector<Uml::Class>::iterator basesIt = baseClassesToGenerate.begin();
	  for (; *basesIt; basesIt++)
	  {
		  // TODO: lowercase this?
		string pck = Utils::getPackageSignature(*basesIt, m_ns_path, m_package_name);
		pck = pck.substr(0, pck.length() - 1);
		pck = Utils::toPackageName(pck);
		string ns_path;
		if (Uml::IsDerivedFrom(basesIt->GetParent().type(), Uml::Namespace::meta)) {
			ns_path = static_cast<Uml::Namespace>(basesIt->parent_ns()).getPath2("::", false);
		}
		std::stringstream ioutput; // The method declarations for the interface will be in a base interface, so we ignore them

		CG<stringstream> cg(*basesIt, pck, m_output, ioutput, basesIt->name(), ns_path);
		m_output << " // Methods for " << basesIt->name() << endl;
		cg.accessChildren();
		cg.accessAttributes();
		cg.associations();
	  }
  }


  close( );
}

//! Creates and opens the JAVA source file.
/*!
  Generates the Java source file for the class.
  Throws udm_exception in case of I/O error.
*/
void ClassGen::open()
{
  m_output.open( (m_package_name + "//" + m_cl_name + ".java").c_str() );

  if ( !m_output.good() )
    throw udm_exception( "Error opening for write " + m_cl_name + ".java" );

  m_ioutput.open( (m_package_name + "//I" + m_cl_name + ".java").c_str() );

  if ( !m_ioutput.good() )
    throw udm_exception( "Error opening for write I" + m_cl_name + ".java" );
}

//! Closes the JAVA source file.
void ClassGen::close()
{
  // close the class
  m_output << "}" << endl;

  // close the file
  m_output.close();
  m_output.clear();

  // close the class
  m_ioutput << "}" << endl;

  // close the file
  m_ioutput.close();
  m_ioutput.clear();
}

//! Generates the header of the source file.
void ClassGen::header( )
{
  // header
  m_output << "/* Generated on " << Utils::getTime( ) << " */" << endl << endl;
  m_output << Utils::getCopyrightText();

  //package
  m_output << "package " << Utils::toPackageName( m_package_name ) << ";" << endl << endl;
  m_ioutput << "package " << Utils::toPackageName( m_package_name ) << ";" << endl << endl;

  // imports
  m_output << "import edu.vanderbilt.isis.udm.*;" << endl << endl;
  m_ioutput << "import edu.vanderbilt.isis.udm.UdmException;" << endl << endl;

  // generate class documentation
  m_output << "/**" << endl;
  m_output << " * Domain specific class of <code>" << m_cl_name << "</code>." << endl;
  m_output << " */ " << endl;
}

//! Generates the constructor for this class.
void ClassGen::constructor( )
{
  findBaseClass( );

  // generate the signature of the class
  m_output << "public ";


  m_output << "class " << m_cl_name << " extends " << m_base_name << " implements I" << m_cl_name << endl;
  m_ioutput << "public interface I" << m_cl_name << " ";
  set< ::Uml::Class> bases = m_cl.baseTypes();
  set< ::Uml::Class>::iterator basesIt = bases.begin();
  for (; basesIt != bases.end(); basesIt++)
  {
	m_ioutput << string((basesIt != bases.begin()) ? ", " : "extends ");
    string pck = Utils::getPackageSignature( *basesIt, m_ns_path, m_package_name );
	m_ioutput << pck + string("I") + (string)basesIt->name();
  }

  m_output << "{" << endl;
  m_ioutput << "{" << endl;

  //////
  // generate meta information
  m_output << "\t// meta information" << endl;

  if ( !m_cl.isAbstract() )
  {
    m_output << "\tpublic static final java.lang.String META_TYPE = \"" << m_cl_name << "\";" << endl;
    m_output << "\tpublic static final java.lang.String META_TYPE_NS = \"" << m_ns_path_orig << "\";" << endl;
    m_ioutput << "\tpublic static final java.lang.String META_TYPE = \"" << m_cl_name << "\";" << endl;
    m_ioutput << "\tpublic static final java.lang.String META_TYPE_NS = \"" << m_ns_path_orig << "\";" << endl;
    m_output << "\tprivate static UdmPseudoObject metaClass;" << endl;
    m_output << endl;
  }

  //////
  //  generate the constructor
  m_output << "\t/**" << endl;
  m_output << "\t * Constructor." << endl;
  m_output << "\t * @param  upo The object that helps the initialization of the instance " << endl;
  m_output << "\t * @param  metaDiagram The diagram of the data network" << endl;
  m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
  m_output << "\t */ " << endl;
  m_output << "\tpublic " << m_cl_name << "(UdmPseudoObject upo, Diagram metaDiagram) \n\t\tthrows UdmException" << endl;
  m_output << "\t{" << endl;
  m_output << "\t\tsuper(upo, metaDiagram);" << endl;
  m_output << "\t}" << endl << endl;
}

//! Find the base class. Note: java does not support multiple inheritance.
void ClassGen::findBaseClass( )
{
  set< ::Uml::Class> bases = m_cl.baseTypes();

  if ( bases.size() )
  {
    string pck = Utils::getPackageSignature( *bases.begin(), m_ns_path, m_package_name );
    m_base_name = pck + (string)bases.begin()->name();
  }
}

//! Generates the function that returns the meta class for non-abstract classes.
void ClassGen::getMetaClass( )
{
  //  generate the function that returns the meta class for non-abstract classes
  if ( !m_cl.isAbstract() )
  {
    m_output << "\t/**" << endl;
    m_output << "\t * Returns the meta class." << endl;
    m_output << "\t * @return  The meta class" << endl;
    m_output << "\t */ " << endl;
    m_output << "\tUdmPseudoObject getMetaClass()" << endl;
    m_output << "\t{" << endl;
    m_output << "\t\treturn metaClass;" << endl;
    m_output << "\t}" << endl << endl;
  }
}

//! Generates the composition roles ( create ).
void ClassGen::construction( )
{
  // abstract classes do not have these functions
  if ( m_cl.isAbstract() )
    return;

  m_output << "\t/* Construction */" << endl << endl;

  // the first non-abstract ancestor
  ::Uml::Class ret_type = Utils::getFirstNonAbstractAncestor( m_cl );
  string ret_type_name = ret_type.name( );

  set< ::Uml::Class> conts = Uml::AncestorContainerClasses( m_cl );
  for ( set< ::Uml::Class>::iterator c_i = conts.begin(); c_i != conts.end(); c_i++ )
  {
    string c_i_name = c_i->name();
    ::Uml::Composition comp = Uml::matchChildToParent( m_cl, *c_i );

    m_output << "\t/**" << endl;
    m_output << "\t * Creates an instance of the class in the container specified by the parameter. " << endl;
    m_output << "\t * @param  parent The parent container" << endl;
    m_output << "\t * @return  An instance of the class <code>" << ret_type_name << "</code>" << endl;
    m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
    m_output << "\t */ " << endl;

    if ( comp )
    {
      // without childroles
      m_output << "\tpublic static " << Utils::getPackageSignature(ret_type, m_ns_path,  m_package_name ) << ret_type_name;
      m_output<< " create("<< Utils::getPackageSignature(*c_i, m_ns_path,  m_package_name ) << c_i_name << " parent) \n\t\tthrows UdmException " << endl;
      m_output << "\t{" << endl;
      m_output << "\t\tDiagram metaDiagram = parent.getDiagram();" << endl;
      m_output << "\t\treturn new " << m_cl_name << "(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);" << endl;
      m_output << "\t}" << endl;
    }
    else
    {
      // with childroles
      set< ::Uml::CompositionChildRole> ccrs = ::Uml::CompositionPeerChildRoles(*c_i);
      for( set< ::Uml::CompositionChildRole>::iterator ccrs_i = ccrs.begin(); ccrs_i != ccrs.end(); ccrs_i++ )
      {
        ::Uml::Class parent = Uml::theOther(*ccrs_i).target();
        ::Uml::Class child = ccrs_i->target();
        string child_name = child.name();
        ::Uml::Composition comp = Uml::matchChildToParent(child, parent);
        if ( !comp )
        {
          //will reach here only if more than one composition is possible
          m_output << "\tpublic static " << Utils::getPackageSignature(ret_type, m_ns_path,  m_package_name ) << ret_type_name;
          m_output << " create" << (string)(ccrs_i->name()) << "(" << Utils::getPackageSignature(*c_i, m_ns_path,  m_package_name ) << c_i_name << " parent)\n\t\tthrows UdmException " << endl;
          m_output << "\t{" << endl;
          m_output << "\t\tDiagram metaDiagram = parent.getDiagram();" << endl;
          m_output << "\t\treturn new " << m_cl_name << "(parent.createObject(META_TYPE, META_TYPE_NS, \"" << (string)(ccrs_i->name()) << "\"), metaDiagram);" << endl;
          m_output << "\t}" << endl;
        }
      }
    }

    m_output << endl;
  }
}

bool isInterfaceNeeded(const ::Uml::Class& klass) {
	// The interface is needed if klass has a subclass that will not inherit from it in the Java code
	set< ::Uml::Class> subtypes = klass.subTypes();
	set< ::Uml::Class>::iterator subtypesIt = subtypes.begin();
	for (; subtypesIt != subtypes.end(); subtypesIt++) {
		set< ::Uml::Class> bases = subtypesIt->baseTypes();
		if (*bases.begin() != klass)
			// Only the first base will be inherited
			return true;
		if (isInterfaceNeeded(*subtypesIt))
			return true;
	}

	return false;
}

string getInterfaceIfNeeded(const Uml::Class& klass) {
	return string(isInterfaceNeeded(klass) ? "I" : "") + string(klass.name());
}


//! Generates containments ( createChild + getXXXChildren ).
template <class OS_I>
void ClassGen::CG<OS_I>::accessChildren( )
{
  m_output << "\t/* Accessing children */" << endl << endl;

  set<Uml::Class> allContainedClasses;

  // set of contained classes
  set< ::Uml::Class> conts = Uml::ContainedClasses( m_cl );
  for ( set< ::Uml::Class>::iterator c_i = conts.begin(); c_i != conts.end(); c_i++ )
  {
    // the descendants of the contained class
    set < ::Uml::Class> conts_der = Uml::DescendantClasses( *c_i );
	std::copy(conts_der.begin(), conts_der.end(), std::inserter(allContainedClasses, allContainedClasses.begin()));
  }

  {
    for ( set< ::Uml::Class>::iterator c_d_i = allContainedClasses.begin(); c_d_i != allContainedClasses.end(); c_d_i++ )
    {
      if ( c_d_i->isAbstract() )
        continue; // FIXME: KMS this looks wrong

      ::Uml::Composition comp = Uml::matchChildToParent( *c_d_i, m_cl );
      string c_i_name = getInterfaceIfNeeded(*c_d_i);
      string pkg_name = Utils::getPackageSignature( *c_d_i, m_ns_path, m_package_name );

      // without child role
      if ( comp )
      {
        ::Uml::CompositionChildRole ccr = comp.childRole();
        string ccr_name = ccr.name();

        if ( (ccr.max() == -1) || (ccr.max() > 1) )
        {
          m_output << "\t/**" << endl;
          m_output << "\t * Returns all the children of type <code>" << c_i_name << "</code> of this container. " << endl;
          m_output << "\t * @return  The children in an array" << endl;
          m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
          m_output << "\t */ " << endl;
          m_output << "\tpublic " << pkg_name << c_i_name << "[] get" << c_i_name << "Children()\n\t\tthrows UdmException " << endl;
		  m_ioutput << "\tpublic " << pkg_name << c_i_name << "[] get" << c_i_name << "Children() throws UdmException;" << endl;
          m_output << "\t{" << endl;

          if ( strcmp(ccr_name.c_str(), "") == 0 )
          {
            m_output << "\t\tUdmPseudoObjectContainer container = getChildren(null, ";
            m_output << pkg_name << c_i_name << ".META_TYPE, " << pkg_name << c_i_name << ".META_TYPE_NS);" << endl;
          }
          else
          {
            m_output << "\t\tUdmPseudoObjectContainer container = getChildren(\"" << ccr_name << "\", " ;
            m_output << pkg_name << c_i_name << ".META_TYPE, " << pkg_name << c_i_name << ".META_TYPE_NS);" << endl;
          }

          m_output << "\t\t" << pkg_name << c_i_name << "[] res = new " << pkg_name << c_i_name << "[container.getLength()];" << endl;
          m_output << "\t\tfor (int i=0; i < container.getLength(); i++) " << endl;
          m_output << "\t\t{" << endl;
          m_output << "\t\t\tres[i] = (" << pkg_name << c_i_name << ")" << pkg_name << "Utils.wrapWithSubclass(container.getAt(i), metaDiagram);" << endl;
          m_output << "\t\t}" << endl;
          m_output << "\t\treturn res;" << endl;
          m_output << "\t}" << endl << endl;
        }
        else
        {
          m_output << "\t/**" << endl;
          m_output << "\t * Return the child of type <code>" << c_i_name << "</code> of this container. " << endl;
          m_output << "\t * @return  The child" << endl;
          m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
          m_output << "\t */ " << endl;
          m_output << "\tpublic " << pkg_name << c_i_name << " get" << c_i_name << "Child()\n\t\tthrows UdmException " << endl;
          m_ioutput << "\tpublic " << pkg_name << c_i_name << " get" << c_i_name << "Child() throws UdmException;" << endl;
          m_output << "\t{" << endl;

          if ( strcmp(ccr_name.c_str(), "") == 0 )
          {
            m_output << "\t\tUdmPseudoObjectContainer container = getChildren(null, " ;
            m_output << pkg_name << c_i_name << ".META_TYPE, " << pkg_name << c_i_name << ".META_TYPE_NS); " << endl;
          }
          else
          {
            m_output << "\t\tUdmPseudoObjectContainer container = getChildren(\"" << ccr_name << "\",";
            m_output << pkg_name << c_i_name << ".META_TYPE, " << pkg_name << c_i_name << ".META_TYPE_NS); " << endl;
          }

          m_output << "\t\tif (container.getLength() > 0)" << endl << endl;
          m_output << "\t\t\treturn (" << pkg_name << c_i_name << ")" << pkg_name << "Utils.wrapWithSubclass(container.getAt(0), metaDiagram);" << endl;
          m_output << "\t\treturn null;" << endl << endl;
          m_output << "\t}" << endl << endl;
        }
      }
      // with child role
      else
      {
        //generate a common getter function for these types of children
        m_output << "\t/**" << endl;
        m_output << "\t * Returns all the children of type <code>" << c_i_name << "</code> of this container. " << endl;
        m_output << "\t * @return  The children in an array" << endl;
        m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
        m_output << "\t */ " << endl;
        m_output << "\tpublic " << pkg_name << c_i_name << "[] get" << c_i_name << "Children()\n\t\tthrows UdmException " << endl;
        m_ioutput << "\tpublic " << pkg_name << c_i_name << "[] get" << c_i_name << "Children() throws UdmException; " << endl;
        m_output << "\t{" << endl;
        m_output << "\t\tUdmPseudoObjectContainer container = getChildren(null," ;
        m_output << pkg_name << c_i_name << ".META_TYPE, " << pkg_name << c_i_name << ".META_TYPE_NS);" << endl;
        m_output << "\t\t" << pkg_name << c_i_name << "[] array = new " << pkg_name << c_i_name << "[container.getLength()];" << endl;
        m_output << "\t\tfor (int i = 0; i < container.getLength(); i++) " << endl;
        m_output << "\t\t{" << endl;
        m_output << "\t\t\tarray[i] = (" << pkg_name << c_i_name << ")" << pkg_name << "Utils.wrapWithSubclass(container.getAt(i), metaDiagram);" << endl;
        m_output << "\t\t}" << endl;
        m_output << "\t\treturn array;" << endl;
        m_output << "\t}" << endl;
        //end of common getter function generation

        set < ::Uml::CompositionChildRole> ccrs = ::Uml::CompositionPeerChildRoles( m_cl );
        for ( set< ::Uml::CompositionChildRole>::iterator ccrs_i = ccrs.begin(); ccrs_i != ccrs.end(); ccrs_i++ )
        {
          ::Uml::Class parent = Uml::theOther(*ccrs_i).target();
          ::Uml::Class child = ccrs_i->target();
		  string child_name = getInterfaceIfNeeded(child);
          pkg_name = Utils::getPackageSignature( child, m_ns_path, m_package_name );
          ::Uml::Composition comp = Uml::matchChildToParent( child, parent );

          if ( !comp )
          {
            //will reach here only if more than one composition is possible

            //string child role name
            string ccr_name = ccrs_i->name();
            m_output << "\t/**" << endl;
            m_output << "\t *  Composition role name <code>" << ccr_name << "</code>." << endl;
            m_output << "\t */" << endl;
            m_output << "\tpublic static final java.lang.String " << Utils::getCCRString(*ccrs_i) << " = \"" << (string)ccrs_i->name() << "\";";
            m_output << endl << endl;

            //getRoleName
            if ( (ccrs_i->max() == -1) || (ccrs_i->max() > 1) )
            {
              m_output << "\t/**" << endl;
              m_output << "\t * Returns the children <code>" << child_name << "</code> of this container. " << endl;
              m_output << "\t * which have role <code>" << ccr_name << "</code>." << endl;
              m_output << "\t * @return  The children in an array" << endl;
              m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
              m_output << "\t */ " << endl;
              m_output << "\tpublic " << pkg_name << child_name << "[] get" << ccr_name
                  << "Children()\n\t\tthrows UdmException " << endl;
              m_ioutput << "\tpublic " << pkg_name << child_name << "[] get" << ccr_name
                  << "Children() throws UdmException; " << endl;
              m_output << "\t{" << endl;
              m_output << "\t\tUdmPseudoObjectContainer container = getChildren(";
              m_output << Utils::getCCRString( *ccrs_i) << "," << pkg_name << child_name << ".META_TYPE, " << pkg_name << child_name << ".META_TYPE_NS);" << endl;
              m_output << "\t\t" << pkg_name << child_name << "[] array = new " << pkg_name << child_name << "[container.getLength()];" << endl;
              m_output << "\t\tfor (int i = 0; i < container.getLength(); i++) " << endl;
              m_output << "\t\t{" << endl;
              m_output << "\t\t\tarray[i] = (" << pkg_name << child_name << ")" << pkg_name << "Utils.wrapWithSubclass(container.getAt(i), metaDiagram);" << endl;
              m_output << "\t\t}" << endl;
              m_output << "\t\treturn array;" << endl;
              m_output << "\t}" << endl;
            }
            else
            {
              m_output << "\t/**" << endl;
              m_output << "\t * Returns the child <code>" << child_name << "</code> of this container. " << endl;
              m_output << "\t * which has role <code>" << ccr_name << "</code>." << endl;
              m_output << "\t * @return  The children in an array" << endl;
              m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
              m_output << "\t */ " << endl;
              m_output << "\tpublic " << pkg_name << child_name << " get" << ccr_name << "Child()\n\t\tthrows UdmException " << endl;
              m_ioutput << "\tpublic " << pkg_name << child_name << " get" << ccr_name << "Child() throws UdmException;" << endl;
              m_output << "\t{" << endl;
              m_output << "\t\tUdmPseudoObjectContainer container = getChildren(" ;
              m_output << Utils::getCCRString(*ccrs_i) << "," << pkg_name << child_name << ".META_TYPE, " << pkg_name << child_name << ".META_TYPE_NS);" << endl;
              m_output << "\t\tif (container.getLength() > 0)" << endl << endl;
              m_output << "\t\t\t return (" << pkg_name << child_name << ")" << pkg_name << "Utils.wrapWithSubclass(container.getAt(0), metaDiagram);" << endl;
              m_output << "\t\treturn null;" << endl << endl;
              m_output << "\t}" << endl;
            }
          } 
        } 
      }
    }
  }
}

//! Generates attribute setters and getters.
template <class OS_I>
void ClassGen::CG<OS_I>::accessAttributes( )
{
  m_output << "\t/* Attribute setters, getters */" << endl << endl;

  // set of attributes
  set < ::Uml::Attribute> atts = m_cl.attributes();
  for ( set< ::Uml::Attribute>::iterator atts_i = atts.begin(); atts_i != atts.end(); atts_i++ )
  {
    string att_name = atts_i->name();

    // name of the attribute
    m_output << "\t/**" << endl;
    m_output << "\t * Attribute for <code>" << att_name << "</code>." << endl;
    m_output << "\t */" << endl;
    m_output << "\tpublic static final java.lang.String " << att_name << " = \"" << att_name << "\";" << endl;
    m_ioutput << "\tpublic static final java.lang.String " << att_name << " = \"" << att_name << "\";" << endl;
    m_output << endl;

    // type of the attribute
    string jtype, upo_call;
    bool   array = ((atts_i->max() > 1) || (atts_i->max() < 0));

    if ( ((string)atts_i->type() == "String") || ((string)atts_i->type() == "Text") )
    {
        if ( array )
        {
          jtype = "java.lang.String []";
          upo_call = "StrValues";
        }
        else
        {
          jtype = "java.lang.String";
          upo_call = "StringVal";
        }
    }
    else if ( (string)atts_i->type() == "Real" )
    {
        if ( array )
        {
          jtype = "double []";
          upo_call = "RealValues";
        }
        else
        {
          jtype = "double";
          upo_call = "RealVal";
        }
    }
    else if ( (string)atts_i->type() == "Boolean" )
    {
        if ( array )
        {
          jtype = "boolean []";
          upo_call = "BoolValues";
        }
        else
        {
          jtype = "boolean";
          upo_call = "BooleanVal";
        }
    }
    else
    {
        if ( array )
        {
          jtype = "long []";
          upo_call = "IntValues";
        }
        else
        {
          jtype = "long";
          upo_call = "IntVal";
        }
    } ;

    // setter
    m_output << "\t/**" << endl;
    m_output << "\t * Sets the value of the attribute <code>" << att_name << "</code> to a value specified by the parameter." << endl;
    m_output << "\t * @param _v The new value of the attribute" << endl;
    m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
    m_output << "\t */ " << endl;
    m_output << "\tpublic void set" << att_name << "(" << jtype << " _v)\n\t\tthrows UdmException " << endl;
    m_ioutput << "\tpublic void set" << att_name << "(" << jtype << " _v) throws UdmException;" << endl;
    m_output << "\t{" << endl;
    m_output << "\t\tset" << upo_call << "(" << att_name << ", _v);" << endl;
    m_output << "\t}" << endl;
    m_output << endl;

    // getter
    m_output << "\t/**" << endl;
    m_output << "\t * Returns the value of the attribute <code>" << att_name << "</code>." << endl;
    m_output << "\t * @return  The value" << endl;
    m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
    m_output << "\t */ " << endl;
    m_output << "\tpublic " << jtype << " get" << att_name << "()\n\t\tthrows UdmException " << endl;
    m_ioutput << "\tpublic " << jtype << " get" << att_name << "() throws UdmException;" << endl;
    m_output << "\t{" << endl;
    m_output << "\t\treturn get" << upo_call << "(" << att_name << ");" << endl;
    m_output << "\t}" << endl;
    m_output << endl;
  } 
}

//! Generate association functions.
template <class OS_I>
void ClassGen::CG<OS_I>::associations( )
{
  m_output << "\t/* Associations */" << endl << endl;

  // association classes
  ::Uml::Association ass = Uml::GetAncestorAssociation( m_cl );

  if ( ass )
  {
    // set of association roles
    set < ::Uml::AssociationRole> ass_roles = ass.roles();
    for( set< ::Uml::AssociationRole>::iterator a_r_i = ass_roles.begin(); a_r_i != ass_roles.end(); a_r_i++ )
    {
      ::Uml::Class target_cl = a_r_i->target();

	  string tname = getInterfaceIfNeeded(target_cl);
      string ar_name = a_r_i->name();

      string pkg_name = Utils::getPackageSignature(target_cl, m_ns_path, m_package_name);

      m_output << "\t/*" << endl;
      m_output << "\t * Association with role name <code>" << ar_name << "</code>." << endl;
      m_output << "\t */" << endl;

      // setter for the association
      m_output << "\t/**" << endl;
      m_output << "\t * Sets the end of the association with role name <code>" << ar_name << "</code>." << endl;
      m_output << "\t * @param a The end of the association" << endl;
      m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
      m_output << "\t */ " << endl;
      m_output << "\tpublic void set" << ar_name << "(" << pkg_name << tname << " a)\n\t\tthrows UdmException" << endl;
      m_ioutput << "\tpublic void set" << ar_name << "(" << pkg_name << tname << " a) throws UdmException;" << endl;
      m_output << "\t{" << endl;
      m_output << "\t\tUdmPseudoObjectContainer container = new UdmPseudoObjectContainer(1);" << endl;
      m_output << "\t\tcontainer.setAt(0, (UdmPseudoObject)a);" << endl;
      m_output << "\t\tsetAssociation(\"" << ar_name << "\", container, UdmHelper.TARGET_FROM_CLASS);" << endl;
      m_output << "\t}" << endl;
      m_output << endl;

      // getter for the association
      m_output << "\t/**" << endl;
      m_output << "\t * Returns the end of the association with role name <code>" << ar_name << "</code>." << endl;
      m_output << "\t * @return The end of the association" << endl;
      m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
      m_output << "\t */ " << endl;
      m_output << "\tpublic " << pkg_name << tname << " get" << ar_name << "()\n\t\tthrows UdmException" << endl;
      m_ioutput << "\tpublic " << pkg_name << tname << " get" << ar_name << "() throws UdmException;" << endl;
      m_output << "\t{" << endl;
      m_output << "\t\tUdmPseudoObjectContainer container = getAssociation(\"" << ar_name << "\", UdmHelper.TARGET_FROM_CLASS);" << endl;
      m_output << "\t\tif (container.getLength() > 0)" << endl << endl;
      m_output << "\t\t\treturn (" << pkg_name << tname << ") " << pkg_name << "Utils.wrapWithSubclass(container.getAt(0), metaDiagram);" << endl;
      m_output << "\t\treturn null;" << endl << endl;
      m_output << "\t}" << endl;
      m_output << endl;
    } 
  }

  // associations
  // set of associations
  set < ::Uml::AssociationRole> assroles = m_cl.associationRoles();

  if ( assroles.size() )
    {
    for( set< ::Uml::AssociationRole>::iterator ar_i = assroles.begin(); ar_i != assroles.end(); ar_i++ )
    {
      string asr_name = ar_i->name();
      string to_asr_name = (string)Uml::theOther(*ar_i).name();

      ::Uml::Class to_class = (::Uml::Class)((Uml::theOther(*ar_i)).target());
      string helper_mode = "TARGET_FROM_PEER";
      ::Uml::Association ass = ar_i->parent();
      ::Uml::Class ass_class = ass.assocClass();

      if ( ass_class )
      {
          to_class = ass_class;
          helper_mode = "CLASS_FROM_TARGET";
      }

      string tname = getInterfaceIfNeeded(to_class);

      m_output << "\t/*" << endl;
      m_output << "\t * Association with role name <code>" << to_asr_name << "</code>." << endl;
      m_output << "\t */" << endl;
      m_output << endl;

      // the canonical form of the to_class
      string pckg_signature = Utils::getPackageSignature(to_class, m_ns_path, m_package_name);

	  if ( (Uml::theOther(*ar_i).max() == 0) || (Uml::theOther(*ar_i).max() == 1) )
      {
        //single cardinality
        m_output << "\t/**" << endl;
        m_output << "\t * Sets the other end of the association with role name <code>" << to_asr_name << "</code>." << endl;
        m_output << "\t * @param a The other end of the association" << endl;
        m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
        m_output << "\t */ " << endl;
        m_output << "\tpublic void set" << to_asr_name << "(" << pckg_signature << tname << " a)\n\t\tthrows UdmException" << endl;
        m_ioutput << "\tpublic void set" << to_asr_name << "(" << pckg_signature << tname << " a) throws UdmException;" << endl;
        m_output << "\t{" << endl;
		m_output << "\t\tsetAssociation(\"" << to_asr_name << "\", a, UdmHelper." << helper_mode << ");" << endl;
        m_output << "\t}" << endl;
        m_output << endl;

        m_output << "\t/**" << endl;
        m_output << "\t * Returns the other end of the association with role name <code>" << to_asr_name << "</code>." << endl;
        m_output << "\t * @return The other end of the association" << endl;
        m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
        m_output << "\t */ " << endl;
        m_output << "\tpublic " << pckg_signature << tname << " get" << to_asr_name << "()\n\t\tthrows UdmException" << endl;
        m_ioutput << "\tpublic " << pckg_signature << tname << " get" << to_asr_name << "() throws UdmException;" << endl;
        m_output << "\t{" << endl;
        m_output << "\t\tUdmPseudoObject result = getSingleAssociatedObject(\"" << to_asr_name << "\", UdmHelper." << helper_mode << ");" << endl;
		if (isInterfaceNeeded(to_class)) {
			m_output << "\t\treturn (result == null) ? null : (" << pckg_signature << tname << ")Utils.wrapWithSubclass(new UdmPseudoObject(result, getDiagram()), getDiagram());" << endl;
		} else {
	        m_output << "\t\treturn (result == null) ? null : new " << pckg_signature << tname << "(result, getDiagram());" << endl;
		}
        m_output << "\t}" << endl;
        m_output << endl;
      }
      else
      {
        m_output << "\t/**" << endl;
        m_output << "\t * Sets the other ends of the association with role name <code>" << to_asr_name << "</code>." << endl;
        m_output << "\t * @param a The other ends of the association" << endl;
        m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
        m_output << "\t */ " << endl;
        m_output << "\tpublic void set" << to_asr_name << "(" << pckg_signature << tname << "[] a)\n\t\tthrows UdmException" << endl;
        m_ioutput << "\tpublic void set" << to_asr_name << "(" << pckg_signature << tname << "[] a) throws UdmException;" << endl;
        m_output << "\t{" << endl;
        m_output << "\t\tsetAssociation(\"" << to_asr_name << "\", new UdmPseudoObjectContainer(a), UdmHelper." << helper_mode << ");" << endl;
        m_output << "\t}" << endl;
        m_output << endl;

        m_output << "\t/**" << endl;
        m_output << "\t * Returns the other ends of the association with role name <code>" << to_asr_name << "</code>." << endl;
        m_output << "\t * @return The other ends of the association" << endl;
        m_output << "\t * @throws  UdmException If any Udm related exception occurred" << endl;
        m_output << "\t */ " << endl;
        m_output << "\tpublic " << pckg_signature << tname << "[] get" << to_asr_name << "()\n\t\tthrows UdmException" << endl;
        m_ioutput << "\tpublic " << pckg_signature << tname << "[] get" << to_asr_name << "() throws UdmException;" << endl;
        m_output << "\t{" << endl;
        m_output << "\t\tUdmPseudoObjectContainer objs = getAssociation(\"" << to_asr_name << "\", UdmHelper." << helper_mode << ");" << endl;
        m_output << "\t\treturn (" << pckg_signature << tname << "[]) " << pckg_signature << "Utils.wrapWithSubclass(objs, " << pckg_signature << tname << ".class, getDiagram());" << endl;
        m_output << "\t}" << endl;
        m_output << endl;
      }
    } 
  }
}
