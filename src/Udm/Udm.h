/*
Copyright (c) Vanderbilt University, 2000-2001
ALL RIGHTS RESERVED
Vanderbilt University disclaims all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall Vanderbilt University be liable for
any special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.
*/

#ifndef MOBIES_UDM_H
#define MOBIES_UDM_H

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <UdmDom.h>
#include <UmlExt.h>

#include <unordered_set>


using namespace std;

struct UdmOpts {
	// output mode
	enum Mode {
		CXX_DEFAULT,		// C++
		CXX_GENERIC,		// C++ with support for generic programming
		C_SHARP,
		JAVA,
		PYTHON
	} mode;

	enum MetaInitType {
		STATIC_INIT,		// static meta 
		DYNAMIC_INIT,		// dynamic meta (default)
		CORBA_INIT		// CORBA meta
	} meta_init;


	// C++ options
	// 
	string macro;

	// support for visitor pattern in C++
	bool visitor_sup;

	// How to group classes and namespaces
	enum CxxSourceUnit {
		UNIT_DIAGRAM,	// all classes from a diagram and its sub-namespaces into a single .cpp
		UNIT_NAMESPACE, // all classes from a namespace into a single .cpp
		UNIT_CLASS,	// a .cpp file per class
	} cxx_source_unit;

	// DTD and XSD options
	// Generate DTDs
	bool generate_dtd;

	// Use XSD inheritance features when generating XSDs
	bool uxsdi;

	// Generate XSD elements like they would have text attributes
	bool xsd_el_ta;

	// Integrate the XSD into the generated API
	bool integrate_xsd;


	map<string, string> ns_map;
	set<string> ns_ignore;
	set<string> ns_qualified_attrs;

	// Record time when code was generated
	bool rec_tstamp;

	// Output directory
	string out_dir;
};

// not cross-link approved
void GenerateCSInit(const ::Uml::Diagram &diagram, ostream &output, string fname);
void GenerateNewCSInit(const ::Uml::Diagram &diagram, ostream &output, string fname);
void GenerateCSApi(const ::Uml::Diagram &diagram, ostream &output, string fname);
void GenerateCS(const ::Uml::Diagram &diagram,  ostream &output, string fname);
void GenerateNewCS(const ::Uml::Diagram &diagram,  ostream &output, string fname);
void CheckDiagram(const ::Uml::Diagram & diagram);

// utils
std::string GetTime();
std::string UmlClassCPPName(const ::Uml::Class &cl);
std::string UmlClassCPPName(const ::Uml::Class &cl, const ::Uml::Namespace &ns);
std::string NameToFilename(const std::string src);
std::string join(const std::string &sep, const vector<string> &v);
//generate DTD (only if asked) and XSD
void GenerateDSD(const ::Uml::Namespace &ns, const UdmOpts &opts, bool qualified_attrs_ns);
void GenerateDSD(const ::Uml::Diagram &dgr, const string &fname, const UdmOpts &opts, bool qualified_attrs_ns);


namespace UdmCPPGen
{
    
    template<class T>
    struct name_less : binary_function <T, T, bool> {
        bool operator() (const T& x, const T& y) const {
            return static_cast<const std::string>(x.name()) < static_cast<const std::string>(y.name());
        }
    };

void OutFmts(ostream &out, const string &idt, const vector<boost::format> &v, bool semicolon = true);
void OutVecFmts(ostream &out, const string &idt, const vector< vector<boost::format> > &v, const string &label = "");

class DiagramGen;
class UdmGen;

class InheritanceSolver {
	typedef map< ::Uml::Class, set< ::Uml::Class> > cltoclsmap;
	typedef map< ::Uml::Class, bool > cltoboolmap;

	cltoclsmap virtualchildren;
	cltoclsmap explicitinits;
	cltoboolmap objectneeded_map;
	set< ::Uml::Class> virtualbaseclasses;
	
	struct sortByActiveNamespace : public binary_function< ::Uml::Class, ::Uml::Class, bool>
	{
	private:
		::Uml::Namespace active_ns;
	public:
		sortByActiveNamespace(const ::Uml::Namespace &ns);
		bool operator()(const ::Uml::Class &a, const ::Uml::Class &b) const;
	};

    protected:
    vector< ::Uml::Class> good_inheritence_order;

	friend class DiagramGen;
	const UdmGen& gen;
    
public:
	InheritanceSolver(const ::Uml::Diagram &diagram, const UdmGen& gen, bool sort_by_namespace = false);
	virtual string getAncestorList(const ::Uml::Class &cl) const;
	string getInitializers(const ::Uml::Class &cl, const string &argument) const;
};


class UdmGen : boost::noncopyable {
	// maximum size of Boost MPL sequences created
	static int mpl_seq_max_size;

public:
	::Uml::Diagram diagram;
	UdmOpts opts;

	std::vector< ::Uml::Class> DiagramClasses;

	struct unique_hash { //: public unary_function<_Kty, size_t>  {
		size_t operator()(const std::pair< Udm::ObjectImpl::uniqueId_type, Udm::ObjectImpl::uniqueId_type>& key) const {
			return key.first + key.second * 32;
		}
	};

