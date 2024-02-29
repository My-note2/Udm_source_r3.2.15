#pragma once
#ifndef MOBIES_UML_H
#define MOBIES_UML_H

// header file Uml.h generated from diagram Uml
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

#include "Uml_export.h"

namespace Uml {

	extern ::Uml::Diagram meta;
	class UDM_DLL CompositionChildRole;
	class UDM_DLL GenericRole;
	class UDM_DLL CompositionParentRole;
	class UDM_DLL AssociationRole;
	class UDM_DLL Class;
	class UDM_DLL Diagram;
	class UDM_DLL Association;
	class UDM_DLL Composition;
	class UDM_DLL Namespace;
	class UDM_DLL TaggedValue;
	class UDM_DLL ConstraintDefinition;
	class UDM_DLL Attribute;
	class UDM_DLL Constraint;

	UDM_DLL void Initialize();

	extern UDM_DLL  ::Udm::UdmDiagram diagram;

	class UDM_DLL GenericRole : public ::Udm::Object {
	public:
		GenericRole();
		GenericRole(::Udm::ObjectImpl *impl);
		GenericRole(const GenericRole &master);

#ifdef UDM_RVALUE
		GenericRole(GenericRole &&master);

		static GenericRole Cast(::Udm::Object &&a);
		GenericRole& operator=(GenericRole &&a);
		GenericRole& operator=(const GenericRole &a);

#endif
		static GenericRole Cast(const ::Udm::Object &a);
		static GenericRole Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		GenericRole CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< GenericRole> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< GenericRole, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< GenericRole, Pred>(impl); };
		GenericRole CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< GenericRole> Derived();
		template <class Pred> ::Udm::DerivedAttr< GenericRole, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< GenericRole, Pred>(impl); };
		::Udm::ArchetypeAttr< GenericRole> Archetype() const;
		::Udm::BooleanAttr isNavigable() const;
		::Udm::StringAttr name() const;
		::Udm::ParentAttr< ::Udm::Object> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_isNavigable;
		static ::Uml::Attribute meta_name;

	};

	class UDM_DLL CompositionChildRole :  public GenericRole {
	public:
		CompositionChildRole();
		CompositionChildRole(::Udm::ObjectImpl *impl);
		CompositionChildRole(const CompositionChildRole &master);

#ifdef UDM_RVALUE
		CompositionChildRole(CompositionChildRole &&master);

		static CompositionChildRole Cast(::Udm::Object &&a);
		CompositionChildRole& operator=(CompositionChildRole &&a);
		CompositionChildRole& operator=(const CompositionChildRole &a);

#endif
		static CompositionChildRole Cast(const ::Udm::Object &a);
		static CompositionChildRole Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		CompositionChildRole CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< CompositionChildRole> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< CompositionChildRole, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< CompositionChildRole, Pred>(impl); };
		CompositionChildRole CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< CompositionChildRole> Derived();
		template <class Pred> ::Udm::DerivedAttr< CompositionChildRole, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< CompositionChildRole, Pred>(impl); };
		::Udm::ArchetypeAttr< CompositionChildRole> Archetype() const;
		::Udm::IntegerAttr max() const;
		::Udm::IntegerAttr min() const;
		::Udm::PointerAttr< Class> target() const;
		::Udm::ParentAttr< ::Uml::Composition> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_max;
		static ::Uml::Attribute meta_min;
		static ::Uml::AssociationRole meta_target;
		static ::Uml::CompositionParentRole meta_parent;

	};

	class UDM_DLL CompositionParentRole :  public GenericRole {
	public:
		CompositionParentRole();
		CompositionParentRole(::Udm::ObjectImpl *impl);
		CompositionParentRole(const CompositionParentRole &master);

#ifdef UDM_RVALUE
		CompositionParentRole(CompositionParentRole &&master);

		static CompositionParentRole Cast(::Udm::Object &&a);
		CompositionParentRole& operator=(CompositionParentRole &&a);
		CompositionParentRole& operator=(const CompositionParentRole &a);

#endif
		static CompositionParentRole Cast(const ::Udm::Object &a);
		static CompositionParentRole Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		CompositionParentRole CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< CompositionParentRole> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< CompositionParentRole, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< CompositionParentRole, Pred>(impl); };
		CompositionParentRole CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< CompositionParentRole> Derived();
		template <class Pred> ::Udm::DerivedAttr< CompositionParentRole, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< CompositionParentRole, Pred>(impl); };
		::Udm::ArchetypeAttr< CompositionParentRole> Archetype() const;
		::Udm::PointerAttr< Class> target() const;
		::Udm::ParentAttr< ::Uml::Composition> parent() const;

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_target;
		static ::Uml::CompositionParentRole meta_parent;

	};

	class UDM_DLL AssociationRole :  public GenericRole {
	public:
		AssociationRole();
		AssociationRole(::Udm::ObjectImpl *impl);
		AssociationRole(const AssociationRole &master);

#ifdef UDM_RVALUE
		AssociationRole(AssociationRole &&master);

		static AssociationRole Cast(::Udm::Object &&a);
		AssociationRole& operator=(AssociationRole &&a);
		AssociationRole& operator=(const AssociationRole &a);

#endif
		static AssociationRole Cast(const ::Udm::Object &a);
		static AssociationRole Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		AssociationRole CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< AssociationRole> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< AssociationRole, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< AssociationRole, Pred>(impl); };
		AssociationRole CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< AssociationRole> Derived();
		template <class Pred> ::Udm::DerivedAttr< AssociationRole, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< AssociationRole, Pred>(impl); };
		::Udm::ArchetypeAttr< AssociationRole> Archetype() const;
		::Udm::IntegerAttr max() const;
		::Udm::IntegerAttr min() const;
		::Udm::BooleanAttr isPrimary() const;
		::Udm::PointerAttr< AssociationRole> rp_helper() const;
		::Udm::PointerAttr< AssociationRole> rp_helper_user() const;
		::Udm::PointerAttr< Class> target() const;
		::Udm::ParentAttr< ::Uml::Association> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_max;
		static ::Uml::Attribute meta_min;
		static ::Uml::Attribute meta_isPrimary;
		static ::Uml::AssociationRole meta_rp_helper;
		static ::Uml::AssociationRole meta_rp_helper_user;
		static ::Uml::AssociationRole meta_target;
		static ::Uml::CompositionParentRole meta_parent;

	};

	class UDM_DLL Class : public ::Udm::Object {
	public:
		Class();
		Class(::Udm::ObjectImpl *impl);
		Class(const Class &master);

#ifdef UDM_RVALUE
		Class(Class &&master);

		static Class Cast(::Udm::Object &&a);
		Class& operator=(Class &&a);
		Class& operator=(const Class &a);

#endif
		static Class Cast(const ::Udm::Object &a);
		static Class Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Class CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Class> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Class, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Class, Pred>(impl); };
		Class CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Class> Derived();
		template <class Pred> ::Udm::DerivedAttr< Class, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Class, Pred>(impl); };
		::Udm::ArchetypeAttr< Class> Archetype() const;
		::Udm::BooleanAttr isAbstract() const;
		::Udm::StringAttr stereotype() const;
		::Udm::StringAttr name() const;
		::Udm::StringAttr from() const;
		::Udm::AssocAttr< CompositionChildRole> childRoles() const;
		template <class Pred> ::Udm::AssocAttr< CompositionChildRole, Pred> childRoles_sorted(const Pred &) const { return ::Udm::AssocAttr< CompositionChildRole, Pred>(impl, meta_childRoles); };
		::Udm::AssocAttr< CompositionParentRole> parentRoles() const;
		template <class Pred> ::Udm::AssocAttr< CompositionParentRole, Pred> parentRoles_sorted(const Pred &) const { return ::Udm::AssocAttr< CompositionParentRole, Pred>(impl, meta_parentRoles); };
		::Udm::AssocAttr< AssociationRole> associationRoles() const;
		template <class Pred> ::Udm::AssocAttr< AssociationRole, Pred> associationRoles_sorted(const Pred &) const { return ::Udm::AssocAttr< AssociationRole, Pred>(impl, meta_associationRoles); };
		::Udm::PointerAttr< Association> association() const;
		::Udm::AssocAttr< Class> baseTypes() const;
		template <class Pred> ::Udm::AssocAttr< Class, Pred> baseTypes_sorted(const Pred &) const { return ::Udm::AssocAttr< Class, Pred>(impl, meta_baseTypes); };
		::Udm::AssocAttr< Class> subTypes() const;
		template <class Pred> ::Udm::AssocAttr< Class, Pred> subTypes_sorted(const Pred &) const { return ::Udm::AssocAttr< Class, Pred>(impl, meta_subTypes); };
		::Udm::ChildrenAttr< ::Uml::ConstraintDefinition> definitions() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::ConstraintDefinition, Pred> definitions_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::ConstraintDefinition, Pred>(impl, meta_definitions); };
		::Udm::ChildrenAttr< ::Uml::Attribute> attributes() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Attribute, Pred> attributes_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Attribute, Pred>(impl, meta_attributes); };
		::Udm::ChildrenAttr< ::Uml::Constraint> constraints() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Constraint, Pred> constraints_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Constraint, Pred>(impl, meta_constraints); };
		::Udm::ChildrenAttr< ::Uml::TaggedValue> tagValues() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::TaggedValue, Pred> tagValues_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::TaggedValue, Pred>(impl, meta_tagValues); };
		::Udm::ChildrenAttr< ::Uml::TaggedValue> TaggedValue_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::TaggedValue, Pred> TaggedValue_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::TaggedValue, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::ConstraintDefinition> ConstraintDefinition_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::ConstraintDefinition, Pred> ConstraintDefinition_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::ConstraintDefinition, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::Attribute> Attribute_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Attribute, Pred> Attribute_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Attribute, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::Constraint> Constraint_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Constraint, Pred> Constraint_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Constraint, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::Uml::Diagram> parent() const;
		::Udm::ParentAttr< ::Uml::Namespace> parent_ns() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_isAbstract;
		static ::Uml::Attribute meta_stereotype;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_from;
		static ::Uml::AssociationRole meta_childRoles;
		static ::Uml::AssociationRole meta_parentRoles;
		static ::Uml::AssociationRole meta_associationRoles;
		static ::Uml::AssociationRole meta_association;
		static ::Uml::AssociationRole meta_baseTypes;
		static ::Uml::AssociationRole meta_subTypes;
		static ::Uml::CompositionChildRole meta_definitions;
		static ::Uml::CompositionChildRole meta_attributes;
		static ::Uml::CompositionChildRole meta_constraints;
		static ::Uml::CompositionChildRole meta_tagValues;
		static ::Uml::CompositionParentRole meta_parent;
		static ::Uml::CompositionParentRole meta_parent_ns;

	};

	class UDM_DLL Diagram : public ::Udm::Object {
	public:
		Diagram();
		Diagram(::Udm::ObjectImpl *impl);
		Diagram(const Diagram &master);

#ifdef UDM_RVALUE
		Diagram(Diagram &&master);

		static Diagram Cast(::Udm::Object &&a);
		Diagram& operator=(Diagram &&a);
		Diagram& operator=(const Diagram &a);

#endif
		static Diagram Cast(const ::Udm::Object &a);
		static Diagram Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Diagram CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Diagram> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Diagram, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Diagram, Pred>(impl); };
		Diagram CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Diagram> Derived();
		template <class Pred> ::Udm::DerivedAttr< Diagram, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Diagram, Pred>(impl); };
		::Udm::ArchetypeAttr< Diagram> Archetype() const;
		::Udm::StringAttr version() const;
		::Udm::StringAttr name() const;
		::Udm::ChildrenAttr< ::Uml::Class> classes() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Class, Pred> classes_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Class, Pred>(impl, meta_classes); };
		::Udm::ChildrenAttr< ::Uml::Association> associations() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Association, Pred> associations_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Association, Pred>(impl, meta_associations); };
		::Udm::ChildrenAttr< ::Uml::Composition> compositions() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Composition, Pred> compositions_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Composition, Pred>(impl, meta_compositions); };
		::Udm::ChildrenAttr< ::Uml::Namespace> namespaces() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Namespace, Pred> namespaces_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Namespace, Pred>(impl, meta_namespaces); };
		::Udm::ChildrenAttr< ::Uml::Class> Class_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Class, Pred> Class_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Class, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::Association> Association_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Association, Pred> Association_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Association, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::Composition> Composition_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Composition, Pred> Composition_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Composition, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::Namespace> Namespace_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Namespace, Pred> Namespace_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Namespace, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::Udm::Object> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_version;
		static ::Uml::Attribute meta_name;
		static ::Uml::CompositionChildRole meta_classes;
		static ::Uml::CompositionChildRole meta_associations;
		static ::Uml::CompositionChildRole meta_compositions;
		static ::Uml::CompositionChildRole meta_namespaces;

	};

	class UDM_DLL Association : public ::Udm::Object {
	public:
		Association();
		Association(::Udm::ObjectImpl *impl);
		Association(const Association &master);

#ifdef UDM_RVALUE
		Association(Association &&master);

		static Association Cast(::Udm::Object &&a);
		Association& operator=(Association &&a);
		Association& operator=(const Association &a);

#endif
		static Association Cast(const ::Udm::Object &a);
		static Association Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Association CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Association> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Association, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Association, Pred>(impl); };
		Association CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Association> Derived();
		template <class Pred> ::Udm::DerivedAttr< Association, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Association, Pred>(impl); };
		::Udm::ArchetypeAttr< Association> Archetype() const;
		::Udm::BooleanAttr nonpersistent() const;
		::Udm::StringAttr name() const;
		::Udm::PointerAttr< Class> assocClass() const;
		::Udm::ChildrenAttr< ::Uml::AssociationRole> roles() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::AssociationRole, Pred> roles_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::AssociationRole, Pred>(impl, meta_roles); };
		::Udm::ChildrenAttr< ::Uml::TaggedValue> tagValues() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::TaggedValue, Pred> tagValues_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::TaggedValue, Pred>(impl, meta_tagValues); };
		::Udm::ChildrenAttr< ::Uml::GenericRole> GenericRole_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::GenericRole, Pred> GenericRole_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::GenericRole, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::AssociationRole> AssociationRole_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::AssociationRole, Pred> AssociationRole_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::AssociationRole, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::TaggedValue> TaggedValue_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::TaggedValue, Pred> TaggedValue_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::TaggedValue, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::Uml::Diagram> parent() const;
		::Udm::ParentAttr< ::Uml::Namespace> parent_ns() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_nonpersistent;
		static ::Uml::Attribute meta_name;
		static ::Uml::AssociationRole meta_assocClass;
		static ::Uml::CompositionChildRole meta_roles;
		static ::Uml::CompositionChildRole meta_tagValues;
		static ::Uml::CompositionParentRole meta_parent;
		static ::Uml::CompositionParentRole meta_parent_ns;

	};

	class UDM_DLL Composition : public ::Udm::Object {
	public:
		Composition();
		Composition(::Udm::ObjectImpl *impl);
		Composition(const Composition &master);

#ifdef UDM_RVALUE
		Composition(Composition &&master);

		static Composition Cast(::Udm::Object &&a);
		Composition& operator=(Composition &&a);
		Composition& operator=(const Composition &a);

#endif
		static Composition Cast(const ::Udm::Object &a);
		static Composition Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Composition CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Composition> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Composition, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Composition, Pred>(impl); };
		Composition CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Composition> Derived();
		template <class Pred> ::Udm::DerivedAttr< Composition, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Composition, Pred>(impl); };
		::Udm::ArchetypeAttr< Composition> Archetype() const;
		::Udm::BooleanAttr nonpersistent() const;
		::Udm::StringAttr name() const;
		::Udm::ChildAttr< ::Uml::CompositionParentRole> parentRole() const;
		::Udm::ChildAttr< ::Uml::CompositionChildRole> childRole() const;
		::Udm::ChildrenAttr< ::Uml::CompositionChildRole> CompositionChildRole_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::CompositionChildRole, Pred> CompositionChildRole_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::CompositionChildRole, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::GenericRole> GenericRole_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::GenericRole, Pred> GenericRole_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::GenericRole, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::CompositionParentRole> CompositionParentRole_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::CompositionParentRole, Pred> CompositionParentRole_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::CompositionParentRole, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::Uml::Diagram> parent() const;
		::Udm::ParentAttr< ::Uml::Namespace> parent_ns() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_nonpersistent;
		static ::Uml::Attribute meta_name;
		static ::Uml::CompositionChildRole meta_parentRole;
		static ::Uml::CompositionChildRole meta_childRole;
		static ::Uml::CompositionParentRole meta_parent;
		static ::Uml::CompositionParentRole meta_parent_ns;

	};

	class UDM_DLL Namespace : public ::Udm::Object {
	public:
		Namespace();
		Namespace(::Udm::ObjectImpl *impl);
		Namespace(const Namespace &master);

#ifdef UDM_RVALUE
		Namespace(Namespace &&master);

		static Namespace Cast(::Udm::Object &&a);
		Namespace& operator=(Namespace &&a);
		Namespace& operator=(const Namespace &a);

#endif
		static Namespace Cast(const ::Udm::Object &a);
		static Namespace Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Namespace CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Namespace> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Namespace, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Namespace, Pred>(impl); };
		Namespace CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Namespace> Derived();
		template <class Pred> ::Udm::DerivedAttr< Namespace, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Namespace, Pred>(impl); };
		::Udm::ArchetypeAttr< Namespace> Archetype() const;
		::Udm::StringAttr name() const;
		::Udm::ChildrenAttr< ::Uml::Namespace> namespaces() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Namespace, Pred> namespaces_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Namespace, Pred>(impl, meta_namespaces); };
		::Udm::ChildrenAttr< ::Uml::Class> classes() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Class, Pred> classes_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Class, Pred>(impl, meta_classes); };
		::Udm::ChildrenAttr< ::Uml::Composition> compositions() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Composition, Pred> compositions_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Composition, Pred>(impl, meta_compositions); };
		::Udm::ChildrenAttr< ::Uml::Association> associations() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Association, Pred> associations_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Association, Pred>(impl, meta_associations); };
		::Udm::ChildrenAttr< ::Uml::Class> Class_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Class, Pred> Class_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Class, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::Association> Association_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Association, Pred> Association_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Association, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::Composition> Composition_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Composition, Pred> Composition_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Composition, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::Uml::Namespace> Namespace_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::Uml::Namespace, Pred> Namespace_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::Uml::Namespace, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::Uml::Diagram> parent() const;
		::Udm::ParentAttr< ::Uml::Namespace> parent_ns() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::CompositionChildRole meta_namespaces;
		static ::Uml::CompositionChildRole meta_classes;
		static ::Uml::CompositionChildRole meta_compositions;
		static ::Uml::CompositionChildRole meta_associations;
		static ::Uml::CompositionParentRole meta_parent;
		static ::Uml::CompositionParentRole meta_parent_ns;

	};

	class UDM_DLL TaggedValue : public ::Udm::Object {
	public:
		TaggedValue();
		TaggedValue(::Udm::ObjectImpl *impl);
		TaggedValue(const TaggedValue &master);

#ifdef UDM_RVALUE
		TaggedValue(TaggedValue &&master);

		static TaggedValue Cast(::Udm::Object &&a);
		TaggedValue& operator=(TaggedValue &&a);
		TaggedValue& operator=(const TaggedValue &a);

#endif
		static TaggedValue Cast(const ::Udm::Object &a);
		static TaggedValue Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		TaggedValue CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< TaggedValue> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< TaggedValue, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< TaggedValue, Pred>(impl); };
		TaggedValue CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< TaggedValue> Derived();
		template <class Pred> ::Udm::DerivedAttr< TaggedValue, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< TaggedValue, Pred>(impl); };
		::Udm::ArchetypeAttr< TaggedValue> Archetype() const;
		::Udm::StringAttr value() const;
		::Udm::StringAttr name() const;
		::Udm::ParentAttr< ::Uml::Association> assocParent() const;
		::Udm::ParentAttr< ::Uml::Class> classParent() const;
		::Udm::ParentAttr< ::Udm::Object> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_value;
		static ::Uml::Attribute meta_name;
		static ::Uml::CompositionParentRole meta_assocParent;
		static ::Uml::CompositionParentRole meta_classParent;

	};

	class UDM_DLL ConstraintDefinition : public ::Udm::Object {
	public:
		ConstraintDefinition();
		ConstraintDefinition(::Udm::ObjectImpl *impl);
		ConstraintDefinition(const ConstraintDefinition &master);

#ifdef UDM_RVALUE
		ConstraintDefinition(ConstraintDefinition &&master);

		static ConstraintDefinition Cast(::Udm::Object &&a);
		ConstraintDefinition& operator=(ConstraintDefinition &&a);
		ConstraintDefinition& operator=(const ConstraintDefinition &a);

#endif
		static ConstraintDefinition Cast(const ::Udm::Object &a);
		static ConstraintDefinition Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		ConstraintDefinition CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< ConstraintDefinition> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< ConstraintDefinition, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< ConstraintDefinition, Pred>(impl); };
		ConstraintDefinition CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< ConstraintDefinition> Derived();
		template <class Pred> ::Udm::DerivedAttr< ConstraintDefinition, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< ConstraintDefinition, Pred>(impl); };
		::Udm::ArchetypeAttr< ConstraintDefinition> Archetype() const;
		::Udm::StringAttr stereotype() const;
		::Udm::StringAttr name() const;
		::Udm::StringAttr parameterList() const;
		::Udm::StringAttr expression() const;
		::Udm::StringAttr returnType() const;
		::Udm::ParentAttr< ::Uml::Class> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_stereotype;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_parameterList;
		static ::Uml::Attribute meta_expression;
		static ::Uml::Attribute meta_returnType;
		static ::Uml::CompositionParentRole meta_parent;

	};

	class UDM_DLL Attribute : public ::Udm::Object {
	public:
		Attribute();
		Attribute(::Udm::ObjectImpl *impl);
		Attribute(const Attribute &master);

#ifdef UDM_RVALUE
		Attribute(Attribute &&master);

		static Attribute Cast(::Udm::Object &&a);
		Attribute& operator=(Attribute &&a);
		Attribute& operator=(const Attribute &a);

#endif
		static Attribute Cast(const ::Udm::Object &a);
		static Attribute Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Attribute CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Attribute> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Attribute, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Attribute, Pred>(impl); };
		Attribute CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Attribute> Derived();
		template <class Pred> ::Udm::DerivedAttr< Attribute, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Attribute, Pred>(impl); };
		::Udm::ArchetypeAttr< Attribute> Archetype() const;
		::Udm::StringAttr name() const;
		::Udm::StringAttr type() const;
		::Udm::StringAttrArr defvalue() const;
		::Udm::IntegerAttr min() const;
		::Udm::IntegerAttr max() const;
		::Udm::StringAttr visibility() const;
		::Udm::BooleanAttr ordered() const;
		::Udm::BooleanAttr nonpersistent() const;
		::Udm::BooleanAttr registry() const;
		::Udm::ParentAttr< ::Uml::Class> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_type;
		static ::Uml::Attribute meta_defvalue;
		static ::Uml::Attribute meta_min;
		static ::Uml::Attribute meta_max;
		static ::Uml::Attribute meta_visibility;
		static ::Uml::Attribute meta_ordered;
		static ::Uml::Attribute meta_nonpersistent;
		static ::Uml::Attribute meta_registry;
		static ::Uml::CompositionParentRole meta_parent;

	};

	class UDM_DLL Constraint : public ::Udm::Object {
	public:
		Constraint();
		Constraint(::Udm::ObjectImpl *impl);
		Constraint(const Constraint &master);

#ifdef UDM_RVALUE
		Constraint(Constraint &&master);

		static Constraint Cast(::Udm::Object &&a);
		Constraint& operator=(Constraint &&a);
		Constraint& operator=(const Constraint &a);

#endif
		static Constraint Cast(const ::Udm::Object &a);
		static Constraint Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Constraint CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Constraint> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Constraint, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Constraint, Pred>(impl); };
		Constraint CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Constraint> Derived();
		template <class Pred> ::Udm::DerivedAttr< Constraint, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Constraint, Pred>(impl); };
		::Udm::ArchetypeAttr< Constraint> Archetype() const;
		::Udm::StringAttr description() const;
		::Udm::StringAttr name() const;
		::Udm::StringAttr expression() const;
		::Udm::ParentAttr< ::Uml::Class> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_description;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_expression;
		static ::Uml::CompositionParentRole meta_parent;

	};

}

#endif // MOBIES_UML_H
