#pragma once
#ifndef MOBIES_UDMPROJECT_H
#define MOBIES_UDMPROJECT_H

// header file UdmProject.h generated from diagram UdmProject
// generated with Udm version 3.33 on Thu Nov 05 17:19:40 2015

#include <UdmBase.h>

#if !defined(UDM_VERSION_MAJOR) || !defined(UDM_VERSION_MINOR)
#    error "Udm headers too old, they do not define UDM_VERSION"
#elif UDM_VERSION_MAJOR < 3
#    error "Udm headers too old, minimum version required 3.33"
#elif UDM_VERSION_MAJOR == 3 && UDM_VERSION_MINOR < 33
#    error "Udm headers too old, minimum version required 3.33"
#endif

#include <Uml.h>


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace UdmProject {

	extern ::Uml::Diagram meta;
	class Datanetwork;
	class Project;

	void Initialize();

	extern  ::Udm::UdmDiagram diagram;

	class Datanetwork : public ::Udm::Object {
	public:
		Datanetwork();
		Datanetwork(::Udm::ObjectImpl *impl);
		Datanetwork(const Datanetwork &master);

#ifdef UDM_RVALUE
		Datanetwork(Datanetwork &&master);

		static Datanetwork Cast(::Udm::Object &&a);
		Datanetwork& operator=(Datanetwork &&a);
		Datanetwork& operator=(const Datanetwork &a);

#endif
		static Datanetwork Cast(const ::Udm::Object &a);
		static Datanetwork Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Datanetwork CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Datanetwork> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Datanetwork, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Datanetwork, Pred>(impl); };
		Datanetwork CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Datanetwork> Derived();
		template <class Pred> ::Udm::DerivedAttr< Datanetwork, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Datanetwork, Pred>(impl); };
		::Udm::ArchetypeAttr< Datanetwork> Archetype() const;
		::Udm::StringAttr metaDgr() const;
		::Udm::StringAttr systemname() const;
		::Udm::StringAttr metalocator() const;
		::Udm::ParentAttr< ::UdmProject::Project> cross_associations_Project_parent() const;
		::Udm::ParentAttr< ::UdmProject::Project> instances_Project_parent() const;
		::Udm::ParentAttr< ::UdmProject::Project> crosslinks_Project_parent() const;
		::Udm::ParentAttr< ::UdmProject::Project> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_metaDgr;
		static ::Uml::Attribute meta_systemname;
		static ::Uml::Attribute meta_metalocator;
		static ::Uml::CompositionParentRole meta_cross_associations_Project_parent;
		static ::Uml::CompositionParentRole meta_instances_Project_parent;
		static ::Uml::CompositionParentRole meta_crosslinks_Project_parent;

	};

	class Project : public ::Udm::Object {
	public:
		Project();
		Project(::Udm::ObjectImpl *impl);
		Project(const Project &master);

#ifdef UDM_RVALUE
		Project(Project &&master);

		static Project Cast(::Udm::Object &&a);
		Project& operator=(Project &&a);
		Project& operator=(const Project &a);

#endif
		static Project Cast(const ::Udm::Object &a);
		static Project Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Project CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Project> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Project, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Project, Pred>(impl); };
		Project CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Project> Derived();
		template <class Pred> ::Udm::DerivedAttr< Project, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Project, Pred>(impl); };
		::Udm::ArchetypeAttr< Project> Archetype() const;
		::Udm::StringAttr name() const;
		::Udm::ChildAttr< ::UdmProject::Datanetwork> cross_associations() const;
		::Udm::ChildrenAttr< ::UdmProject::Datanetwork> instances() const;
		template <class Pred> ::Udm::ChildrenAttr< ::UdmProject::Datanetwork, Pred> instances_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::UdmProject::Datanetwork, Pred>(impl, meta_instances); };
		::Udm::ChildAttr< ::UdmProject::Datanetwork> crosslinks() const;
		::Udm::ChildrenAttr< ::UdmProject::Datanetwork> Datanetwork_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::UdmProject::Datanetwork, Pred> Datanetwork_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::UdmProject::Datanetwork, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::Udm::Object> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::CompositionChildRole meta_cross_associations;
		static ::Uml::CompositionChildRole meta_instances;
		static ::Uml::CompositionChildRole meta_crosslinks;

	};

}

#endif // MOBIES_UDMPROJECT_H