	std::unordered_set < std::pair< Udm::ObjectImpl::uniqueId_type, Udm::ObjectImpl::uniqueId_type>, unique_hash> IsDerivedFrom;

	UdmGen(const ::Uml::Diagram &p_diagram, const UdmOpts &p_opts) : diagram(p_diagram), opts(p_opts) {
		::Uml::DiagramClasses allclasses = ::Uml::DiagramClasses(diagram);
		for (::Uml::DiagramClasses::iterator i = allclasses.begin(); i != allclasses.end(); ++i) {
			DiagramClasses.push_back(*i);
		}
		for (std::vector< ::Uml::Class>::const_iterator i = DiagramClasses.begin(); i != DiagramClasses.end(); i++)
		{
			for (std::vector< ::Uml::Class>::const_iterator j = DiagramClasses.begin(); j != DiagramClasses.end(); j++)
			{
				if (Uml::IsDerivedFrom(*i, *j))
				{
					IsDerivedFrom.emplace(std::make_pair< Udm::ObjectImpl::uniqueId_type, Udm::ObjectImpl::uniqueId_type>(i->uniqueId(), j->uniqueId()));
				}
			}
		}
	}

	bool isDerivedFrom(const ::Uml::Class &a, const ::Uml::Class &b) const {
		return IsDerivedFrom.find(std::make_pair< Udm::ObjectImpl::uniqueId_type, Udm::ObjectImpl::uniqueId_type>(a.uniqueId(), b.uniqueId())) != IsDerivedFrom.end();
	}

	void Generate(const ::Uml::Diagram &cross_dgr, const string &filename);
	vector<boost::format> HPreamble(const string &fname) const;
	boost::format HPostamble(const string &fname) const;
	boost::format HExport(const string &fname) const;
	boost::format HIncludeExport(const string &fname) const;
	void GenHExport(const string &fname);

	vector<boost::format> CPPPreamble(const string &fname, const ::Uml::Diagram &cross_dgr) const;

	vector<boost::format> CPPURIMapping() const;
	vector<boost::format> CPPXsdStorage() const;

	static void SetMPLSequenceMaxSize(int size);
};


class ClassGen {

protected:
	// start of class definition
	boost::format class_def_start;

	// typedefs
	vector<boost::format> typedefs;

	// constructors
	vector<boost::format> ctor_decls;
	vector<boost::format> ctor_rvalue_decls;
	vector<boost::format> ctor_defs;
	vector<boost::format> ctor_rvalue_defs;

	// destructors
	vector<boost::format> dtor_decls;
	vector<boost::format> dtor_defs;

	// methods
	vector<boost::format> meth_decls;
	vector<boost::format> meth_rvalue_decls;
	vector<boost::format> meth_defs;
	vector<boost::format> meth_rvalue_defs;
	// methods accessing composition relationships (for MPL code)
	vector<boost::format> meth_defs_mpl_comp;
	// methods accessing association relationships (for MPL code)
	vector<boost::format> meth_defs_mpl_assoc;
	// method specializations
	vector<boost::format> meth_speclzs_mpl_comp;
	vector<boost::format> meth_speclzs_mpl_assoc;

	// meta static members
	vector<boost::format> meta_decls;
	vector<boost::format> meta_defs;

	vector<boost::format> comments;

	vector<boost::format> parents_tl;
	vector<boost::format> parent_kinds_tl;

	vector<boost::format> children_tl;
	vector<boost::format> children_kinds_tl;

	vector<boost::format> associations_tl;
	vector<boost::format> associations_ac_tl;

	vector<boost::format> cross_associations_tl;
	vector<boost::format> cross_associations_ac_tl;

	vector<boost::format> aclass_ends_tl;

	vector<boost::format> cross_aclass_ends_tl;

	// initialize meta members

	//   when creating diagram from scratch
	vector<boost::format> meta_create;
	vector<boost::format> meta_init;
	vector<boost::format> meta_init_links;
	//   when creating diagram from another one
	vector<boost::format> meta_init2;
	vector<boost::format> meta_init_links2;
	//   static meta initialization
	vector<boost::format> old_meta_create;
	vector<boost::format> old_meta_init;
	vector<boost::format> old_meta_init_links;
	//   CORBA meta initialization
	vector<boost::format> corba_meta_create;
	vector<boost::format> corba_meta_init;
	vector<boost::format> corba_meta_init_links;

	::Uml::Class c;

	const UdmGen& gen;
	string idt;	// indentation to use

	// store typedefs and typenames to create typelists
	struct TLHelper {
		// create types for roles:
		// - CompositionParentRole: PR_<relation name>
		// - CompositionChildRole:  CR_<relation name>
		// - AssociationRole:       AR_<relation name>
		// - AssociationRole from association class: ACE_<relation name>
		vector<boost::format> roles2type;
		// create types for these pairs:
		// - for CompositionParentRole, CompositionChildRole and AssociationRole of classless association:
		//   pair<ReturnedType, RoleType>
		// - for AssociationRole of associations with an association class
		//   pair< ReturnedType, pair<AssociationClass, RoleType> >
		vector<boost::format> typedefs;
		// name the types created above
		vector<string> typenames_single;	// the target can be a single item
		vector<string> typenames_multi;		// the target is a set of items
	};

