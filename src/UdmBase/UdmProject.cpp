// cpp (meta datanetwork format) source file UdmProject.cpp
// generated from diagram UdmProject
// generated on Thu Nov 05 17:19:40 2015

#include "UdmProject.h"
#include <UmlExt.h>
#include <UdmStatic.h>

#include <UdmDom.h>
#include "UdmProject_xsd.h"
using namespace std;

namespace UdmProject {

	Datanetwork::Datanetwork() {}
	Datanetwork::Datanetwork(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Datanetwork::Datanetwork(const Datanetwork &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Datanetwork::Datanetwork(Datanetwork &&master) : UDM_OBJECT(master) {};

	Datanetwork Datanetwork::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Datanetwork& Datanetwork::operator=(Datanetwork &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	Datanetwork& Datanetwork::operator=(const Datanetwork &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	Datanetwork Datanetwork::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Datanetwork Datanetwork::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Datanetwork Datanetwork::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Datanetwork> Datanetwork::Instances() { return ::Udm::InstantiatedAttr< Datanetwork>(impl); }
	Datanetwork Datanetwork::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Datanetwork> Datanetwork::Derived() { return ::Udm::DerivedAttr< Datanetwork>(impl); }
	::Udm::ArchetypeAttr< Datanetwork> Datanetwork::Archetype() const { return ::Udm::ArchetypeAttr< Datanetwork>(impl); }
	::Udm::StringAttr Datanetwork::metaDgr() const { return ::Udm::StringAttr(impl, meta_metaDgr); }
	::Udm::StringAttr Datanetwork::systemname() const { return ::Udm::StringAttr(impl, meta_systemname); }
	::Udm::StringAttr Datanetwork::metalocator() const { return ::Udm::StringAttr(impl, meta_metalocator); }
	::Udm::ParentAttr< ::UdmProject::Project> Datanetwork::cross_associations_Project_parent() const { return ::Udm::ParentAttr< ::UdmProject::Project>(impl, meta_cross_associations_Project_parent); }
	::Udm::ParentAttr< ::UdmProject::Project> Datanetwork::instances_Project_parent() const { return ::Udm::ParentAttr< ::UdmProject::Project>(impl, meta_instances_Project_parent); }
	::Udm::ParentAttr< ::UdmProject::Project> Datanetwork::crosslinks_Project_parent() const { return ::Udm::ParentAttr< ::UdmProject::Project>(impl, meta_crosslinks_Project_parent); }
	::Udm::ParentAttr< ::UdmProject::Project> Datanetwork::parent() const { return ::Udm::ParentAttr< ::UdmProject::Project>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Datanetwork::meta;
	::Uml::Attribute Datanetwork::meta_metaDgr;
	::Uml::Attribute Datanetwork::meta_systemname;
	::Uml::Attribute Datanetwork::meta_metalocator;
	::Uml::CompositionParentRole Datanetwork::meta_cross_associations_Project_parent;
	::Uml::CompositionParentRole Datanetwork::meta_instances_Project_parent;
	::Uml::CompositionParentRole Datanetwork::meta_crosslinks_Project_parent;

	Project::Project() {}
	Project::Project(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Project::Project(const Project &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Project::Project(Project &&master) : UDM_OBJECT(master) {};

	Project Project::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Project& Project::operator=(Project &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	Project& Project::operator=(const Project &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	Project Project::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Project Project::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Project Project::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Project> Project::Instances() { return ::Udm::InstantiatedAttr< Project>(impl); }
	Project Project::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Project> Project::Derived() { return ::Udm::DerivedAttr< Project>(impl); }
	::Udm::ArchetypeAttr< Project> Project::Archetype() const { return ::Udm::ArchetypeAttr< Project>(impl); }
	::Udm::StringAttr Project::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::ChildAttr< ::UdmProject::Datanetwork> Project::cross_associations() const { return ::Udm::ChildAttr< ::UdmProject::Datanetwork>(impl, meta_cross_associations); }
	::Udm::ChildrenAttr< ::UdmProject::Datanetwork> Project::instances() const { return ::Udm::ChildrenAttr< ::UdmProject::Datanetwork>(impl, meta_instances); }
	::Udm::ChildAttr< ::UdmProject::Datanetwork> Project::crosslinks() const { return ::Udm::ChildAttr< ::UdmProject::Datanetwork>(impl, meta_crosslinks); }
	::Udm::ChildrenAttr< ::UdmProject::Datanetwork> Project::Datanetwork_kind_children() const { return ::Udm::ChildrenAttr< ::UdmProject::Datanetwork>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::Udm::Object> Project::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Project::meta;
	::Uml::Attribute Project::meta_name;
	::Uml::CompositionChildRole Project::meta_cross_associations;
	::Uml::CompositionChildRole Project::meta_instances;
	::Uml::CompositionChildRole Project::meta_crosslinks;

	::Uml::Diagram meta;

	void CreateMeta() {
		// classes, with attributes, constraints and constraint definitions
		Datanetwork::meta = ::Uml::CreateClass();
		Datanetwork::meta_metaDgr = ::Uml::CreateAttribute();
		Datanetwork::meta_systemname = ::Uml::CreateAttribute();
		Datanetwork::meta_metalocator = ::Uml::CreateAttribute();

		Project::meta = ::Uml::CreateClass();
		Project::meta_name = ::Uml::CreateAttribute();

		Datanetwork::meta_cross_associations_Project_parent = ::Uml::CreateCompositionParentRole();
		Project::meta_cross_associations = ::Uml::CreateCompositionChildRole();
		Datanetwork::meta_instances_Project_parent = ::Uml::CreateCompositionParentRole();
		Project::meta_instances = ::Uml::CreateCompositionChildRole();
		Datanetwork::meta_crosslinks_Project_parent = ::Uml::CreateCompositionParentRole();
		Project::meta_crosslinks = ::Uml::CreateCompositionChildRole();

	}

	void InitMeta() {
		// classes, with attributes, constraints and constraint definitions
		::Uml::InitClass(Datanetwork::meta, meta, "Datanetwork", false, NULL, NULL);
		::Uml::InitAttribute(Datanetwork::meta_metaDgr, Datanetwork::meta, "metaDgr", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(Datanetwork::meta_systemname, Datanetwork::meta, "systemname", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(Datanetwork::meta_metalocator, Datanetwork::meta, "metalocator", "String", false, false, 0, 1, false, "public", vector<string>());

		::Uml::InitClass(Project::meta, meta, "Project", false, NULL, NULL);
		::Uml::InitAttribute(Project::meta_name, Project::meta, "name", "String", false, false, 1, 1, false, "public", vector<string>());

	}

	void InitMetaLinks() {
		// compositions
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Datanetwork::meta_cross_associations_Project_parent, comp, "cross_associations_Project_parent", true, Project::meta);
			::Uml::InitCompositionChildRole(Project::meta_cross_associations, comp, "cross_associations", true, 0, 1, Datanetwork::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Datanetwork::meta_instances_Project_parent, comp, "instances_Project_parent", true, Project::meta);
			::Uml::InitCompositionChildRole(Project::meta_instances, comp, "instances", true, 0, -1, Datanetwork::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Datanetwork::meta_crosslinks_Project_parent, comp, "crosslinks_Project_parent", true, Project::meta);
			::Uml::InitCompositionChildRole(Project::meta_crosslinks, comp, "crosslinks", true, 0, 1, Datanetwork::meta);

		}

	}

	void _SetXsdStorage()
	{
		UdmDom::str_xsd_storage::StoreXsd("UdmProject.xsd", UdmProject_xsd::getString());
	}

	void Initialize()
	{
		static bool first = true;
		if (!first) return;
		first = false;
		::Uml::Initialize();

	
		UDM_ASSERT( meta == ::Udm::null );

		meta = ::Uml::CreateDiagram();


		CreateMeta();
		InitMeta();
		InitMetaLinks();

		::Uml::InitDiagram(meta, "UdmProject", "1.00");


		_SetXsdStorage();

	}


	 ::Udm::UdmDiagram diagram = { &meta, Initialize };
	static struct _regClass
	{
		_regClass()
		{
			::Udm::MetaDepository::StoreDiagram("UdmProject", diagram);
		}
		~_regClass()
		{
			::Udm::MetaDepository::RemoveDiagram("UdmProject");
		}
	} __regUnUsed;

}

