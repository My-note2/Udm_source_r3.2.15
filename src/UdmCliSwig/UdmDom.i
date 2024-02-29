%module UdmDom

%include "UdmCli.i"

CSHARP_NAMESPACE3(UdmGme, Udm.Native.UdmGme, DomDataNetwork)

%ignore UdmDom::GetTime;
%ignore UdmDom::DomDataNetwork::Search;
%ignore UdmDom::DomDataNetwork::DoMapping;
%ignore UdmDom::DomDataNetwork::FindFile;
%ignore UdmDom::isXmi;
%ignore UdmDom::DomDataNetwork::multiroles;
%ignore UdmDom::DomDataNetwork::MultiRolesEnabled;
%ignore UdmDom::DomDataNetwork::EnableMultiRoles;

%ignore UdmDom::DomDataNetwork::meta_class_cache;

%ignore UdmDom::DummyFL;
%ignore UdmDom::Dummy_For_Linker;

// TODO: support this
%ignore UdmDom::DomDataNetwork::DTDPath;

%{
#include "UdmDom.h"
%}

// FIXME: this should work, but produces an error
// #define XML_WIN32
// #define XML_VISUALCPP

#define XERCES_CPP_NAMESPACE_BEGIN
#define XERCES_CPP_NAMESPACE_END
#define XERCES_CPP_NAMESPACE_USE


%include "UdmDom.h"