	// typelists for ReturnedType
	void BuildTL(const set< ::Uml::Class> &s, const string &tl_name, const string &description, vector<boost::format> &result, bool sort_tl = false);
	// types and typelists for pair<ReturnedType, Role> or pair< ReturnedType, pair<AssociationClass, Role> >
	// Role can be: CompositionParentRole, CompositionChildRole, AssociationRole
	void BuildTL(const TLHelper &tlhlp, const string &tl_single_name, const string &tl_multi_name, const string &description, vector<boost::format> &result);

public:
	ClassGen(const ::Uml::Class &p_c, const UdmGen &p_gen) : c(p_c), gen(p_gen) {}

	void Process(const ::Uml::Diagram &cross_diagram, const InheritanceSolver &is);
	void OutDecls(ostream &out) const;
	void OutDefs(ostream &out) const;
	void OutMetaCreate(ostream &out) const;
	void OutMetaInit(ostream &out) const;
	void OutMetaInitLinks(ostream &out) const;
	void OutMetaInit2(ostream &out) const;
	void OutMetaInitLinks2(ostream &out) const;

protected:
	void Basic(const InheritanceSolver &is);
	void Attributes();
	void Associations(const ::Uml::Diagram &cross_diagram);
	void Children();
	void Parents();
	void AssocEnds(const ::Uml::Diagram &cross_diagram);
	void Constraints();
	void Visitor();
	void OtherLinks();
};


template <class C>
class VisitorClassGen : public ClassGen {
	C c;

public:
	VisitorClassGen(const C &p_c, const UdmGen &p_gen) : c(p_c), ClassGen(NULL, p_gen) { }

	void Process()
	{
		if (gen.opts.visitor_sup)
		{
			idt = string(c.depth_level() + 2, '\t');

			class_def_start = boost::format("class %1%Visitor : public ::Udm::BaseVisitor") % gen.opts.macro;

			set< ::Uml::Class> classes = c.classes();
			for (set< ::Uml::Class>::const_iterator cl_i = classes.begin(); cl_i != classes.end(); cl_i++)
			{
				if (!cl_i->isAbstract()) {
					meth_decls.push_back( boost::format("virtual void Visit_%1%(const %1% &)") % (string)cl_i->name() );
					meth_defs.push_back( boost::format("void Visitor::Visit_%1%(const %1% &) {}") % (string)cl_i->name() );
				}
			}

			meth_decls.push_back( boost::format("virtual void Visit_Object(const ::Udm::Object &)") );
			meth_defs.push_back( boost::format("void Visitor::Visit_Object(const ::Udm::Object &) {}") );

			dtor_decls.push_back( boost::format("virtual ~Visitor()") );
			dtor_defs.push_back( boost::format("Visitor::~Visitor() {}") );
		}
	}
};


template <class C>
class CGen {

protected:
	boost::format meta_decl;
	boost::format meta_def;

	// initialize meta members

	//   when creating diagram from scratch
	vector<boost::format> meta_create;
	vector<boost::format> meta_init;
	vector<boost::format> meta_create_assocs;
	vector< vector<boost::format> > meta_init_assocs;
	vector<boost::format> meta_create_comps;
	vector< vector<boost::format> > meta_init_comps;
	vector<boost::format> meta_init_links;
	//   when creating diagram from another one
	vector<boost::format> meta_init2;
	vector<boost::format> meta_init_links2;
	//   static meta initialization
	vector<boost::format> old_meta_create;
	vector<boost::format> old_meta_init;
	vector<boost::format> old_meta_create_assocs;
	vector< vector<boost::format> > old_meta_init_assocs;
	vector<boost::format> old_meta_create_comps;
	vector< vector<boost::format> > old_meta_init_comps;
	vector<boost::format> old_meta_init_links;
	//   CORBA meta initialization
	vector<boost::format> corba_meta_create;
	vector<boost::format> corba_meta_init;
	vector<boost::format> corba_meta_create_assocs;
	vector< vector<boost::format> > corba_meta_init_assocs;
	vector<boost::format> corba_meta_create_comps;
	vector< vector<boost::format> > corba_meta_init_comps;
	vector<boost::format> corba_meta_init_links;

	// void CreateMeta() { ... }
	boost::format begin_meta_create;
	boost::format end_meta_create;

	// void InitMeta() { ... }
	boost::format begin_meta_init;
	boost::format end_meta_init;

	// void InitMetaLinks() { ... }
	boost::format begin_meta_init_links;
	boost::format end_meta_init_links;

	// void InitMeta(const C &parent) { ... }
	boost::format begin_meta_init2;
	boost::format end_meta_init2;

