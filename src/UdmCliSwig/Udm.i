#define UDM_DLL
%module Udm

%include "UdmException.i"
%include "std_string.i"
%include "std_vector.i"
%include "csharp_std_set.i"

// Swig has poor support for generated code namespaces, so we need a "using Uml;"
%typemap(csimports) SWIGTYPE
%{
using System;
using System.Runtime.InteropServices;

using global::Udm.Native.Uml;
using Attribute = global::Udm.Native.Uml.Attribute;
%}

%pragma(csharp) moduleimports=%{
using System;
using System.Runtime.InteropServices;

using global::Udm.Native.Uml;
using Attribute = global::Udm.Native.Uml.Attribute;
%} 

CSHARP_NAMESPACE3(Udm, Udm.Native, BackendSemantics)
CSHARP_NAMESPACE3(Udm, Udm.Native, BaseVisitor)
CSHARP_NAMESPACE3(Udm, Udm.Native, BooleanArrAttrItem)
CSHARP_NAMESPACE3(Udm, Udm.Native, BooleanAttr)
CSHARP_NAMESPACE3(Udm, Udm.Native, BooleanAttrArr)
CSHARP_NAMESPACE3(Udm, Udm.Native, DataNetwork)
CSHARP_NAMESPACE3(Udm, Udm.Native, DataNetworkSpecifier)
CSHARP_NAMESPACE3(Udm, Udm.Native, DataNetworkSpecifier_vector)
CSHARP_NAMESPACE3(Udm, Udm.Native, DynamicMetaSpecifier)
CSHARP_NAMESPACE3(Udm, Udm.Native, IntegerArrAttrItem)
CSHARP_NAMESPACE3(Udm, Udm.Native, IntegerAttr)
CSHARP_NAMESPACE3(Udm, Udm.Native, IntegerAttrArr)
CSHARP_NAMESPACE3(Udm, Udm.Native, MetaDepository)
CSHARP_NAMESPACE3(Udm, Udm.Native, NullObject)
CSHARP_NAMESPACE3(Udm, Udm.Native, Object)
CSHARP_NAMESPACE3(Udm, Udm.Native, ObjectImpl)
CSHARP_NAMESPACE3(Udm, Udm.Native, std::set<UDM_NAMESPACE::Object>);
CSHARP_NAMESPACE3(Udm, Udm.Native, ParentAttr< ::UDM_NAMESPACE::Object>)
CSHARP_NAMESPACE3(Udm, Udm.Native, RealArrAttrItem)
CSHARP_NAMESPACE3(Udm, Udm.Native, RealAttr)
CSHARP_NAMESPACE3(Udm, Udm.Native, RealAttrArr)
CSHARP_NAMESPACE3(Udm, Udm.Native, SmartDataNetwork)
CSHARP_NAMESPACE3(Udm, Udm.Native, StaticDataNetworkSpecifier)
CSHARP_NAMESPACE3(Udm, Udm.Native, StaticUdmProject)
CSHARP_NAMESPACE3(Udm, Udm.Native, StringArrAttrItem)
CSHARP_NAMESPACE3(Udm, Udm.Native, StringAttr)
CSHARP_NAMESPACE3(Udm, Udm.Native, StringAttrArr)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempBooleanArrAttrItem)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempBooleanAttr)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempBooleanAttrArr)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempIntegerArrAttrItem)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempIntegerAttr)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempIntegerAttrArr)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempRealArrAttrItem)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempRealAttr)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempRealAttrArr)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempStringArrAttrItem)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempStringAttr)
CSHARP_NAMESPACE3(Udm, Udm.Native, TempStringAttrArr)
CSHARP_NAMESPACE3(Udm, Udm.Native, UdmDiagram)
CSHARP_NAMESPACE3(Udm, Udm.Native, UdmProject)
CSHARP_NAMESPACE3(std, Udm.Native, vector<bool>)
CSHARP_NAMESPACE3(std, Udm.Native, vector<double>)
CSHARP_NAMESPACE3(std, Udm.Native, vector<std::string>)
CSHARP_NAMESPACE3(std, Udm.Native, vector<__int64>)

