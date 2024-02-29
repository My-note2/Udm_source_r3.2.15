
%include "UdmException.i"
%import "Udm.i"
%import "Uml.i"
%include "std_vector.i"
%include "csharp_std_set.i"

%pragma(csharp) imclassimports=%{

using System;
using System.Runtime.InteropServices;

using UdmException = global::Udm.Native.UdmException;
using Attribute = global::Udm.Native.Uml.Attribute;
%}


%typemap(csimports) SWIGTYPE
%{
using System;
using System.Runtime.InteropServices;

using global::Udm.Native;
%}


%{
#include <algorithm>
%}

%define %udm_class(Namespace, Class)
  %template(Class ## _ArchetypeAttr) UDM_NAMESPACE::ArchetypeAttr<Namespace::Class>;
  %template(Class ## _AssocAttr) UDM_NAMESPACE::AssocAttr<Namespace::Class>;
  %template(Class ## _AssocEndAttr) UDM_NAMESPACE::AssocEndAttr<Namespace::Class>;
  %template(Class ## _CrossAssocAttr) UDM_NAMESPACE::CrossAssocAttr<Namespace::Class>;
  %template(Class ## _ChildAttr) UDM_NAMESPACE::ChildAttr<Namespace::Class>;
  %template(Class ## _ChildrenAttr) UDM_NAMESPACE::ChildrenAttr<Namespace::Class>;
  %template(Class ## _DerivedAttr) UDM_NAMESPACE::DerivedAttr<Namespace::Class>;
  %template(Class ## _InstantiatedAttr) UDM_NAMESPACE::InstantiatedAttr<Namespace::Class>;
  %template(Class ## _ParentAttr) UDM_NAMESPACE::ParentAttr<Namespace::Class>;
  %template(Class ## _PointerAttr) UDM_NAMESPACE::PointerAttr<Namespace::Class>;

  %template(Class ## _vector) std::vector<Namespace::Class>;
  %template(Class ## _set) std::set<Namespace::Class>;
  // TODO: any standard extensions?
  //%extend Class {
%enddef