	// void InitMetaLinks(const C &parent) { ... }
	boost::format begin_meta_init_links2;
	boost::format end_meta_init_links2;

	vector<boost::format> begin_ns;
	vector<boost::format> end_ns;

	vector<boost::format> other_decls;
	vector<boost::format> other_defs;

	vector<boost::format> fwd_decls;

	vector<boost::format> meth_decls;
	vector<boost::format> meth_defs;

	// map path of each namespace contained by this CGen to NamespaceGen 
	typedef map<string, CGen< ::Uml::Namespace> > NSGEN_MAP;
	// map path of each class containted by this CGen to ClassGen
	typedef map<string, ClassGen> CLGEN_MAP;

	NSGEN_MAP nsgen_map;
	CLGEN_MAP clgen_map;

	VisitorClassGen<C> vgen;

	C c;	// the container, Namespace or Diagram

	::Uml::Namespace current_ns;	// the namespace itself it this is a
					// NamespaceGen object, or NULL if
					// this is a DiagramGen

	const UdmGen& gen;
	string idt;	// indentation to use

	virtual void CustomProcess(const ::Uml::Diagram &cross_diagram, const InheritanceSolver &is);

	friend class DiagramGen;

public:
	CGen(const C &p_c, const UdmGen &p_gen) : c(p_c), gen(p_gen), vgen(p_c, p_gen), current_ns(NULL) { }

