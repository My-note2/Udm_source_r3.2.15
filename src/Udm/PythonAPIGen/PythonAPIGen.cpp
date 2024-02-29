#include "../Udm.h"
#include "PythonAPIGen.h"

#include <iterator>


PythonInheritanceSolver::PythonInheritanceSolver(const Uml::Diagram &diagram, const ::UdmCPPGen::UdmGen& gen, bool sort_by_namespace) : ::UdmCPPGen::InheritanceSolver(diagram, gen, sort_by_namespace) {};
string PythonInheritanceSolver::getAncestorList(const ::Uml::Class &cl ) const
{
    string ret;
	typedef set< ::Uml::Class, ::UdmCPPGen::name_less<Uml::Class> > SortedClasses;
	::UdmCPPGen::name_less< ::Uml::Class> sort;
	SortedClasses bases = cl.baseTypes_sorted< ::UdmCPPGen::name_less<Uml::Class> >(sort);
	if( bases.size() == 0)
        ret += "(UdmPython)";
	else
	{
        ret += "(";
		string sep = "";
		SortedClasses::iterator h;
		for(h = bases.begin(); h != bases.end(); h++)
		{
			if(*h != cl)
			{
				ret += sep;
				ret += h->name();
				sep = ", ";
			}
		}
        ret += ")";
	}
    
	return ret;

}

void GeneratePythonClass(const ::Uml::Class &cl, const string & pckg_hierarcy, const string& diagName);

//! Constructor.
/*!
  Creates and initializes the Python API generation.
*/
PythonAPIGen::PythonAPIGen( const ::Uml::Diagram &diagram
                       , const map<string, string>& ns_map
                       , const string& inputfile )
:   m_diagram( diagram ), m_ns_map( ns_map ), m_inputfile( inputfile )
{
}

//! Destructor.
/*!
  .
*/
PythonAPIGen::~PythonAPIGen ( void )
{
}
//================================================

