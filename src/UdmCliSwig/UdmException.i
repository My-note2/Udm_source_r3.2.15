#ifndef _UdmException_i_
#define _UdmException_i_

// swig can get confused with namespace x = y;
// it doesn't matter if we lie to swig at this point
#define UDM_NAMESPACE Udm

// #define __int64 long long
%apply long long { __int64 };
#typedef long long __int64;

%define UDM_EXCEPTION_HANDLING(SUFFIX)
%insert(runtime) %{
  // Code to handle throwing of C# UdmException from C/C++ code.
  // The equivalent delegate to the callback, CSharpExceptionCallback_t, is CustomExceptionDelegate
  // and the equivalent customExceptionCallback instance is customDelegate
  typedef void (SWIGSTDCALL* CSharpExceptionCallback_t)(const char *);
  static CSharpExceptionCallback_t customExceptionCallback = NULL;

  extern "C" SWIGEXPORT
  void SWIGSTDCALL UdmExceptionRegisterCallback ## SUFFIX(CSharpExceptionCallback_t customCallback) {
    customExceptionCallback = customCallback;
  }

  // Note that SWIG detects any method calls named starting with
  // SWIG_CSharpSetPendingException for warning 845
  static void SWIG_CSharpSetPendingExceptionUdm(const char *msg) {
    customExceptionCallback(msg);
  }
%}

%pragma(csharp) imclasscode=%{
  class UdmExceptionHelper {
    // C# delegate for the C/C++ customExceptionCallback
    public delegate void CustomExceptionDelegate(string message);
    static CustomExceptionDelegate customDelegate =
                                   new CustomExceptionDelegate(SetPendingCustomException);

    [DllImport("$dllimport", EntryPoint=("UdmExceptionRegisterCallback" + "SUFFIX"))]
    public static extern
           void UdmExceptionRegisterCallback(CustomExceptionDelegate customCallback);

    static void SetPendingCustomException(string message) {
      SWIGPendingException.Set(new UdmException(message));
    }

    public UdmExceptionHelper() {
      UdmExceptionRegisterCallback(customDelegate);
    }
  }
  static UdmExceptionHelper exceptionHelper = new UdmExceptionHelper();
%}

%exception %{
try {
  $action
} catch (udm_exception &e) {
  SWIG_CSharpSetPendingExceptionUdm(e.what());
}
%}
%enddef

UDM_EXCEPTION_HANDLING($imclassname)

// Wrap NAMESPACE::SWIGTYPE with csharp namespaces
// based on csharp.swg
%define CSHARP_SWIGTYPEMAPS(SWIGTYPE, CNAMESPACE, CSNAMESPACE)

%typemap(cstype) CNAMESPACE ## SWIGTYPE "CSNAMESPACE$&csclassname"
%typemap(cstype) CNAMESPACE ## SWIGTYPE [] "CSNAMESPACE$csclassname"
%typemap(cstype) CNAMESPACE ## SWIGTYPE * "CSNAMESPACE$csclassname"
%typemap(cstype) CNAMESPACE ## SWIGTYPE & "CSNAMESPACE$csclassname"
%typemap(cstype) CNAMESPACE ## SWIGTYPE (CLASS::*) "CSNAMESPACE$csclassname"

%typemap(csin) CNAMESPACE ## SWIGTYPE "CSNAMESPACE$&csclassname.getCPtr($csinput)"
%typemap(csin) CNAMESPACE ## SWIGTYPE *, CNAMESPACE ## SWIGTYPE &, CNAMESPACE ## SWIGTYPE [] "CSNAMESPACE$csclassname.getCPtr($csinput)"
%typemap(csin) CNAMESPACE ## SWIGTYPE (CLASS::*) "CSNAMESPACE$csclassname.getCMemberPtr($csinput)"

%typemap(csout, excode=SWIGEXCODE) CNAMESPACE ## SWIGTYPE {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      CSNAMESPACE$&csclassname ret = new CSNAMESPACE$&csclassname($imcall, true);$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) CNAMESPACE ## SWIGTYPE & {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      CSNAMESPACE$csclassname ret = new CSNAMESPACE$csclassname($imcall, $owner);$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) CNAMESPACE ## SWIGTYPE *, CNAMESPACE ## SWIGTYPE [] {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      IntPtr cPtr = $imcall;
      CSNAMESPACE$csclassname ret = (cPtr == IntPtr.Zero) ? null : new CSNAMESPACE$csclassname(cPtr, $owner);$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) CNAMESPACE ## SWIGTYPE (CLASS::*) {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      string cMemberPtr = $imcall;
      CSNAMESPACE$csclassname ret = (cMemberPtr == null) ? null : new CSNAMESPACE$csclassname(cMemberPtr, $owner);$excode
      return ret;
    }
  }

%typemap(csvarout, excode=SWIGEXCODE2) CNAMESPACE ## SWIGTYPE %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        CSNAMESPACE$&csclassname ret = new CSNAMESPACE$&csclassname($imcall, true);$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) CNAMESPACE ## SWIGTYPE & %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        CSNAMESPACE$csclassname ret = new CSNAMESPACE$csclassname($imcall, $owner);$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) CNAMESPACE ## SWIGTYPE *, CNAMESPACE ## SWIGTYPE [] %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        IntPtr cPtr = $imcall;
        CSNAMESPACE$csclassname ret = (cPtr == IntPtr.Zero) ? null : new CSNAMESPACE$csclassname(cPtr, $owner);$excode
        return ret;
      }
    } %}