	void Process(const ::Uml::Diagram &cross_diagram, const InheritanceSolver &is, bool recursively = true)
	{
		idt = string(c.depth_level(), '\t');

		CustomProcess(cross_diagram, is);

		// some callers may be interested only in the container itself:
		// diagram wants the namespace start and stop statements
		// of another diagram
		if (recursively)
		{
			set< ::Uml::Namespace> nses = c.namespaces();
			for (set< ::Uml::Namespace>::const_iterator i = nses.begin(); i != nses.end(); i++)
			{
				CGen< ::Uml::Namespace> ngen(*i, gen);
				ngen.Process(cross_diagram, is, recursively);
				nsgen_map.insert(make_pair(i->getPath2("::", false), ngen));
			}

			set< ::Uml::Class> classes = c.classes();
			for (set< ::Uml::Class>::const_iterator i = classes.begin(); i != classes.end(); i++)
			{
				fwd_decls.push_back( boost::format("class %1%%2%") % gen.opts.macro % (string)i->name() );

				// clgen_map.emplace(piecewise_construct, make_tuple(), make_tuple());
				ClassGen& cgen = clgen_map.insert(make_pair(i->getPath2("::", false), ClassGen(*i, gen))).first->second;

				cgen.Process(cross_diagram, is);
			}

			set< ::Uml::Association> associations = c.associations();
			for (set< ::Uml::Association>::const_iterator i = associations.begin(); i != associations.end(); i++)
			{
				::Uml::Association a = *i;
				::Uml::Class aclass = a.assocClass();

				vector<boost::format> init;
				vector<boost::format> old_init;
				vector<boost::format> corba_init;

				init.push_back( boost::format("::Uml::Association ass = ::Uml::Association::Create(meta)") );
				init.push_back( boost::format("::Uml::InitAssociationProps(ass, \"%1%\")") % (string) a.name() );

				old_init.push_back( boost::format("::Uml::Association ass = ::Uml::CreateAssociation()") );
				old_init.push_back( boost::format("::Uml::InitAssociation(ass, meta, \"%1%\")") % (string) a.name() );

				corba_init.push_back( boost::format("::Uml::Association ass = ::Uml::CreateCORBAAssociation()") );
				corba_init.push_back( boost::format("::Uml::InitCORBAAssociation(ass, meta, \"%1%\")") % (string) a.name() );

				set< ::Uml::AssociationRole> roles = a.roles();
				UDM_ASSERT(roles.size() == 2);

				if (aclass)
				{
					old_init.push_back( boost::format("::Uml::InitAssociationClass(ass, %1%::meta)") % UmlClassCPPName(aclass, current_ns) );
					corba_init.push_back( boost::format("::Uml::InitCORBAAssociationClass(ass, %1%::meta)") % UmlClassCPPName(aclass, current_ns) );
				}

				for (set< ::Uml::AssociationRole>::iterator i = roles.begin(); i != roles.end(); i++) 
				{
					::Uml::AssociationRole zz = ::Uml::theOther(*i);
					string aname = MakeRoleName(zz);
					string tname = UmlClassCPPName(i->target(), current_ns);

					init.push_back( boost::format("%1%::meta_%2% = ::Uml::AssociationRole::Create(ass)") % tname % aname );
					init.push_back( boost::format("::Uml::InitAssociationRoleProps(%1%::meta_%2%, \"%2%\", %3%, %4%, %5%, %6%)")
								% tname
								% aname
								% (zz.isNavigable() ? "true" : "false")
								% (zz.isPrimary() ? "true" : "false")
								% zz.min()
								% zz.max()
								);

					old_meta_create_assocs.push_back( boost::format("%1%::meta_%2% = ::Uml::CreateAssociationRole()") % tname % aname );

					old_init.push_back( boost::format("::Uml::InitAssociationRole(%1%::meta_%2%, ass, \"%2%\", %3%, %4%, %5%, %6%, %7%::meta)")
								% tname
								% aname
								% (zz.isNavigable() ? "true" : "false")
								% (zz.isPrimary() ? "true" : "false")
								% zz.min()
								% zz.max()
								% UmlClassCPPName(zz.target(), current_ns)
								);

					corba_meta_create_assocs.push_back( boost::format("%1%::meta_%2% = ::Uml::CreateCORBAAssociationRole()") % tname % aname );

					corba_init.push_back( boost::format("::Uml::InitCORBAAssociationRole(%1%::meta_%2%, ass, \"%2%\", %3%, %4%, %5%, %6%, %7%::meta)")
								% tname
								% aname
								% (zz.isNavigable() ? "true" : "false")
								% (zz.isPrimary() ? "true" : "false")
								% zz.min()
								% zz.max()
								% UmlClassCPPName(zz.target(), current_ns)
								);

					if (aclass)
					{
						old_init.push_back( boost::format("%1%::meta_%2%_end_ = %3%::meta_%4%_rev = %5%::meta_%2%")
									% UmlClassCPPName(aclass, current_ns)
									% aname
									% UmlClassCPPName(zz.target(), current_ns)
									% ::Uml::MakeRoleName(*i)
									% tname
									);
						corba_init.push_back( boost::format("%1%::meta_%2%_end_ = %3%::meta_%4%_rev = %5%::meta_%2%")
									% UmlClassCPPName(aclass, current_ns)
									% aname
									% UmlClassCPPName(zz.target(), current_ns)
									% ::Uml::MakeRoleName(*i)
									% tname
									);
					}
				}

				meta_init_assocs.push_back(init);
				old_meta_init_assocs.push_back(old_init);
				corba_meta_init_assocs.push_back(corba_init);
			}

			set< ::Uml::Composition> compositions = c.compositions();
			for (set< ::Uml::Composition>::const_iterator i = compositions.begin(); i != compositions.end(); i++)
			{
				::Uml::Composition comp = *i;

				vector<boost::format> init;
				vector<boost::format> old_init;
				vector<boost::format> corba_init;

				init.push_back( boost::format("::Uml::Composition comp = ::Uml::Composition::Create(meta)") );
				init.push_back( boost::format("::Uml::InitCompositionProps(comp, \"%1%\", %2%)") % (string) comp.name() % (comp.nonpersistent() ? "true" : "false"));

				old_init.push_back( boost::format("::Uml::Composition comp = ::Uml::CreateComposition()") );
				old_init.push_back( boost::format("::Uml::InitComposition(comp, meta, \"%1%\")") % (string) comp.name() );

				corba_init.push_back( boost::format("::Uml::Composition comp = ::Uml::CreateCORBAComposition()") );
				corba_init.push_back( boost::format("::Uml::InitCORBAComposition(comp, meta, \"%1%\")") % (string) comp.name() );

				::Uml::CompositionParentRole pr = comp.parentRole();
				string pname = MakeRoleName(pr);
				string parent_tgt = UmlClassCPPName(pr.target(), current_ns);
				::Uml::CompositionChildRole cr = comp.childRole();
				string cname = MakeRoleName(cr);
				string child_tgt = UmlClassCPPName(cr.target(), current_ns);

				init.push_back( boost::format("%1%::meta_%2% = ::Uml::CompositionParentRole::Create(comp)") % child_tgt % pname );
				init.push_back( boost::format("::Uml::InitCompositionParentRoleProps(%1%::meta_%2%, \"%2%\", %3%)")
							% child_tgt
							% pname
							% (pr.isNavigable() ? "true" : "false")
							);

				init.push_back( boost::format("%1%::meta_%2% = ::Uml::CompositionChildRole::Create(comp)") % parent_tgt % cname );

				init.push_back( boost::format("::Uml::InitCompositionChildRoleProps(%1%::meta_%2%, \"%3%\", %4%, %5%, %6%)")
						% parent_tgt
						% cname
						% ::Uml::MakeShortRoleName(cr)
						% (cr.isNavigable() ? "true" : "false")
						% cr.min()
						% cr.max()
						);

				old_meta_create_comps.push_back( boost::format("%1%::meta_%2% = ::Uml::CreateCompositionParentRole()") % child_tgt % pname );

				old_init.push_back( boost::format("::Uml::InitCompositionParentRole(%1%::meta_%2%, comp, \"%2%\", %3%, %4%::meta)")
							% child_tgt
							% pname
							% (pr.isNavigable() ? "true" : "false")
							% parent_tgt
						    
							);

				old_meta_create_comps.push_back( boost::format("%1%::meta_%2% = ::Uml::CreateCompositionChildRole()") % parent_tgt % cname );

				old_init.push_back( boost::format("::Uml::InitCompositionChildRole(%1%::meta_%2%, comp, \"%3%\", %4%, %5%, %6%, %7%::meta)")
							% parent_tgt
							% cname
							% ::Uml::MakeShortRoleName(cr)
							% (cr.isNavigable() ? "true" : "false")
							% cr.min()
							% cr.max()
							% child_tgt
							);

				corba_meta_create_comps.push_back( boost::format("%1%::meta_%2% = ::Uml::CreateCORBACompositionParentRole()") % child_tgt % pname );

				corba_init.push_back( boost::format("::Uml::InitCORBACompositionParentRole(%1%::meta_%2%, comp, \"%2%\", %3%, %4%::meta)")
							% child_tgt
							% pname
							% (pr.isNavigable() ? "true" : "false")
							% parent_tgt
						    
							);

				corba_meta_create_comps.push_back( boost::format("%1%::meta_%2% = ::Uml::CreateCORBACompositionChildRole()") % parent_tgt % cname );

				corba_init.push_back( boost::format("::Uml::InitCORBACompositionChildRole(%1%::meta_%2%, comp, \"%3%\", %4%, %5%, %6%, %7%::meta)")
							% parent_tgt
							% cname
							% ::Uml::MakeShortRoleName(cr)
							% (cr.isNavigable() ? "true" : "false")
							% cr.min()
							% cr.max()
							% child_tgt
							);

				meta_init_comps.push_back(init);
				old_meta_init_comps.push_back(old_init);
				corba_meta_init_comps.push_back(corba_init);
			}

			if (gen.opts.visitor_sup)
				vgen.Process();

			begin_meta_create = boost::format("void CreateMeta() {");
			end_meta_create   = boost::format("}");

			begin_meta_init = boost::format("void InitMeta() {");
			end_meta_init   = boost::format("}");

			begin_meta_init_links = boost::format("void InitMetaLinks() {");
			end_meta_init_links   = boost::format("}");
		}
	}