//! Entry point of Python API generation.
/*!
  .
*/
void PythonAPIGen::generate()
{
  
  // Generate the Python version of the input UDM XML file
  //Utils::XML2Python( string( m_diagram.name() ) + "_udm_xml", m_inputfile, m_meta_dir );

 
   open();
	
	

  /*CREATE MODUL DESCRIPTION*/
  
  m_output << "\"\"\"Generated" << endl;
  //...................................
  m_output << "\"\"\"" << endl << endl;

  /*CREATE IMPORT*/
  m_output << "import sys" << endl;
  m_output << "import udm" << endl;
  m_output << "import Uml" << endl;
  m_output << "from UdmPython import *" << endl << endl;


    /*GENERATE PYTHON CLASSES*/
    set< ::Uml::Class> uml_classes = m_diagram.classes();
    UdmOpts opts;
    ::UdmCPPGen::UdmGen gen(m_diagram, opts);
    PythonInheritanceSolver is(m_diagram, gen);

    for ( vector < ::Uml::Class>::iterator gio_i = is.good_inheritence_order.begin(); gio_i != is.good_inheritence_order.end(); gio_i++)
        generateClass(*gio_i,is );
    
   

  /*INITIALIZE THE META STATIC VARIABLES*/
  
  
    m_output << endl;
    m_output << "def init_classes(diagram):" << endl;
    for( set< ::Uml::Class>::iterator uc_i = uml_classes.begin(); uc_i != uml_classes.end(); uc_i++ )
    {
        m_output << "\t" << uc_i->name() << ".Meta = GetUmlClassByName(diagram,\"" << uc_i->name()   << "\")"<< endl;
    }

    m_output << endl;
    m_output << "def init_attributes():" << endl;
    for( set< ::Uml::Class>::iterator uc_i = uml_classes.begin(); uc_i != uml_classes.end(); uc_i++ )
    {
        set< ::Uml::Attribute> attrs = uc_i->attributes();
        for( set< ::Uml::Attribute>::iterator attrs_i = attrs.begin(); attrs_i != attrs.end(); attrs_i++)
                m_output << "\t" << uc_i->name() << ".meta_" << attrs_i->name() << " = GetUmlAttributeByName(" << uc_i->name() << ".Meta, \"" << attrs_i->name() << "\")"<< endl;
    }
    m_output << "\tpass" << endl;
    m_output << endl;
    m_output << "def init_childroles():" << endl;
    for( set< ::Uml::Class>::iterator uc_i = uml_classes.begin(); uc_i != uml_classes.end(); uc_i++ )
    {
        ::Uml::Class c = *uc_i;
        string cl_name = c.name();
        
        set< ::Uml::Class> childrenkinds;
        set< ::Uml::CompositionParentRole> children = c.parentRoles();
        
        for( set< ::Uml::CompositionParentRole>::iterator i = children.begin(); i != children.end(); i++)
        {
            ::Uml::CompositionChildRole the_other = Uml::theOther(*i);
            
            ::Uml::Class thischildkind = (::Uml::Class)the_other.target();
            //childrenkinds.insert(thischildkind);
            
            string rel_name = ::Uml::MakeRoleName(the_other);
            
            m_output << "\t" << cl_name << ".meta_" << rel_name << " = GetUmlChildRoleByTypesAndRolenames(" << thischildkind.name() << ".Meta," << cl_name << ".Meta, \"" << the_other.name() <<  "\", \"" << i->name() << "\")" << endl;

        }
    }
    m_output << "\tpass" << endl;
    m_output << endl;
    m_output << "def init_parentroles():" << endl;
    for( set< ::Uml::Class>::iterator uc_i = uml_classes.begin(); uc_i != uml_classes.end(); uc_i++ )
    {
        ::Uml::Class c = *uc_i;
        string cl_name = c.name();
        
        set< ::Uml::Class> parentkinds;
        set< ::Uml::CompositionChildRole> parents = c.childRoles();
        
        for( set< ::Uml::CompositionChildRole>::iterator i = parents.begin(); i != parents.end(); i++)
        {
            ::Uml::CompositionParentRole the_other = Uml::theOther(*i);
            
            ::Uml::Class thisparentkind = (::Uml::Class)the_other.target();
            //parentkinds.insert(thischildkind);
            
            string rel_name = ::Uml::MakeRoleName(the_other);
            
            m_output << "\t" << cl_name << ".meta_" << rel_name << " = GetUmlParentRoleByTypesAndRolenames(" << cl_name << ".Meta," << thisparentkind.name() << ".Meta, \"" << i->name() <<  "\", \"" << the_other.name() << "\")" << endl;
        }
        
    }
    
    m_output << "\tpass" << endl;
    m_output << endl;
    m_output << "def init_peer_associationroles():" << endl;
    for( set< ::Uml::Class>::iterator uc_i = uml_classes.begin(); uc_i != uml_classes.end(); uc_i++ )
    {
        ::Uml::Class c = *uc_i;
        string cl_name = c.name();
        set< ::Uml::AssociationRole> assocs = c.associationRoles();
        for( set< ::Uml::AssociationRole>::iterator i = assocs.begin(); i != assocs.end(); i++) 
	    {
	        ::Uml::Association pp = (*i).parent();
            ::Uml::Class aclass = pp.assocClass();
            ::Uml::AssociationRole the_other = Uml::theOther(*i);
            string rel_name = ::Uml::MakeRoleName(the_other);
            ::Uml::Class oclass = the_other.target();
            if (!aclass)
                m_output << "\t" << cl_name << ".meta_" << rel_name << " = GetUmlAssocRoleByPeerAndRole(" << cl_name << ".Meta," << oclass.name() << ".Meta, \"" << i->name() << "\")" << endl;
            else
                m_output << "\t" << cl_name << ".meta_" << rel_name << " = GetUmlAssocRoleByAClassAndRole(" << cl_name << ".Meta,"  << aclass.name() << ".Meta,\"" << i->name() << "\")" << endl;
            
        }
    
    }
    m_output << "\tpass" << endl;
    m_output << endl;
    m_output << "def init_class_associationroles():" << endl;
    for( set< ::Uml::Class>::iterator uc_i = uml_classes.begin(); uc_i != uml_classes.end(); uc_i++ )
    {
        ::Uml::Class c = *uc_i;
        string cl_name = c.name();
        ::Uml::Association assoc = c.association();
        if (assoc)
        {
            set< ::Uml::AssociationRole> assocs = assoc.roles();
            for( set< ::Uml::AssociationRole>::iterator i = assocs.begin(); i != assocs.end(); i++)
            {
                string rel_name = ::Uml::MakeRoleName(*i);
                UDM_ASSERT(rel_name.size() > 0);
                
                ::Uml::AssociationRole the_other = ::Uml::theOther(*i);
                string orel_name = ::Uml::MakeRoleName(the_other);
                string otclass_name = ((::Uml::Class)(the_other.target())).name();
                
                ::Uml::Class tclass = (::Uml::Class)(*i).target();
                string tclass_name = tclass.name();
                
				m_output << "\t" << cl_name << ".meta_" << orel_name << "_end_ = " << otclass_name << ".meta_" << rel_name << "_rev = " << tclass_name << ".meta_" << orel_name << endl;
            }
        }
    }
    
    
		
  m_output << "\tpass" << endl;
  m_output << endl << endl;

  /*INITIALIZE THE META STATIC VARIABLE*/

    m_output << "def initialize(diagram):" << endl;
    m_output << "\t" << "try:" << endl;
    m_output << "\t\t" << "module_initialized" << endl;
    m_output << "\t" << "except NameError:" << endl;
    m_output << "\t\t" << "if sys.modules[__name__] != Uml:" << endl;
    m_output << "\t\t\t" << "Uml.initialize(udm.uml_diagram())" << endl;
    m_output << "\t\t" << "if sys.modules[__name__] == Uml:" << endl;
    m_output << "\t\t\t" << "meta_map = udm.map_uml_names(diagram)" << endl;
    m_output << "\t\t\t" << "#the only meta-object needed to bootstrap" << endl;
    m_output << "\t\t\t" << "Class.Meta = meta_map.Class" << endl;
    m_output << "\t\t" << "init_classes(Uml.Diagram(diagram))" << endl;
    m_output << "\t\t" << "init_childroles()" << endl;
    m_output << "\t\t" << "init_parentroles()" << endl;
    m_output << "\t\t" << "init_attributes()" << endl;
    m_output << "\t\t" << "init_peer_associationroles()" << endl;
    m_output << "\t\t" << "init_class_associationroles()" << endl;
    m_output << "\t\t" << "module_initialized = True" << endl;
    m_output << endl;
	

  
  // close the file
  m_output.close();

}