%typemap(csvarout, excode=SWIGEXCODE2) CNAMESPACE ## SWIGTYPE (CLASS::*) %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        string cMemberPtr = $imcall;
        CSNAMESPACE$csclassname ret = (cMemberPtr == null) ? null : new CSNAMESPACE$csclassname(cMemberPtr, $owner);$excode
        return ret;
      }
    } %}

%enddef

CSHARP_SWIGTYPEMAPS(SWIGTYPE, ,)

%define CSHARP_NAMESPACE3(CNAMESPACE, CSNAMESPACE, SWIGTYPE)
CSHARP_SWIGTYPEMAPS(SWIGTYPE, CNAMESPACE::, global::CSNAMESPACE.)
%enddef

%define CSHARP_NAMESPACE(NAMESPACE, SWIGTYPE)
CSHARP_NAMESPACE3(NAMESPACE, NAMESPACE, SWIGTYPE)
// Bug in swig
// https://sourceforge.net/tracker/?func=detail&aid=2933129&group_id=1645&atid=101645
CSHARP_NAMESPACE3(::NAMESPACE, NAMESPACE, SWIGTYPE)
%enddef

%typemap(cscode) UDM_NAMESPACE::Object %{
  public static readonly Object GlobalLock = new Object();
      public override bool Equals(object that)
      {
        if (that is UdmObject)
		{
		  return this.Equals((UdmObject)that);
		}
		else
		{
		  return false;
		}
      }
	  public override int GetHashCode()
	  {
	    return uniqueId();
	  }
%}

// based on csharp.swg
// Changes: s/internal/public/
// Proxy classes (base classes, ie, not derived classes)
%typemap(csbody) SWIGTYPE %{
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public $csclassname(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr($csclassname obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
%}

// Derived proxy classes
%typemap(csbody_derived) SWIGTYPE %{
  private HandleRef swigCPtr;

  public $csclassname(IntPtr cPtr, bool cMemoryOwn) : base($imclassname.$csclazznameSWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr($csclassname obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
%}


%typemap(csout, excode=SWIGEXCODE) bool,               const bool &               {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      bool ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) char,               const char &               {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      char ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) signed char,        const signed char &        {
   lock (global::Udm.Native.UdmObject.GlobalLock) {
      sbyte ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) unsigned char,      const unsigned char &      {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      byte ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) short,              const short &              {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      short ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) unsigned short,     const unsigned short &     {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      ushort ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) int,                const int &                {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      int ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) unsigned int,       const unsigned int &       {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      uint ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) long,               const long &               {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      int ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) unsigned long,      const unsigned long &      {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      uint ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) long long,          const long long &          {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      long ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) unsigned long long, const unsigned long long & {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      ulong ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) float,              const float &              {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      float ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) double,             const double &             {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      double ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) char *, char *&, char[ANY], char[] {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      string ret = $imcall;$excode
      return ret;
    }
  }
%typemap(csout, excode=SWIGEXCODE) void {
    lock (global::Udm.Native.UdmObject.GlobalLock) {
      $imcall;$excode
    }
  }

%typemap(csvarin, excode=SWIGEXCODE2) char *, char *&, char[ANY], char[] %{
    set {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        $imcall;$excode
      }
    } %}

%typemap(csvarout, excode=SWIGEXCODE2) bool,               const bool &               %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        bool ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) char,               const char &               %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        char ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) signed char,        const signed char &        %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        sbyte ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) unsigned char,      const unsigned char &      %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        byte ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) short,              const short &              %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        short ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) unsigned short,     const unsigned short &     %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        ushort ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) int,                const int &                %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        int ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) unsigned int,       const unsigned int &       %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        uint ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) long,               const long &               %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        int ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) unsigned long,      const unsigned long &      %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        uint ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) long long,          const long long &          %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        long ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) unsigned long long, const unsigned long long & %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        ulong ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) float,              const float &              %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        float ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) double,             const double &             %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        double ret = $imcall;$excode
        return ret;
      }
    } %}


%typemap(csvarout, excode=SWIGEXCODE2) char *, char *&, char[ANY], char[] %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        string ret = $imcall;$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) void %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        $imcall;$excode
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) SWIGTYPE %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        $&csclassname ret = new $&csclassname($imcall, true);$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) SWIGTYPE & %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        $csclassname ret = new $csclassname($imcall, $owner);$excode
        return ret;
      }
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) SWIGTYPE *, SWIGTYPE [] %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        IntPtr cPtr = $imcall;
        $csclassname ret = (cPtr == IntPtr.Zero) ? null : new $csclassname(cPtr, $owner);$excode
        return ret;
      }
    } %}

%typemap(csvarout, excode=SWIGEXCODE2) SWIGTYPE (CLASS::*) %{
    get {
      lock (global::Udm.Native.UdmObject.GlobalLock) {
        string cMemberPtr = $imcall;
        $csclassname ret = (cMemberPtr == null) ? null : new $csclassname(cMemberPtr, $owner);$excode
        return ret;
      }
    } %}

%typemap(csdestruct, methodname="Dispose", methodmodifiers="public") SWIGTYPE {
    lock(global::Udm.Native.UdmObject.GlobalLock) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          $imcall;
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

%typemap(csdestruct_derived, methodname="Dispose", methodmodifiers="public") SWIGTYPE {
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
    }
  }

%typemap(directordisconnect, methodname="swigDirectorDisconnect") SWIGTYPE %{
  protected void $methodname() {
    lock(global::Udm.Native.UdmObject.GlobalLock) {
      swigCMemOwn = false;
      $imcall;
    }
  }
%}


#endif
