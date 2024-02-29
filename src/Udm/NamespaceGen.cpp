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


namespace UdmCPPGen {

template <>
void CGen< ::Uml::Namespace>::CustomProcess(const ::Uml::Diagram &cross_diagram, const InheritanceSolver &is)
{
	string ns_name = c.name();

	current_ns = c;

	//meta_decl = boost::format("static ::Uml::Namespace meta");
	meta_decl = boost::format("extern ::Uml::Namespace meta");
	meta_def  = boost::format("::Uml::Namespace meta");

	meta_create.push_back( boost::format("%1%::meta = ::Uml::Namespace::Create(meta)") % ns_name );
	meta_create.push_back( boost::format("%1%::CreateMeta()") % ns_name );

	meta_init.push_back( boost::format("::Uml::InitNamespaceProps(%1%::meta, \"%1%\")") % ns_name );
	meta_init.push_back( boost::format("%1%::InitMeta()") % ns_name );

	meta_init_links.push_back( boost::format("%1%::InitMetaLinks()") % ns_name );

	begin_meta_init2 = boost::format("void InitMeta(const ::Uml::Namespace &parent) {");
	end_meta_init2   = boost::format("}");

	begin_meta_init_links2 = boost::format("void InitMetaLinks(const ::Uml::Namespace &parent) {");
	end_meta_init_links2   = boost::format("}");

	meta_init2.push_back( boost::format("::Uml::SetNamespace(%1%::meta, parent, \"%1%\")") % ns_name );
	meta_init2.push_back( boost::format("%1%::InitMeta(%1%::meta)") % ns_name );

	meta_init_links2.push_back( boost::format("%1%::InitMetaLinks(%1%::meta)") % ns_name );

	old_meta_create.push_back( boost::format("%1%::meta = ::Uml::CreateNamespace()") % ns_name);
	old_meta_create.push_back( boost::format("%1%::CreateMeta()") % ns_name );

	old_meta_init.push_back( boost::format("::Uml::InitNamespace(%1%::meta, meta, \"%1%\")") % ns_name );
	old_meta_init.push_back( boost::format("%1%::InitMeta()") % ns_name );

	old_meta_init_links.push_back( boost::format("%1%::InitMetaLinks()") % ns_name );

	corba_meta_create.push_back( boost::format("%1%::meta = ::Uml::CreateCORBANamespace()") % ns_name);
	corba_meta_create.push_back( boost::format("%1%::CreateMeta()") % ns_name );

	corba_meta_init.push_back( boost::format("::Uml::InitCORBANamespace(%1%::meta, meta, \"%1%\")") % ns_name );
	corba_meta_init.push_back( boost::format("%1%::InitMeta()") % ns_name );

	corba_meta_init_links.push_back( boost::format("%1%::InitMetaLinks()") % ns_name );

	vector< ::Uml::Namespace> ns_parents = ::Uml::GetParentNamespaces(c);

	if (ns_parents.size())
	{
		for (unsigned int i = 0; i < ns_parents.size(); i++)
			begin_ns.push_back( boost::format("%1%namespace %2% {")
						% string(ns_parents[i].depth_level(), '\t')
						% (string)ns_parents[i].name() );
	}
	begin_ns.push_back( boost::format("%1%namespace %2% {")
				% string(c.depth_level(), '\t')
				% ns_name );

	end_ns.push_back( boost::format("%1%} // namespace %2%")
				% string(c.depth_level(), '\t')
				% (string)c.getPath2("::", false) );
	if (ns_parents.size())
	{
		for (int i = ns_parents.size() - 1; i >= 0; i--)
			end_ns.push_back( boost::format("%1%} // namespace %2%")
						% string(ns_parents[i].depth_level(), '\t')
						% (string)ns_parents[i].getPath2("::", false) );
	}
}

} // namespace UdmCPPGen