	void CollectMaps(NSGEN_MAP &p_nsgen_map, CLGEN_MAP &p_clgen_map)
	{
		p_nsgen_map.insert(nsgen_map.begin(), nsgen_map.end());
		p_clgen_map.insert(clgen_map.begin(), clgen_map.end());

		for (NSGEN_MAP::iterator i = nsgen_map.begin(); i != nsgen_map.end(); i++)
			i->second.CollectMaps(p_nsgen_map, p_clgen_map);
	}

	void BeginNS(ostream &out, bool parents_too = true) const
	{
		if (parents_too)
			for (vector<boost::format>::const_iterator i = begin_ns.begin(); i != begin_ns.end(); i++)
				// no indentation by idt, the lines are already fully indented
				out << *i << endl;
		else
			out << begin_ns.back() << endl;
		out << endl;
	}

	void EndNS(ostream &out, bool parents_too = true) const
	{
		if (parents_too)
			for (vector<boost::format>::const_iterator i = end_ns.begin(); i != end_ns.end(); i++)
				// no indentation by idt, the lines are already fully indented
				out << *i << endl;
		else
			out << end_ns.front() << endl;
		out << endl;
	}

	void OutDecls(ostream &out) const
	{
		string lidt = idt + "\t";

		if (meta_decl.size())
			out << lidt << meta_decl << ";" << endl;

		OutFmts(out, lidt, fwd_decls);

		if (gen.opts.visitor_sup)
			vgen.OutDecls(out);

		OutFmts(out, lidt, meth_decls);
		OutFmts(out, lidt, other_decls);
	}

	void OutMetaCreate(ostream &out) const
	{
		string lidt = idt + "\t";

		switch (gen.opts.meta_init)
		{
		case UdmOpts::DYNAMIC_INIT:
			OutFmts(out, lidt, meta_create);
			break;
		case UdmOpts::STATIC_INIT:
			OutFmts(out, lidt, old_meta_create);
			break;
		case UdmOpts::CORBA_INIT:
			OutFmts(out, lidt, corba_meta_create);
			break;
		default:
			throw udm_exception("unknown type of meta initialization");
		}
	}

	void OutMetaInit(ostream &out) const
	{
		string lidt = idt + "\t";

		switch (gen.opts.meta_init)
		{
		case UdmOpts::DYNAMIC_INIT:
			OutFmts(out, lidt, meta_init);
			break;
		case UdmOpts::STATIC_INIT:
			OutFmts(out, lidt, old_meta_init);
			break;
		case UdmOpts::CORBA_INIT:
			OutFmts(out, lidt, corba_meta_init);
			break;
		default:
			throw udm_exception("unknown type of meta initialization");
		}
	}

	void OutMetaInitLinks(ostream &out) const
	{
		string lidt = idt + "\t";

		switch (gen.opts.meta_init)
		{
		case UdmOpts::DYNAMIC_INIT:
			OutFmts(out, lidt, meta_init_links);
			break;
		case UdmOpts::STATIC_INIT:
			OutFmts(out, lidt, old_meta_init_links);
			break;
		case UdmOpts::CORBA_INIT:
			OutFmts(out, lidt, corba_meta_init_links);
			break;
		default:
			throw udm_exception("unknown type of meta initialization");
		}
	}