void PythonAPIGen::open()
{
	//opening ofstream
	string fname = string(m_diagram.name());
	fname+=+".py";
	m_output.open(fname.c_str());

	if ( !m_output.good() )
		throw udm_exception( "Error opening for write "+fname );
}

void PythonAPIGen::generateClass(::Uml::Class &cls, const PythonInheritanceSolver &is)
{

	m_output << "class " << cls.name() << is.getAncestorList(cls) <<  ":" << endl;
   	m_output << "\t\"\"\"Generated\"\"\"" << endl;
 

	generateAttributes(cls);
	
	//cast method
	m_output << "\t@staticmethod" << endl;
	m_output << "\tdef cast(obj):"<< endl;
	m_output << "\t\t\"\"\"Object cast\"\"\"" << endl;
	m_output << "\t\treturn " << cls.name() << "(obj)" << endl;
	m_output << "\t" << endl;

	
	generateChildrenAccess(cls);
	generateAssociations(cls);
    generateParentAccess(cls);
}

void PythonAPIGen::generateAttributes(::Uml::Class &cls)
{
	
	set < ::Uml::Attribute> atts = cls.attributes();
	for ( set< ::Uml::Attribute>::iterator atts_i = atts.begin(); atts_i != atts.end(); atts_i++ )
	{
		string att_name = atts_i->name();
        if ((atts_i->max() > 1) || (atts_i->max() == -1) )
		{
            m_output << "\tdef " << att_name << "(self):" << endl;
            
            if (atts_i->type() == "String" )
                m_output << "\t\t return StrArrayAttr(self, " << cls.name() << ".meta_" <<  atts_i->name() << ")"<< endl;
            if (atts_i->type() == "Real" )
                m_output << "\t\t return RealArrayAttr(self, " << cls.name() << ".meta_" <<  atts_i->name() <<")"<<  endl;
            if (atts_i->type() == "Boolean" )
                m_output << "\t\t return BooleanArrayAttr(self, " << cls.name() << ".meta_" <<  atts_i->name() <<")"<<  endl;
            if (atts_i->type() == "Integer" )
                m_output << "\t\t return IntArrayAttr(self, " << cls.name() << ".meta_" <<  atts_i->name() << ")"<< endl;
            m_output << endl;
        }
	}
}

