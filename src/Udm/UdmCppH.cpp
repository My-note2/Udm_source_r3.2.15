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


#include "Uml.h"
#include "UmlExt.h"
#include <map>
#include "Udm.h"

// assign a name to a role
// if name is assigned, use that name
// otherwise, if navigable, get the peer class name and append _assoc, _parent, _child or _children 
//									(for _parent, prepend ChildRole name, if applicable)
//			  if not navigable, name is arolexxx, prolexxx, or crolexxx, where xxx is the uniqueid

using namespace Uml;
	
void CheckClass(const ::Uml::Class & cl)
{
	set<string> unique_names;
	//reserved names
	unique_names.insert("parent");
	unique_names.insert("meta");
	unique_names.insert("archetype");
	unique_names.insert("derived");
	unique_names.insert("instances");
	unique_names.insert("desynched_atts");
	unique_names.insert((string)cl.name());
	// typelists
	unique_names.insert("AssociationsSingle");
	unique_names.insert("AssociationsMulti");
	unique_names.insert("AssociationsWAClassSingle");
	unique_names.insert("AssociationsWAClassMulti");
	unique_names.insert("CrossAssociationsSingle");
	unique_names.insert("CrossAssociationsMulti");
	unique_names.insert("CrossAssociationsWAClassSingle");
	unique_names.insert("CrossAssociationsWAClassMulti");
	unique_names.insert("ChildrenSingle");
	unique_names.insert("ChildrenMulti");
	unique_names.insert("Parents");
	unique_names.insert("AClassEnds");
	unique_names.insert("CrossAClassEnds");

	unsigned int i;

	//attributes
	set< ::Uml::Attribute> attrs = ::Uml::AncestorAttributes(cl);
	set< ::Uml::Attribute>::iterator attrs_i = attrs.begin();
	while (attrs_i != attrs.end())
	{
		::Uml::Attribute att = *attrs_i++;
	
		string att_type = att.type();

		string att_name = att.name();

		i = unique_names.size();
		unique_names.insert(att_name);
		if (i == unique_names.size()) goto err_att;
		
		i = unique_names.size();
		unique_names.insert(string("meta_") + att_name);
		if (i == unique_names.size()) goto err_att;

		continue;
err_att:
		throw udm_exception("Reserved or duplicate attribute name: " + att_name);
	}



	//associationroles
	set< ::Uml::AssociationRole> assocs = cl.associationRoles();
	set< ::Uml::AssociationRole>::iterator assocs_i = assocs.begin();
	while (assocs_i != assocs.end())
	{
		
		::Uml::AssociationRole ar = *assocs_i++;
		string aname(MakeRoleName(::Uml::theOther(ar)));
		
		i = unique_names.size();
		unique_names.insert(aname);
		if (i == unique_names.size()) goto err_ar;
		
		i = unique_names.size();
		unique_names.insert(string("meta_") + aname);
		if (i == unique_names.size()) goto err_ar;

		// types generated for roles
		i = unique_names.size();
		unique_names.insert("AR_" + aname);
		if (i == unique_names.size()) goto err_ar;

		continue;
err_ar:
		throw udm_exception("Reserved or duplicate  association role name: " + aname);

	}
	

	
	//compositionparentroles
	//check meta_child declarations
	set< ::Uml::CompositionParentRole> cprs = cl.parentRoles();
	
	set< ::Uml::CompositionParentRole>::iterator cprs_i = cprs.begin();
	set< ::Uml::Class> kinds;

	while (cprs_i != cprs.end())
	{
		::Uml::CompositionParentRole cpr = *cprs_i++;
		string aname(::Uml::MakeRoleName(::Uml::theOther(cpr)));

		::Uml::Class thischildkind = (::Uml::Class)::Uml::theOther(cpr).target();
		kinds.insert(thischildkind);

		i = unique_names.size();
		unique_names.insert(string("meta_") + aname);
		if (i == unique_names.size()) goto err_cpr;

		// types generated for roles
		i = unique_names.size();
		unique_names.insert("CR_" + aname);
		if (i == unique_names.size()) goto err_cpr;
		
		continue;
err_cpr:
		throw udm_exception("Reserved or duplicate child role name: " + aname);
	}

	//check _kind_children stuff
	set< ::Uml::Class>::iterator kinds_i = kinds.begin();
	while (kinds_i != kinds.end())
	{
		::Uml::Class childkind = *kinds_i++;

		string kind_children_name;
		if (((Namespace)cl.parent_ns()) != ((Namespace)childkind.parent_ns()))
			kind_children_name = childkind.getPath2("_", false);
		else
			kind_children_name = childkind.name();

		i = unique_names.size();
		unique_names.insert(kind_children_name + "_kind_children");
		if (i == unique_names.size()) goto err_kinds;
		continue;
err_kinds:
		throw udm_exception("Reserved or duplicate children kind name: " + kind_children_name);
	}

	//check parent, meta_parent declarations 

	set< ::Uml::CompositionChildRole> ccrs = cl.childRoles();
	set< ::Uml::CompositionChildRole>::iterator ccrs_i = ccrs.begin();
	while (cprs_i != cprs.end())
	{
		::Uml::CompositionChildRole ccr = *ccrs_i++;
		string aname(::Uml::MakeRoleName(::Uml::theOther(ccr)));

		i = unique_names.size();
		unique_names.insert(aname );
		if (i == unique_names.size()) goto err_ccr;

		i = unique_names.size();
		unique_names.insert(string("meta_") + aname );
		if (i == unique_names.size()) goto err_ccr;

		// types generated for roles
		i = unique_names.size();
		unique_names.insert("PR_" + aname);
		if (i == unique_names.size()) goto err_ccr;

		continue;
err_ccr:
		throw udm_exception("Reserved or duplicate parent role name: " + aname);
	}


};

