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
#include <UdmUtil.h>

#include <boost/algorithm/string/replace.hpp>


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


static string GetCPPNamefromFromStr(const string& fromstr)
{
	string::size_type i = fromstr.find(':');
	string ret = "::" + fromstr.substr(0, i);
	if (i != string::npos)
		ret += "::" + UdmUtil::replace_delimiter(fromstr.substr(i + 1, string::npos), ":", "::");
	return ret;
}


static string AttrType2Name(const Udm::StringAttr &p)
{
	string pv = p;
	if (pv.compare("String") && pv.compare("Boolean") && pv.compare("Integer") && pv.compare("Real") && pv.compare("Text")) 
	{
		throw udm_exception("invalid attribute type '" + pv + "'");
	}

	//we use the same API for Text and String attributes
	if (!pv.compare("Text"))
	{
		return "String";
	}

	return pv;
}


namespace UdmCPPGen {

void ClassGen::Process(const ::Uml::Diagram &cross_dgr, const InheritanceSolver &is)
{
	idt = string(c.depth_level() + 1, '\t');

	Basic(is);
	Attributes();
	Associations(cross_dgr);
	Children();
	Parents();
	AssocEnds(cross_dgr);
	Constraints();
	Visitor();
	OtherLinks();
}


void ClassGen::Basic(const InheritanceSolver &is)
{
	string cl_name = c.name();

	// class Macro C : Base-List
	class_def_start = boost::format("class %1%%2% : %3%")
				% gen.opts.macro
				% cl_name
				% is.getAncestorList(c);

	// static ::Uml::Class meta
	meta_decls.push_back( boost::format("static ::Uml::Class meta") );
	// ::Uml::Class C::meta
	meta_defs.push_back( boost::format("::Uml::Class %1%::meta") % cl_name );

	meta_create.push_back( boost::format("%1%::meta = ::Uml::Class::Create(meta)") % cl_name );

	meta_init.push_back( boost::format("::Uml::InitClassProps(%1%::meta, \"%1%\", %2%, %3%, %4%)")
				% cl_name
				% (c.isAbstract() ? "true" : "false")
				% (((string) c.stereotype()).empty() ? "NULL" : "\"" + (string) c.stereotype() + "\"")
				% (((string) c.from()).empty() ? "NULL" : "\"" + (string) c.from() + "\"")
				);

	meta_init2.push_back( boost::format("::Uml::SetClass(%1%::meta, parent, \"%1%\")") % cl_name );

	old_meta_create.push_back( boost::format("%1%::meta = ::Uml::CreateClass()") % cl_name );
	old_meta_init.push_back( boost::format("::Uml::InitClass(%1%::meta, meta, \"%1%\", %2%, %3%, %4%)")
					% cl_name
					% (c.isAbstract() ? "true" : "false")
					% (((string) c.stereotype()).empty() ? "NULL" : "\"" + (string) c.stereotype() + "\"")
					% (((string) c.from()).empty() ? "NULL" : "\"" + (string) c.from() + "\"")
					);

	corba_meta_create.push_back( boost::format("%1%::meta = ::Uml::CreateCORBAClass()") % cl_name );
	corba_meta_init.push_back( boost::format("::Uml::InitCORBAClass(%1%::meta, meta, \"%1%\", %2%, \"%3%\")")
					% cl_name
					% (c.isAbstract() ? "true" : "false")
					% (string) c.stereotype()
					);

	// C()
	ctor_decls.push_back( boost::format("%1%()") % cl_name );
	// C::C() {}
	ctor_defs.push_back( boost::format("%1%::%1%() {}") % cl_name );

	// C(::Udm::ObjectImpl *impl)
	ctor_decls.push_back( boost::format("%1%(::Udm::ObjectImpl *impl)") % cl_name );
	// C::C(::Udm::ObjectImpl *impl) : init_list {}
	ctor_defs.push_back( boost::format("%1%::%1%(::Udm::ObjectImpl *impl) : %2% {}") % cl_name % is.getInitializers(c, "impl") );

	// C(const C &master)
	ctor_decls.push_back( boost::format("%1%(const %1% &master)") % cl_name );
	// C::C(const C &master) : init_list {}
	ctor_defs.push_back( boost::format("%1%::%1%(const %1% &master) : %2% {}") % cl_name % is.getInitializers(c, "master") );

	// C(C &&master)
	ctor_rvalue_decls.push_back( boost::format("%1%(%1% &&master)") % cl_name );
	// C::C(C &&master) : init_list {}
	ctor_rvalue_defs.push_back( boost::format("%1%::%1%(%1% &&master) : %2% {}") % cl_name % is.getInitializers(c, "master") );

	// static C Cast(::Udm::Object &&a)
	meth_rvalue_decls.push_back( boost::format("static %1% Cast(::Udm::Object &&a)") % cl_name );
	// static C C::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); }
	meth_rvalue_defs.push_back( boost::format("%1% %1%::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); }") % cl_name );

	// C& operator=(C &&a)
	meth_rvalue_decls.push_back( boost::format("%1%& operator=(%1% &&a)") % cl_name );
	// C& C::operator=(C &&a) { ::Udm::Object::operator =(std::move(a)); return *this; }
	meth_rvalue_defs.push_back( boost::format("%1%& %1%::operator=(%1% &&a) { ::Udm::Object::operator =(std::move(a)); return *this; }") % cl_name );

	// "A implicitly-declared copy assignment operator for class T is defined as deleted if any of the following is true:
	// ... T has a user-declared move assignment operator"
	// C& operator=(const C &a)
	meth_rvalue_decls.push_back(boost::format("%1%& operator=(const %1% &a)") % cl_name);
	// C& C::operator=(const C &a) { ::Udm::Object::operator =(std::move(a)); return *this; }
	meth_rvalue_defs.push_back(boost::format("%1%& %1%::operator=(const %1% &a) { ::Udm::Object::operator =(a); return *this; }") % cl_name);

	// static C Cast(const ::Udm::Object &a)
	meth_decls.push_back( boost::format("static %1% Cast(const ::Udm::Object &a)") % cl_name );
	// static C C::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	meth_defs.push_back( boost::format("%1% %1%::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }") % cl_name );

	// static C Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE)
	meth_decls.push_back( boost::format("static %1% Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE)") % cl_name );
	// static C C::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	meth_defs.push_back( boost::format("%1% %1%::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }") % cl_name );

	// C CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE)
	meth_decls.push_back( boost::format("%1% CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE)") % cl_name );
	// C C::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	meth_defs.push_back( boost::format("%1% %1%::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }") % cl_name );

	// ::Udm::InstantiatedAttr<C> Instances()
	meth_decls.push_back( boost::format("::Udm::InstantiatedAttr< %1%> Instances()") % cl_name );
	// ::Udm::InstantiatedAttr<C> C::Instances() { return ::Udm::InstantiatedAttr<C>(impl); }
	meth_defs.push_back( boost::format("::Udm::InstantiatedAttr< %1%> %1%::Instances() { return ::Udm::InstantiatedAttr< %1%>(impl); }") % cl_name );

	// template <class Pred> ::Udm::InstantiatedAttr<C, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr<C, Pred>(impl); }
	meth_decls.push_back( boost::format("template <class Pred> ::Udm::InstantiatedAttr< %1%, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< %1%, Pred>(impl); }") % cl_name );

	// C CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE)
	meth_decls.push_back( boost::format("%1% CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE)") % cl_name );
	// C C::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	meth_defs.push_back( boost::format("%1% %1%::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }") % cl_name );

	// ::Udm::DerivedAttr<C> Derived()
	meth_decls.push_back( boost::format("::Udm::DerivedAttr< %1%> Derived()") % cl_name );
	// ::Udm::DerivedAttr<C> C::Derived() { return ::Udm::DerivedAttr<C>(impl); }
	meth_defs.push_back( boost::format("::Udm::DerivedAttr< %1%> %1%::Derived() { return ::Udm::DerivedAttr< %1%>(impl); }") % cl_name );

	// template <class Pred> ::Udm::DerivedAttr<C, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr<C, Pred>(impl); }
	meth_decls.push_back( boost::format("template <class Pred> ::Udm::DerivedAttr< %1%, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< %1%, Pred>(impl); }") % cl_name );

	// ::Udm::ArchetypeAttr<C> Archetype() const
	meth_decls.push_back( boost::format("::Udm::ArchetypeAttr< %1%> Archetype() const") % cl_name );
	// ::Udm::ArchetypeAttr<C> C::Archetype() const { return ::Udm::ArchetypeAttr<C>(impl); }
	meth_defs.push_back( boost::format("::Udm::ArchetypeAttr< %1%> %1%::Archetype() const { return ::Udm::ArchetypeAttr< %1%>(impl); }") % cl_name );


	if (gen.opts.mode == UdmOpts::CXX_GENERIC)
	{
		// map role types to the static data members
		meth_defs_mpl_assoc.push_back( boost::format("template <typename T> const ::Uml::AssociationRole& _type2ARole() const;") );
		meth_defs_mpl_assoc.push_back( boost::format("template <typename T> const std::pair<const ::Uml::AssociationRole*, const ::Uml::AssociationRole*> _type2ACARole() const;") );
		meth_defs_mpl_comp.push_back( boost::format("template <typename T> const ::Uml::CompositionParentRole& _type2CPRole() const;") );
		meth_defs_mpl_comp.push_back( boost::format("template <typename T> const ::Uml::CompositionChildRole& _type2CCRole() const;") );

		// member function templates

		// 1. associations without an association class
		// template <class PeerType, class RoleType> ::Udm::PointerAttr<PeerType> peer() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class PeerType, class RoleType> ::Udm::PointerAttr<PeerType> peer() const { boost::function_requires< ::Udm::WithRoleInTListConcept<PeerType, RoleType, AssociationsSingle> >(); return ::Udm::PointerAttr<PeerType>(impl, _type2ARole<RoleType>()); }") );
		// template <class PeersType, class RoleType> ::Udm::AssocAttr<PeersType> peers() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class PeersType, class RoleType> ::Udm::AssocAttr<PeersType> peers() const { boost::function_requires< ::Udm::WithRoleInTListConcept<PeersType, RoleType, AssociationsMulti> >(); return ::Udm::AssocAttr<PeersType>(impl, _type2ARole<RoleType>()); }") );
		// template <class PeersType, class RoleType, class Pred> ::Udm::AssocAttr<PeersType, Pred> peers_sorted(const Pred &) const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class PeersType, class RoleType, class Pred> ::Udm::AssocAttr<PeersType, Pred> peers_sorted() const { boost::function_requires< ::Udm::WithRoleInTListConcept<PeersType, RoleType, AssociationsMulti> >(); return ::Udm::AssocAttr<PeersType, Pred>(impl, _type2ARole<RoleType>()); }") );

		// 2. associations with association class
		// template <class ConnectorType, class PeerType, class RoleType> ::Udm::AClassPointerAttr<ConnectorType, PeerType> connector() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class ConnectorType, class PeerType, class RoleType> ::Udm::AClassPointerAttr<ConnectorType, PeerType> connector() const { boost::function_requires< ::Udm::WithAClassAndRoleInTListConcept<PeerType, ConnectorType, RoleType, AssociationsWAClassSingle> >(); std::pair<const ::Uml::AssociationRole*, const ::Uml::AssociationRole*> roles_mp = _type2ACARole<RoleType>(); return ::Udm::AClassPointerAttr<ConnectorType, PeerType>(impl, *roles_mp.first, *roles_mp.second); }") );
		// template <class ConnectorsType, class PeerType, class RoleType> ::Udm::AClassAssocAttr<ConnectorsType, PeerType> connectors() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class ConnectorsType, class PeerType, class RoleType> ::Udm::AClassAssocAttr<ConnectorsType, PeerType> connectors() const { boost::function_requires< ::Udm::WithAClassAndRoleInTListConcept<PeerType, ConnectorsType, RoleType, AssociationsWAClassMulti> >(); std::pair<const ::Uml::AssociationRole*, const ::Uml::AssociationRole*> roles_mp = _type2ACARole<RoleType>(); return ::Udm::AClassAssocAttr<ConnectorsType, PeerType>(impl, *roles_mp.first, *roles_mp.second); }") );
		// template <class ConnectorsType, class PeerType, class RoleType, class Pred> ::Udm::AClassAssocAttr<ConnectorsType, PeerType, Pred> connectors_sorted() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class ConnectorsType, class PeerType, class RoleType, class Pred> ::Udm::AClassAssocAttr<ConnectorsType, PeerType, Pred> connectors_sorted() const { boost::function_requires< ::Udm::WithAClassAndRoleInTListConcept<PeerType, ConnectorsType, RoleType, AssociationsWAClassMulti> >(); std::pair<const ::Uml::AssociationRole*, const ::Uml::AssociationRole*> roles_mp = _type2ACARole<RoleType>(); return ::Udm::AClassAssocAttr<ConnectorsType, PeerType, Pred>(impl, *roles_mp.first, *roles_mp.second); }") );

		// 3. cross diagram associations without an association class
		// template <class PeerType, class RoleType> ::Udm::CrossPointerAttr<PeerType> cross_peer() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class PeerType, class RoleType> ::Udm::CrossPointerAttr<PeerType> cross_peer() const { boost::function_requires< ::Udm::WithRoleInTListConcept<PeerType, RoleType, CrossAssociationsSingle> >(); return ::Udm::CrossPointerAttr<PeerType>(impl, _type2ARole<RoleType>()); }") );
		// template <class PeersType, class RoleType> ::Udm::CrossAssocAttr<PeersType> cross_peers() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class PeersType, class RoleType> ::Udm::CrossAssocAttr<PeersType> cross_peers() const { boost::function_requires< ::Udm::WithRoleInTListConcept<PeersType, RoleType, CrossAssociationsMulti> >(); return ::Udm::CrossAssocAttr<PeersType>(impl, _type2ARole<RoleType>()); }") );
		// template <class PeersType, class RoleType, class Pred> ::Udm::CrossAssocAttr<PeersType, Pred> cross_peers_sorted(const Pred &) const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class PeersType, class RoleType, class Pred> ::Udm::CrossAssocAttr<PeersType, Pred> cross_peers_sorted() const { boost::function_requires< ::Udm::WithRoleInTListConcept<PeersType, RoleType, CrossAssociationsMulti> >(); return ::Udm::CrossAssocAttr<PeersType, Pred>(impl, _type2ARole<RoleType>()); }") );

		// 4. cross diagram associations with association class
		// template <class ConnectorType, class PeerType, class RoleType> :Udm::AClassCrossPointerAttr<ConnectorType, PeerType> cross_connector() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class ConnectorType, class PeerType, class RoleType> ::Udm::AClassCrossPointerAttr<ConnectorType, PeerType> cross_connector() const { boost::function_requires< ::Udm::WithAClassAndRoleInTListConcept<PeerType, ConnectorType, RoleType, CrossAssociationsWAClassSingle> >(); std::pair<const ::Uml::AssociationRole*, const ::Uml::AssociationRole*> roles_mp = _type2ACARole<RoleType>(); return ::Udm::AClassCrossPointerAttr<ConnectorType, PeerType>(impl, *roles_mp.first, *roles_mp.second); }") );
		// template <class ConnectorsType, class PeerType, class RoleType> ::Udm::AClassCrossAssocAttr<ConnectorsType, PeerType> cross_connectors() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class ConnectorsType, class PeerType, class RoleType> ::Udm::AClassCrossAssocAttr<ConnectorsType, PeerType> cross_connectors() const { boost::function_requires< ::Udm::WithAClassAndRoleInTListConcept<PeerType, ConnectorsType, RoleType, CrossAssociationsWAClassMulti> >(); std::pair<const ::Uml::AssociationRole*, const ::Uml::AssociationRole*> roles_mp = _type2ACARole<RoleType>(); return ::Udm::AClassCrossAssocAttr<ConnectorsType, PeerType>(impl, *roles_mp.first, *roles_mp.second); }") );
		// template <class ConnectorsType, class PeerType, class RoleType, class Pred> ::Udm::AClassCrossAssocAttr<ConnectorsType, PeerType, Pred> cross_connectors_sorted() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class ConnectorsType, class PeerType, class RoleType, class Pred> ::Udm::AClassCrossAssocAttr<ConnectorsType, PeerType, Pred> cross_connectors_sorted() const { boost::function_requires< ::Udm::WithAClassAndRoleInTListConcept<PeerType, ConnectorsType, RoleType, CrossAssociationsWAClassMulti> >(); std::pair<const ::Uml::AssociationRole*, const ::Uml::AssociationRole*> roles_mp = _type2ACARole<RoleType>(); return ::Udm::AClassCrossAssocAttr<ConnectorsType, PeerType, Pred>(impl, *roles_mp.first, *roles_mp.second); }") );

		// 5. children
		// template <class ChildType, class RoleType> ::Udm::ChildAttr<ChildType> child() const { ... }
		meth_defs_mpl_comp.push_back( boost::format("template <class ChildType, class RoleType> ::Udm::ChildAttr<ChildType> child() const { boost::function_requires< ::Udm::WithRoleInTListConcept_B<ChildType, RoleType, ChildrenSingle> >(); return ::Udm::ChildAttr<ChildType>(impl, _type2CCRole<RoleType>()); }") );
		// template <class ChildrenType, class RoleType> ::Udm::ChildrenAttr<ChildrenType> children() const { ... }
		meth_defs_mpl_comp.push_back( boost::format("template <class ChildrenType, class RoleType> ::Udm::ChildrenAttr<ChildrenType> children() const { boost::function_requires< ::Udm::WithRoleInTListConcept_B<ChildrenType, RoleType, ChildrenMulti> >(); return ::Udm::ChildrenAttr<ChildrenType>(impl, _type2CCRole<RoleType>()); }") );
		// template <class ChildrenType, class RoleType, class Pred> ::Udm::ChildrenAttr<ChildrenType, Pred> children_sorted() const { ... }
		meth_defs_mpl_comp.push_back( boost::format("template <class ChildrenType, class RoleType, class Pred> ::Udm::ChildrenAttr<ChildrenType, Pred> children_sorted() const { boost::function_requires< ::Udm::WithRoleInTListConcept_B<ChildrenType, RoleType, ChildrenMulti> >(); return ::Udm::ChildrenAttr<ChildrenType, Pred>(impl, _type2CCRole<RoleType>()); }") );
		// template <class ChildrenType> ::Udm::ChildrenAttr<ChildrenType> children_kind() const { ... }
		meth_defs_mpl_comp.push_back( boost::format("template <class ChildrenType> ::Udm::ChildrenAttr<ChildrenType> children_kind() const { boost::function_requires< ::Udm::InTListConcept<ChildrenType, ChildrenKinds> >(); return ::Udm::ChildrenAttr<ChildrenType>(impl, ::Udm::NULLCHILDROLE); }") );
		// template <class ChildrenType, class Pred> ::Udm::ChildrenAttr<ChildrenType, Pred> children_kind_sorted() const { ... }
		meth_defs_mpl_comp.push_back( boost::format("template <class ChildrenType, class Pred> ::Udm::ChildrenAttr<ChildrenType, Pred> children_kind_sorted() const { boost::function_requires< ::Udm::InTListConcept<ChildrenType, ChildrenKinds> >(); return ::Udm::ChildrenAttr<ChildrenType, Pred>(impl, ::Udm::NULLCHILDROLE); }") );

		// 6. parents
		// template <class ParentType, class RoleType> ::Udm::ParentAttr<ParentType> parent() const { ... }
		meth_defs_mpl_comp.push_back( boost::format("template <class ParentType, class RoleType> ::Udm::ParentAttr<ParentType> parent() const { boost::function_requires< ::Udm::WithRoleInTListConcept<ParentType, RoleType, Parents> >(); return ::Udm::ParentAttr<ParentType>(impl, _type2CPRole<RoleType>()); }") );
		// template <class ParentType> ::Udm::ParentAttr<ParentType> parent_kind() const { ... }
		meth_defs_mpl_comp.push_back( boost::format("template <class ParentType> ::Udm::ParentAttr<ParentType> parent_kind() const { boost::function_requires< ::Udm::InTListConcept<ParentType, ParentKinds> >(); return ::Udm::ParentAttr<ParentType>(impl, ::Udm::NULLPARENTROLE); }") );

		// 7. association class ends
		// template <class EndType, class RoleType> ::Udm::AssocEndAttr<EndType> end() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class EndType, class RoleType> ::Udm::AssocEndAttr<EndType> end() const { boost::function_requires< ::Udm::WithRoleInTListConcept<EndType, RoleType, AClassEnds> >(); return ::Udm::AssocEndAttr<EndType>(impl, _type2ARole<RoleType>()); }") );

		// 8. cross diagram association class ends
		// template <class EndType, class RoleType> ::Udm::CrossAssocEndAttr<EndType> cross_end() const { ... }
		meth_defs_mpl_assoc.push_back( boost::format("template <class EndType, class RoleType> ::Udm::CrossAssocEndAttr<EndType> cross_end() const { boost::function_requires< ::Udm::WithRoleInTListConcept<EndType, RoleType, CrossAClassEnds> >(); return ::Udm::CrossAssocEndAttr<EndType>(impl, _type2ARole<RoleType>()); }") );

		// typedef for stereotype
		string stereotype = c.stereotype();
		if (stereotype != "Atom" && stereotype != "Connection"
			&& stereotype != "FCO" && stereotype != "Folder"
			&& stereotype != "Model" && stereotype != "Reference"
			&& stereotype != "Set")
			stereotype = "Unknown";
		typedefs.push_back( boost::format("typedef ::Udm::%1%MetaTag MetaKind") % stereotype );
	}
}


void ClassGen::Attributes()
{
	string cl_name = c.name();

	set< ::Uml::Attribute> attributes = c.attributes();
	for( set< ::Uml::Attribute>::iterator i = attributes.begin(); i != attributes.end(); i++)
	{
		Uml::Attribute attr = *i;
		string attr_name = attr.name();

		// static ::Uml::Attribute meta_d
		meta_decls.push_back( boost::format("static ::Uml::Attribute meta_%1%") % attr_name );
		// ::Uml::Attribute C::meta_d
		meta_defs.push_back( boost::format("::Uml::Attribute %2%::meta_%1%") % attr_name % cl_name );

		string attr_udm_type = (attr.nonpersistent() ? "Temp" : "")
			+ AttrType2Name( attr.type() )
			+ "Attr"
			+ (attr.max() == 1 || attr.max() == 0 ? "" : "Arr"); // array of values or single value
		// ::Udm::RealAttr d() const
		// ::Udm::TempRealAttr d() const
		// ::Udm::RealAttrArr d() const
		// ::Udm::TempRealAttrArr d() const
		meth_decls.push_back( boost::format("::Udm::%1% %2%() const") % attr_udm_type % attr_name );
		// ::Udm::RealAttr C::d() const { return ::Udm::RealAttr(impl, meta_d); }
		// ::Udm::TempRealAttr C::d() const { return ::Udm::TempRealAttr(impl, meta_d); }
		// ::Udm::RealAttrArr C::d() const { return ::Udm::RealAttrArr(impl, meta_d); }
		// ::Udm::TempRealAttrArr C::d() const { return ::Udm::TempRealAttrArr(impl, meta_d); }
		meth_defs.push_back( boost::format("::Udm::%1% %3%::%2%() const { return ::Udm::%1%(impl, meta_%2%); }")
				% attr_udm_type
				% attr_name
				% cl_name );

		meta_create.push_back( boost::format("%2%::meta_%1% = ::Uml::Attribute::Create(%2%::meta)") % attr_name % cl_name );

		if (attr.defvalue())
		{
			vector<boost::format> dva_fmt;

			dva_fmt.push_back( boost::format("vector<string> %2%_%1%_dva") % attr_name % cl_name );

			vector<string> dva = attr.defvalue();
			vector<string>::const_iterator dva_ci = dva.begin();
			while (dva_ci != dva.end())
			{
				dva_fmt.push_back( boost::format("%2%_%1%_dva.push_back(\"%3%\")") % attr_name % cl_name % *dva_ci );
				dva_ci++;
			}

			for (vector<boost::format>::const_iterator i = dva_fmt.begin(); i != dva_fmt.end(); i++)
			{
				meta_init.push_back( boost::format(*i) );
				old_meta_init.push_back( boost::format(*i) );
				corba_meta_init.push_back( boost::format(*i) );
			}
		}

		meta_init.push_back( boost::format("::Uml::InitAttributeProps(%2%::meta_%1%, \"%1%\", \"%3%\", %4%, %5%, %6%, %7%, %8%, \"%9%\", %10%)")
					% attr_name
					% cl_name
					% (string) attr.type()
					% (attr.nonpersistent() ? "true" : "false")
					% (attr.registry() ? "true" : "false")
					% attr.min()
					% attr.max()
					% (attr.ordered() ? "true" : "false")
					% (string) attr.visibility()
					% ( attr.defvalue() ? cl_name + "_" + attr_name + "_dva" : "vector<string>()" )
					);

		meta_init2.push_back( boost::format("::Uml::SetAttribute(%2%::meta_%1%, %2%::meta, \"%1%\")") % attr_name % cl_name );

		old_meta_create.push_back( boost::format("%2%::meta_%1% = ::Uml::CreateAttribute()") % attr_name % cl_name );

		corba_meta_create.push_back( boost::format("%2%::meta_%1% = ::Uml::CreateCORBAAttribute()") % attr_name % cl_name );

		old_meta_init.push_back( boost::format("::Uml::InitAttribute(%2%::meta_%1%, %2%::meta, \"%1%\", \"%3%\", %4%, %5%, %6%, %7%, %8%, \"%9%\", %10%)")
						% attr_name
						% cl_name
						% (string) attr.type()
						% (attr.nonpersistent() ? "true" : "false")
						% (attr.registry() ? "true" : "false")
						% attr.min()
						% attr.max()
						% (attr.ordered() ? "true" : "false")
						% (string) attr.visibility()
						% (attr.defvalue() ? cl_name + "_" + attr_name + "_dva" : "vector<string>()")
						);

		corba_meta_init.push_back( boost::format("::Uml::InitCORBAAttribute(%2%::meta_%1%, %2%::meta, \"%1%\", \"%3%\", %4%, %5%, %6%, %7%, \"%8%\", %9%)")
						% attr_name
						% cl_name
						% (string) attr.type()
						% (attr.nonpersistent() ? "true" : "false")
						% attr.min()
						% attr.max()
						% (attr.ordered() ? "true" : "false")
						% (string) attr.visibility()
						% (attr.defvalue() ? cl_name + "_" + attr_name + "_dva" : "vector<string>()")
						);
	}
}


void ClassGen::Associations(const ::Uml::Diagram &cross_dgr)
{
	string cl_name = c.name();

	set< ::Uml::AssociationRole> assocs = c.associationRoles();

#if 0
	if (assocs.size())
		meta_init_links.push_back( boost::format("set< ::Uml::AssociationRole> %1%_ar") % cl_name );
#endif

	// for typelist for associations without an association class
	TLHelper assoc_tlhlp;
	// for typelist for associations with an association class
	TLHelper assoc_ac_tlhlp;

	for( set< ::Uml::AssociationRole>::iterator i = assocs.begin(); i != assocs.end(); i++) 
	{
		::Uml::Association pp = (*i).parent();
		::Uml::Class aclass = pp.assocClass();

		::Uml::AssociationRole the_other = Uml::theOther(*i);

		string rel_name = ::Uml::MakeRoleName(the_other);
		::Uml::Class oclass = the_other.target();
		string oclass_cpp_name = UmlClassCPPName(oclass, c.parent_ns());

		//meta_init_links.push_back( boost::format("%1%_ar.insert(%2%::meta_%3%)") % cl_name % UmlClassCPPName(oclass, c.parent_ns()) % ::Uml::MakeRoleName(*i) );
		meta_init_links.push_back( boost::format("%1%::meta_%2%.target() = %3%::meta") % UmlClassCPPName(i->target(), c.parent_ns()) % rel_name % UmlClassCPPName(oclass, c.parent_ns()) );

		meta_init_links2.push_back( boost::format("::Uml::SetAssocRole(%1%::meta_%2%, %1%::meta, %3%::meta, \"%4%\")") % cl_name % rel_name % UmlClassCPPName(oclass, c.parent_ns()) % (string) i->name() );

		if (!aclass)
		{
			// static ::Uml::AssociationRole meta_A
			meta_decls.push_back( boost::format("static ::Uml::AssociationRole meta_%1%") % rel_name );
			// ::Uml::AssociationRole C::meta_A
			meta_defs.push_back( boost::format("::Uml::AssociationRole %2%::meta_%1%") % rel_name % cl_name );

			if (the_other.isNavigable()) 
			{
				string tl_typename = "_AR_" + rel_name + "__" + oclass.getPath2("_", false);

				if (the_other.max() == 1)
				{
					// ::Udm::PointerAttr<T> A() const
					meth_decls.push_back( boost::format("::Udm::PointerAttr< %1%> %2%() const") % oclass_cpp_name % rel_name );
					// ::Udm::PointerAttr<T> C::A() const { return ::Udm::PointerAttr<T>(impl, meta_A); }
					meth_defs.push_back( boost::format("::Udm::PointerAttr< %1%> %3%::%2%() const { return ::Udm::PointerAttr< %1%>(impl, meta_%2%); }") % oclass_cpp_name % rel_name % cl_name );

					if (gen.opts.mode == UdmOpts::CXX_GENERIC)
						assoc_tlhlp.typenames_single.push_back(tl_typename);
				}
				else
				{
					// ::Udm::AssocAttr<T> A() const
					meth_decls.push_back( boost::format("::Udm::AssocAttr< %1%> %2%() const") % oclass_cpp_name % rel_name );
					// ::Udm::AssocAttr<T> C::A() const { return ::Udm::AssocAttr<T>(impl, meta_A); }
					meth_defs.push_back( boost::format("::Udm::AssocAttr< %1%> %3%::%2%() const { return ::Udm::AssocAttr< %1%>(impl, meta_%2%); }") % oclass_cpp_name % rel_name % cl_name );

					// template <class Pred> ::Udm::AssocAttr<T, Pred> A_sorted(const Pred &) const { return ::Udm::AssocAttr<T, Pred>(impl, meta_A); }
					meth_decls.push_back( boost::format("template <class Pred> ::Udm::AssocAttr< %1%, Pred> %2%_sorted(const Pred &) const { return ::Udm::AssocAttr< %1%, Pred>(impl, meta_%2%); }") % oclass_cpp_name % rel_name );

					if (gen.opts.mode == UdmOpts::CXX_GENERIC)
						assoc_tlhlp.typenames_multi.push_back(tl_typename);
				}

				if (gen.opts.mode == UdmOpts::CXX_GENERIC)
				{
					assoc_tlhlp.roles2type.push_back( boost::format("class AR_%1% {}") % rel_name );
					assoc_tlhlp.typedefs.push_back( boost::format("typedef boost::mpl::pair< %2%, AR_%1% > %3%") % rel_name % oclass_cpp_name % tl_typename );

					meth_speclzs_mpl_assoc.push_back( boost::format("template <> const ::Uml::AssociationRole& %1%::_type2ARole< %1%::AR_%2% >() const { return meta_%2%; }") % cl_name % rel_name );
				}
			}
			else 
			{
				comments.push_back( boost::format("//Access method for non-navigable association %1% omitted") % rel_name );
			}
		}
		else 
		{
			string aclass_cpp_name = UmlClassCPPName(aclass, c.parent_ns());

			// static ::Uml::AssociationRole meta_A
			meta_decls.push_back( boost::format("static ::Uml::AssociationRole meta_%1%") % rel_name );
			// ::Uml::AssociationRole C::meta_A
			meta_defs.push_back( boost::format("::Uml::AssociationRole %2%::meta_%1%") % rel_name % cl_name );

			// static ::Uml::AssociationRole meta_A_rev
			meta_decls.push_back( boost::format("static ::Uml::AssociationRole meta_%1%_rev") % rel_name );
			// ::Uml::AssociationRole C::meta_A_rev
			meta_defs.push_back( boost::format("::Uml::AssociationRole %2%::meta_%1%_rev") % rel_name % cl_name );

			// set the _rev's and _end_'s
			meta_init_links2.push_back( boost::format("%1%::meta_%2%_end_ = %3%::meta_%4%_rev = %5%::meta_%6%") % aclass_cpp_name % rel_name % oclass_cpp_name % (string) i->name() % cl_name % rel_name );

			if (the_other.isNavigable()) 
			{
				string tl_typename = "_AR_" + rel_name + "__" + aclass.getPath2("_", false) + "__" + oclass.getPath2("_", false);

				if (the_other.max() == 1)
				{
					// ::Udm::AClassPointerAttr<T1, T2> A()
					meth_decls.push_back( boost::format("::Udm::AClassPointerAttr< %1%, %2%> %3%() const") % aclass_cpp_name % oclass_cpp_name % rel_name );
					// ::Udm::AClassPointerAttr<T1, T2> C::A() const { return ::Udm::AClassPointerAttr<T1, T2>(impl, meta_A, meta_A_rev);  }
					meth_defs.push_back( boost::format("::Udm::AClassPointerAttr< %1%, %2%> %4%::%3%() const { return ::Udm::AClassPointerAttr< %1%, %2%>(impl, meta_%3%, meta_%3%_rev); }") % aclass_cpp_name % oclass_cpp_name % rel_name % cl_name );

					if (gen.opts.mode == UdmOpts::CXX_GENERIC)
						assoc_ac_tlhlp.typenames_single.push_back(tl_typename);
				}
				else
				{	
					// ::Udm::AClassAssocAttr<T1, T2> A() const
					meth_decls.push_back( boost::format("::Udm::AClassAssocAttr< %1%, %2%> %3%() const") % aclass_cpp_name % oclass_cpp_name % rel_name );
					// ::Udm::AClassAssocAttr<T1, T2> C::A() const { return ::Udm::AClassAssocAttr<T1, T2>(impl, meta_A, meta_A_rev); }
					meth_defs.push_back( boost::format("::Udm::AClassAssocAttr< %1%, %2%> %4%::%3%() const { return ::Udm::AClassAssocAttr< %1%, %2%>(impl, meta_%3%, meta_%3%_rev); }") % aclass_cpp_name % oclass_cpp_name % rel_name % cl_name );
					// template <class Pred> ::Udm::AClassAssocAttr<T1, T2> A_sorted(const Pred &) const { return ::Udm::AClassAssocAttr<T1, T2, Pred>(impl, meta_A, meta_A_rev); }
					meth_decls.push_back( boost::format("template <class Pred> ::Udm::AClassAssocAttr< %1%, %2%, Pred> %3%_sorted(const Pred &) const { return ::Udm::AClassAssocAttr< %1%, %2%, Pred>(impl, meta_%3%, meta_%3%_rev); }") % aclass_cpp_name % oclass_cpp_name % rel_name );

					if (gen.opts.mode == UdmOpts::CXX_GENERIC)
						assoc_ac_tlhlp.typenames_multi.push_back(tl_typename);
				}

				if (gen.opts.mode == UdmOpts::CXX_GENERIC)
				{
					assoc_ac_tlhlp.roles2type.push_back( boost::format("class AR_%1% {}") % rel_name );
					assoc_ac_tlhlp.typedefs.push_back( boost::format("typedef boost::mpl::pair< %3%, boost::mpl::pair< %2%, AR_%1% > > %4%") % rel_name % aclass_cpp_name % oclass_cpp_name % tl_typename );

					meth_speclzs_mpl_assoc.push_back( boost::format("template <> const std::pair<const ::Uml::AssociationRole*, const ::Uml::AssociationRole*> %1%::_type2ACARole< %1%::AR_%2% >() const { return std::make_pair(&meta_%2%, &meta_%2%_rev); }") % cl_name % rel_name );
				}
			}
			else 
			{
				comments.push_back( boost::format("//Access method for non-navigable association %1% omitted") % rel_name );
			}
		}
	}

	if (gen.opts.mode == UdmOpts::CXX_GENERIC)
	{
		BuildTL(assoc_tlhlp, "AssociationsSingle", "AssociationsMulti", "associations by returned type and role", associations_tl);
		BuildTL(assoc_ac_tlhlp, "AssociationsWAClassSingle", "AssociationsWAClassMulti", "associations by returned type, association class and role", associations_ac_tl);
	}


#if 0
	if (assocs.size())
		meta_init_links.push_back( boost::format("%1%::meta.associationRoles() = %1%_ar") % cl_name );
#endif

	// for typelist for cross associations without an association class
	TLHelper cross_assoc_tlhlp;
	// for typelist for cross associations with an association class
	TLHelper cross_assoc_ac_tlhlp;

	::Uml::Class cross_cl;

	if (cross_dgr && (cross_dgr != ::Uml::GetDiagram(c)))
		cross_cl = ::Uml::GetClassFromCrossDgr(cross_dgr, c);

	if (cross_cl)
	{
		set< ::Uml::AssociationRole> assocs = cross_cl.associationRoles();
		for( set< ::Uml::AssociationRole>::iterator i = assocs.begin(); i != assocs.end(); i++) 
		{
			::Uml::Association pp = (*i).parent();
			::Uml::Class aclass = pp.assocClass();

			::Uml::AssociationRole the_other = Uml::theOther(*i);

			string rel_name = ::Uml::MakeRoleName(the_other);

			string oname = ((::Uml::Class) the_other.target()).name();
			//Attention !!!
			oname = oname.substr(0, oname.find(Udm::cross_delimiter));
			//
			string cname = GetCPPNamefromFromStr(((::Uml::Class)the_other.target()).from());

			if (!aclass)
			{
				// static ::Uml::AssociationRole meta_A
				meta_decls.push_back( boost::format("static ::Uml::AssociationRole meta_%1%") % rel_name );
				// ::Uml::AssociationRole C::meta_A
				meta_defs.push_back( boost::format("::Uml::AssociationRole %2%::meta_%1%") % rel_name % cl_name );

				meta_init_links.push_back( boost::format("%2%::meta_%1% = %3%::meta_%1%") % rel_name % cl_name % UmlClassCPPName(cross_cl) );

				meta_init_links2.push_back( boost::format("%2%::meta_%1% = %3%::meta_%1%") % rel_name % cl_name % UmlClassCPPName(cross_cl) );

				old_meta_init_links.push_back( boost::format("%2%::meta_%1% = %3%::meta_%1%") % rel_name % cl_name % UmlClassCPPName(cross_cl) );

				corba_meta_init_links.push_back( boost::format("%2%::meta_%1% = %3%::meta_%1%") % rel_name % cl_name % UmlClassCPPName(cross_cl) );

				if (the_other.isNavigable()) 
				{
					string tl_typename = "_AR_" + rel_name;
					{
						string s = cname + "::" + oname;
						boost::replace_all(s, "::", "_");
						tl_typename += "__" + s;
					}

					if (the_other.max() == 1 )
					{
						// ::Udm::CrossPointerAttr<T> A() const
						meth_decls.push_back( boost::format("::Udm::CrossPointerAttr< %1%> %2%() const") % (cname + "::" + oname) % rel_name );
						// ::Udm::CrossPointerAttr<T> C::A() const { return ::Udm::CrossPointerAttr<T>(impl, meta_A); }
						meth_defs.push_back( boost::format("::Udm::CrossPointerAttr< %1%> %3%::%2%() const { return ::Udm::CrossPointerAttr< %1%>(impl, meta_%2%); }") % (cname + "::" + oname) % rel_name % cl_name );

						if (gen.opts.mode == UdmOpts::CXX_GENERIC)
							cross_assoc_tlhlp.typenames_single.push_back(tl_typename);
					}
					else
					{
						// ::Udm::CrossAssocAttr<T> A() const
						meth_decls.push_back( boost::format("::Udm::CrossAssocAttr< %1%> %2%() const") % (cname + "::" + oname) % rel_name );
						// ::Udm::CrossAssocAttr<T> C::A() const { return ::Udm::CrossAssocAttr<T>(impl, meta_A); }
						meth_defs.push_back( boost::format("::Udm::CrossAssocAttr< %1%> %3%::%2%() const { return ::Udm::CrossAssocAttr< %1%>(impl, meta_%2%); }") % (cname + "::" + oname) % rel_name % cl_name );
						// template <class Pred> ::Udm::CrossAssocAttr<T, Pred> A_sorted(const Pred &) const { return ::Udm::CrossAssocAttr<T, Pred>(impl, meta_A); }
						meth_decls.push_back( boost::format("template <class Pred> ::Udm::CrossAssocAttr< %1%, Pred> %2%_sorted(const Pred &) const { return ::Udm::CrossAssocAttr< %1%, Pred>(impl, meta_%2%); }") % (cname + "::" + oname) % rel_name );

						if (gen.opts.mode == UdmOpts::CXX_GENERIC)
							cross_assoc_tlhlp.typenames_multi.push_back(tl_typename);
					}

					if (gen.opts.mode == UdmOpts::CXX_GENERIC)
					{
						cross_assoc_tlhlp.roles2type.push_back( boost::format("class AR_%1% {}") % rel_name );
						cross_assoc_tlhlp.typedefs.push_back( boost::format("typedef boost::mpl::pair< %2%, AR_%1% > %3%") % rel_name % (cname + "::" + oname) % tl_typename);

						meth_speclzs_mpl_assoc.push_back( boost::format("template <> const ::Uml::AssociationRole& %1%::_type2ARole< %1%::AR_%2% >() const { return meta_%2%; }") % cl_name % rel_name );
					}
				}
				else 
				{
					comments.push_back( boost::format("//Access method for non-navigable association %1% omitted") % rel_name );
				}
			}
			else 
			{
				string clname = aclass.name();
				//Attention !!!
				clname = clname.substr(0, clname.find(Udm::cross_delimiter));
				//end of dangerous zone
	
				string cl_dgr = GetCPPNamefromFromStr(aclass.from());

				// static ::Uml::AssociationRole meta_A
				meta_decls.push_back( boost::format("static ::Uml::AssociationRole meta_%1%") % rel_name );
				// ::Uml::AssociationRole C::meta_A
				meta_defs.push_back( boost::format("::Uml::AssociationRole %2%::meta_%1%") % rel_name % cl_name );

				meta_init_links.push_back( boost::format("%2%::meta_%1% = %3%::meta_%1%") % rel_name % cl_name % UmlClassCPPName(cross_cl) );

				old_meta_init_links.push_back( boost::format("%2%::meta_%1% = %3%::meta_%1%") % rel_name % cl_name % UmlClassCPPName(cross_cl) );

				corba_meta_init_links.push_back( boost::format("%2%::meta_%1% = %3%::meta_%1%") % rel_name % cl_name % UmlClassCPPName(cross_cl) );

				// static ::Uml::AssociationRole meta_A_rev
				meta_decls.push_back( boost::format("static ::Uml::AssociationRole meta_%1%_rev") % rel_name );
				// ::Uml::AssociationRole C::meta_A_rev
				meta_defs.push_back( boost::format("::Uml::AssociationRole %2%::meta_%1%_rev") % rel_name % cl_name );

				meta_init_links.push_back( boost::format("%2%::meta_%1%_rev = %3%::meta_%4%") % rel_name % cl_name % UmlClassCPPName(the_other.target()) % (string) i->name() );

				old_meta_init_links.push_back( boost::format("%2%::meta_%1%_rev = %3%::meta_%4%") % rel_name % cl_name % UmlClassCPPName(the_other.target()) % (string) i->name() );

				corba_meta_init_links.push_back( boost::format("%2%::meta_%1%_rev = %3%::meta_%4%") % rel_name % cl_name % UmlClassCPPName(the_other.target()) % (string) i->name() );


				if (the_other.isNavigable()) 
				{
					string tl_typename = "_AR_" + rel_name;
					{
						string s = cl_dgr + "::" + clname;
						boost::replace_all(s, "::", "_");
						tl_typename += "__" + s;

						s = cname + "::" + oname;
						boost::replace_all(s, "::", "_");
						tl_typename += "__" + s;
					}

					if (the_other.max() == 1)
					{
						// ::Udm::AClassCrossPointerAttr<T1, T2> A() const
						meth_decls.push_back( boost::format("::Udm::AClassCrossPointerAttr< %1%, %2%> %3%() const") % (cl_dgr + "::" + clname) % (cname + "::" + oname) % rel_name );
						// ::Udm::AClassCrossPointerAttr<T1, T2> C::A() const { return ::Udm::AClassCrossPointerAttr<T1, T2>(impl, meta_A, meta_A_rev); }
						meth_defs.push_back( boost::format("::Udm::AClassCrossPointerAttr< %1%, %2%> %4%::%3%() const { return ::Udm::AClassCrossPointerAttr< %1%, %2%>(impl, meta_%3%, meta_%3%_rev); }") % (cl_dgr + "::" + clname) % (cname + "::" + oname) % rel_name % cl_name );

						if (gen.opts.mode == UdmOpts::CXX_GENERIC)
							cross_assoc_ac_tlhlp.typenames_single.push_back(tl_typename);
					}
					else
					{
						// ::Udm::AClassCrossAssocAttr<T1, T2> A() const
						meth_decls.push_back( boost::format("::Udm::AClassCrossAssocAttr< %1%, %2%> %3%() const") % (cl_dgr + "::" + clname) % (cname + "::" + oname) % rel_name );
						// ::Udm::AClassCrossAssocAttr<T1, T2> C::A() const { return ::Udm::AClassCrossAssocAttr<T1, T2>(impl, meta_A, meta_A_rev); }
						meth_defs.push_back( boost::format("::Udm::AClassCrossAssocAttr< %1%, %2%> %4%::%3%() const { return ::Udm::AClassCrossAssocAttr< %1%, %2%>(impl, meta_%3%, meta_%3%_rev); }") % (cl_dgr + "::" + clname) % (cname + "::" + oname) % rel_name % cl_name );
						// template <class Pred> ::Udm::AClassCrossAssocAttr<T1, T2, Pred> A_sorted(const Pred &) const { return ::Udm::AClassCrossPointerAttr<T1, T2, Pred>(impl, meta_A, meta_A_rev); }
						meth_decls.push_back( boost::format("template <class Pred> ::Udm::AClassCrossAssocAttr< %1%, %2%, Pred> %3%_sorted(const Pred &) const { return ::Udm::AClassCrossAssocAttr< %1%, %2%, Pred>(impl, meta_%3%, meta_%3%_rev); }") % (cl_dgr + "::" + clname) % (cname + "::" + oname) % rel_name );

						if (gen.opts.mode == UdmOpts::CXX_GENERIC)
							cross_assoc_ac_tlhlp.typenames_multi.push_back(tl_typename);
					}

					if (gen.opts.mode == UdmOpts::CXX_GENERIC)
					{
						cross_assoc_ac_tlhlp.roles2type.push_back( boost::format("class AR_%1% {}") % rel_name );
						cross_assoc_ac_tlhlp.typedefs.push_back( boost::format("typedef boost::mpl::pair< %3%, boost::mpl::pair< %2%, AR_%1% > > %4%") % rel_name % (cl_dgr + "::" + clname) % (cname + "::" + oname) % tl_typename);

						meth_speclzs_mpl_assoc.push_back( boost::format("template <> const std::pair<const ::Uml::AssociationRole*, const ::Uml::AssociationRole*> %1%::_type2ACARole< %1%::AR_%2% >() const { return std::make_pair(&meta_%2%, &meta_%2%_rev); }") % cl_name % rel_name );
					}
				}
				else 
				{
					comments.push_back( boost::format("//Access method for non-navigable association %1% omitted") % rel_name );
				}
			}
		}
	}

	if (gen.opts.mode == UdmOpts::CXX_GENERIC)
	{
		BuildTL(cross_assoc_tlhlp, "CrossAssociationsSingle", "CrossAssociationsMulti", "cross associations by returned type and role", cross_associations_tl);
		BuildTL(cross_assoc_ac_tlhlp, "CrossAssociationsWAClassSingle", "CrossAssociationsWAClassMulti", "cross associations by returned type, association class and role", cross_associations_ac_tl);
	}
}


void ClassGen::Children()
{
	string cl_name = c.name();

	set< ::Uml::Class> childrenkinds;
	set< ::Uml::CompositionParentRole> children = c.parentRoles();

#if 0
	if (children.size())
		meta_init_links.push_back( boost::format("set< ::Uml::CompositionParentRole> %1%_cpr") % cl_name );
#endif

	// for typelist
	TLHelper children_tlhlp;

	for( set< ::Uml::CompositionParentRole>::iterator i = children.begin(); i != children.end(); i++) 
	{
		::Uml::CompositionChildRole the_other = Uml::theOther(*i);

		::Uml::Class thischildkind = (::Uml::Class)the_other.target();
		childrenkinds.insert(thischildkind);

		string rel_name = ::Uml::MakeRoleName(the_other);
		string child_name = UmlClassCPPName(thischildkind);

		// static ::Uml::CompositionChildRole meta_R
		meta_decls.push_back( boost::format("static ::Uml::CompositionChildRole meta_%1%") % rel_name );
		// ::Uml::CompositionChildRole C::meta_R
		meta_defs.push_back( boost::format("::Uml::CompositionChildRole %2%::meta_%1%") % rel_name % cl_name );

		//meta_init_links.push_back( boost::format("%1%_cpr.insert(%2%::meta_%3%)") % cl_name % UmlClassCPPName(thischildkind, c.parent_ns()) % ::Uml::MakeRoleName(*i) );
		meta_init_links.push_back( boost::format("%2%::meta_%3%.target() = %1%::meta") % cl_name % UmlClassCPPName(thischildkind, c.parent_ns()) % ::Uml::MakeRoleName(*i) );

		meta_init_links2.push_back( boost::format("::Uml::SetChildRole(%1%::meta_%2%, %1%::meta, %3%::meta, \"%4%\", \"%5%\")") % cl_name % rel_name % UmlClassCPPName(thischildkind, c.parent_ns()) % (string) i->name() % (string) the_other.name() );

		if (the_other.isNavigable()) 
		{
			string tl_typename = "_CR_" + rel_name + "__" + thischildkind.getPath2("_", false);

			if (the_other.max() == 1 )
			{
				// ::Udm::ChildAttr<T> R() const
				meth_decls.push_back( boost::format("::Udm::ChildAttr< %1%> %2%() const") % child_name % rel_name );
				// ::Udm::ChildAttr<T> C::R() const { return ::Udm::ChildAttr<T>(impl, meta_R); }
				meth_defs.push_back( boost::format("::Udm::ChildAttr< %1%> %3%::%2%() const { return ::Udm::ChildAttr< %1%>(impl, meta_%2%); }") % child_name % rel_name % cl_name );

				if (gen.opts.mode == UdmOpts::CXX_GENERIC)
					children_tlhlp.typenames_single.push_back(tl_typename);
			}
			else
			{
				// ::Udm::ChildrenAttr<T> R() const
				meth_decls.push_back( boost::format("::Udm::ChildrenAttr< %1%> %2%() const") % child_name % rel_name );
				// ::Udm::ChildrenAttr<T> C::R() const { return ::Udm::ChildrenAttr<T>(impl, meta_R); }
				meth_defs.push_back( boost::format("::Udm::ChildrenAttr< %1%> %3%::%2%() const { return ::Udm::ChildrenAttr< %1%>(impl, meta_%2%); }") % child_name % rel_name % cl_name );
				// template <class Pred> ::Udm::ChildrenAttr<T, Pred> R_sorted(const Pred &) const { return ::Udm::ChildrenAttr<T, Pred>(impl, meta_R); }
				meth_decls.push_back( boost::format("template <class Pred> ::Udm::ChildrenAttr< %1%, Pred> %2%_sorted(const Pred &) const { return ::Udm::ChildrenAttr< %1%, Pred>(impl, meta_%2%); }") % child_name % rel_name );

				if (gen.opts.mode == UdmOpts::CXX_GENERIC)
					children_tlhlp.typenames_multi.push_back(tl_typename);
			}

			if (gen.opts.mode == UdmOpts::CXX_GENERIC)
			{
				children_tlhlp.roles2type.push_back( boost::format("class CR_%1% {}") % rel_name );
				children_tlhlp.typedefs.push_back( boost::format("typedef boost::mpl::pair< %2%, CR_%1% > %3%") % rel_name % child_name % tl_typename );

				meth_speclzs_mpl_comp.push_back( boost::format("template <> const ::Uml::CompositionChildRole& %1%::_type2CCRole< %1%::CR_%2% >() const { return meta_%2%; }") % cl_name % rel_name );
			}
		}
		else 
		{
			comments.push_back( boost::format("//Access method for non-navigable association %1% omitted") % rel_name );
		}
	}

	if (gen.opts.mode == UdmOpts::CXX_GENERIC)
		BuildTL(children_tlhlp, "ChildrenSingle", "ChildrenMulti", "children by returned type and role", children_tl);


#if 0
	if (children.size())
		meta_init_links.push_back( boost::format("%1%::meta.parentRoles() = %1%_cpr") % cl_name );
#endif

	set< ::Uml::Class> childrenkinds_for_tl;
	set< ::Uml::CompositionChildRole> childroles = ::Uml::AncestorCompositionPeerChildRoles(c);
	for( set< ::Uml::CompositionChildRole>::const_iterator ci = childroles.begin(); ci != childroles.end(); ++ci)
	{
		set< ::Uml::Class> descendants = Uml::DescendantClasses(ci->target());

		if (gen.opts.mode == UdmOpts::CXX_GENERIC)
			childrenkinds_for_tl.insert(descendants.begin(), descendants.end());
	}
	if (gen.opts.mode == UdmOpts::CXX_GENERIC)
		BuildTL(childrenkinds_for_tl, "ChildrenKinds", "children by returned type", children_kinds_tl, true);

	::Uml::Diagram dgr = ::Uml::GetDiagram(c);
	::Uml::Namespace ns = c.parent_ns();
	::Uml::DiagramClasses allclasses = ::Uml::DiagramClasses(dgr);

	for (std::vector< ::Uml::Class>::const_iterator j = this->gen.DiagramClasses.begin(); j != this->gen.DiagramClasses.end(); j++)
	{
		for (set< ::Uml::Class>::iterator k = childrenkinds.begin(); k != childrenkinds.end(); k++) 
		{
			if (gen.isDerivedFrom(*j, *k) || gen.isDerivedFrom(*k, *j))
			{
				string kind_children_name;
				if (ns != j->parent_ns())
					kind_children_name = j->getPath2("_", false);
				else
					kind_children_name = j->name();

				string kind_children_cpp = UmlClassCPPName(*j);

				// ::Udm::ChildrenAttr<T> R_kind_children() const
				meth_decls.push_back( boost::format("::Udm::ChildrenAttr< %1%> %2%_kind_children() const") % kind_children_cpp % kind_children_name );
				// ::Udm::ChildrenAttr<T> C::R_kind_children() const { return ::Udm::ChildrenAttr<T>(impl, ::Udm::NULLCHILDROLE); }
				meth_defs.push_back( boost::format("::Udm::ChildrenAttr< %1%> %3%::%2%_kind_children() const { return ::Udm::ChildrenAttr< %1%>(impl, ::Udm::NULLCHILDROLE); }") % kind_children_cpp % kind_children_name % cl_name );
				// template <class Pred> ::Udm::ChildrenAttr<T, Pred> R_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr<T, Pred>(impl, ::Udm::NULLCHILDROLE); }
				meth_decls.push_back( boost::format("template <class Pred> ::Udm::ChildrenAttr< %1%, Pred> %2%_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< %1%, Pred>(impl, ::Udm::NULLCHILDROLE); }") % kind_children_cpp % kind_children_name );

				break;
			}
		}
	}
}

void ClassGen::Parents()
{
	string cl_name = c.name();

	set< ::Uml::CompositionChildRole> parents = c.childRoles();

#if 0
	if (parents.size())
		meta_init_links.push_back( boost::format("set< ::Uml::CompositionChildRole> %1%_ccr") % cl_name );
#endif

	// for typelist
	TLHelper parents_tlhlp;

	for( set< ::Uml::CompositionChildRole>::iterator i = parents.begin(); i != parents.end(); i++) 
	{
		::Uml::CompositionParentRole the_other = Uml::theOther(*i);
		::Uml::Class parent = the_other.target();

		string rel_name = ::Uml::MakeRoleName(the_other);
		string parent_name = UmlClassCPPName(parent);

		// static ::Uml::CompositionParentRole meta_R
		meta_decls.push_back( boost::format("static ::Uml::CompositionParentRole meta_%1%") % rel_name );
		// ::Uml::CompositionParentRole C::meta_R
		meta_defs.push_back( boost::format("::Uml::CompositionParentRole %2%::meta_%1%") % rel_name % cl_name );

		//meta_init_links.push_back( boost::format("%1%_ccr.insert(%2%::meta_%3%)") % cl_name % UmlClassCPPName(parent, c.parent_ns()) % ::Uml::MakeRoleName(*i) );
		meta_init_links.push_back( boost::format("%2%::meta_%3%.target() = %1%::meta") % cl_name % UmlClassCPPName(parent, c.parent_ns()) % ::Uml::MakeRoleName(*i) );

		meta_init_links2.push_back( boost::format("::Uml::SetParentRole(%1%::meta_%2%, %1%::meta, %3%::meta, \"%4%\", \"%5%\")") % cl_name % rel_name % UmlClassCPPName(parent, c.parent_ns()) % (string) i->name() % (string) the_other.name() );

		if (the_other.isNavigable()) 
		{
			// ::Udm::ParentAttr<T> R() const
			meth_decls.push_back( boost::format("::Udm::ParentAttr< %1%> %2%() const") % parent_name % rel_name );
			// ::Udm::ParentAttr<T> C::R() const { return ::Udm::ParentAttr<T>(impl, meta_R); }
			// FIXME this is slow in StaticDataNetwork if there's only one child role, since it needs to get all the children of the parent
			meth_defs.push_back( boost::format("::Udm::ParentAttr< %1%> %3%::%2%() const { return ::Udm::ParentAttr< %1%>(impl, meta_%2%); }") % parent_name % rel_name % cl_name );

			if (gen.opts.mode == UdmOpts::CXX_GENERIC)
			{
				parents_tlhlp.roles2type.push_back( boost::format("class PR_%1% {}") % rel_name );
				string tl_typename = "_PR_" + rel_name + "__" + parent.getPath2("_", false);
				parents_tlhlp.typenames_single.push_back(tl_typename);
				parents_tlhlp.typedefs.push_back( boost::format("typedef boost::mpl::pair< %2%, PR_%1% > %3%") % rel_name % parent_name % tl_typename );

				meth_speclzs_mpl_comp.push_back( boost::format("template <> const ::Uml::CompositionParentRole& %1%::_type2CPRole< %1%::PR_%2% >() const { return meta_%2%; }") % cl_name % rel_name );
			}
		}
		else 
		{
			comments.push_back( boost::format("//Access method for non-navigable association %1% omitted") % rel_name );
		}
	}

	if (gen.opts.mode == UdmOpts::CXX_GENERIC)
		BuildTL(parents_tlhlp, "Parents", "", "parent by returned type and role", parents_tl);


#if 0
	if (parents.size())
		meta_init_links.push_back( boost::format("%1%::meta.childRoles() = %1%_ccr") % cl_name );
#endif

	set< ::Uml::Class> parentkinds_for_tl;


	bool parent_defined = false;
	set< ::Uml::CompositionParentRole> parroles = Uml::AncestorCompositionPeerParentRoles(c);
	for( set< ::Uml::CompositionParentRole>::iterator ri = parroles.begin(); ri != parroles.end(); ri++) 
	{
		if (gen.opts.mode == UdmOpts::CXX_GENERIC)
			parentkinds_for_tl.insert(ri->target());

		if(string(ri->name()) == "parent")
		{
			parent_defined = true;
			break;
		}
	}

	if (gen.opts.mode == UdmOpts::CXX_GENERIC)
		BuildTL(parentkinds_for_tl, "ParentKinds", "parent by returned type", parent_kinds_tl);
	
	if(!parent_defined) 
	{
		// see also UDM-72
		set< ::Uml::Class> ancestors = Uml::AncestorClasses(c);
		set< ::Uml::Class> descendants = Uml::DescendantClasses(c);
		set< ::Uml::Class> ancestorsAndDescendants = ancestors;
		ancestorsAndDescendants.insert(descendants.begin(), descendants.end());
		set< ::Uml::Class> containers;
		for (set< ::Uml::Class>::iterator classIt = ancestorsAndDescendants.begin(); 
			classIt != ancestorsAndDescendants.end(); classIt++)
		{
			set< ::Uml::Class> containerClasses = Uml::ContainerClasses(*classIt);
			containers.insert(containerClasses.begin(), containerClasses.end());
		}

		set< ::Uml::Class> ans = Uml::CommonAncestorClasses(containers);
		if (ans.size() == 1)
		{
			::Uml::Class ans_class = (::Uml::Class)(*ans.begin());
			// ::Udm::ParentAttr<T> parent() const
			meth_decls.push_back( boost::format("::Udm::ParentAttr< %1%> parent() const") % UmlClassCPPName(ans_class) );
			// ::Udm::ParentAttr<T> C::parent() const { return ::Udm::ParentAttr<T>(impl, ::Udm::NULLPARENTROLE); }
			meth_defs.push_back( boost::format("::Udm::ParentAttr< %1%> %2%::parent() const { return ::Udm::ParentAttr< %1%>(impl, ::Udm::NULLPARENTROLE); }") % UmlClassCPPName(ans_class) % cl_name );
		} else {
			// ::Udm::ParentAttr< ::Udm::Object> parent() const
			meth_decls.push_back( boost::format("::Udm::ParentAttr< ::Udm::Object> parent() const") );
			// ::Udm::ParentAttr< ::Udm::Object> C::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }
			meth_defs.push_back( boost::format("::Udm::ParentAttr< ::Udm::Object> %1%::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }") % cl_name );

			for(set< ::Uml::Class>::iterator ccc = ans.begin(); ccc != ans.end(); ccc++) 
			{
				// ::Udm::ParentAttr<T> T_parent() const
				meth_decls.push_back( boost::format("::Udm::ParentAttr< %1%> %2%_parent() const") % UmlClassCPPName(*ccc) % (string) ccc->name() );
				// ::Udm::ParentAttr<T> C::T_parent() const { return ::Udm::ParentAttr<T>(impl, ::Udm::NULLPARENTROLE); }
				meth_defs.push_back( boost::format("::Udm::ParentAttr< %1%> %3%::%2%_parent() const { return ::Udm::ParentAttr< %1%>(impl, ::Udm::NULLPARENTROLE); }") % UmlClassCPPName(*ccc) % (string) ccc->name() % cl_name );
			}
		}
	}
}


void ClassGen::AssocEnds(const ::Uml::Diagram &cross_dgr)
{
	string cl_name = c.name();

	// for typelist
	TLHelper aclass_tlhlp;

	::Uml::Association assoc = c.association();
	if(assoc) 
	{
		set< ::Uml::AssociationRole> assocs = assoc.roles();

		// connect to the association (by using the parent() of first association role)
		::Uml::AssociationRole one_role = *assocs.begin();
		meta_init_links.push_back( boost::format("%1%::meta.association() = %2%::meta_%3%.parent()") % cl_name % UmlClassCPPName(one_role.target(), c.parent_ns()) % ::Uml::MakeRoleName(::Uml::theOther(one_role)) );

		for( set< ::Uml::AssociationRole>::iterator i = assocs.begin(); i != assocs.end(); i++) 
		{
			string rel_name = ::Uml::MakeRoleName(*i);
			UDM_ASSERT(rel_name.size() > 0);

			::Uml::AssociationRole the_other = ::Uml::theOther(*i);
			string orel_name = ::Uml::MakeRoleName(the_other);
			string otclass_cpp_name = UmlClassCPPName(the_other.target());

			::Uml::Class tclass = (::Uml::Class)(*i).target();
			string tclass_cpp_name = UmlClassCPPName(tclass);

			if (rel_name.size()) 
			{
				// static ::Uml::AssociationRole meta_R_end_
				meta_decls.push_back( boost::format("static ::Uml::AssociationRole meta_%1%_end_") % rel_name );

				// ::Uml::AssociationRole C::meta_R_end_
				meta_defs.push_back( boost::format("::Uml::AssociationRole %2%::meta_%1%_end_") % rel_name % cl_name );

				// ::Udm::AssocEndAttr<T> R_end() const
				meth_decls.push_back( boost::format("::Udm::AssocEndAttr< %1%> %2%_end() const") % tclass_cpp_name % rel_name );
				// ::Udm::AssocEndAttr<T> C::R_end() const { return ::Udm::AssocEndAttr<T>(impl, meta_R_end_); }
				meth_defs.push_back( boost::format("::Udm::AssocEndAttr< %1%> %3%::%2%_end() const { return ::Udm::AssocEndAttr< %1%>(impl, meta_%2%_end_); }") % tclass_cpp_name % rel_name % cl_name );

				// association_class::meta_OTHER_ROLE_end_ = OTHER_ROLE_target::meta_ROLE_rev = ROLE_target::meta_OTHER_ROLE
				meta_init_links.push_back( boost::format("%1%::meta_%2%_end_ = %3%::meta_%4%_rev = %5%::meta_%6%") % cl_name % orel_name % UmlClassCPPName(the_other.target(), c.parent_ns()) % rel_name % UmlClassCPPName(tclass, c.parent_ns()) % orel_name );

				::Uml::AssociationRole orp_helper = the_other.rp_helper();
				if (orp_helper) {
					typedefs.push_back( boost::format("typedef std::pair< %1%, std::vector< ::Udm::Object> > %2%_chain_t") % tclass_cpp_name % rel_name );
					meth_decls.push_back( boost::format("::Udm::AssocEndChainAttr< %1%, %2%_chain_t > %2%_chain() const") % tclass_cpp_name % rel_name );
					meth_defs.push_back( boost::format("::Udm::AssocEndChainAttr< %1%, %3%::%2%_chain_t > %3%::%2%_chain() const { return ::Udm::AssocEndChainAttr< %1%, %3%::%2%_chain_t >(impl, meta_%2%_end_); }") % tclass_cpp_name % rel_name % cl_name );

					string orp_helper_name = ::Uml::MakeRoleName(orp_helper);
					meta_init_links.push_back( boost::format("%1%::meta_%2%_end_.rp_helper() = %1%::meta_%3%") % cl_name % orel_name % orp_helper_name );
				}

				if (gen.opts.mode == UdmOpts::CXX_GENERIC)
				{
					aclass_tlhlp.roles2type.push_back( boost::format("class ACE_%1% {}") % rel_name );
					string tl_typename = "_ACE_" + rel_name + "__" + tclass.getPath2("_", false);
					aclass_tlhlp.typenames_single.push_back(tl_typename);
					aclass_tlhlp.typedefs.push_back( boost::format("typedef boost::mpl::pair< %2%, ACE_%1% > %3%") % rel_name % tclass_cpp_name % tl_typename );

					meth_speclzs_mpl_assoc.push_back( boost::format("template <> const ::Uml::AssociationRole& %1%::_type2ARole< %1%::ACE_%2% >() const { return meta_%2%_end_; }") % cl_name % rel_name );
				}
			}

		}
	}

	if (gen.opts.mode == UdmOpts::CXX_GENERIC)
		BuildTL(aclass_tlhlp, "AClassEnds", "", "association class ends by returned type and role", aclass_ends_tl);


	// for typelist
	TLHelper cross_aclass_tlhlp;

	::Uml::Class cross_cl;

	if (cross_dgr && (cross_dgr != ::Uml::GetDiagram(c)))
		cross_cl = ::Uml::GetClassFromCrossDgr(cross_dgr, c);

	if (cross_cl)
	{
		::Uml::Association cross_cl_assoc = cross_cl.association();
		if (cross_cl_assoc)
		{
			set< ::Uml::AssociationRole> assocs = cross_cl_assoc.roles();
			for( set< ::Uml::AssociationRole>::iterator i = assocs.begin(); i != assocs.end(); i++) 
			{
				string rel_name = ::Uml::MakeRoleName(*i);

				if (rel_name.size()) 
				{
					string oname = ((::Uml::Class)(*i).target()).name();
					//Attention !
					oname = oname.substr(0, oname.find(Udm::cross_delimiter));
					//end of dangerous zone 
					string from = GetCPPNamefromFromStr(((::Uml::Class)(*i).target()).from());

					// static ::Uml::AssociationRole meta_R_end_
					meta_decls.push_back( boost::format("static ::Uml::AssociationRole meta_%1%_end_") % rel_name );

					// ::Uml::AssociationRole C::meta_R_end_
					meta_defs.push_back( boost::format("::Uml::AssociationRole %2%::meta_%1%_end_") % rel_name % cl_name );

					// ::Udm::CrossAssocEndAttr<T> R_end() const
					meth_decls.push_back( boost::format("::Udm::CrossAssocEndAttr< %1%> %2%_end() const") % (from + "::" + oname) % rel_name );
					// ::Udm::CrossAssocEndAttr<T> C::R_end() const { return ::Udm::CrossAssocEndAttr<T>(impl, meta_R_end_); }
					meth_defs.push_back( boost::format("::Udm::CrossAssocEndAttr< %1%> %3%::%2%_end() const { return ::Udm::CrossAssocEndAttr< %1%>(impl, meta_%2%_end_); }") % (from + "::" + oname) % rel_name % cl_name );

					meta_init_links.push_back( boost::format("%2%::meta_%1%_end_ = %3%::meta_%1%") % rel_name % cl_name % UmlClassCPPName(::Uml::theOther(*i).target()) );

					old_meta_init_links.push_back( boost::format("%2%::meta_%1%_end_ = %3%::meta_%1%") % rel_name % cl_name % UmlClassCPPName(::Uml::theOther(*i).target()) );

					corba_meta_init_links.push_back( boost::format("%2%::meta_%1%_end_ = %3%::meta_%1%") % rel_name % cl_name % UmlClassCPPName(::Uml::theOther(*i).target()) );

					if (gen.opts.mode == UdmOpts::CXX_GENERIC)
					{
						cross_aclass_tlhlp.roles2type.push_back( boost::format("class ACE_%1% {}") % rel_name );

						string tl_typename = "_ACE_" + rel_name;
						{
							string s = from + "::" + oname;
							boost::replace_all(s, "::", "_");
							tl_typename += "__" + s;
						}
						cross_aclass_tlhlp.typenames_single.push_back(tl_typename);
						cross_aclass_tlhlp.typedefs.push_back( boost::format("typedef boost::mpl::pair< %2%, ACE_%1% > %3%") % rel_name % (from + "::" + oname) % tl_typename );

						meth_speclzs_mpl_assoc.push_back( boost::format("template <> const ::Uml::AssociationRole& %1%::_type2ARole< %1%::ACE_%2% >() const { return meta_%2%_end_; }") % cl_name % rel_name );
					}
				}
			}
		}
	}

	if (gen.opts.mode == UdmOpts::CXX_GENERIC)
		BuildTL(cross_aclass_tlhlp, "CrossAClassEnds", "", "cross association class by returned type and role", cross_aclass_ends_tl);
}


void ClassGen::Constraints()
{
	string cl_name = c.name();

	set< ::Uml::Constraint> constraints = c.constraints();
	set< ::Uml::Constraint>::iterator c_i = constraints.begin();
	while (c_i != constraints.end())
	{
		::Uml::Constraint constraint = *c_i;
		string constraint_name = constraint.name();

		// static ::Uml::Constraint meta_R;
		meta_decls.push_back( boost::format("static ::Uml::Constraint meta_%1%") % constraint_name );
		// ::Uml::Constraint C::meta_R
		meta_defs.push_back( boost::format("::Uml::Constraint %2%::meta_%1%") % constraint_name % cl_name );

		meta_create.push_back( boost::format("%2%::meta_%1% = ::Uml::Constraint::Create(%2%::meta)") % constraint_name % cl_name );

		meta_init.push_back( boost::format("::Uml::InitConstraintProps(%2%::meta_%1%, \"%1%\", \"%3%\", \"%4%\")")
					% constraint_name
					% cl_name
					% (string) constraint.description()
					% (string) constraint.expression()
					);

		meta_init2.push_back( boost::format("::Uml::SetConstraint(%2%::meta_%1%, %2%::meta, \"%1%\")") % constraint_name % cl_name );

		old_meta_create.push_back( boost::format("%2%::meta_%1% = ::Uml::CreateConstraint()") % constraint_name % cl_name );
		old_meta_init.push_back( boost::format("::Uml::InitConstraint(%2%::meta_%1%, %2%::meta, \"%1%\", \"%3%\", \"%4%\")")
						% constraint_name
						% cl_name
						% (string) constraint.description()
						% (string) constraint.expression()
						);

		corba_meta_create.push_back( boost::format("%2%::meta_%1% = ::Uml::CreateCORBAConstraint()") % constraint_name % cl_name );
		corba_meta_init.push_back( boost::format("::Uml::InitCORBAConstraint(%2%::meta_%1%, %2%::meta, \"%1%\", \"%3%\", \"%4%\")")
						% constraint_name
						% cl_name
						% (string) constraint.description()
						% (string) constraint.expression()
						);

		c_i++;
	}

	//constraint definitions
	set< ::Uml::ConstraintDefinition> definitions= c.definitions();
	set< ::Uml::ConstraintDefinition>::iterator d_i = definitions.begin();
	while (d_i != definitions.end())
	{
		::Uml::ConstraintDefinition cdef = *d_i;
		string cdef_name = cdef.name();

		// static ::Uml::ConstraintDefinition meta_R;
		meta_decls.push_back( boost::format("static ::Uml::ConstraintDefinition meta_%1%") % cdef_name );
		// ::Uml::ConstraintDefinition C::meta_R
		meta_defs.push_back( boost::format("::Uml::ConstraintDefinition %2%::meta_%1%") % cdef_name % cl_name );

		meta_create.push_back( boost::format("%2%::meta_%1% = ::Uml::ConstraintDefinition::Create(%2%::meta)") % cdef_name % cl_name );

		meta_init.push_back( boost::format("::Uml::InitConstraintDefinitionProps(%2%::meta_%1%, \"%1%\", \"%3%\", \"%4%\", \"%5%\", \"%6%\")")
					% cdef_name
					% cl_name
					% (string) cdef.stereotype()
					% (string) cdef.parameterList()
					% (string) cdef.expression()
					% (string) cdef.returnType()
					);

		meta_init2.push_back( boost::format("::Uml::SetConstraintDefinition(%2%::meta_%1%, %2%::meta, \"%1%\")") % cdef_name % cl_name );

		old_meta_create.push_back( boost::format("%2%::meta_%1% = ::Uml::CreateConstraintDefinition()") % cdef_name % cl_name );
		old_meta_init.push_back( boost::format("::Uml::InitConstraintDefinition(%2%::meta_%1%, %2%::meta, \"%1%\", \"%3%\", \"%4%\", \"%5%\", \"%6%\")")
						% cdef_name
						% cl_name
						% (string) cdef.stereotype()
						% (string) cdef.parameterList()
						% (string) cdef.expression()
						% (string) cdef.returnType()
						);

		corba_meta_create.push_back( boost::format("%2%::meta_%1% = ::Uml::CreateCORBAConstraintDefinition()") % cdef_name % cl_name );
		corba_meta_init.push_back( boost::format("::Uml::InitCORBAConstraintDefinition(%2%::meta_%1%, %2%::meta, \"%1%\", \"%3%\", \"%4%\", \"%5%\", \"%6%\")")
						% cdef_name
						% cl_name
						% (string) cdef.stereotype()
						% (string) cdef.parameterList()
						% (string) cdef.expression()
						% (string) cdef.returnType()
						);

		d_i++;
	}
}


void ClassGen::Visitor()
{
	if (gen.opts.visitor_sup && !c.isAbstract())
	{
		// void Accept(Visitor &v)
		meth_decls.push_back( boost::format("void Accept(Visitor &v)") );
		// void C::Accept(Visitor &v) { v.Visit_C(*this); }
		meth_defs.push_back( boost::format("void %1%::Accept(Visitor &v) { v.Visit_%1%(*this); }") % (string) c.name() );
	}
}

void ClassGen::OtherLinks()
{
	string cl_name = c.name();
	::Uml::Namespace parent_ns = c.parent_ns();

	// inheritance
	set< ::Uml::Class> st = c.subTypes();
	for (set< ::Uml::Class>::const_iterator i = st.begin(); i != st.end(); i++)
	{
		meta_init_links.push_back( boost::format("%1%::meta.subTypes() += %2%::meta") % cl_name % UmlClassCPPName(*i, parent_ns) );
		old_meta_init_links.push_back( boost::format("::Uml::AddInheritance(%1%::meta, %2%::meta)") % cl_name % UmlClassCPPName(*i, parent_ns) );
		corba_meta_init_links.push_back( boost::format("::Uml::AddCORBAInheritance(%1%::meta, %2%::meta)") % cl_name % UmlClassCPPName(*i, parent_ns) );
	}
}

static bool cmpClasses(const Uml::Class &a, const Uml::Class &b)
{
	if (&a == &b || a == b)
		return false;
	else if (Uml::IsDerivedFrom(a, b))
		return true;
	else if (Uml::IsDerivedFrom(b, a))
		return false;
	else
		return a < b;
}

void ClassGen::BuildTL(const set< ::Uml::Class> &s, const string &tl_name, const string &description, vector<boost::format> &result, bool sort_tl)
{
	result.clear();

	result.push_back( boost::format("// typelist for %1% relations") % description );

	vector<string> class_names;
	class_names.reserve(s.size());

	if (sort_tl)
	{
		vector< ::Uml::Class> sorted_v(s.begin(), s.end());
		sort(sorted_v.begin(), sorted_v.end(), cmpClasses);

		for (vector< ::Uml::Class>::const_iterator i = sorted_v.begin(); i != sorted_v.end(); i++)
			class_names.push_back( UmlClassCPPName(*i) );
	}
	else
		for (set< ::Uml::Class>::const_iterator i = s.begin(); i != s.end(); i++)
			class_names.push_back( UmlClassCPPName(*i) );

	result.push_back( boost::format("typedef boost::mpl::vector< %1%> %2%") % join(", ", class_names) % tl_name );
	gen.SetMPLSequenceMaxSize(class_names.size());
}

void ClassGen::BuildTL(const TLHelper &hlp, const string &tl_single_name, const string &tl_multi_name, const string &description, vector<boost::format> &result)
{
	result.clear();

	result.push_back( boost::format("// types and typelist for %1% relations") % description );

	if (hlp.typedefs.size())
	{
		result.insert(result.end(), hlp.roles2type.begin(), hlp.roles2type.end());
		result.insert(result.end(), hlp.typedefs.begin(), hlp.typedefs.end());

		// the typelist(s)
		if (tl_single_name.length())
		{
			result.push_back( boost::format("typedef boost::mpl::vector< %1%> %2%") % join(", ", hlp.typenames_single) % tl_single_name );
			gen.SetMPLSequenceMaxSize(hlp.typenames_single.size());
		}

		if (tl_multi_name.length())
		{
			result.push_back( boost::format("typedef boost::mpl::vector< %1%> %2%") % join(", ", hlp.typenames_multi) % tl_multi_name );
			gen.SetMPLSequenceMaxSize(hlp.typenames_multi.size());
		}
	}
	else
	{
		if (tl_single_name.length())
			result.push_back( boost::format("typedef boost::mpl::vector< > %1%") % tl_single_name );

		if (tl_multi_name.length())
			result.push_back( boost::format("typedef boost::mpl::vector< > %1%") % tl_multi_name );
	}
}

void ClassGen::OutDecls(ostream &out) const
{
	string lidt = idt.substr(1);

	out << lidt << class_def_start << " {" << endl;
	out << lidt << "public:" << endl;

	OutFmts(out, idt, typedefs);

	OutFmts(out, idt, ctor_decls);
	OutFmts(out, idt, dtor_decls);

	if (ctor_rvalue_decls.size() || meth_rvalue_decls.size()) {
		out << "#ifdef UDM_RVALUE" << endl;
		OutFmts(out, idt, ctor_rvalue_decls);
		OutFmts(out, idt, meth_rvalue_decls);
		out << "#endif" << endl;
	}

	OutFmts(out, idt, meth_decls);
	OutFmts(out, idt, meta_decls);
	OutFmts(out, idt, comments, false);

	if (gen.opts.mode == UdmOpts::CXX_GENERIC) {
		out << "#ifndef MPL_NO_COMPOSITIONS" << endl;
		OutFmts(out, idt, meth_defs_mpl_comp, false);
		OutFmts(out, idt, parents_tl);
		OutFmts(out, idt, parent_kinds_tl);
		OutFmts(out, idt, children_tl);
		OutFmts(out, idt, children_kinds_tl);
		out << "#endif // MPL_NO_COMPOSITIONS" << endl;
	}

	if (gen.opts.mode == UdmOpts::CXX_GENERIC) {
		out << "#ifndef MPL_NO_ASSOCIATIONS" << endl;
		OutFmts(out, idt, meth_defs_mpl_assoc, false);
		OutFmts(out, idt, associations_tl);
		OutFmts(out, idt, associations_ac_tl);
		OutFmts(out, idt, cross_associations_tl);
		OutFmts(out, idt, cross_associations_ac_tl);
		OutFmts(out, idt, aclass_ends_tl);
		OutFmts(out, idt, cross_aclass_ends_tl);
		out << "#endif // MPL_NO_ASSOCIATIONS" << endl;
	}

	out << lidt << "};" << endl << endl;
}

void ClassGen::OutDefs(ostream &out) const
{
	string lidt = idt.substr(1);

	OutFmts(out, lidt, ctor_defs, false);
	OutFmts(out, lidt, dtor_defs, false);

	if (ctor_rvalue_defs.size() || meth_rvalue_defs.size()) {
		out << "#ifdef UDM_RVALUE" << endl;
		OutFmts(out, lidt, ctor_rvalue_defs);
		OutFmts(out, lidt, meth_rvalue_defs);
		out << "#endif" << endl;
	}

	OutFmts(out, lidt, meth_defs, false);
	OutFmts(out, lidt, meta_defs);

	if (gen.opts.mode == UdmOpts::CXX_GENERIC) {
		out << "#ifndef MPL_NO_COMPOSITIONS" << endl;
		OutFmts(out, lidt, meth_speclzs_mpl_comp, false);
		out << "#endif // MPL_NO_COMPOSITIONS" << endl;

		out << "#ifndef MPL_NO_ASSOCIATIONS" << endl;
		OutFmts(out, lidt, meth_speclzs_mpl_assoc, false);
		out << "#endif // MPL_NO_ASSOCIATIONS" << endl;
	}
}

void ClassGen::OutMetaCreate(ostream &out) const
{
	switch (gen.opts.meta_init)
	{
	case UdmOpts::DYNAMIC_INIT:
		OutFmts(out, idt, meta_create);
		break;
	case UdmOpts::STATIC_INIT:
		OutFmts(out, idt, old_meta_create);
		break;
	case UdmOpts::CORBA_INIT:
		OutFmts(out, idt, corba_meta_create);
		break;
	default:
		throw udm_exception("unknown type of meta initialization");
	}
}

void ClassGen::OutMetaInit(ostream &out) const
{
	switch (gen.opts.meta_init)
	{
	case UdmOpts::DYNAMIC_INIT:
		OutFmts(out, idt, meta_init);
		break;
	case UdmOpts::STATIC_INIT:
		OutFmts(out, idt, old_meta_init);
		break;
	case UdmOpts::CORBA_INIT:
		OutFmts(out, idt, corba_meta_init);
		break;
	default:
		throw udm_exception("unknown type of meta initialization");
	}
}

void ClassGen::OutMetaInitLinks(ostream &out) const
{
	switch (gen.opts.meta_init)
	{
	case UdmOpts::DYNAMIC_INIT:
		OutFmts(out, idt, meta_init_links);
		break;
	case UdmOpts::STATIC_INIT:
		OutFmts(out, idt, old_meta_init_links);
		break;
	case UdmOpts::CORBA_INIT:
		OutFmts(out, idt, corba_meta_init_links);
		break;
	default:
		throw udm_exception("unknown type of meta initialization");
	}
}

void ClassGen::OutMetaInit2(ostream &out) const
{
	OutFmts(out, idt, meta_init2);
}

void ClassGen::OutMetaInitLinks2(ostream &out) const
{
	OutFmts(out, idt, meta_init_links2);
}


} // namespace UdmCPPGen