namespace Udm {
%typemap(csdestruct_derived, methodname="Dispose", methodmodifiers="public") SmartDataNetwork {
    lock(global::Udm.Native.UdmObject.GlobalLock) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          $imcall;
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
      base.Dispose();
	  // this serves a dual purpose: Disposing the wrapper, and preventing the wrapper from being GCed before now
	  if (wrapper != null) wrapper.Dispose(); // this is the only modified line
    }
  }
}


using namespace std;
%{
using namespace std;
%}

// Some strings aren't marked const
// %apply const std::string& { UDM_NAMESPACE::string& };
%apply const std::string& { std::string& };
%apply const std::string& { string& };
%apply const std::string& { const string& };
%apply std::string { string };
%apply std::string { const string };

%pragma(csharp) imclassimports=%{

using System;
using System.Runtime.InteropServices;

public class UdmException : System.ApplicationException {
  public UdmException(string message) 
    : base(message) {
  }
}
%}

// These are not dllexport'ed
%ignore Uml::theOther1;
%ignore UDM_NAMESPACE::UdmStaticData;
%ignore UDM_NAMESPACE::_UdmStaticData;
%ignore UDM_NAMESPACE::UdmProject::GetRealObject;
%ignore UDM_NAMESPACE::UdmProject::GetPlaceHolder;
%ignore UDM_NAMESPACE::NullObject::e;

%ignore UdmUtil::stacktrace;
%ignore UDM_NAMESPACE::ObjectImpl::getPath;
%ignore UDM_NAMESPACE::Object::toString;

// Defined in Uml.i
%ignore Uml::IsDerivedFrom;

// TODO: support these
%ignore UDM_NAMESPACE::DynamicMetaSpecifier::metalocator;
%ignore UDM_NAMESPACE::udmvariant::strval;

%{
#include "UdmBase.h"
#include "UdmUtil.h"
typedef UDM_NAMESPACE::Object::AssociationInfo AssociationInfo;
typedef UDM_NAMESPACE::Object::CompositionInfo CompositionInfo;
namespace UDM_NAMESPACE {
typedef UDM_NAMESPACE::DataNetwork::backendtabt DataNetwork_backendtabt;
udm_exception NullObject::e("Object handle is null");
}
#include "Uml.h"
%}

%rename(Equals) UDM_NAMESPACE::ArrAttrItem::operator==;
%rename(Set) UDM_NAMESPACE::TempStringAttr::operator=;
%rename(Get) UDM_NAMESPACE::TempStringAttr::operator string;
%rename(Get) UDM_NAMESPACE::TempStringAttrArr::operator [];
// TempStringAttrArr::operator vector<string> ?
%rename(Set) UDM_NAMESPACE::TempIntegerAttr::operator=;
%rename(Get) UDM_NAMESPACE::TempIntegerAttr::operator __int64;
%rename(Get) UDM_NAMESPACE::TempIntegerAttrArr::operator [];
%rename(AsVector) UDM_NAMESPACE::TempIntegerAttrArr::operator vector<__int64> const;
%rename(Set) UDM_NAMESPACE::TempRealAttr::operator =;
%rename(Get) UDM_NAMESPACE::TempRealAttr::operator double;
%rename(Get) UDM_NAMESPACE::TempRealAttrArr::operator [];
%rename(AsVector) UDM_NAMESPACE::TempRealAttrArr::operator vector<double>() const;
%rename(Set) UDM_NAMESPACE::TempBooleanAttr::operator =;
%rename(Get) UDM_NAMESPACE::TempBooleanAttr::operator bool;
%rename(Get) UDM_NAMESPACE::TempBooleanAttrArr::operator [];
%rename(AsVector) UDM_NAMESPACE::TempBooleanAttrArr::operator vector<bool>() const;
%rename(Set) UDM_NAMESPACE::StringAttr::operator=;
%rename(Get) UDM_NAMESPACE::StringAttr::operator string;
%rename(Get) UDM_NAMESPACE::StringAttrArr::operator [];
%rename(AsVector) UDM_NAMESPACE::StringAttrArr::operator vector<string>() const ;
%rename(Set) UDM_NAMESPACE::IntegerAttr::operator=;
%rename(Get) UDM_NAMESPACE::IntegerAttr::operator __int64;
%rename(Get) UDM_NAMESPACE::IntegerAttrArr::operator [];
%rename(Set) UDM_NAMESPACE::RealAttr::operator =;
%rename(Get) UDM_NAMESPACE::RealAttr::operator double;
%rename(Get) UDM_NAMESPACE::RealAttrArr::operator [];
%rename(AsVector) UDM_NAMESPACE::RealAttrArr::operator vector<bool>() const;
%rename(Set) UDM_NAMESPACE::BooleanAttr::operator =;
%rename(Get) UDM_NAMESPACE::BooleanAttr::operator bool;
%rename(Get) UDM_NAMESPACE::BooleanAttrArr::operator [];