void PythonAPIGen::generateParentAccess(::Uml::Class &c)
{
    string cl_name = c.name();
    set <  ::Uml::CompositionChildRole> parents = c.childRoles();
    for( set< ::Uml::CompositionChildRole>::iterator i = parents.begin(); i != parents.end(); i++)
	{
		::Uml::CompositionParentRole the_other = Uml::theOther(*i);
		::Uml::Class parent = the_other.target();
        
		string rel_name = ::Uml::MakeRoleName(the_other);
		string parent_name = UmlClassCPPName(parent);
        
        if (the_other.isNavigable())
		{
            m_output << "\tdef " << rel_name << "(self):" <<endl;
            m_output << "\t\t# return parent contained via rolename: " << the_other.name() << ", rel_name: " << rel_name << endl;
            m_output << "\t\tparent = self._get_parent( "<< cl_name << ".meta_" << rel_name <<")" << endl;
            m_output << "\t\t" << "return globals()[parent._type().name](parent)" << endl;
            
        }
        bool parent_defined = false;
        set< ::Uml::CompositionParentRole> parroles = Uml::AncestorCompositionPeerParentRoles(c);
        for( set< ::Uml::CompositionParentRole>::iterator ri = parroles.begin(); ri != parroles.end(); ri++)
        {
            
            if(string(ri->name()) == "parent")
            {
                parent_defined = true;
                break;
            }
        }
        
        if (!parent_defined)
        {
            m_output << "\tdef parent(self):" <<endl;
            m_output << "\t\t# return generic parent  "  << endl;
            m_output << "\t\tparent = self._get_parent()" << endl;
            m_output << "\t\t" << "return globals()[parent._type().name](parent)" << endl;
        }
    }
    
}

void PythonAPIGen::generateChildrenAccess(::Uml::Class &cls)
{
    
    string cl_name = cls.name();
    
	set< ::Uml::Class> childrenkinds;
	set< ::Uml::CompositionParentRole> children = cls.parentRoles();
    
    for( set< ::Uml::CompositionParentRole>::iterator i = children.begin(); i != children.end(); i++)
	{
        ::Uml::CompositionChildRole the_other = Uml::theOther(*i);
        
		::Uml::Class thischildkind = (::Uml::Class)the_other.target();
		childrenkinds.insert(thischildkind);
        
		string rel_name = ::Uml::MakeRoleName(the_other);
		
        if (the_other.isNavigable())
        {
            m_output << "\tdef " << rel_name << "(self):" <<endl;
            
            if(the_other.max() == 1)
            {
                m_output << "\t\t# return the single child contained via rolename: " << the_other.name() << ", rel_name: " << rel_name << endl;
                m_output << "\t\tchilds = self._get_children(child_role=" << cl_name << ".meta_" << rel_name <<")" << endl;
                m_output << "\t\t" << "if len(childs) > 0:" << endl;
                m_output << "\t\t\t" << "return globals()[childs[0]._type().name](childs[0])" << endl;
                m_output << "\t\t" << "else: " << endl;
                m_output << "\t\t\t" << "return None" << endl;
                
            }
            else
            {
                m_output << "\t\t# return the children contained via rolename: " << the_other.name() << ", rel_name: " << rel_name << endl;
                m_output << "\t\tchilds = self._get_children(child_role=" << cl_name << ".meta_" << rel_name <<")" << endl;
                m_output << "\t\tlist = []" << endl;
                m_output << "\t\t" << "for i in childs:" << endl;
                m_output << "\t\t\t" << "list.append(globals()[i._type().name](i))"  << endl;
                m_output << "\t\treturn list" << endl;
                
            }
        }
        else m_output << "\t# access method for non-navigable children " << rel_name << " omitted";
    }
    
    
    ::Uml::Diagram dgr = ::Uml::GetDiagram(cls);
	::Uml::DiagramClasses allclasses = ::Uml::DiagramClasses(dgr);
    
	for (::Uml::DiagramClasses::iterator j = allclasses.begin(); j != allclasses.end(); j++)
	{
		for (set< ::Uml::Class>::iterator k = childrenkinds.begin(); k != childrenkinds.end(); k++)
		{
			if (Uml::IsDerivedFrom(*j, *k) || Uml::IsDerivedFrom(*k, *j))
			{
				string kind_children_name = j->name();
				
                
                m_output << "\tdef " << kind_children_name << "_kind_children(self):" << endl;
                m_output << "\t\t# return the children of type: "  << kind_children_name << endl;
                m_output << "\t\tchilds = self._get_children(child_type=" << kind_children_name << ".Meta)" << endl;
                m_output << "\t\tlist = []" << endl;
                m_output << "\t\t" << "for i in childs:" << endl;
                m_output << "\t\t\t" << "list.append(globals()[i._type().name](i))"  << endl;
                m_output << "\t\treturn list" << endl;


				break;
			}
		}
	}
    
     //childrens end ----------------------------------------------------------------
}

