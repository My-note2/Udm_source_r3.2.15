//###############################################################################################################################################
//
//	Object Constraint Language Expression Checker For UML
//	GMECommon.h
//
//===============================================================================================================================================
//
//	Copyright (c) Vanderbilt University, 2000-2002
//	ALL RIGHTS RESERVED
//
//	Vanderbilt University disclaims all warranties with regard to this
//	software, including all implied warranties of merchantability
//	and fitness.  In no event shall Vanderbilt University be liable for
//	any special, indirect or consequential damages or any damages
//	whatsoever resulting from loss of use, data or profits, whether
//	in an action of contract, negligence or other tortious action,
//	arising out of or in connection with the use or performance of
//	this software.
//
//###############################################################################################################################################

#ifndef GMECommon_h
#define GMECommon_h

#include "StdAfx.h"
#include "GMECOM.h"
#include "OCLCommon.h"
#include "Mga.h"

// From GME2000\Common\CommonSmart.h

template<class T>
class CComObjPtr
{
public:
	CComObjPtr() { p = NULL; }
	CComObjPtr(T *q) { if ((p = q) != NULL) q->AddRef(); }
	CComObjPtr(const CComObjPtr<T> &q) { if((p = q.p) != NULL) p->AddRef(); }
#ifdef _DEBUG
 	~CComObjPtr() { if(p) p->Release(); p = NULL; }
#else
	~CComObjPtr() { if(p) p->Release(); }
#endif
	void Release() { if(p) { p->Release(); p = NULL; } }

	operator T*() const { return p; }
T& operator*() const { ASSERT( p != NULL ); return *p; }
	T* operator->() const { ASSERT( p != NULL ); return p; }
	T* operator=(T *q) { if(q) q->AddRef(); if(p) p->Release(); p = q; return q; }
	T* operator=(const CComObjPtr<T> &q) { return operator=(q.p); }
	bool operator==(T *q) const { return p == q; }
	bool operator!=(T *q) const { return p != q; }

	T* Detach() { T* r = p; p = NULL; return r; }
	void Attach(T* q) { if(p) p->Release(); p = q; }

	template<class Q>
	HRESULT QueryInterface(CComObjPtr<Q> &q) const
	{
		ASSERT( p != NULL && q == NULL );
		return p->QueryInterface(__uuidof(Q), (void**)&q.p);
	}

	HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		ASSERT( p == NULL );
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}

	HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
		ASSERT(p == NULL);
		if (SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		return hr;
	}

public:
	T *p;
};

namespace GME
{
	enum ErrorImage
	{
		IMG_SYNTACTICAL_ERROR = 0,
		IMG_SEMANTICAL_ERROR = 1,
		IMG_VIOLATION_ERROR = 2,
		IMG_VIOLATION_WARNING = 3
	};

	typedef CComObjPtr<IMgaObject> SPObject;
	typedef std::vector<SPObject> ObjectVector;
	typedef CComObjPtr<IMgaFCO> SPFCO;
	typedef std::vector<SPFCO> FCOVector;
	typedef CComObjPtr<IMgaMetaBase> SPMeta;
	typedef std::vector<SPMeta> MetaBaseVector;
	typedef CComObjPtr<IMgaConnPoint> SPCP;
	typedef std::vector<SPCP> CPVector;
	typedef CComObjPtr<IMgaMetaPointerSpec> SPPointer;
	typedef std::vector<SPPointer> PointerVector;
	typedef CComObjPtr<IMgaMetaModel> SPModel;
	typedef std::map< std::string, SPModel> ModelMap;

//##############################################################################################################################################
//
//	F U N C T I O N S
//
//##############################################################################################################################################

	std::string Convert( const CString& strIn );
	CString Convert( const std::string& strIn );

	std::string UpperFirst( const std::string& strValue );
	std::string LowerFirst( const std::string& strValue );
	CString UpperFirst( const CString& strValue );
	CString LowerFirst( const CString& strValue );

	std::string GetConnPointRole( CComPtr<IMgaConnPoint> spCP );
	std::string GetObjectKind( CComPtr<IMgaObject> spObject );
	std::string GetFCORole( CComPtr<IMgaFCO> spFCO );

	objtype_enum GetObjectType( CComPtr<IMgaObject> spObject );
	objtype_enum GetObjectType( CComPtr<IMgaMetaBase> spObject );

	std::string GetObjectName( CComPtr<IMgaObject> spObject );
	std::string GetObjectName( CComPtr<IMgaMetaBase> spObject );