%rename(Equals) UDM_NAMESPACE::Object::operator==;
%rename(IsNull) UDM_NAMESPACE::Object::operator bool;

%rename(AsSet) UDM_NAMESPACE::AssocAttr::operator set<CLASS,Pred >() const;
%rename(AsSet) UDM_NAMESPACE::AClassCrossAssocAttr::operator set<CLASS,Pred >() const;
%rename(AsSet) UDM_NAMESPACE::AClassAssocAttr::operator set<CLASS,Pred >() const;
%rename(AsSet) UDM_NAMESPACE::ChildrenAttr::operator set<CLASS,Pred >() const;
%rename(AsSet) UDM_NAMESPACE::DerivedAttr::operator set<CLASS,Pred >() const;
%rename(AsSet) UDM_NAMESPACE::InstantiatedAttr::operator set<CLASS,Pred >() const;
%rename(AsSet) UDM_NAMESPACE::CrossAssocAttr::operator set<CLASS,Pred >() const;

%rename(AsVector) UDM_NAMESPACE::ChildrenAttr::operator vector<CLASS >() const;

%rename(DeRef) UDM_NAMESPACE::ArrAttrItem::operator CLASS() const;
%rename(DeRef) UDM_NAMESPACE::CrossPointerAttr::operator CLASS() const;
%rename(DeRef) UDM_NAMESPACE::PointerAttr::operator CLASS() const;
%rename(DeRef) UDM_NAMESPACE::CrossAssocEndAttr::operator CLASS() const;
%rename(DeRef) UDM_NAMESPACE::AssocEndAttr::operator CLASS() const;
%rename(DeRef) UDM_NAMESPACE::AClassCrossPointerAttr::operator CLASS() const;
%rename(DeRef) UDM_NAMESPACE::AClassPointerAttr::operator CLASS() const;
%rename(DeRef) UDM_NAMESPACE::ChildAttr::operator CLASS() const;
%rename(DeRef) UDM_NAMESPACE::ParentAttr::operator CLASS() const;
%rename(DeRef) UDM_NAMESPACE::ArchetypeAttr::operator CLASS() const;

%rename(Set) *::operator =;
%rename(RemoveMember) *::operator -=;
%rename(AddMember) *::operator +=;

// Don't get confused with System.Object
%rename(UdmObject) UDM_NAMESPACE::Object;

%include "UdmBase.h"

//TODO: do we need these?
//%template(BoolArrAttrItem) UDM_NAMESPACE::ArrAttrItem<bool>;
//%template(StringArrAttrItem) UDM_NAMESPACE::ArrAttrItem<string>;
//%template(StringAdditiveArrAttrItem) UDM_NAMESPACE::AdditiveArrAttrItem< string >;
//%template(DoubleArrAttrItem) UDM_NAMESPACE::ArrAttrItem<double>;
//%template(DoubleAdditiveArrAttrItem) UDM_NAMESPACE::AdditiveArrAttrItem< double >;
//%template(IntArrAttrItem) UDM_NAMESPACE::ArrAttrItem<__int64>;
//%template(IntAdditiveArrAttrItem) UDM_NAMESPACE::AdditiveArrAttrItem< __int64 >;

