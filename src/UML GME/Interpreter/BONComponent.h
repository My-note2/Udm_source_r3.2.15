    /*
     *	Copyright (c) Vanderbilt University, 2000-2001
     *	ALL RIGHTS RESERVED
     *
     *	Vanderbilt University disclaims all warranties with regard to this
     *	software, including all implied warranties of merchantability
     *	and fitness.  In no event shall Vanderbilt University be liable for
     *	any special, indirect or consequential damages or any damages
     *	whatsoever resulting from loss of use, data or profits, whether
     *	in an action of contract, negligence or other tortious action,
     *	arising out of or in connection with the use or performance of
     *	this software.
     */
/*
CHANGELOG
=========
	-	05/20/06	-	endre
		- add support for nested UML namespaces

	-	12/31/05	-	endre

		- use UdmDom data networks and UdmProjects to build the result of interpretation

	-	11/24/05	-	endre

		- add CAssociationBase::DumpCrossXML to fix the generation of Association elements in the cross XML file
		(previously no XML was generated if the association was in a namespace)

	-	27/02/04	-	endre
				
		-	added the call GetPackage() for CClassBuilder and CClassCopyBuilder and the abstract method for CClassBase

  
	-	24/02/04	-	endre
	
		-	Added the CPackageBuilder and CPackageBuilderList custom BON classes to reflect the newly introduced Package model
		-	Changed the Gather* functions to work with CPackageBuilder-s instead of folders.

*/

    
    #ifndef UML_INTERPRETER_H
    #define UML_INTERPRETER_H
    
    #include "Builder.h"
    #include "CardinalityObject.h"
    #include "AttributeObject.h"

    #include "Uml.h"