	std::string ObjectTypeToString( objtype_enum eType );
	objtype_enum StringToObjectType( const std::string& strType );

	std::string GetStringAttribute( CComPtr<IMgaFCO> spFCO, const std::string& strName );

	// Navigating , Gathering

	bool AddObject( CComPtr<IMgaFCO> spFCO, FCOVector& vecFCOs );
	bool AddObject( CComPtr<IMgaObject> spObject, ObjectVector& vecObjects );
	bool ContainsObject( CComPtr<IMgaFCO> spFCO, FCOVector& vecFCOs );
	bool ContainsObject( CComPtr<IMgaObject> spObject, ObjectVector& vecObjects );

	void GetAssociationEnds( CComPtr<IMgaFCO> spFCO, const std::string& strRole, const std::string& strKind, FCOVector& vecFCOs );
	void GetMetaObjects( CComPtr<IMgaMetaProject> spProject, const std::string& strName, objtype_enum eType, MetaBaseVector& vecMetaBases );
	void GetKindObjects( CComPtr<IMgaProject> spProject, const std::string& strKind, ObjectVector& vecObjects );
	void GetKindFolders( CComPtr<IMgaFolder> spFolder, const std::string& strKind, bool bKind, ObjectVector& vecObjects );
	void GetInheritances( CComPtr<IMgaFCO> spFCO, bool bIsParent, FCOVector& vecFCOs );

	std::string GetNamespacePath( CComPtr<IMgaFCO> spFCO );
	std::string GetDiagramName( CComPtr<IMgaFCO> spFCO );
	std::string GetQualifiedName( CComPtr<IMgaFCO> spFCO, bool isAssocRole = false );

/*
	bool IsInLibrary( CComPtr<IMgaObject> spObject );
	StringVector GetLibraryPath( CComPtr<IMgaObject> spObject );

	std::string CreateType( const std::string& strKind, objtype_enum eType );
	bool ResolveType( const std::string& strFullName, std::string& strKind, objtype_enum& eType );

	void AddAssociation( OclMeta::TypeManager* pManager, OclMeta::Association* pAssociation, OclMeta::AssociationVector& vecAssociations, bool bCantBeSet );
	void AddAssociations( OclMeta::AssociationVector& vecAssociationsToAdd, OclMeta::AssociationVector& vecAssociations );

//##############################################################################################################################################
//
//	F U N C T I O N S
//
//##############################################################################################################################################

	// MetaModeling Time

	bool IsAbstract( CComPtr<IMgaFCO> spFCO );
	bool GetAllObjects( CComPtr<IMgaFCO>& spFCO, CComPtr<IMgaFCOs>& spRefs );
	void GetAssociationEnds( CComPtr<IMgaFCO> spFCO, const std::string& strRole, const std::string& strKind, FCOVector& vecFCOs );
	void GetAssociationEnds( CComPtr<IMgaFCO> spFCO, const std::string& strRole, const std::string& strKind, FCOVector& vecFCOs, FCOVector& vecConnections );
	void GetInheritances( CComPtr<IMgaFCO> spFCO, const std::string& strInheritanceType, bool bIsParent, FCOVector& vecFCOs );
	void GetEquivalences( CComPtr<IMgaFCO> spFCO, const std::string& strEquivalenceType, bool bToLeft, FCOVector& vecFCOs );

	// Modeling Time

	void GetMetaObjects( CComPtr<IMgaMetaProject> spProject, const std::string& strName, objtype_enum eType, MetaBaseVector& vecMetaBases );
	void GetDerivedObjects( CComPtr<IMgaProject> spProject, const std::string& strType, ObjectVector& vecObjects );
	void GetKindObjects( CComPtr<IMgaProject> spProject, const std::string& strKind, ObjectVector& vecObjects );
	void GetKindFolders( CComPtr<IMgaFolder> spFolder, const std::string& strKind, bool bKind, ObjectVector& vecObjects );

	std::string GetPointerRole( CComPtr<IMgaMetaPointerSpec> spPointer );
	ModelMap GetModels( CComPtr<IMgaMetaProject> spProject );
	StringVector GetPointerDescriptions( CComPtr<IMgaMetaPointerSpec> spPointer );
	StringVector TranslateDescriptionsToKinds( ModelMap& mapModels, const StringVector& vecDescriptions );

	std::string GetObjectPath( CComPtr<IMgaObject> spObject );
	std::string GetObjectDescription( CComPtr<IMgaObject> spObject );
	std::string GetConnectionPointDescription( CComPtr<IMgaConnPoint> spCP );


*/

}; // namespace GME

#endif // GMECommon