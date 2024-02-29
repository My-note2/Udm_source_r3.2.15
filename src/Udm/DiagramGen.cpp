/*
Copyright (c) Vanderbilt University, 2008
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

#include "Udm.h"


static string GetDgrfromFromStr(const string& fromstr)
{
	return fromstr.substr(0, fromstr.find(':'));
}


static string GetNsfromFromStr(const string& fromstr )
{
	string::size_type i = fromstr.find(':');
	if (i == string::npos)
		return "";
	return fromstr.substr(i +1, string::npos);
}

static bool cmpClasses(const Uml::Class &a, const Uml::Class &b)
{
	return UmlClassCPPName(a) < UmlClassCPPName(b);
}


namespace UdmCPPGen {

typedef set< ::Uml::Class> UmlClasses;
typedef map<string,  UmlClasses> UmlClassesByName;

template <>
void CGen< ::Uml::Diagram>::CustomProcess(const ::Uml::Diagram &cross_diagram, const InheritanceSolver &is)
{
	string dgr_name = c.name();

	meta_decl = boost::format("extern ::Uml::Diagram meta");
	meta_def  = boost::format("::Uml::Diagram meta");

	begin_ns.push_back( boost::format("namespace %1% {") % dgr_name );
	end_ns.push_back( boost::format("}") );

	begin_meta_init2 = boost::format("void InitMeta(const ::Uml::Diagram &parent) {");
	end_meta_init2   = boost::format("}");

	begin_meta_init_links2 = boost::format("void InitMetaLinks(const ::Uml::Diagram &parent) {");
	end_meta_init_links2   = boost::format("}");

	other_decls.push_back( boost::format("extern %1% ::Udm::UdmDiagram diagram")
				% gen.opts.macro );

	other_defs.push_back( boost::format("%1% ::Udm::UdmDiagram diagram = { &meta, Initialize }") % gen.opts.macro );

	other_defs.push_back( boost::format("static struct _regClass\n\
\t{\n\
\t\t_regClass()\n\
\t\t{\n\
\t\t\t::Udm::MetaDepository::StoreDiagram(\"%1%\", diagram);\n\
\t\t}\n\
\t\t~_regClass()\n\
\t\t{\n\
\t\t\t::Udm::MetaDepository::RemoveDiagram(\"%1%\");\n\
\t\t}\n\
\t} __regUnUsed\
")
				% dgr_name );

	vector<boost::format> uri_mapping_stmts = gen.CPPURIMapping();
	if (uri_mapping_stmts.size())
	{
		meth_defs.push_back( boost::format("void _SetURIMapping()\n\
\t{") );
		meth_defs.insert(meth_defs.end(), uri_mapping_stmts.begin(), uri_mapping_stmts.end());
		meth_defs.push_back( boost::format("}\n") );
	}

	vector<boost::format> xsd_storage_stmts = gen.CPPXsdStorage();
	if (xsd_storage_stmts.size())
	{
		meth_defs.push_back( boost::format("void _SetXsdStorage()\n\
\t{") );
		meth_defs.insert(meth_defs.end(), xsd_storage_stmts.begin(), xsd_storage_stmts.end());
		meth_defs.push_back( boost::format("}\n") );
	}

	meth_decls.push_back( boost::format("%1%void Initialize()") % gen.opts.macro );
	meth_defs.push_back( boost::format("%1%void Initialize()\n\
\t{\n\
\t\tstatic bool first = true;\n\
\t\tif (!first) return;\n\
\t\tfirst = false;\n\
\t\t::Uml::Initialize();\n\
")
				% gen.opts.macro
				);

	if (cross_diagram && (cross_diagram != c))
		meth_defs.push_back( boost::format("\t%1%::Initialize();") % (string) cross_diagram.name() );

	meth_defs.push_back( boost::format("\n\
\t\tUDM_ASSERT( meta == ::Udm::null );\n\
")
				);

	if (gen.opts.meta_init == UdmOpts::DYNAMIC_INIT)
		meth_defs.push_back( boost::format("\t::UdmStatic::StaticDataNetwork * meta_dn = new ::UdmStatic::StaticDataNetwork(::Uml::diagram);\n\
\t\tmeta_dn->CreateNew(\"%1%.mem\", \"\", ::Uml::Diagram::meta, ::Udm::CHANGES_LOST_DEFAULT);\n\
\t\tmeta = ::Uml::Diagram::Cast(meta_dn->GetRootObject());\n\
\n\
\t\t::Uml::InitDiagramProps(meta, \"%1%\", \"%2%\");\n\
\n\
")
					% dgr_name
					% (string) c.version()
					);
	else if (gen.opts.meta_init == UdmOpts::STATIC_INIT)
		meth_defs.push_back( boost::format("\tmeta = ::Uml::CreateDiagram();\n\
\n\
")
					);
	else
		meth_defs.push_back( boost::format("\tmeta = ::Uml::CreateCORBADiagram();\n\
\n\
")
					);

	// create a static DN for Uml meta
	if (gen.opts.meta_init == UdmOpts::STATIC_INIT && dgr_name == "Uml")
		meth_defs.push_back( boost::format("\t::UdmStatic::StaticDataNetwork* dn = new ::UdmStatic::StaticDataNetwork(diagram);\n\
\t\tdn->rootobject = meta;\n\
\t\tstatic_cast< ::UdmStatic::StaticObject*>(dn->rootobject.__impl())->mydn = dn;\n\
\t\tdn->systemname = \"%1%\";\n\
\t\tdn->sem = ::Udm::CHANGES_LOST_DEFAULT;\n\
\n\
")
					% dgr_name
					);

	meth_defs.push_back( boost::format("\tCreateMeta();\n\
\t\tInitMeta();\n\
\t\tInitMetaLinks();\n\
")
				);

	if (gen.opts.meta_init == UdmOpts::STATIC_INIT)
		meth_defs.push_back( boost::format("\t::Uml::InitDiagram(meta, \"%1%\", \"%2%\");\n\
\n\
")
					% dgr_name
					% (string) c.version()
					);

	else if (gen.opts.meta_init == UdmOpts::CORBA_INIT)
		meth_defs.push_back( boost::format("\t::Uml::InitCORBADiagram(meta, \"%1%\", \"%2%\");\n\
\n\
")
					% dgr_name
					% (string) c.version()
					);

	if (uri_mapping_stmts.size())
		meth_defs.push_back( boost::format("\t_SetURIMapping();\n") );

	if (xsd_storage_stmts.size())
		meth_defs.push_back( boost::format("\t_SetXsdStorage();\n") );

	meth_defs.push_back( boost::format("}\n") );

	if (gen.opts.meta_init == UdmOpts::DYNAMIC_INIT)
	{
		meth_decls.push_back( boost::format("%1%void Initialize(const ::Uml::Diagram &dgr)") % gen.opts.macro );
		meth_defs.push_back( boost::format("%1%void Initialize(const ::Uml::Diagram &dgr)\n\
\t{\n\
\t\t%2%\n\
\t\tif (meta == dgr) return;\n\
\t\tmeta = dgr;\n\
\n\
\t\tInitMeta(dgr);\n\
\t\tInitMetaLinks(dgr);\n\
\n\
\t\t%3%\n\
\t\t%4%\n\
\t}\n\
")
					% gen.opts.macro
					% (cross_diagram && (cross_diagram != c) ? "UDM_ASSERT(::" + (string) cross_diagram.name() + "::meta != ::Udm::null);" : "")
					% (uri_mapping_stmts.size() ? "_SetURIMapping();" : "")
					% (xsd_storage_stmts.size() ? "_SetXsdStorage();" : "")
				     );
	}

}

void DiagramGen::CustomProcess(const ::Uml::Diagram &cross_diagram, const InheritanceSolver &p_is)
{
	CGen< ::Uml::Diagram>::CustomProcess(cross_diagram, p_is);

	string diagram_name = c.name();

	// cross forward declarations
	if (cross_diagram && cross_diagram != c)
	{
		map<string, UmlClassesByName> cr_cls_nsp;

		// build map of the sources of cross classes:
		// [ diagram => [ namespace1 => [ class1, class2, ... ], namespace2 => [ class1, ... ] ]
		set< ::Uml::Class> cross_classes = cross_diagram.classes();
		for (set< ::Uml::Class>::iterator i = cross_classes.begin(); i != cross_classes.end(); i++)
		{
			string from = i->from();
			if (from.size() == 0) continue;

			string dgr_from = GetDgrfromFromStr(from);
			if (dgr_from == diagram_name) continue;	// no need for forward declarations of classes from the diagram itself

			string ns_from = GetNsfromFromStr(from);

			map<string, UmlClasses> & cr_cls = cr_cls_nsp[dgr_from];
			UmlClasses & clss = cr_cls[ns_from];

			clss.insert(*i);
		}

		for (map<string, UmlClassesByName>::iterator j = cr_cls_nsp.begin(); j != cr_cls_nsp.end(); j++)
		{
			::Uml::Diagram dgr_from = ::Uml::diagramByName(c, j->first);

			map<string, UmlClasses> cr_cls = j->second;
			for (map<string, UmlClasses>::iterator jj = cr_cls.begin(); jj != cr_cls.end(); jj++)
			{
				set< ::Uml::Class>& clss = jj->second;
				
				if (clss.size())
				{
					CrossFwdDecls decl;
					decl.begin_ns.push_back( boost::format("namespace %1% {") % j->first );

					if (!jj->first.empty())
					{
						::Uml::Namespace ns_from = ::Uml::namespaceByPath(dgr_from, jj->first, ":");
						if (!ns_from)
							throw udm_exception(std::string("Couldn't find ") + jj->first + " in namespace "
								+ static_cast<std::string>(dgr_from.name()));
						NamespaceGen ngen(ns_from, gen);
						ngen.Process(cross_diagram, p_is, false);
						decl.begin_ns.insert(decl.begin_ns.end(), ngen.begin_ns.begin(), ngen.begin_ns.end());
						decl.end_ns.insert(decl.end_ns.end(), ngen.end_ns.begin(), ngen.end_ns.end());
					}

					for(set< ::Uml::Class>::iterator jjj = clss.begin(); jjj != clss.end(); jjj++)
					{
						string class_name = jjj->name();
						//Attention - dangerous zone
						class_name = class_name.substr(0, class_name.find(Udm::cross_delimiter));
						//end of dangerous zone

						::Uml::Class cl = ::Uml::classByName(dgr_from, jj->first, class_name, ":");
						decl.decls.push_back( boost::format("%3%class %1%%2%")
									% gen.opts.macro
									% class_name
									% string(cl.depth_level(), '\t') );
					}

					decl.end_ns.push_back( boost::format("}") );

					cross_fwd_decls.push_back(decl);
				}
			}
		}
	}


	// IsDescendant pairs
	if (gen.opts.mode == UdmOpts::CXX_GENERIC) {

		::Uml::DiagramClasses all_classes(c);

		vector< ::Uml::Class> all_classes_v;
		for (::Uml::DiagramClasses::iterator i = all_classes.begin(); i != all_classes.end(); i++)
			all_classes_v.push_back(*i);
		sort(all_classes_v.begin(), all_classes_v.end(), cmpClasses);

		for (vector< ::Uml::Class>::const_iterator parent = all_classes_v.begin(); parent != all_classes_v.end(); parent++) {
			for (vector< ::Uml::Class>::const_iterator child = all_classes_v.begin(); child != all_classes_v.end(); child++) {
				vector< ::Uml::ChildRoleChain> chains;
				if (::Uml::GetChildRoleChain(*parent, *child, chains))
					is_descendant_pairs.push_back( boost::format("template <> struct IsDescendant < %1%, %2%> : _True_ {};" )
								       % UmlClassCPPName(*parent)
								       % UmlClassCPPName(*child)
								       );
			}
		}
	}
}

void DiagramGen::OutCrossFwdDecls(ostream &out)
{
	for (vector<CrossFwdDecls>::const_iterator decls_i = cross_fwd_decls.begin(); decls_i != cross_fwd_decls.end(); decls_i++)
	{
		for (vector<boost::format>::const_iterator i = decls_i->begin_ns.begin(); i != decls_i->begin_ns.end(); i++)
			out << *i << endl;

		OutFmts(out, "", decls_i->decls);

		for (vector<boost::format>::const_iterator i = decls_i->end_ns.begin(); i != decls_i->end_ns.end(); i++)
			out << *i << endl;

		out << endl;
	}
}

void DiagramGen::OutIsDescendantPairs(ostream &out)
{
	if (gen.opts.mode != UdmOpts::CXX_GENERIC)
		return;

	out << boost::format("\
#ifdef PARADIGM_HAS_DESCENDANT_PAIRS\n\
\n\
\tstruct _False_ { enum { value = 0 }; };\n\
\tstruct _True_  { enum { value = 1 }; };\n\
\n\
\ttemplate <class T, class U>\n\
\tstruct IsDescendant : _False_ {};\n\
\n");

	for (vector<boost::format>::const_iterator i = is_descendant_pairs.begin(); i != is_descendant_pairs.end(); i++)
		out << "\t" << *i << endl;

	out << boost::format("\
\n\
#endif // PARADIGM_HAS_DESCENDANT_PAIRS\n\
\n");

	out << endl;
}

void DiagramGen::OutH(ostream &out)
{
	NSGEN_MAP all_nsgen_map;
	CLGEN_MAP all_clgen_map;

	CollectMaps(all_nsgen_map, all_clgen_map);


	OutCrossFwdDecls(out);

	BeginNS(out);

	OutDecls(out);


	::Uml::DiagramNamespaces nses(c);
	for (::Uml::DiagramNamespaces::iterator nses_i = nses.begin(); nses_i != nses.end(); nses_i++)
	{
		string ns_path = nses_i->getPath2("::", false);
		NSGEN_MAP::const_iterator i = all_nsgen_map.find(ns_path);
		if (i == all_nsgen_map.end()) throw udm_exception("all_nsgen_map does not contain a mapping for the namespace " + ns_path);

		NamespaceGen ngen = i->second;
		ngen.BeginNS(out);
		ngen.OutDecls(out);
		ngen.EndNS(out);
	}


	::Uml::Namespace current_ns;
	for (vector< ::Uml::Class>::const_iterator gio_i = is.good_inheritence_order.begin(); gio_i != is.good_inheritence_order.end(); gio_i++)
	{
		::Uml::Class cl = *gio_i;
		::Uml::Namespace ns = cl.parent_ns();
			
		if (current_ns != ns)
		{
			if (gio_i != is.good_inheritence_order.begin()) {
				if (current_ns)
				{
					string current_ns_path = current_ns.getPath2("::", false);
					NSGEN_MAP::const_iterator i = all_nsgen_map.find(current_ns_path);
					if (i == all_nsgen_map.end()) throw udm_exception("all_nsgen_map does not contain a mapping for the namespace " + current_ns_path);

					NamespaceGen ngen = i->second;
					ngen.EndNS(out);
				}
			}

			current_ns = ns;

			if (current_ns)
			{
				string current_ns_path = current_ns.getPath2("::", false);
				NSGEN_MAP::const_iterator i = all_nsgen_map.find(current_ns_path);
				if (i == all_nsgen_map.end()) throw udm_exception("all_nsgen_map does not contain a mapping for the namespace " + current_ns_path);

				NamespaceGen ngen = i->second;
				ngen.BeginNS(out);

			}
		}

		string cl_path = cl.getPath2("::", false);
		CLGEN_MAP::const_iterator i = all_clgen_map.find(cl_path);
		if (i == all_clgen_map.end()) throw udm_exception("all_clgen_map does not contain a mapping for the class " + cl_path);

		const ClassGen& cgen = i->second;
		cgen.OutDecls(out);
	}

	if (is.good_inheritence_order.size() > 0 && current_ns)
	{
		string current_ns_path = current_ns.getPath2("::", false);
		NSGEN_MAP::const_iterator i = all_nsgen_map.find(current_ns_path);
		if (i == all_nsgen_map.end()) throw udm_exception("all_nsgen_map does not contain a mapping for the namespace " + current_ns_path);

		NamespaceGen ngen = i->second;
		ngen.EndNS(out);
	}
	

	OutIsDescendantPairs(out);

	EndNS(out);
}


} // namespace UdmCPPGen