void CheckDiagram(const ::Uml::Diagram & diagram)
{
	//check the UML diagram against duplicate & reserved role, attribute & classnames

	//classes of diagram
	set<string> unique_names;
	set< ::Uml::Class> cls = diagram.classes();
	set< ::Uml::Class>::iterator cls_i = cls.begin();
	while (cls_i != cls.end())
	{
		::Uml::Class cl = *cls_i++;
		unsigned int i = unique_names.size();
		unique_names.insert(cl.name());
		if (i == unique_names.size())
			throw udm_exception(string("Duplicate class name found: ") + (string)cl.name());

		CheckClass(cl);
	}

	//namespaces of diagram
	set<string> unique_ns_names;
	set< ::Uml::Namespace> nses = diagram.namespaces();
	for (set< ::Uml::Namespace>::const_iterator nses_i = nses.begin(); nses_i != nses.end(); nses_i++)
	{
		::Uml::Namespace ns = *nses_i;
		unsigned int i = unique_ns_names.size();
		unique_ns_names.insert(ns.name());
		if (i == unique_ns_names.size())
			throw udm_exception(string("Duplicate namespace name found: ") + (string)ns.name());
	}


	::Uml::DiagramNamespaces all_nses(diagram);
	for (::Uml::DiagramNamespaces::iterator all_nses_i = all_nses.begin(); all_nses_i != all_nses.end(); all_nses_i++)
	{
		::Uml::Namespace ns = *all_nses_i;

		//namespaces of namespace
		set< ::Uml::Namespace> nses = ns.namespaces();
		set< ::Uml::Namespace>::const_iterator nses_i = nses.begin();

		set<string> unique_ns_names;
		while (nses_i != nses.end())
		{
			::Uml::Namespace ns_ns = *nses_i++;

			unsigned int i = unique_ns_names.size();
			unique_ns_names.insert(ns_ns.name());
			if (i == unique_ns_names.size())
				throw udm_exception(string("Duplicate namespace name found: ") + ns_ns.getPath2("/"));
		}


		//classes of namespace
		set< ::Uml::Class> cls = ns.classes();
		set< ::Uml::Class>::iterator cls_i = cls.begin();

		set<string> unique_names;
		while (cls_i != cls.end())
		{
			::Uml::Class cl = *cls_i++;
			unsigned int i = unique_names.size();
			unique_names.insert(cl.name());
			if (i == unique_names.size())
				throw udm_exception(string("Duplicate class name found: ") + cl.getPath2("/"));

			CheckClass(cl);
		}
	}
};
