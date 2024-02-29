%module Uml

%include "UdmException.i"

CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, CompositionChildRole)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, Diagram)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, CompositionParentRole)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, AssociationRole)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, Association)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, TaggedValue)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, Composition)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, Namespace)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, Class)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, Constraint)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, GenericRole)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, Attribute)
CSHARP_NAMESPACE3(Uml, Udm.Native.Uml, ConstraintDefinition)

%include "UdmCli.i"

// Not dll exported
%ignore Uml::CreateCORBADiagram;
%ignore Uml::CreateCORBANamespace;
%ignore Uml::CreateCORBAClass;
%ignore Uml::CreateCORBAAttribute;
%ignore Uml::CreateCORBAAssociation;
%ignore Uml::CreateCORBAAssociationRole;
%ignore Uml::CreateCORBAComposition;
%ignore Uml::CreateCORBACompositionParentRole;
%ignore Uml::CreateCORBACompositionChildRole;

%ignore Uml::InitCORBADiagram;
%ignore Uml::InitCORBANamespace;
%ignore Uml::InitCORBANamespace;
%ignore Uml::InitCORBAClass;
%ignore Uml::InitCORBAClass;
%ignore Uml::InitCORBAAttribute;
%ignore Uml::InitCORBAAssociation;
%ignore Uml::InitCORBAAssociation;
%ignore Uml::InitCORBAAssociationClass;
%ignore Uml::InitCORBAAssociationRole;
%ignore Uml::InitCORBAComposition;
%ignore Uml::InitCORBAComposition;
%ignore Uml::InitCORBACompositionParentRole;
%ignore Uml::InitCORBACompositionChildRole;
%ignore Uml::AddCORBAInheritance;

%ignore Uml::meta;

%{
#include "Uml.h"
#include "UmlExt.h"
%}

namespace Uml {
  using namespace std;
}

%include "Uml.h"
%include "UmlExt.h"

%udm_class(Uml, CompositionChildRole);
%udm_class(Uml, Diagram);
%udm_class(Uml, CompositionParentRole);
%udm_class(Uml, AssociationRole);
%udm_class(Uml, Association);
%udm_class(Uml, TaggedValue);
%udm_class(Uml, Composition);
%udm_class(Uml, Namespace);
%udm_class(Uml, Class);
%udm_class(Uml, Constraint);
%udm_class(Uml, GenericRole);
%udm_class(Uml, Attribute);
%udm_class(Uml, ConstraintDefinition);

// from UmlExt
%template() Uml::ElemCollection<Diagram,Association>;
%template() Uml::ElemCollection<Diagram,Class>;
%template() Uml::ElemCollection<Diagram,Composition>;
%template() Uml::ElemCollection<Namespace,Association>;
%template() Uml::ElemCollection<Namespace,Class>;
%template() Uml::ElemCollection<Namespace,Composition>;
%template() Uml::NamespaceCollection<Diagram>;
%template() Uml::NamespaceCollection<Namespace>;

//%template(ChildRoleChain) std::vector<Uml::CompositionChildRole>;
//%template() std::vector<ChildRoleChain>;
%template() std::vector<CompositionChildRole_vector>;