#include "Comdef.h"

    class CClassDiagramBuilder;
    
    class CClassBuilder;
    class CClassCopyBuilder;
    class CClassBase;
    class CCompositeClass;
    class CAttribute;
	class CPackageBuilder;
	class CNamespaceBuilder;
    
    class CCompositionBuilder;
    class CAssociationBase;
    class CAssociationBuilder;
    class CDirectAssociationBuilder;
    
    class CConstraintBuilder;
    class CConstraintDefinitionBuilder;
    
    typedef CTypedPtrList<CPtrList, CClassDiagramBuilder*>		CClassDiagramList;
	typedef CTypedPtrList<CPtrList, CNamespaceBuilder*>			CNamespaceList;
    
    typedef CTypedPtrList<CPtrList, CClassBase*>				CClassBaseList;
    typedef CTypedPtrList<CPtrList, CCompositeClass*>			CCompositeClassList;
    typedef CTypedPtrList<CPtrList, CClassCopyBuilder*>			CClassCopyList;
    typedef CTypedPtrList<CPtrList, CClassBuilder*>				CClassList;
    typedef CTypedPtrList<CPtrList, CAttribute*>				CAttributeList;
    
    typedef CTypedPtrList<CPtrList, CCompositionBuilder*>		CCompositionList;
    
    typedef CTypedPtrList<CPtrList, CAssociationBase*>			CAssociationBaseList;
    typedef CTypedPtrList<CPtrList, CDirectAssociationBuilder*>	CDirectAssociationList;
    typedef CTypedPtrList<CPtrList, CAssociationBuilder*>		CAssociationList;
    
    typedef CTypedPtrList<CPtrList, CConstraintBuilder*>		CConstraintBuilderList;
    typedef CTypedPtrList<CPtrList, CConstraintDefinitionBuilder*>	CConstraintDefinitionBuilderList;
    typedef CTypedPtrList<CPtrList, CPackageBuilder*>	CPackageBuilderList;

    #define NEW_BON_INVOKE
    //#define DEPRECATED_BON_INVOKE_IMPLEMENTED
    //#define IMPLEMENT_OLD_INTERFACE_ONLY
    
    class CComponent{
    public:
    	CComponent() : focusfolder(NULL) { ; }
    	CBuilderFolder *focusfolder;
    	CBuilderFolderList selectedfolders;
    	void InvokeEx(CBuilder &builder,CBuilderObject *focus, CBuilderObjectList &selected, long param);
    //	void Invoke(CBuilder &builder, CBuilderObjectList &selected, long param);
    protected:
    	CString name;
		//CBuilderFolderList packageList;
		CPackageBuilderList packageList;
		::Uml::Diagram cross_uml_dgr;
		_bstr_t m_strConnection;

    public:
    	static CComponent *theInstance;
    public:
    
		int GatherPackageFolders(CBuilderFolder *folder,CPackageBuilderList &packages);
		::Uml::Diagram& GetCrossUmlDiagram() { return cross_uml_dgr; };

	protected:
		CString GetFilePath(CBuilder &builder, CBuilderObject *focus, char *FILE_EXT, char *UDM_FILTER);
		bool isUMT;
    };
    
    
	class CContainer
	{
	public:
		~CContainer();
		void TraverseModels(void *pointer);
		void AddCompositeClass(CCompositeClass *cls);
    	void AddComposition(CCompositionBuilder *comp);
    	void AddAssociation(CAssociationBase *ass);
    	void BuildInheritance();
    	bool CheckInheritance();
    	void BuildCompositions();
    	void BuildAssociations();

		void BuildUMLClasses();
		void BuildUMLInheritance();
		void BuildUMLAssociations();
		void BuildUMLCompositions();

		void BuildCrossUMLClasses();
		void BuildCrossUMLInheritance();
		void BuildCrossUMLCompositions();
		void BuildCrossUMLAssociations();
	protected:
		CNamespaceList namespaces;
    	CCompositeClassList compositeClasses;
    	CCompositionList compositions;
    	CAssociationBaseList associations;
	};


	class CPackageBuilder : public CBuilderModel, public CContainer
    {
		DECLARE_CUSTOMMODEL(CPackageBuilder, CBuilderModel)
    public:
		void Build();
		CString GetVersion() const;
		CString GetNameorAlias() const;

		void BuildUML();
		::Uml::Diagram GetUmlDiagram() { return uml_dgr; }
		void SetUmlDiagram(::Uml::Diagram &dgr) { uml_dgr = dgr; }
    protected:
		::Uml::Diagram uml_dgr;
    public:
    };
    
    
	class CNamespaceBuilder : public CBuilderModel, public CContainer
	{
		DECLARE_CUSTOMMODEL(CNamespaceBuilder, CBuilderModel)
	public:
		void Build();
		CPackageBuilder* GetPackage() const;
		CNamespaceBuilder * GetNamespace() const;

		void BuildUML();
		::Uml::Namespace GetUmlNamespace() { return uml_ns; }
	protected:
		::Uml::Namespace uml_ns;
	};


	class CClassDiagramBuilder : public CBuilderModel
    {
    	DECLARE_CUSTOMMODEL(CClassDiagramBuilder, CBuilderModel)
    public:
    protected:
    public:
		void Build();
		CPackageBuilder * GetPackage() const;
		CNamespaceBuilder * GetNamespace() const;
    };
    

	class CCompositeClass
    {
    	friend class CAssociationBase;
    public:
		CCompositeClass(CClassBuilder *cls,CBuilderObjectList &copies);
    	
    protected:
    	CClassBuilder *cls;
    	CClassBaseList parts;						// contains cls and all copies
    	CCompositeClassList baseClasses;
    	CCompositeClassList subClasses;
    	CCompositionList childCompositions;			// contains all compositions where this is the parent
    	CCompositionList parentCompositions; 		// contains all compositions where this is a child
    	CAssociationBaseList srcAssociations;
    	CAssociationBaseList dstAssociations;
    	CAssociationBase *association;				// if this is an associationClass
    	bool flag;
		::Uml::Class uml_cls;
		::Uml::Class cross_uml_cls;

	public:
    	CString GetName();
    	bool IsAbstract();
    	void SetAssociation(CAssociationBase *assoc)	{ association = assoc; }
    	void FindSubClasses(CBuilderObjectList &triangles);
    	void FindSubClasses();
    	void RegisterSubClasses(CBuilderObjectList &subs);
    	void AddBaseClass(CCompositeClass *base);
    	void AddSubClass(CCompositeClass *sub);
    	bool CheckInheritance(CStringList &trace);
    	void DumpInheritance();
    	void BuildCompositions();
		void RegisterComposition(CCompositionBuilder *composition);
    	void BuildAssociations();
    	void RegisterAssociation(CAssociationBase *assoc,CCompositeClass *dst);
		bool IsCrossClass();
		bool HasCrossBases();

		void BuildUML();
		void BuildUMLInheritance();
		void BuildCrossUML();
		void BuildCrossUMLInheritance();
		void BuildCrossUMLCompositions();
		::Uml::Class GetUmlClass() { return uml_cls; }
		::Uml::Class GetCrossUmlClass() { return cross_uml_cls; }
    };
    
    class CClassBase
    {
    	friend class CCompositeClass;
    public:
    	CClassBase() : composite(0), builder(0), flag(false) {}
		CCompositeClass *composite;
    protected:
    //	CCompositeClass *composite;
    	CBuilderObject *builder;
    	bool flag;
    public:
    	CCompositeClass *GetComposite()		{ return composite; }
    	bool GetFlag()						{ return flag; }
    	void SetFlag(bool val)				{ flag = val; }
		virtual int dummy(){return 0;};
		virtual CPackageBuilder * GetPackage() = 0;
    
    };
    
    class CClassBuilder : public CBuilderAtom, public CClassBase
    {
    	friend class CCompositeClass;
    	DECLARE_CUSTOMATOM(CClassBuilder, CBuilderAtom)
    protected:
    	CString stereotype;
    	//CAttributeList attrs;
    	//list of attributes parsed by ANTLR
    	std::vector<AttributeObject> attributes;
    	CConstraintBuilderList constraints;
    	CConstraintDefinitionBuilderList constraintdefs;
    
    public:
    	void GetUMLAttributes();
    	void GetConstraints();
    	void GetConstraintDefinitions();
    	virtual void Initialize();
		CPackageBuilder * GetPackage();
		CClassDiagramBuilder * GetClassDiagram();

    };
    
    class CClassCopyBuilder : public CBuilderAtomReference, public CClassBase
    {
    	friend class CCompositeClass;
    	DECLARE_CUSTOMATOMREF(CClassCopyBuilder, CBuilderAtomReference)
    public:
    	virtual void Initialize();
		CPackageBuilder * GetPackage();
    };
    
    class CRole
    {
    public:
    
    	int minc;
    	int maxc;
    	CString cardinality;
    
    	CardinalityObject card;
    	CCompositeClass *cls;
    	CString name;
    	void ParseCardinality();
    	CRole();
    public:
    };
    
    class CAttribute
    {
    public:
    	CAttribute() {}
    public:
    	CString attrType;
    	CString defValue;
    public:
    };
    
    class CCompositionBuilder : public CBuilderConnection
    {
    	friend class CCompositeClass;
    	DECLARE_CUSTOMCONNECTION(CCompositionBuilder, CBuilderConnection)
    protected:
    	CRole parent;
    	CRole child;
		::Uml::Composition uml_comp;
    public:
		CNamespaceBuilder * GetNamespace() const;
		CPackageBuilder * GetPackage() const;
    
    	virtual void Initialize();

		void BuildUML();
		::Uml::Composition GetUmlComposition() { return uml_comp; }
	};
    
    class CAssociationBase
    {
    public:
    	CAssociationBase();
    protected:
    	CRole source;
    	CRole dest;
    	CCompositeClass *associationClass;
    	CString association;
		const CBuilderModel * parent_classdgr;
		::Uml::Association uml_ass;
		::Uml::Association cross_uml_ass;
		void _BuildUML(::Uml::Association &ass, bool is_cross);
    public:
		CString path;
    	bool IsEquivalent(CAssociationBase *ass);
    	bool IsCrossPackage();
    	virtual void SetSourceAndDestination(CCompositeClass *s,CCompositeClass *d) = 0;
    	virtual void SetRolesAndCardinalities() = 0;
		CNamespaceBuilder* GetNamespace() const { return BUILDER_CAST(CClassDiagramBuilder, parent_classdgr)->GetNamespace(); }
		CPackageBuilder* GetPackage() const { return BUILDER_CAST(CClassDiagramBuilder, parent_classdgr)->GetPackage(); }
		void BuildUML();
		void BuildCrossUML();
		::Uml::Association GetUmlAssociation() { return uml_ass; }
		::Uml::Association GetCrossUmlAssociation() { return cross_uml_ass; }
    };
    
    class CDirectAssociationBuilder : public CBuilderConnection, public CAssociationBase
    {
    	DECLARE_CUSTOMCONNECTION(CDirectAssociationBuilder, CBuilderConnection)
    protected:
    public:
    	virtual void Initialize();
    	void SetSourceAndDestination(CCompositeClass *s,CCompositeClass *d);
    	void SetRolesAndCardinalities();
    };
    
    class CAssociationBuilder : public CBuilderAtom, public CAssociationBase
    {
    	DECLARE_CUSTOMATOM(CAssociationBuilder, CBuilderAtom)
    protected:
    public:
    	virtual void Initialize();
    	void SetSourceAndDestination(CCompositeClass *s,CCompositeClass *d);
    	void SetRolesAndCardinalities();
    };
    
    /*
    class CConstraintBase
    {
    	CString expr;
    	CString desc;
    };
    */
    
    class CConstraintBuilder : public CBuilderAtom
    {
    	DECLARE_CUSTOMATOM(CConstraintBuilder, CBuilderAtom)
    public:
    	CString expr;
    	CString desc;
    	virtual void Initialize();
		void BuildUML(::Uml::Class &uml_class);
    };
    
    class CConstraintDefinitionBuilder : public CBuilderAtom
    {
    	DECLARE_CUSTOMATOM(CConstraintDefinitionBuilder, CBuilderAtom)
    public :
    	CString expr;
    	CString retType;
    	bool stereo;
    	CString paramList;
    	virtual void Initialize();
		void BuildUML(::Uml::Class &uml_class);
    };

    #endif // whole file