	void OutMetaInit2(ostream &out) const
	{
		OutFmts(out, idt + "\t", meta_init2);
	}

	void OutMetaInitLinks2(ostream &out) const
	{
		OutFmts(out, idt + "\t", meta_init_links2);
	}

	void OutMetaCreateAssocs(ostream &out) const
	{
		string lidt = idt + "\t\t";

		switch (gen.opts.meta_init)
		{
		case UdmOpts::DYNAMIC_INIT:
			OutFmts(out, lidt, meta_create_assocs);
			break;
		case UdmOpts::STATIC_INIT:
			OutFmts(out, lidt, old_meta_create_assocs);
			break;
		case UdmOpts::CORBA_INIT:
			OutFmts(out, lidt, corba_meta_create_assocs);
			break;
		default:
			throw udm_exception("unknown type of meta initialization");
		}
	}

	void OutMetaCreateComps(ostream &out) const
	{
		string lidt = idt + "\t\t";

		switch (gen.opts.meta_init)
		{
		case UdmOpts::DYNAMIC_INIT:
			OutFmts(out, lidt, meta_create_comps);
			break;
		case UdmOpts::STATIC_INIT:
			OutFmts(out, lidt, old_meta_create_comps);
			break;
		case UdmOpts::CORBA_INIT:
			OutFmts(out, lidt, corba_meta_create_comps);
			break;
		default:
			throw udm_exception("unknown type of meta initialization");
		}
	}

	void OutMetaInitAssocs(ostream &out) const
	{
		if (gen.opts.meta_init == UdmOpts::DYNAMIC_INIT)
			OutVecFmts(out, idt + "\t\t", meta_init_assocs, "associations");
	}

	void OutMetaInitComps(ostream &out) const
	{
		if (gen.opts.meta_init == UdmOpts::DYNAMIC_INIT)
			OutVecFmts(out, idt + "\t\t", meta_init_comps, "compositions");
	}

	void OutMetaInitLinksAssocs(ostream &out) const
	{
		string lidt = idt + "\t\t";

		switch (gen.opts.meta_init)
		{
		case UdmOpts::DYNAMIC_INIT:
			break;
		case UdmOpts::STATIC_INIT:
			OutVecFmts(out, lidt, old_meta_init_assocs, "associations");
			break;
		case UdmOpts::CORBA_INIT:
			OutVecFmts(out, lidt, corba_meta_init_assocs, "associations");
			break;
		default:
			throw udm_exception("unknown type of meta initialization");
		}
	}

	void OutMetaInitLinksComps(ostream &out) const
	{
		string lidt = idt + "\t\t";

		switch (gen.opts.meta_init)
		{
		case UdmOpts::DYNAMIC_INIT:
			break;
		case UdmOpts::STATIC_INIT:
			OutVecFmts(out, lidt, old_meta_init_comps, "compositions");
			break;
		case UdmOpts::CORBA_INIT:
			OutVecFmts(out, lidt, corba_meta_init_comps, "compositions");
			break;
		default:
			throw udm_exception("unknown type of meta initialization");
		}
	}

