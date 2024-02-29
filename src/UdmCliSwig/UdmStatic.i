%module UdmStatic;

%include "UdmCli.i"

CSHARP_NAMESPACE3(UdmStatic, Udm.Native.UdmStatic, StaticDataNetwork)
CSHARP_NAMESPACE3(UdmStatic, Udm.Native.UdmStatic, StaticObject)

// Not dll exported
%ignore UdmStatic::StaticObject::e;
%ignore UdmStatic::GetIdForObject;
%ignore UdmStatic::GetObjectForId;
%ignore UdmStatic::InsertObject;
%ignore UdmStatic::StaticObject::StaticObject;
%ignore UdmStatic::StaticObject::ValidatePointers;
%ignore UdmStatic::StaticObject::Destroy;
%ignore UdmStatic::StaticObject::Serialize;
%ignore UdmStatic::StaticDataNetwork::DeSerialize;
%ignore UdmStatic::StaticObject::FindCorrespondingObjectInStOrITree;
%ignore UdmStatic::StaticObject::FindCorrespondingObjectInStOrI;
%ignore UdmStatic::CreateComposition;
%ignore UdmStatic::CreateAssociation;
%ignore UdmStatic::StaticDataNetwork::SafeObjectById;

%{
#include "UdmStatic.h"
#include "Uml.h"
%}

%include "UdmStatic.h"

