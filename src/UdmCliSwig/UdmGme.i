%module UdmGme

%include "UdmCli.i"

CSHARP_NAMESPACE3(UdmGme, Udm.Native.UdmGme, GmeDataNetwork)

%ignore UdmGme::GmeDataNetwork::semantics;
%ignore UdmGme::GmeDataNetwork::meta_role_filter_cache;
%ignore UdmGme::GmeDataNetwork::amapInitialize;
%ignore UdmGme::GmeDataNetwork::amap;
%ignore UdmGme::GmeDataNetwork::priv;
%ignore UdmGme::GmeDataNetwork::CountWriteOps;
%ignore UdmGme::GmeDataNetwork::meta_class_cache;
%ignore UdmGme::DummyFL;
%ignore UdmGme::Dummy_For_Linker;

%{
#include "UdmGme.h"
%}

%define %cs_custom_cast(TYPE, CSTYPE)
%typemap(ctype) TYPE, TYPE& "void*"
%typemap(in) TYPE  %{ $1 = (TYPE)$input; %} 
%typemap(in) TYPE& %{ $1 = (TYPE*)&$input; %} 
%typemap(imtype, out="CSTYPE") TYPE, TYPE& "CSTYPE" 
%typemap(cstype, out="CSTYPE") TYPE, TYPE& "CSTYPE" 
%typemap(csin) TYPE, TYPE& "$csinput"
%typemap(csdirectorin) TYPE, TYPE& "$iminput"
%typemap(csout, excode=SWIGEXCODE) TYPE {
      IntPtr ret = $imcall;$excode
      return ret;
    }
 
%enddef

struct IUnknown;
typedef IUnknown* LPUNKNOWN;
%cs_custom_cast(LPUNKNOWN, IntPtr)
%cs_custom_cast(IUnknown*, IntPtr)

%include "UdmGme.h"


%{
Udm::UdmDiagram UdmDiagram_Wrap(Uml::Diagram* umld);
namespace UdmGme {
class GmeDN_Wrapper {
   Uml::Diagram uml_diagram;
   Udm::UdmDiagram diagram;
   public:
   GmeDataNetwork dn;
   GmeDN_Wrapper(Uml::Diagram d, LPUNKNOWN pUnknownProject, bool customTransactions) : uml_diagram(d), diagram(UdmDiagram_Wrap(&uml_diagram)), dn(diagram) {
      dn.OpenExisting(pUnknownProject, Udm::CHANGES_LOST_DEFAULT, customTransactions);
   }
   GmeDataNetwork& _getDN() { return dn; }
};
}
%}
namespace UdmGme {
class GmeDN_Wrapper {
   Uml::Diagram uml_diagram;
   Udm::UdmDiagram diagram;
   GmeDataNetwork& dn;
   public:
   GmeDN_Wrapper(Uml::Diagram d, LPUNKNOWN pUnknownProject, bool customTransactions) : uml_diagram(d), diagram(UdmDiagram_Wrap(&uml_diagram)), dn(diagram) {
      dn.OpenExisting(pUnknownProject, Udm::CHANGES_LOST_DEFAULT, customTransactions);
   }
   GmeDataNetwork& _getDN() { return dn; }
};
  %extend GmeDN_Wrapper {
    %typemap(cscode) UdmGme::GmeDN_Wrapper %{
      public global::Udm.Native.UdmGme.GmeDataNetwork dn { get { return _getDN(); } }
	  public global::Udm.Native.UdmObject Gme2Udm(object mgaObject)
	  {
		IntPtr punkMgaObject = global::System.Runtime.InteropServices.Marshal.GetIUnknownForObject(mgaObject);
		try
        {
          return dn.Gme2Udm(punkMgaObject);
        }
        finally
        {
		  global::System.Runtime.InteropServices.Marshal.Release(punkMgaObject);
        }
	  }
	  public static GmeDN_Wrapper Create(global::Udm.Native.Uml.Diagram meta, object mgaProject, bool customTransactions)
	  {
	    IntPtr punkMgaProject = global::System.Runtime.InteropServices.Marshal.GetIUnknownForObject(mgaProject);
		try
        {
          return new GmeDN_Wrapper(meta, punkMgaProject, customTransactions);
        }
        finally
        {
		  global::System.Runtime.InteropServices.Marshal.Release(punkMgaProject);
        }
	  }
    %}
  }
}