void PythonAPIGen::generateAssociations(::Uml::Class &c)
{
    string cl_name = c.name();
	set< ::Uml::AssociationRole> assocs = c.associationRoles();
    
    
	for( set< ::Uml::AssociationRole>::iterator i = assocs.begin(); i != assocs.end(); i++)
	{
		::Uml::Association pp = (*i).parent();
		::Uml::Class aclass = pp.assocClass();
        
		::Uml::AssociationRole the_other = Uml::theOther(*i);
        
		string rel_name = ::Uml::MakeRoleName(the_other);
		::Uml::Class oclass = the_other.target();
        if (the_other.isNavigable())
        {
            m_output << "\tdef " << rel_name << "(self):" << endl;
            int max = the_other.max();
            m_output << "\t\t#return the " << ( max == 1 ?"only peer" : "peers") << " via role_name: " << i->name() << endl;
            m_output << "\t\tpeers = self._get_associations(" << cl_name << ".meta_" << rel_name << ", udm." << (aclass ? "CLASSFROMTARGET" : "TARGETFROMPEER") << ")" << endl;
            
            
            if (max != 1)
            {
                m_output << "\t\tlist=[]" << endl;
                m_output << "\t\t" << "for i in peers:" << endl;
                m_output << "\t\t\t" << "list.append(globals()[i._type().name](i))"  << endl;
                m_output << "\t\treturn list" << endl;
                    
            }
            else
            {
                
                m_output << "\t\tif len(peers) > 0:" << endl;
                m_output << "\t\t\t" << "return globals()[peers[0]._type().name](peers[0])"  << endl;
                m_output << "\t\telse:" << endl;
                m_output << "\t\t\t" << "return None" << endl;
            
            }
        }
        else m_output << "\t# access method for non-navigable associationrole " << rel_name << " omitted";
        
        
    }
    
    ::Uml::Association assoc = c.association();
	if(assoc)
	{
		set< ::Uml::AssociationRole> assocs = assoc.roles();
        for( set< ::Uml::AssociationRole>::iterator i = assocs.begin(); i != assocs.end(); i++)
		{
			string rel_name = ::Uml::MakeRoleName(*i);
			UDM_ASSERT(rel_name.size() > 0);
            
			::Uml::AssociationRole the_other = ::Uml::theOther(*i);
			string orel_name = ::Uml::MakeRoleName(the_other);
			string otclass_cpp_name = UmlClassCPPName(the_other.target());
            
			::Uml::Class tclass = (::Uml::Class)(*i).target();
			string tclass_cpp_name = UmlClassCPPName(tclass);
            
            m_output << "\tdef " << rel_name << "_end(self):" << endl;
            m_output << "\t\t#return the assoc class end via role name:" << i->name() << endl;
            m_output << "\t\tpeers = self._get_associations(" << cl_name << ".meta_" << rel_name << "_end_ ,udm.TARGETFROMCLASS)" << endl;
            m_output << "\t\tif len(peers) > 0:" << endl;
            m_output << "\t\t\t" << "return globals()[peers[0]._type().name](peers[0])"  << endl;
            m_output << "\t\telse:" << endl;
            m_output << "\t\t\t" << "return None" << endl;

            
        }
        
    }
}

