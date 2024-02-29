// cpp (meta datanetwork format) source file Uml.cpp
// generated from diagram Uml
// generated on Thu Nov 05 17:19:40 2015

#include "Uml.h"
#include <UmlExt.h>
#include <UdmStatic.h>

#include <UdmDom.h>
#include "Uml_xsd.h"
using namespace std;

namespace Uml {

	CompositionChildRole::CompositionChildRole() {}
	CompositionChildRole::CompositionChildRole(::Udm::ObjectImpl *impl) : GenericRole(impl) {}
	CompositionChildRole::CompositionChildRole(const CompositionChildRole &master) : GenericRole(master) {}

#ifdef UDM_RVALUE
	CompositionChildRole::CompositionChildRole(CompositionChildRole &&master) : GenericRole(master) {};

	CompositionChildRole CompositionChildRole::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	CompositionChildRole& CompositionChildRole::operator=(CompositionChildRole &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	CompositionChildRole& CompositionChildRole::operator=(const CompositionChildRole &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	CompositionChildRole CompositionChildRole::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	CompositionChildRole CompositionChildRole::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	CompositionChildRole CompositionChildRole::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< CompositionChildRole> CompositionChildRole::Instances() { return ::Udm::InstantiatedAttr< CompositionChildRole>(impl); }
	CompositionChildRole CompositionChildRole::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< CompositionChildRole> CompositionChildRole::Derived() { return ::Udm::DerivedAttr< CompositionChildRole>(impl); }
	::Udm::ArchetypeAttr< CompositionChildRole> CompositionChildRole::Archetype() const { return ::Udm::ArchetypeAttr< CompositionChildRole>(impl); }
	::Udm::IntegerAttr CompositionChildRole::max() const { return ::Udm::IntegerAttr(impl, meta_max); }
	::Udm::IntegerAttr CompositionChildRole::min() const { return ::Udm::IntegerAttr(impl, meta_min); }
	::Udm::PointerAttr< Class> CompositionChildRole::target() const { return ::Udm::PointerAttr< Class>(impl, meta_target); }
	::Udm::ParentAttr< ::Uml::Composition> CompositionChildRole::parent() const { return ::Udm::ParentAttr< ::Uml::Composition>(impl, meta_parent); }

	::Uml::Class CompositionChildRole::meta;
	::Uml::Attribute CompositionChildRole::meta_max;
	::Uml::Attribute CompositionChildRole::meta_min;
	::Uml::AssociationRole CompositionChildRole::meta_target;
	::Uml::CompositionParentRole CompositionChildRole::meta_parent;

	GenericRole::GenericRole() {}
	GenericRole::GenericRole(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	GenericRole::GenericRole(const GenericRole &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	GenericRole::GenericRole(GenericRole &&master) : UDM_OBJECT(master) {};

	GenericRole GenericRole::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	GenericRole& GenericRole::operator=(GenericRole &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	GenericRole& GenericRole::operator=(const GenericRole &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	GenericRole GenericRole::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	GenericRole GenericRole::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	GenericRole GenericRole::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< GenericRole> GenericRole::Instances() { return ::Udm::InstantiatedAttr< GenericRole>(impl); }
	GenericRole GenericRole::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< GenericRole> GenericRole::Derived() { return ::Udm::DerivedAttr< GenericRole>(impl); }
	::Udm::ArchetypeAttr< GenericRole> GenericRole::Archetype() const { return ::Udm::ArchetypeAttr< GenericRole>(impl); }
	::Udm::BooleanAttr GenericRole::isNavigable() const { return ::Udm::BooleanAttr(impl, meta_isNavigable); }
	::Udm::StringAttr GenericRole::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::ParentAttr< ::Udm::Object> GenericRole::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class GenericRole::meta;
	::Uml::Attribute GenericRole::meta_isNavigable;
	::Uml::Attribute GenericRole::meta_name;

	CompositionParentRole::CompositionParentRole() {}
	CompositionParentRole::CompositionParentRole(::Udm::ObjectImpl *impl) : GenericRole(impl) {}
	CompositionParentRole::CompositionParentRole(const CompositionParentRole &master) : GenericRole(master) {}

#ifdef UDM_RVALUE
	CompositionParentRole::CompositionParentRole(CompositionParentRole &&master) : GenericRole(master) {};

	CompositionParentRole CompositionParentRole::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	CompositionParentRole& CompositionParentRole::operator=(CompositionParentRole &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	CompositionParentRole& CompositionParentRole::operator=(const CompositionParentRole &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	CompositionParentRole CompositionParentRole::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	CompositionParentRole CompositionParentRole::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	CompositionParentRole CompositionParentRole::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< CompositionParentRole> CompositionParentRole::Instances() { return ::Udm::InstantiatedAttr< CompositionParentRole>(impl); }
	CompositionParentRole CompositionParentRole::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< CompositionParentRole> CompositionParentRole::Derived() { return ::Udm::DerivedAttr< CompositionParentRole>(impl); }
	::Udm::ArchetypeAttr< CompositionParentRole> CompositionParentRole::Archetype() const { return ::Udm::ArchetypeAttr< CompositionParentRole>(impl); }
	::Udm::PointerAttr< Class> CompositionParentRole::target() const { return ::Udm::PointerAttr< Class>(impl, meta_target); }
	::Udm::ParentAttr< ::Uml::Composition> CompositionParentRole::parent() const { return ::Udm::ParentAttr< ::Uml::Composition>(impl, meta_parent); }

	::Uml::Class CompositionParentRole::meta;
	::Uml::AssociationRole CompositionParentRole::meta_target;
	::Uml::CompositionParentRole CompositionParentRole::meta_parent;

	AssociationRole::AssociationRole() {}
	AssociationRole::AssociationRole(::Udm::ObjectImpl *impl) : GenericRole(impl) {}
	AssociationRole::AssociationRole(const AssociationRole &master) : GenericRole(master) {}

#ifdef UDM_RVALUE
	AssociationRole::AssociationRole(AssociationRole &&master) : GenericRole(master) {};

	AssociationRole AssociationRole::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	AssociationRole& AssociationRole::operator=(AssociationRole &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	AssociationRole& AssociationRole::operator=(const AssociationRole &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	AssociationRole AssociationRole::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	AssociationRole AssociationRole::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	AssociationRole AssociationRole::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< AssociationRole> AssociationRole::Instances() { return ::Udm::InstantiatedAttr< AssociationRole>(impl); }
	AssociationRole AssociationRole::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< AssociationRole> AssociationRole::Derived() { return ::Udm::DerivedAttr< AssociationRole>(impl); }
	::Udm::ArchetypeAttr< AssociationRole> AssociationRole::Archetype() const { return ::Udm::ArchetypeAttr< AssociationRole>(impl); }
	::Udm::IntegerAttr AssociationRole::max() const { return ::Udm::IntegerAttr(impl, meta_max); }
	::Udm::IntegerAttr AssociationRole::min() const { return ::Udm::IntegerAttr(impl, meta_min); }
	::Udm::BooleanAttr AssociationRole::isPrimary() const { return ::Udm::BooleanAttr(impl, meta_isPrimary); }
	::Udm::PointerAttr< AssociationRole> AssociationRole::rp_helper() const { return ::Udm::PointerAttr< AssociationRole>(impl, meta_rp_helper); }
	::Udm::PointerAttr< AssociationRole> AssociationRole::rp_helper_user() const { return ::Udm::PointerAttr< AssociationRole>(impl, meta_rp_helper_user); }
	::Udm::PointerAttr< Class> AssociationRole::target() const { return ::Udm::PointerAttr< Class>(impl, meta_target); }
	::Udm::ParentAttr< ::Uml::Association> AssociationRole::parent() const { return ::Udm::ParentAttr< ::Uml::Association>(impl, meta_parent); }

	::Uml::Class AssociationRole::meta;
	::Uml::Attribute AssociationRole::meta_max;
	::Uml::Attribute AssociationRole::meta_min;
	::Uml::Attribute AssociationRole::meta_isPrimary;
	::Uml::AssociationRole AssociationRole::meta_rp_helper;
	::Uml::AssociationRole AssociationRole::meta_rp_helper_user;
	::Uml::AssociationRole AssociationRole::meta_target;
	::Uml::CompositionParentRole AssociationRole::meta_parent;

	Class::Class() {}
	Class::Class(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Class::Class(const Class &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Class::Class(Class &&master) : UDM_OBJECT(master) {};

	Class Class::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Class& Class::operator=(Class &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	Class& Class::operator=(const Class &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	Class Class::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Class Class::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Class Class::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Class> Class::Instances() { return ::Udm::InstantiatedAttr< Class>(impl); }
	Class Class::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Class> Class::Derived() { return ::Udm::DerivedAttr< Class>(impl); }
	::Udm::ArchetypeAttr< Class> Class::Archetype() const { return ::Udm::ArchetypeAttr< Class>(impl); }
	::Udm::BooleanAttr Class::isAbstract() const { return ::Udm::BooleanAttr(impl, meta_isAbstract); }
	::Udm::StringAttr Class::stereotype() const { return ::Udm::StringAttr(impl, meta_stereotype); }
	::Udm::StringAttr Class::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::StringAttr Class::from() const { return ::Udm::StringAttr(impl, meta_from); }
	::Udm::AssocAttr< CompositionChildRole> Class::childRoles() const { return ::Udm::AssocAttr< CompositionChildRole>(impl, meta_childRoles); }
	::Udm::AssocAttr< CompositionParentRole> Class::parentRoles() const { return ::Udm::AssocAttr< CompositionParentRole>(impl, meta_parentRoles); }
	::Udm::AssocAttr< AssociationRole> Class::associationRoles() const { return ::Udm::AssocAttr< AssociationRole>(impl, meta_associationRoles); }
	::Udm::PointerAttr< Association> Class::association() const { return ::Udm::PointerAttr< Association>(impl, meta_association); }
	::Udm::AssocAttr< Class> Class::baseTypes() const { return ::Udm::AssocAttr< Class>(impl, meta_baseTypes); }
	::Udm::AssocAttr< Class> Class::subTypes() const { return ::Udm::AssocAttr< Class>(impl, meta_subTypes); }
	::Udm::ChildrenAttr< ::Uml::ConstraintDefinition> Class::definitions() const { return ::Udm::ChildrenAttr< ::Uml::ConstraintDefinition>(impl, meta_definitions); }
	::Udm::ChildrenAttr< ::Uml::Attribute> Class::attributes() const { return ::Udm::ChildrenAttr< ::Uml::Attribute>(impl, meta_attributes); }
	::Udm::ChildrenAttr< ::Uml::Constraint> Class::constraints() const { return ::Udm::ChildrenAttr< ::Uml::Constraint>(impl, meta_constraints); }
	::Udm::ChildrenAttr< ::Uml::TaggedValue> Class::tagValues() const { return ::Udm::ChildrenAttr< ::Uml::TaggedValue>(impl, meta_tagValues); }
	::Udm::ChildrenAttr< ::Uml::TaggedValue> Class::TaggedValue_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::TaggedValue>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::ConstraintDefinition> Class::ConstraintDefinition_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::ConstraintDefinition>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::Attribute> Class::Attribute_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::Attribute>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::Constraint> Class::Constraint_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::Constraint>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::Uml::Diagram> Class::parent() const { return ::Udm::ParentAttr< ::Uml::Diagram>(impl, meta_parent); }
	::Udm::ParentAttr< ::Uml::Namespace> Class::parent_ns() const { return ::Udm::ParentAttr< ::Uml::Namespace>(impl, meta_parent_ns); }

	::Uml::Class Class::meta;
	::Uml::Attribute Class::meta_isAbstract;
	::Uml::Attribute Class::meta_stereotype;
	::Uml::Attribute Class::meta_name;
	::Uml::Attribute Class::meta_from;
	::Uml::AssociationRole Class::meta_childRoles;
	::Uml::AssociationRole Class::meta_parentRoles;
	::Uml::AssociationRole Class::meta_associationRoles;
	::Uml::AssociationRole Class::meta_association;
	::Uml::AssociationRole Class::meta_baseTypes;
	::Uml::AssociationRole Class::meta_subTypes;
	::Uml::CompositionChildRole Class::meta_definitions;
	::Uml::CompositionChildRole Class::meta_attributes;
	::Uml::CompositionChildRole Class::meta_constraints;
	::Uml::CompositionChildRole Class::meta_tagValues;
	::Uml::CompositionParentRole Class::meta_parent;
	::Uml::CompositionParentRole Class::meta_parent_ns;

	Diagram::Diagram() {}
	Diagram::Diagram(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Diagram::Diagram(const Diagram &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Diagram::Diagram(Diagram &&master) : UDM_OBJECT(master) {};

	Diagram Diagram::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Diagram& Diagram::operator=(Diagram &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	Diagram& Diagram::operator=(const Diagram &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	Diagram Diagram::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Diagram Diagram::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Diagram Diagram::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Diagram> Diagram::Instances() { return ::Udm::InstantiatedAttr< Diagram>(impl); }
	Diagram Diagram::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Diagram> Diagram::Derived() { return ::Udm::DerivedAttr< Diagram>(impl); }
	::Udm::ArchetypeAttr< Diagram> Diagram::Archetype() const { return ::Udm::ArchetypeAttr< Diagram>(impl); }
	::Udm::StringAttr Diagram::version() const { return ::Udm::StringAttr(impl, meta_version); }
	::Udm::StringAttr Diagram::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::ChildrenAttr< ::Uml::Class> Diagram::classes() const { return ::Udm::ChildrenAttr< ::Uml::Class>(impl, meta_classes); }
	::Udm::ChildrenAttr< ::Uml::Association> Diagram::associations() const { return ::Udm::ChildrenAttr< ::Uml::Association>(impl, meta_associations); }
	::Udm::ChildrenAttr< ::Uml::Composition> Diagram::compositions() const { return ::Udm::ChildrenAttr< ::Uml::Composition>(impl, meta_compositions); }
	::Udm::ChildrenAttr< ::Uml::Namespace> Diagram::namespaces() const { return ::Udm::ChildrenAttr< ::Uml::Namespace>(impl, meta_namespaces); }
	::Udm::ChildrenAttr< ::Uml::Class> Diagram::Class_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::Class>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::Association> Diagram::Association_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::Association>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::Composition> Diagram::Composition_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::Composition>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::Namespace> Diagram::Namespace_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::Namespace>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::Udm::Object> Diagram::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Diagram::meta;
	::Uml::Attribute Diagram::meta_version;
	::Uml::Attribute Diagram::meta_name;
	::Uml::CompositionChildRole Diagram::meta_classes;
	::Uml::CompositionChildRole Diagram::meta_associations;
	::Uml::CompositionChildRole Diagram::meta_compositions;
	::Uml::CompositionChildRole Diagram::meta_namespaces;

	Association::Association() {}
	Association::Association(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Association::Association(const Association &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Association::Association(Association &&master) : UDM_OBJECT(master) {};

	Association Association::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Association& Association::operator=(Association &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	Association& Association::operator=(const Association &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	Association Association::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Association Association::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Association Association::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Association> Association::Instances() { return ::Udm::InstantiatedAttr< Association>(impl); }
	Association Association::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Association> Association::Derived() { return ::Udm::DerivedAttr< Association>(impl); }
	::Udm::ArchetypeAttr< Association> Association::Archetype() const { return ::Udm::ArchetypeAttr< Association>(impl); }
	::Udm::BooleanAttr Association::nonpersistent() const { return ::Udm::BooleanAttr(impl, meta_nonpersistent); }
	::Udm::StringAttr Association::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::PointerAttr< Class> Association::assocClass() const { return ::Udm::PointerAttr< Class>(impl, meta_assocClass); }
	::Udm::ChildrenAttr< ::Uml::AssociationRole> Association::roles() const { return ::Udm::ChildrenAttr< ::Uml::AssociationRole>(impl, meta_roles); }
	::Udm::ChildrenAttr< ::Uml::TaggedValue> Association::tagValues() const { return ::Udm::ChildrenAttr< ::Uml::TaggedValue>(impl, meta_tagValues); }
	::Udm::ChildrenAttr< ::Uml::GenericRole> Association::GenericRole_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::GenericRole>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::AssociationRole> Association::AssociationRole_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::AssociationRole>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::TaggedValue> Association::TaggedValue_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::TaggedValue>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::Uml::Diagram> Association::parent() const { return ::Udm::ParentAttr< ::Uml::Diagram>(impl, meta_parent); }
	::Udm::ParentAttr< ::Uml::Namespace> Association::parent_ns() const { return ::Udm::ParentAttr< ::Uml::Namespace>(impl, meta_parent_ns); }

	::Uml::Class Association::meta;
	::Uml::Attribute Association::meta_nonpersistent;
	::Uml::Attribute Association::meta_name;
	::Uml::AssociationRole Association::meta_assocClass;
	::Uml::CompositionChildRole Association::meta_roles;
	::Uml::CompositionChildRole Association::meta_tagValues;
	::Uml::CompositionParentRole Association::meta_parent;
	::Uml::CompositionParentRole Association::meta_parent_ns;

	Composition::Composition() {}
	Composition::Composition(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Composition::Composition(const Composition &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Composition::Composition(Composition &&master) : UDM_OBJECT(master) {};

	Composition Composition::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Composition& Composition::operator=(Composition &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	Composition& Composition::operator=(const Composition &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	Composition Composition::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Composition Composition::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Composition Composition::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Composition> Composition::Instances() { return ::Udm::InstantiatedAttr< Composition>(impl); }
	Composition Composition::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Composition> Composition::Derived() { return ::Udm::DerivedAttr< Composition>(impl); }
	::Udm::ArchetypeAttr< Composition> Composition::Archetype() const { return ::Udm::ArchetypeAttr< Composition>(impl); }
	::Udm::BooleanAttr Composition::nonpersistent() const { return ::Udm::BooleanAttr(impl, meta_nonpersistent); }
	::Udm::StringAttr Composition::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::ChildAttr< ::Uml::CompositionParentRole> Composition::parentRole() const { return ::Udm::ChildAttr< ::Uml::CompositionParentRole>(impl, meta_parentRole); }
	::Udm::ChildAttr< ::Uml::CompositionChildRole> Composition::childRole() const { return ::Udm::ChildAttr< ::Uml::CompositionChildRole>(impl, meta_childRole); }
	::Udm::ChildrenAttr< ::Uml::CompositionChildRole> Composition::CompositionChildRole_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::CompositionChildRole>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::GenericRole> Composition::GenericRole_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::GenericRole>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::CompositionParentRole> Composition::CompositionParentRole_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::CompositionParentRole>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::Uml::Diagram> Composition::parent() const { return ::Udm::ParentAttr< ::Uml::Diagram>(impl, meta_parent); }
	::Udm::ParentAttr< ::Uml::Namespace> Composition::parent_ns() const { return ::Udm::ParentAttr< ::Uml::Namespace>(impl, meta_parent_ns); }

	::Uml::Class Composition::meta;
	::Uml::Attribute Composition::meta_nonpersistent;
	::Uml::Attribute Composition::meta_name;
	::Uml::CompositionChildRole Composition::meta_parentRole;
	::Uml::CompositionChildRole Composition::meta_childRole;
	::Uml::CompositionParentRole Composition::meta_parent;
	::Uml::CompositionParentRole Composition::meta_parent_ns;

	Namespace::Namespace() {}
	Namespace::Namespace(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Namespace::Namespace(const Namespace &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Namespace::Namespace(Namespace &&master) : UDM_OBJECT(master) {};

	Namespace Namespace::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Namespace& Namespace::operator=(Namespace &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	Namespace& Namespace::operator=(const Namespace &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	Namespace Namespace::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Namespace Namespace::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Namespace Namespace::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Namespace> Namespace::Instances() { return ::Udm::InstantiatedAttr< Namespace>(impl); }
	Namespace Namespace::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Namespace> Namespace::Derived() { return ::Udm::DerivedAttr< Namespace>(impl); }
	::Udm::ArchetypeAttr< Namespace> Namespace::Archetype() const { return ::Udm::ArchetypeAttr< Namespace>(impl); }
	::Udm::StringAttr Namespace::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::ChildrenAttr< ::Uml::Namespace> Namespace::namespaces() const { return ::Udm::ChildrenAttr< ::Uml::Namespace>(impl, meta_namespaces); }
	::Udm::ChildrenAttr< ::Uml::Class> Namespace::classes() const { return ::Udm::ChildrenAttr< ::Uml::Class>(impl, meta_classes); }
	::Udm::ChildrenAttr< ::Uml::Composition> Namespace::compositions() const { return ::Udm::ChildrenAttr< ::Uml::Composition>(impl, meta_compositions); }
	::Udm::ChildrenAttr< ::Uml::Association> Namespace::associations() const { return ::Udm::ChildrenAttr< ::Uml::Association>(impl, meta_associations); }
	::Udm::ChildrenAttr< ::Uml::Class> Namespace::Class_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::Class>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::Association> Namespace::Association_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::Association>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::Composition> Namespace::Composition_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::Composition>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::Uml::Namespace> Namespace::Namespace_kind_children() const { return ::Udm::ChildrenAttr< ::Uml::Namespace>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::Uml::Diagram> Namespace::parent() const { return ::Udm::ParentAttr< ::Uml::Diagram>(impl, meta_parent); }
	::Udm::ParentAttr< ::Uml::Namespace> Namespace::parent_ns() const { return ::Udm::ParentAttr< ::Uml::Namespace>(impl, meta_parent_ns); }

	::Uml::Class Namespace::meta;
	::Uml::Attribute Namespace::meta_name;
	::Uml::CompositionChildRole Namespace::meta_namespaces;
	::Uml::CompositionChildRole Namespace::meta_classes;
	::Uml::CompositionChildRole Namespace::meta_compositions;
	::Uml::CompositionChildRole Namespace::meta_associations;
	::Uml::CompositionParentRole Namespace::meta_parent;
	::Uml::CompositionParentRole Namespace::meta_parent_ns;

	TaggedValue::TaggedValue() {}
	TaggedValue::TaggedValue(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	TaggedValue::TaggedValue(const TaggedValue &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	TaggedValue::TaggedValue(TaggedValue &&master) : UDM_OBJECT(master) {};

	TaggedValue TaggedValue::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	TaggedValue& TaggedValue::operator=(TaggedValue &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	TaggedValue& TaggedValue::operator=(const TaggedValue &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	TaggedValue TaggedValue::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	TaggedValue TaggedValue::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	TaggedValue TaggedValue::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< TaggedValue> TaggedValue::Instances() { return ::Udm::InstantiatedAttr< TaggedValue>(impl); }
	TaggedValue TaggedValue::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< TaggedValue> TaggedValue::Derived() { return ::Udm::DerivedAttr< TaggedValue>(impl); }
	::Udm::ArchetypeAttr< TaggedValue> TaggedValue::Archetype() const { return ::Udm::ArchetypeAttr< TaggedValue>(impl); }
	::Udm::StringAttr TaggedValue::value() const { return ::Udm::StringAttr(impl, meta_value); }
	::Udm::StringAttr TaggedValue::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::ParentAttr< ::Uml::Association> TaggedValue::assocParent() const { return ::Udm::ParentAttr< ::Uml::Association>(impl, meta_assocParent); }
	::Udm::ParentAttr< ::Uml::Class> TaggedValue::classParent() const { return ::Udm::ParentAttr< ::Uml::Class>(impl, meta_classParent); }
	::Udm::ParentAttr< ::Udm::Object> TaggedValue::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class TaggedValue::meta;
	::Uml::Attribute TaggedValue::meta_value;
	::Uml::Attribute TaggedValue::meta_name;
	::Uml::CompositionParentRole TaggedValue::meta_assocParent;
	::Uml::CompositionParentRole TaggedValue::meta_classParent;

	ConstraintDefinition::ConstraintDefinition() {}
	ConstraintDefinition::ConstraintDefinition(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	ConstraintDefinition::ConstraintDefinition(const ConstraintDefinition &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	ConstraintDefinition::ConstraintDefinition(ConstraintDefinition &&master) : UDM_OBJECT(master) {};

	ConstraintDefinition ConstraintDefinition::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	ConstraintDefinition& ConstraintDefinition::operator=(ConstraintDefinition &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	ConstraintDefinition& ConstraintDefinition::operator=(const ConstraintDefinition &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	ConstraintDefinition ConstraintDefinition::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	ConstraintDefinition ConstraintDefinition::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	ConstraintDefinition ConstraintDefinition::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< ConstraintDefinition> ConstraintDefinition::Instances() { return ::Udm::InstantiatedAttr< ConstraintDefinition>(impl); }
	ConstraintDefinition ConstraintDefinition::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< ConstraintDefinition> ConstraintDefinition::Derived() { return ::Udm::DerivedAttr< ConstraintDefinition>(impl); }
	::Udm::ArchetypeAttr< ConstraintDefinition> ConstraintDefinition::Archetype() const { return ::Udm::ArchetypeAttr< ConstraintDefinition>(impl); }
	::Udm::StringAttr ConstraintDefinition::stereotype() const { return ::Udm::StringAttr(impl, meta_stereotype); }
	::Udm::StringAttr ConstraintDefinition::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::StringAttr ConstraintDefinition::parameterList() const { return ::Udm::StringAttr(impl, meta_parameterList); }
	::Udm::StringAttr ConstraintDefinition::expression() const { return ::Udm::StringAttr(impl, meta_expression); }
	::Udm::StringAttr ConstraintDefinition::returnType() const { return ::Udm::StringAttr(impl, meta_returnType); }
	::Udm::ParentAttr< ::Uml::Class> ConstraintDefinition::parent() const { return ::Udm::ParentAttr< ::Uml::Class>(impl, meta_parent); }

	::Uml::Class ConstraintDefinition::meta;
	::Uml::Attribute ConstraintDefinition::meta_stereotype;
	::Uml::Attribute ConstraintDefinition::meta_name;
	::Uml::Attribute ConstraintDefinition::meta_parameterList;
	::Uml::Attribute ConstraintDefinition::meta_expression;
	::Uml::Attribute ConstraintDefinition::meta_returnType;
	::Uml::CompositionParentRole ConstraintDefinition::meta_parent;

	Attribute::Attribute() {}
	Attribute::Attribute(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Attribute::Attribute(const Attribute &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Attribute::Attribute(Attribute &&master) : UDM_OBJECT(master) {};

	Attribute Attribute::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Attribute& Attribute::operator=(Attribute &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	Attribute& Attribute::operator=(const Attribute &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	Attribute Attribute::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Attribute Attribute::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Attribute Attribute::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Attribute> Attribute::Instances() { return ::Udm::InstantiatedAttr< Attribute>(impl); }
	Attribute Attribute::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Attribute> Attribute::Derived() { return ::Udm::DerivedAttr< Attribute>(impl); }
	::Udm::ArchetypeAttr< Attribute> Attribute::Archetype() const { return ::Udm::ArchetypeAttr< Attribute>(impl); }
	::Udm::StringAttr Attribute::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::StringAttr Attribute::type() const { return ::Udm::StringAttr(impl, meta_type); }
	::Udm::StringAttrArr Attribute::defvalue() const { return ::Udm::StringAttrArr(impl, meta_defvalue); }
	::Udm::IntegerAttr Attribute::min() const { return ::Udm::IntegerAttr(impl, meta_min); }
	::Udm::IntegerAttr Attribute::max() const { return ::Udm::IntegerAttr(impl, meta_max); }
	::Udm::StringAttr Attribute::visibility() const { return ::Udm::StringAttr(impl, meta_visibility); }
	::Udm::BooleanAttr Attribute::ordered() const { return ::Udm::BooleanAttr(impl, meta_ordered); }
	::Udm::BooleanAttr Attribute::nonpersistent() const { return ::Udm::BooleanAttr(impl, meta_nonpersistent); }
	::Udm::BooleanAttr Attribute::registry() const { return ::Udm::BooleanAttr(impl, meta_registry); }
	::Udm::ParentAttr< ::Uml::Class> Attribute::parent() const { return ::Udm::ParentAttr< ::Uml::Class>(impl, meta_parent); }

	::Uml::Class Attribute::meta;
	::Uml::Attribute Attribute::meta_name;
	::Uml::Attribute Attribute::meta_type;
	::Uml::Attribute Attribute::meta_defvalue;
	::Uml::Attribute Attribute::meta_min;
	::Uml::Attribute Attribute::meta_max;
	::Uml::Attribute Attribute::meta_visibility;
	::Uml::Attribute Attribute::meta_ordered;
	::Uml::Attribute Attribute::meta_nonpersistent;
	::Uml::Attribute Attribute::meta_registry;
	::Uml::CompositionParentRole Attribute::meta_parent;

	Constraint::Constraint() {}
	Constraint::Constraint(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Constraint::Constraint(const Constraint &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Constraint::Constraint(Constraint &&master) : UDM_OBJECT(master) {};

	Constraint Constraint::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Constraint& Constraint::operator=(Constraint &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	Constraint& Constraint::operator=(const Constraint &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	Constraint Constraint::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Constraint Constraint::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Constraint Constraint::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Constraint> Constraint::Instances() { return ::Udm::InstantiatedAttr< Constraint>(impl); }
	Constraint Constraint::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Constraint> Constraint::Derived() { return ::Udm::DerivedAttr< Constraint>(impl); }
	::Udm::ArchetypeAttr< Constraint> Constraint::Archetype() const { return ::Udm::ArchetypeAttr< Constraint>(impl); }
	::Udm::StringAttr Constraint::description() const { return ::Udm::StringAttr(impl, meta_description); }
	::Udm::StringAttr Constraint::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::StringAttr Constraint::expression() const { return ::Udm::StringAttr(impl, meta_expression); }
	::Udm::ParentAttr< ::Uml::Class> Constraint::parent() const { return ::Udm::ParentAttr< ::Uml::Class>(impl, meta_parent); }

	::Uml::Class Constraint::meta;
	::Uml::Attribute Constraint::meta_description;
	::Uml::Attribute Constraint::meta_name;
	::Uml::Attribute Constraint::meta_expression;
	::Uml::CompositionParentRole Constraint::meta_parent;

	::Uml::Diagram meta;

	void CreateMeta() {
		// classes, with attributes, constraints and constraint definitions
		Association::meta = ::Uml::CreateClass();
		Association::meta_nonpersistent = ::Uml::CreateAttribute();
		Association::meta_name = ::Uml::CreateAttribute();

		AssociationRole::meta = ::Uml::CreateClass();
		AssociationRole::meta_max = ::Uml::CreateAttribute();
		AssociationRole::meta_min = ::Uml::CreateAttribute();
		AssociationRole::meta_isPrimary = ::Uml::CreateAttribute();

		Attribute::meta = ::Uml::CreateClass();
		Attribute::meta_name = ::Uml::CreateAttribute();
		Attribute::meta_type = ::Uml::CreateAttribute();
		Attribute::meta_defvalue = ::Uml::CreateAttribute();
		Attribute::meta_min = ::Uml::CreateAttribute();
		Attribute::meta_max = ::Uml::CreateAttribute();
		Attribute::meta_visibility = ::Uml::CreateAttribute();
		Attribute::meta_ordered = ::Uml::CreateAttribute();
		Attribute::meta_nonpersistent = ::Uml::CreateAttribute();
		Attribute::meta_registry = ::Uml::CreateAttribute();

		Class::meta = ::Uml::CreateClass();
		Class::meta_isAbstract = ::Uml::CreateAttribute();
		Class::meta_stereotype = ::Uml::CreateAttribute();
		Class::meta_name = ::Uml::CreateAttribute();
		Class::meta_from = ::Uml::CreateAttribute();

		Composition::meta = ::Uml::CreateClass();
		Composition::meta_nonpersistent = ::Uml::CreateAttribute();
		Composition::meta_name = ::Uml::CreateAttribute();

		CompositionChildRole::meta = ::Uml::CreateClass();
		CompositionChildRole::meta_max = ::Uml::CreateAttribute();
		CompositionChildRole::meta_min = ::Uml::CreateAttribute();

		CompositionParentRole::meta = ::Uml::CreateClass();

		Constraint::meta = ::Uml::CreateClass();
		Constraint::meta_description = ::Uml::CreateAttribute();
		Constraint::meta_name = ::Uml::CreateAttribute();
		Constraint::meta_expression = ::Uml::CreateAttribute();

		ConstraintDefinition::meta = ::Uml::CreateClass();
		ConstraintDefinition::meta_stereotype = ::Uml::CreateAttribute();
		ConstraintDefinition::meta_name = ::Uml::CreateAttribute();
		ConstraintDefinition::meta_parameterList = ::Uml::CreateAttribute();
		ConstraintDefinition::meta_expression = ::Uml::CreateAttribute();
		ConstraintDefinition::meta_returnType = ::Uml::CreateAttribute();

		Diagram::meta = ::Uml::CreateClass();
		Diagram::meta_version = ::Uml::CreateAttribute();
		Diagram::meta_name = ::Uml::CreateAttribute();

		GenericRole::meta = ::Uml::CreateClass();
		GenericRole::meta_isNavigable = ::Uml::CreateAttribute();
		GenericRole::meta_name = ::Uml::CreateAttribute();

		Namespace::meta = ::Uml::CreateClass();
		Namespace::meta_name = ::Uml::CreateAttribute();

		TaggedValue::meta = ::Uml::CreateClass();
		TaggedValue::meta_value = ::Uml::CreateAttribute();
		TaggedValue::meta_name = ::Uml::CreateAttribute();

		CompositionChildRole::meta_target = ::Uml::CreateAssociationRole();
		Class::meta_childRoles = ::Uml::CreateAssociationRole();
		CompositionParentRole::meta_target = ::Uml::CreateAssociationRole();
		Class::meta_parentRoles = ::Uml::CreateAssociationRole();
		AssociationRole::meta_rp_helper = ::Uml::CreateAssociationRole();
		AssociationRole::meta_rp_helper_user = ::Uml::CreateAssociationRole();
		AssociationRole::meta_target = ::Uml::CreateAssociationRole();
		Class::meta_associationRoles = ::Uml::CreateAssociationRole();
		Association::meta_assocClass = ::Uml::CreateAssociationRole();
		Class::meta_association = ::Uml::CreateAssociationRole();
		Class::meta_baseTypes = ::Uml::CreateAssociationRole();
		Class::meta_subTypes = ::Uml::CreateAssociationRole();

		ConstraintDefinition::meta_parent = ::Uml::CreateCompositionParentRole();
		Class::meta_definitions = ::Uml::CreateCompositionChildRole();
		Attribute::meta_parent = ::Uml::CreateCompositionParentRole();
		Class::meta_attributes = ::Uml::CreateCompositionChildRole();
		Constraint::meta_parent = ::Uml::CreateCompositionParentRole();
		Class::meta_constraints = ::Uml::CreateCompositionChildRole();
		TaggedValue::meta_classParent = ::Uml::CreateCompositionParentRole();
		Class::meta_tagValues = ::Uml::CreateCompositionChildRole();
		Class::meta_parent = ::Uml::CreateCompositionParentRole();
		Diagram::meta_classes = ::Uml::CreateCompositionChildRole();
		Association::meta_parent = ::Uml::CreateCompositionParentRole();
		Diagram::meta_associations = ::Uml::CreateCompositionChildRole();
		Composition::meta_parent = ::Uml::CreateCompositionParentRole();
		Diagram::meta_compositions = ::Uml::CreateCompositionChildRole();
		Namespace::meta_parent = ::Uml::CreateCompositionParentRole();
		Diagram::meta_namespaces = ::Uml::CreateCompositionChildRole();
		AssociationRole::meta_parent = ::Uml::CreateCompositionParentRole();
		Association::meta_roles = ::Uml::CreateCompositionChildRole();
		TaggedValue::meta_assocParent = ::Uml::CreateCompositionParentRole();
		Association::meta_tagValues = ::Uml::CreateCompositionChildRole();
		CompositionParentRole::meta_parent = ::Uml::CreateCompositionParentRole();
		Composition::meta_parentRole = ::Uml::CreateCompositionChildRole();
		CompositionChildRole::meta_parent = ::Uml::CreateCompositionParentRole();
		Composition::meta_childRole = ::Uml::CreateCompositionChildRole();
		Namespace::meta_parent_ns = ::Uml::CreateCompositionParentRole();
		Namespace::meta_namespaces = ::Uml::CreateCompositionChildRole();
		Class::meta_parent_ns = ::Uml::CreateCompositionParentRole();
		Namespace::meta_classes = ::Uml::CreateCompositionChildRole();
		Composition::meta_parent_ns = ::Uml::CreateCompositionParentRole();
		Namespace::meta_compositions = ::Uml::CreateCompositionChildRole();
		Association::meta_parent_ns = ::Uml::CreateCompositionParentRole();
		Namespace::meta_associations = ::Uml::CreateCompositionChildRole();

	}

	void InitMeta() {
		// classes, with attributes, constraints and constraint definitions
		::Uml::InitClass(Association::meta, meta, "Association", false, NULL, NULL);
		vector<string> Association_nonpersistent_dva;
		Association_nonpersistent_dva.push_back("false");
		::Uml::InitAttribute(Association::meta_nonpersistent, Association::meta, "nonpersistent", "Boolean", false, false, 0, 1, false, "public", Association_nonpersistent_dva);
		::Uml::InitAttribute(Association::meta_name, Association::meta, "name", "String", false, false, 0, 1, false, "public", vector<string>());

		::Uml::InitClass(AssociationRole::meta, meta, "AssociationRole", false, NULL, NULL);
		::Uml::InitAttribute(AssociationRole::meta_max, AssociationRole::meta, "max", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(AssociationRole::meta_min, AssociationRole::meta, "min", "Integer", false, false, 1, 1, false, "public", vector<string>());
		vector<string> AssociationRole_isPrimary_dva;
		AssociationRole_isPrimary_dva.push_back("false");
		::Uml::InitAttribute(AssociationRole::meta_isPrimary, AssociationRole::meta, "isPrimary", "Boolean", false, false, 0, 1, false, "public", AssociationRole_isPrimary_dva);

		::Uml::InitClass(Attribute::meta, meta, "Attribute", false, NULL, NULL);
		::Uml::InitAttribute(Attribute::meta_name, Attribute::meta, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(Attribute::meta_type, Attribute::meta, "type", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(Attribute::meta_defvalue, Attribute::meta, "defvalue", "String", false, false, 0, -1, false, "public", vector<string>());
		::Uml::InitAttribute(Attribute::meta_min, Attribute::meta, "min", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(Attribute::meta_max, Attribute::meta, "max", "Integer", false, false, 1, 1, false, "public", vector<string>());
		vector<string> Attribute_visibility_dva;
		Attribute_visibility_dva.push_back("public");
		::Uml::InitAttribute(Attribute::meta_visibility, Attribute::meta, "visibility", "String", false, false, 0, 1, false, "public", Attribute_visibility_dva);
		vector<string> Attribute_ordered_dva;
		Attribute_ordered_dva.push_back("false");
		::Uml::InitAttribute(Attribute::meta_ordered, Attribute::meta, "ordered", "Boolean", false, false, 0, 1, false, "public", Attribute_ordered_dva);
		vector<string> Attribute_nonpersistent_dva;
		Attribute_nonpersistent_dva.push_back("false");
		::Uml::InitAttribute(Attribute::meta_nonpersistent, Attribute::meta, "nonpersistent", "Boolean", false, false, 0, 1, false, "public", Attribute_nonpersistent_dva);
		vector<string> Attribute_registry_dva;
		Attribute_registry_dva.push_back("false");
		::Uml::InitAttribute(Attribute::meta_registry, Attribute::meta, "registry", "Boolean", false, false, 0, 1, false, "public", Attribute_registry_dva);

		::Uml::InitClass(Class::meta, meta, "Class", false, NULL, NULL);
		::Uml::InitAttribute(Class::meta_isAbstract, Class::meta, "isAbstract", "Boolean", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(Class::meta_stereotype, Class::meta, "stereotype", "String", false, false, 0, 1, false, "public", vector<string>());
		::Uml::InitAttribute(Class::meta_name, Class::meta, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(Class::meta_from, Class::meta, "from", "String", false, false, 0, 1, false, "public", vector<string>());

		::Uml::InitClass(Composition::meta, meta, "Composition", false, NULL, NULL);
		vector<string> Composition_nonpersistent_dva;
		Composition_nonpersistent_dva.push_back("false");
		::Uml::InitAttribute(Composition::meta_nonpersistent, Composition::meta, "nonpersistent", "Boolean", false, false, 0, 1, false, "public", Composition_nonpersistent_dva);
		::Uml::InitAttribute(Composition::meta_name, Composition::meta, "name", "String", false, false, 0, 1, false, "public", vector<string>());

		::Uml::InitClass(CompositionChildRole::meta, meta, "CompositionChildRole", false, NULL, NULL);
		::Uml::InitAttribute(CompositionChildRole::meta_max, CompositionChildRole::meta, "max", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(CompositionChildRole::meta_min, CompositionChildRole::meta, "min", "Integer", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClass(CompositionParentRole::meta, meta, "CompositionParentRole", false, NULL, NULL);

		::Uml::InitClass(Constraint::meta, meta, "Constraint", false, NULL, NULL);
		::Uml::InitAttribute(Constraint::meta_description, Constraint::meta, "description", "String", false, false, 0, 1, false, "public", vector<string>());
		::Uml::InitAttribute(Constraint::meta_name, Constraint::meta, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(Constraint::meta_expression, Constraint::meta, "expression", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClass(ConstraintDefinition::meta, meta, "ConstraintDefinition", false, NULL, NULL);
		vector<string> ConstraintDefinition_stereotype_dva;
		ConstraintDefinition_stereotype_dva.push_back("method");
		::Uml::InitAttribute(ConstraintDefinition::meta_stereotype, ConstraintDefinition::meta, "stereotype", "String", false, false, 1, 1, false, "public", ConstraintDefinition_stereotype_dva);
		::Uml::InitAttribute(ConstraintDefinition::meta_name, ConstraintDefinition::meta, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(ConstraintDefinition::meta_parameterList, ConstraintDefinition::meta, "parameterList", "String", false, false, 0, 1, false, "public", vector<string>());
		::Uml::InitAttribute(ConstraintDefinition::meta_expression, ConstraintDefinition::meta, "expression", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttribute(ConstraintDefinition::meta_returnType, ConstraintDefinition::meta, "returnType", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClass(Diagram::meta, meta, "Diagram", false, NULL, NULL);
		::Uml::InitAttribute(Diagram::meta_version, Diagram::meta, "version", "String", false, false, 0, 1, false, "public", vector<string>());
		::Uml::InitAttribute(Diagram::meta_name, Diagram::meta, "name", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClass(GenericRole::meta, meta, "GenericRole", true, NULL, NULL);
		vector<string> GenericRole_isNavigable_dva;
		GenericRole_isNavigable_dva.push_back("true");
		::Uml::InitAttribute(GenericRole::meta_isNavigable, GenericRole::meta, "isNavigable", "Boolean", false, false, 0, 1, false, "public", GenericRole_isNavigable_dva);
		::Uml::InitAttribute(GenericRole::meta_name, GenericRole::meta, "name", "String", false, false, 0, 1, false, "public", vector<string>());

		::Uml::InitClass(Namespace::meta, meta, "Namespace", false, NULL, NULL);
		::Uml::InitAttribute(Namespace::meta_name, Namespace::meta, "name", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClass(TaggedValue::meta, meta, "TaggedValue", false, NULL, NULL);
		::Uml::InitAttribute(TaggedValue::meta_value, TaggedValue::meta, "value", "String", false, false, 0, 1, false, "public", vector<string>());
		::Uml::InitAttribute(TaggedValue::meta_name, TaggedValue::meta, "name", "String", false, false, 1, 1, false, "public", vector<string>());

	}

	void InitMetaLinks() {
		::Uml::AddInheritance(GenericRole::meta, CompositionChildRole::meta);
		::Uml::AddInheritance(GenericRole::meta, CompositionParentRole::meta);
		::Uml::AddInheritance(GenericRole::meta, AssociationRole::meta);

		// associations
		{
			::Uml::Association ass = ::Uml::CreateAssociation();
			::Uml::InitAssociation(ass, meta, "Association");
			::Uml::InitAssociationRole(CompositionChildRole::meta_target, ass, "target", true, false, 1, 1, Class::meta);
			::Uml::InitAssociationRole(Class::meta_childRoles, ass, "childRoles", true, false, 0, -1, CompositionChildRole::meta);

		}
		{
			::Uml::Association ass = ::Uml::CreateAssociation();
			::Uml::InitAssociation(ass, meta, "Association");
			::Uml::InitAssociationRole(CompositionParentRole::meta_target, ass, "target", true, false, 1, 1, Class::meta);
			::Uml::InitAssociationRole(Class::meta_parentRoles, ass, "parentRoles", true, false, 0, -1, CompositionParentRole::meta);

		}
		{
			::Uml::Association ass = ::Uml::CreateAssociation();
			::Uml::InitAssociation(ass, meta, "Association");
			::Uml::InitAssociationRole(AssociationRole::meta_rp_helper, ass, "rp_helper", true, false, 0, 1, AssociationRole::meta);
			::Uml::InitAssociationRole(AssociationRole::meta_rp_helper_user, ass, "rp_helper_user", true, false, 0, 1, AssociationRole::meta);

		}
		{
			::Uml::Association ass = ::Uml::CreateAssociation();
			::Uml::InitAssociation(ass, meta, "Association");
			::Uml::InitAssociationRole(AssociationRole::meta_target, ass, "target", true, false, 1, 1, Class::meta);
			::Uml::InitAssociationRole(Class::meta_associationRoles, ass, "associationRoles", true, false, 0, -1, AssociationRole::meta);

		}
		{
			::Uml::Association ass = ::Uml::CreateAssociation();
			::Uml::InitAssociation(ass, meta, "Association");
			::Uml::InitAssociationRole(Association::meta_assocClass, ass, "assocClass", true, false, 0, 1, Class::meta);
			::Uml::InitAssociationRole(Class::meta_association, ass, "association", true, false, 0, 1, Association::meta);

		}
		{
			::Uml::Association ass = ::Uml::CreateAssociation();
			::Uml::InitAssociation(ass, meta, "Association");
			::Uml::InitAssociationRole(Class::meta_baseTypes, ass, "baseTypes", true, false, 0, -1, Class::meta);
			::Uml::InitAssociationRole(Class::meta_subTypes, ass, "subTypes", true, false, 0, -1, Class::meta);

		}

		// compositions
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(ConstraintDefinition::meta_parent, comp, "parent", true, Class::meta);
			::Uml::InitCompositionChildRole(Class::meta_definitions, comp, "definitions", true, 0, -1, ConstraintDefinition::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Attribute::meta_parent, comp, "parent", true, Class::meta);
			::Uml::InitCompositionChildRole(Class::meta_attributes, comp, "attributes", true, 0, -1, Attribute::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Constraint::meta_parent, comp, "parent", true, Class::meta);
			::Uml::InitCompositionChildRole(Class::meta_constraints, comp, "constraints", true, 0, -1, Constraint::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(TaggedValue::meta_classParent, comp, "classParent", true, Class::meta);
			::Uml::InitCompositionChildRole(Class::meta_tagValues, comp, "tagValues", true, 0, -1, TaggedValue::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Class::meta_parent, comp, "parent", true, Diagram::meta);
			::Uml::InitCompositionChildRole(Diagram::meta_classes, comp, "classes", true, 0, -1, Class::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Association::meta_parent, comp, "parent", true, Diagram::meta);
			::Uml::InitCompositionChildRole(Diagram::meta_associations, comp, "associations", true, 0, -1, Association::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Composition::meta_parent, comp, "parent", true, Diagram::meta);
			::Uml::InitCompositionChildRole(Diagram::meta_compositions, comp, "compositions", true, 0, -1, Composition::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Namespace::meta_parent, comp, "parent", true, Diagram::meta);
			::Uml::InitCompositionChildRole(Diagram::meta_namespaces, comp, "namespaces", true, 0, -1, Namespace::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(AssociationRole::meta_parent, comp, "parent", true, Association::meta);
			::Uml::InitCompositionChildRole(Association::meta_roles, comp, "roles", true, 0, -1, AssociationRole::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(TaggedValue::meta_assocParent, comp, "assocParent", true, Association::meta);
			::Uml::InitCompositionChildRole(Association::meta_tagValues, comp, "tagValues", true, 0, -1, TaggedValue::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(CompositionParentRole::meta_parent, comp, "parent", true, Composition::meta);
			::Uml::InitCompositionChildRole(Composition::meta_parentRole, comp, "parentRole", true, 1, 1, CompositionParentRole::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(CompositionChildRole::meta_parent, comp, "parent", true, Composition::meta);
			::Uml::InitCompositionChildRole(Composition::meta_childRole, comp, "childRole", true, 1, 1, CompositionChildRole::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Namespace::meta_parent_ns, comp, "parent_ns", true, Namespace::meta);
			::Uml::InitCompositionChildRole(Namespace::meta_namespaces, comp, "namespaces", true, 0, -1, Namespace::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Class::meta_parent_ns, comp, "parent_ns", true, Namespace::meta);
			::Uml::InitCompositionChildRole(Namespace::meta_classes, comp, "classes", true, 0, -1, Class::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Composition::meta_parent_ns, comp, "parent_ns", true, Namespace::meta);
			::Uml::InitCompositionChildRole(Namespace::meta_compositions, comp, "compositions", true, 0, -1, Composition::meta);

		}
		{
			::Uml::Composition comp = ::Uml::CreateComposition();
			::Uml::InitComposition(comp, meta, "Composition");
			::Uml::InitCompositionParentRole(Association::meta_parent_ns, comp, "parent_ns", true, Namespace::meta);
			::Uml::InitCompositionChildRole(Namespace::meta_associations, comp, "associations", true, 0, -1, Association::meta);

		}

	}

	void _SetXsdStorage()
	{
		UdmDom::str_xsd_storage::StoreXsd("Uml.xsd", Uml_xsd::getString());
	}

	UDM_DLL void Initialize()
	{
		static bool first = true;
		if (!first) return;
		first = false;
		::Uml::Initialize();

	
		UDM_ASSERT( meta == ::Udm::null );

		meta = ::Uml::CreateDiagram();


		::UdmStatic::StaticDataNetwork* dn = new ::UdmStatic::StaticDataNetwork(diagram);
		dn->rootobject = meta;
		static_cast< ::UdmStatic::StaticObject*>(dn->rootobject.__impl())->mydn = dn;
		dn->systemname = "Uml";
		dn->sem = ::Udm::CHANGES_LOST_DEFAULT;


		CreateMeta();
		InitMeta();
		InitMetaLinks();

		::Uml::InitDiagram(meta, "Uml", "2.04");


		_SetXsdStorage();

	}


	UDM_DLL  ::Udm::UdmDiagram diagram = { &meta, Initialize };
	static struct _regClass
	{
		_regClass()
		{
			::Udm::MetaDepository::StoreDiagram("Uml", diagram);
		}
		~_regClass()
		{
			::Udm::MetaDepository::RemoveDiagram("Uml");
		}
	} __regUnUsed;

}