namespace UDM_NAMESPACE {
  %extend ChildrenAttr {
    %typemap(csinterfaces) UDM_NAMESPACE::ChildrenAttr "IDisposable, System.Collections.Generic.IEnumerable<$typemap(cstype, CLASS)>";
	%typemap(cscode) UDM_NAMESPACE::ChildrenAttr <CLASS, Pred > %{
      public System.Collections.Generic.IEnumerator<$typemap(cstype, CLASS)> GetEnumerator()
      {
        return AsVector().GetEnumerator();
      }
      System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
      {
        return AsVector().GetEnumerator();
      }
    %}
  };
  
  %extend StringAttr {
    %typemap(cscode) UDM_NAMESPACE::StringAttr %{
      public override String ToString()
      {
        return Get();
      }
    %}
  }
}
// TODO: IEnumerable for the following:
// CrossAssocAttr
// AssocAttr
// AClassCrossAssocAttr
// AClassAssocAttr
// DerivedAttr
// InstantiatedAttr

%include "UdmBase.h"

%template(ObjectParentAttr) UDM_NAMESPACE::ParentAttr< ::UDM_NAMESPACE::Object>;
%template(Object_set) std::set<UDM_NAMESPACE::Object>;
%template(ObjectImpl_vector) std::vector<UDM_NAMESPACE::ObjectImpl*>;
%template(DataNetworkSpecifier_vector) std::vector<UDM_NAMESPACE::DataNetworkSpecifier>;
%template(StaticDataNetworkSpecifier_vector) std::vector<UDM_NAMESPACE::StaticDataNetworkSpecifier>;
%template(bool_vector) std::vector<bool>;
%template(double_vector) std::vector<double>;
%template(string_vector) std::vector<std::string>;
%template(Int64_Vector) std::vector<long long>;

%include "UdmUtil.h"

%{
static void udmdiagram_dummy() {}
Udm::UdmDiagram UdmDiagram_Wrap(Uml::Diagram* umld) {
	Udm::UdmDiagram ret;
	ret.dgr = umld;
	ret.init = udmdiagram_dummy;
	return ret;
}
struct UdmDiagram_Wrapper {
   Uml::Diagram uml_diagram;
   UDM_NAMESPACE::UdmDiagram diagram;
   UdmDiagram_Wrapper(Uml::Diagram d) : uml_diagram(d), diagram(UdmDiagram_Wrap(&uml_diagram)) { }
};
%}

struct UdmDiagram_Wrapper {
   Uml::Diagram uml_diagram;
   UDM_NAMESPACE::UdmDiagram diagram;
   UdmDiagram_Wrapper(Uml::Diagram d) : uml_diagram(d), diagram(UdmDiagram_Wrap(&uml_diagram)) { }
};

namespace UDM_NAMESPACE {
%extend SmartDataNetwork {
%typemap(cscode) UDM_NAMESPACE::SmartDataNetwork %{
    UdmDiagram_Wrapper wrapper;
    public SmartDataNetwork(UdmDiagram_Wrapper wrapper) : this(wrapper.diagram)
	{
	    this.wrapper = wrapper;
	}
    public SmartDataNetwork(global::Udm.Native.SmartDataNetwork meta) : this(new global::Udm.Native.UdmDiagram_Wrapper(global::Udm.Native.Uml.Diagram.Cast(meta.GetRootObject())))
	{
	}
    public SmartDataNetwork(global::Udm.Native.Uml.Diagram meta) : this(new global::Udm.Native.UdmDiagram_Wrapper(meta))
	{
	}
%}
}
}

// FIXME: KMS: this should be moved into UdmUtil.h
static int CopyObjectHierarchy(Udm::Object srcRoot, Udm::Object dstRoot) {
    return UdmUtil::CopyObjectHierarchy(srcRoot.__impl(), dstRoot.__impl(), dstRoot.__impl()->__getdn(), UdmUtil::copy_assoc_map());
}
%{
static int CopyObjectHierarchy(Udm::Object srcRoot, Udm::Object dstRoot) {
    return UdmUtil::CopyObjectHierarchy(srcRoot.__impl(), dstRoot.__impl(), dstRoot.__impl()->__getdn(), UdmUtil::copy_assoc_map());
}
%}

%import "Uml.i"