	void OutDefs(ostream &out) const
	{
		if (meta_def.size())
			out << idt << "\t" << meta_def << ";" << endl << endl;

		string lidt = idt; lidt += '\t';

		// CreateMeta() { ... }
		out << lidt << begin_meta_create << endl;

		if (clgen_map.size())
		{
			out << lidt << "\t// classes, with attributes, constraints and constraint definitions" << endl;
			for (CLGEN_MAP::const_iterator i = clgen_map.begin(); i != clgen_map.end(); i++)
				i->second.OutMetaCreate(out);
		}

		OutMetaCreateAssocs(out);
		OutMetaCreateComps(out);

		if (nsgen_map.size())
		{
			out << lidt << "\t// namespaces" << endl;
			for (NSGEN_MAP::const_iterator i = nsgen_map.begin(); i != nsgen_map.end(); i++)
				i->second.OutMetaCreate(out);
		}

		out << lidt << end_meta_create << endl << endl;


		// InitMeta() { ... }
		out << lidt << begin_meta_init << endl;

		if (clgen_map.size())
		{
			out << lidt << "\t// classes, with attributes, constraints and constraint definitions" << endl;
			for (CLGEN_MAP::const_iterator i = clgen_map.begin(); i != clgen_map.end(); i++)
				i->second.OutMetaInit(out);
		}

		OutMetaInitAssocs(out);
		OutMetaInitComps(out);

		if (nsgen_map.size())
		{
			out << lidt << "\t// namespaces" << endl;
			for (NSGEN_MAP::const_iterator i = nsgen_map.begin(); i != nsgen_map.end(); i++)
				i->second.OutMetaInit(out);
		}

		out << lidt << end_meta_init << endl << endl;


		// InitMetaLinks() { ... }
		out << lidt << begin_meta_init_links << endl;

		if (clgen_map.size())
			for (CLGEN_MAP::const_iterator i = clgen_map.begin(); i != clgen_map.end(); i++)
				i->second.OutMetaInitLinks(out);

		OutMetaInitLinksAssocs(out);
		OutMetaInitLinksComps(out);

		if (nsgen_map.size())
		{
			out << lidt << "\t// namespaces" << endl;
			for (NSGEN_MAP::const_iterator i = nsgen_map.begin(); i != nsgen_map.end(); i++)
				i->second.OutMetaInitLinks(out);
		}

		out << lidt << end_meta_init_links << endl << endl;


		if (gen.opts.meta_init == UdmOpts::DYNAMIC_INIT)
		{

			// InitMeta(const C &parent) { ... }
			out << lidt << begin_meta_init2 << endl;

			if (clgen_map.size())
			{
				out << lidt << "\t// classes, with attributes, constraints and constraint definitions" << endl;
				for (CLGEN_MAP::const_iterator i = clgen_map.begin(); i != clgen_map.end(); i++)
					i->second.OutMetaInit2(out);
			}

			if (nsgen_map.size())
			{
				out << lidt << "\t// namespaces" << endl;
				for (NSGEN_MAP::const_iterator i = nsgen_map.begin(); i != nsgen_map.end(); i++)
					i->second.OutMetaInit2(out);
			}

			out << lidt << end_meta_init2 << endl << endl;


			// InitMetaLinks(const C &parent) { ... }
			out << lidt << begin_meta_init_links2 << endl;

			if (clgen_map.size())
			{
				out << lidt << "\t// classes" << endl;
				for (CLGEN_MAP::const_iterator i = clgen_map.begin(); i != clgen_map.end(); i++)
					i->second.OutMetaInitLinks2(out);
			}

			if (nsgen_map.size())
			{
				out << lidt << "\t// namespaces" << endl;
				for (NSGEN_MAP::const_iterator i = nsgen_map.begin(); i != nsgen_map.end(); i++)
					i->second.OutMetaInitLinks2(out);
			}

			out << lidt << end_meta_init_links2 << endl << endl;

		}


		if (gen.opts.visitor_sup)
			vgen.OutDefs(out);

		for (vector<boost::format>::const_iterator i = meth_defs.begin(); i != meth_defs.end(); i++)
			out << "\t" << *i << endl;
		if (meth_defs.size())
			out << endl;

		for (vector<boost::format>::const_iterator i = other_defs.begin(); i != other_defs.end(); i++)
			out << "\t" << *i << ";" << endl;
		if (other_defs.size())
			out << endl;
	}

	void OutCPP(ostream &out) const
	{
		BeginNS(out, false);

		set< ::Uml::Namespace> nses = c.namespaces();
		for (set< ::Uml::Namespace>::const_iterator nses_i = nses.begin(); nses_i != nses.end(); nses_i++)
		{
			string ns_path = nses_i->getPath2("::", false);
			NSGEN_MAP::const_iterator i = nsgen_map.find(ns_path);
			if (i == nsgen_map.end()) throw udm_exception("nsgen_map does not contain a mapping for the namespace " + ns_path);

			const CGen< ::Uml::Namespace>& ngen = i->second;
			ngen.OutCPP(out);
		}

		set< ::Uml::Class> classes = c.classes();
		for (set< ::Uml::Class>::const_iterator classes_i = classes.begin(); classes_i != classes.end(); classes_i++)
		{
			string cl_path = classes_i->getPath2("::", false);
			CLGEN_MAP::const_iterator i = clgen_map.find(cl_path);
			if (i == clgen_map.end()) throw udm_exception("clgen_map does not contain a mapping for the class " + cl_path);

			const ClassGen& cgen = i->second;
			cgen.OutDefs(out);
		}

		OutDefs(out);

		EndNS(out, false);
	}
};


typedef CGen< ::Uml::Namespace> NamespaceGen;


class DiagramGen : public CGen< ::Uml::Diagram> {

protected:

	struct CrossFwdDecls {
		vector<boost::format> begin_ns;
		vector<boost::format> end_ns;

		vector<boost::format> decls;
	};
	vector<CrossFwdDecls> cross_fwd_decls;

	// statements with specializations of IsDescendant, for which the
	// value is the true value, _True_
	vector<boost::format> is_descendant_pairs;

	InheritanceSolver is;

	void CustomProcess(const ::Uml::Diagram &cross_diagram, const InheritanceSolver &p_is);

public:
	DiagramGen(const ::Uml::Diagram &p_c, const UdmGen &p_gen) : CGen< ::Uml::Diagram>(p_c, p_gen), is(p_c, p_gen, p_gen.opts.cxx_source_unit == UdmOpts::UNIT_NAMESPACE) { }

	void Process(const ::Uml::Diagram &cross_diagram, bool recursively = true)
	{
		CGen< ::Uml::Diagram>::Process(cross_diagram, is, recursively);
	}

	void OutCrossFwdDecls(ostream &out);
	void OutIsDescendantPairs(ostream &out);

	void OutH(ostream &out);
};

} // namespace UdmCPPGen

#endif //MOBIES_UDM_H
