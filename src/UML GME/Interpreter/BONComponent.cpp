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
	-	05/20/06	-	endre
		- add support for nested UML namespaces

	-	03/20/06	-	endre

		- set association and composition names even if they have not
		  been changed from the default "Association", respectively
		  "Composition"

	-	12/31/05	-	endre

		- use UdmDom data networks and UdmProjects to build the result of interpretation

	-	11/24/05	-	endre

				- change cross class names to class_name + "_cross_ph_" + diagram_name [ + "_cross_ph_" + namespace_name ]
				- add CAssociationBase::DumpCrossXML to fix the generation of Association elements in the cross XML file
				(previously no XML was generated if the association was in a namespace)

	-	11/10/04	-	endre

				- support for silent mode added.

	-  13/07/04	-	endre

				- isNavigable property was not saved correctly in the XML file.
				(if there is no role name, the role is considered not navigable)

	-	03/04/04	-	endre

				- an attempt to use the cross-link machinery for temporary links between the objects of the same datanetworks
				the rule as suggested by Adi: Associations that are among classes of the same package, but the connection itself
				is drawn outside the package are considered semi-cross associations, and will be instantiated in the cross-datanetwork 
				of the Udm project files.

				
	-	27/02/04	-	endre
				
				-	added the call GetPackage() for CClassBuilder and CCompositeClassBuilder 

	-	26/02/04	-	endre

				-	changed all CompositeClass->GetFolder() calls to GetParent()->GetParent() calls because 
					the parent of classes now are Models.
	-	24/02/04	-	endre

				-	changed the code to:
						- work with package models instead of package folders
						- ignore class diagram in the root folder
						- generate the version information in the output XML
	-	12/12/03	-	endre

					Bugfix: replaced the "_" delimiter characther with the 
					"_cross_ph_" delimiter string, to allow regular class-names 
					and diagram names to contain the "_" character


	-   12/02/03	-	endre

					bugfix: cross-link generation bug in situation of multiple inheritence

	-   11/28/03	-	endre
					
					  XSD support added

	-   11/118/03	- aditya
					added support for interpterer to work with UMT
					changed error messages to int_exceptions that are caught by 
					ComponentDll.cpp

	-	10/17/02	- Attribute specifier now is conformant with
					the UML Notation Guide standard 1.4

	-	06/17/02	-	endre

		  Added support for attribute arrays


	-	06/03/02	-	endre

		CCompositionBuilder::GetRoleID()	- had a bug which could not handle self-containment.
		Removed thsi functions and introduced two, GetParentRoleId(), and getChildrenRoleId(),
		which undoubtly code-doubling, but at least works;)

  */

#include "stdafx.h"

#include "Lattrib.hpp"
#include "Pattrib.hpp"


#include "CardinalityObject.h"
#include "AttributeObject.h"
#include <afxdlgs.h>
#include <set>


#include "BONComponent.h"
#include "int_exception.h"
#include "Console.h"

#include "Resource.h"

#include "Uml.h"
#include "UmlExt.h"
#include "UdmDom.h"
#include "UdmProject.h"
#include "UdmStatic.h"

char* GetRelativeFilename(const char *currentDirectory,const char *absoluteFilename);

CComponent *CComponent::theInstance = 0;

using namespace std;


//////////////////////////////// CComponent //////////////////////////////////


BOOL CALLBACK resenum(
  HMODULE hModule,   // module handle
  LPCTSTR lpszType,  // resource type
  LPTSTR lpszName,   // resource name
  long  lParam    // application-defined parameter
)
{
	
	AfxMessageBox(CString(lpszName) + CString(":") + CString(lpszType));
	return TRUE;
};


//void CComponent::Invoke(CBuilder &builder,CBuilderObjectList &selected, long param)
void CComponent::InvokeEx(CBuilder &builder,CBuilderObject *focus, CBuilderObjectList &selected, long param)
{	
	try
	{
		CBuilderFolder *root = builder.GetRootFolder();
		IMgaProject *proj = builder.GetProject();
		COMVERIFY(proj->get_ProjectConnStr(m_strConnection.GetAddress()));
		CComPtr<IMgaMetaProject> mproj;
		COMVERIFY( proj->get_RootMeta(&mproj));
		CBstr pn;
		COMVERIFY( mproj->get_Name(pn) );
		CString paradigmName = pn;
		if(paradigmName  == "UMLModelTransformer")	isUMT = true;
		else isUMT = false;
		
		theInstance = this;		// cannot use this in Initialize() functions!!!
		
		name = root->GetName();

		//getting the packages
		CPackageBuilderList *packages = &packageList;
		GatherPackageFolders(root, *packages);
		
		if(packages->GetCount() == 0)
		{	throw int_exception("Uml2Xml Error: There are no Class Diagrams inside of Packages in the project. No output will be generated");
		}

		if(packages->GetCount() == 1)
		{
			//only one diagram
			CPackageBuilder * package = packages->GetHead();
			package->Build();
			package->BuildInheritance();
			package->CheckInheritance();
			package->BuildCompositions();
			package->BuildAssociations();

			CString name = package->GetNameorAlias();

			CString filepath;

			if (param & GME_SILENT_MODE && (!isUMT) )
				filepath = name + "_udm.xml";
			else
				filepath = GetFilePath(builder, focus, "xml", "XML files (*.xml)|*.xml||");

			if(filepath == "")
			{	throw int_exception("");
			}

			::UdmDom::DomDataNetwork dn(::Uml::diagram);
			dn.CreateNew((LPCTSTR) filepath, "Uml", ::Uml::Diagram::meta, Udm::CHANGES_LOST_DEFAULT);

			package->SetUmlDiagram(::Uml::Diagram::Cast(dn.GetRootObject()));
			package->BuildUML();
			package->BuildUMLClasses();
			package->BuildUMLInheritance();
			package->BuildUMLCompositions();
			package->BuildUMLAssociations();

			dn.CloseWithUpdate();

			if(isUMT)
			{	
				CString sysCall = "call \"%UDM_PATH%\\bin\\Udm.exe\" \"" + filepath + "\"";
				if (!(param & GME_SILENT_MODE))
					sysCall += "|| pause";
				if(system(LPCSTR(sysCall))!=0)
					throw int_exception("Uml2Xml Error: Unable to run command: "+ sysCall);
			}

			return;
		}


		CString filepath;
		
		if (param & GME_SILENT_MODE && (!isUMT) )
			filepath = name + "_udm.udm";
		else
			filepath = GetFilePath(builder, focus,"udm","Udm Project Files (*.udm)|*.udm||");
		if(filepath == "")
		{	throw int_exception("");
			
		}

		{
			vector<Udm::DataNetworkSpecifier> dnsvec;
			POSITION mPos = packages->GetHeadPosition();
			while (mPos)
			{
				CPackageBuilder * package = packages->GetNext(mPos);
				dnsvec.push_back(Udm::DataNetworkSpecifier((LPCTSTR) (package->GetNameorAlias() + ".xml"), "Uml", ::Uml::Diagram::meta));
			}

			Udm::UdmProject pr;
			pr.CreateNewMeta((LPCTSTR) name, (LPCTSTR) filepath, dnsvec, Udm::CHANGES_PERSIST_ALWAYS);
			cross_uml_dgr = ::Uml::Diagram::Cast(pr.GetCrossMetaNetwork().GetRootObject());

			mPos = packages->GetHeadPosition();
			while (mPos)
				packages->GetNext(mPos)->Build();

			mPos = packages->GetHeadPosition();
			while (mPos)
			{
				CPackageBuilder * package = packages->GetNext(mPos);
				package->BuildInheritance();
				package->CheckInheritance();
				package->BuildCompositions();
				package->BuildAssociations();
				if (package->GetNameorAlias() == name)
				{
					throw int_exception("Error: package " + package->GetNameorAlias() + " must have a different name than the root folder");
				}

				::Uml::Diagram dgr = ::Uml::Diagram::Cast(pr.GetDataNetwork((LPCTSTR) (package->GetNameorAlias() + ".xml")).GetRootObject());

				package->SetUmlDiagram(dgr);
				package->BuildUML();
				package->BuildUMLClasses();
				package->BuildUMLInheritance();
				package->BuildUMLCompositions();
				package->BuildUMLAssociations();
			}

			mPos = packages->GetHeadPosition();
			while (mPos)
				packages->GetNext(mPos)->BuildCrossUMLClasses();

			mPos = packages->GetHeadPosition();
			while (mPos)
				packages->GetNext(mPos)->BuildCrossUMLInheritance();

			mPos = packages->GetHeadPosition();
			while (mPos)
			{
				CPackageBuilder * package = packages->GetNext(mPos);
				package->BuildCrossUMLCompositions();
				package->BuildCrossUMLAssociations();
			}
		}

		if(isUMT)
		{	CString sysCall = "call \"%UDM_PATH%\\bin\\Udm.exe\" \"" + filepath + "\"";
			if (!(param & GME_SILENT_MODE))
				sysCall += "|| pause";
			if(system(LPCSTR(sysCall))!=0)
				throw int_exception("Uml2Xml Error: Unable to run command: "+ sysCall);
		}

	}
	catch (int_exception &e)
	{
		std::string err = e.what();
		//do we have stg to say?
		if (err != "")
		{
			if (!(param & GME_SILENT_MODE))
				AfxMessageBox(err.c_str());
			else {
				std::cerr << err << std::endl;
				throw (HRESULT)E_FAIL;
			}
		}
	}
	catch (udm_exception &e)
	{
		std::string err = std::string("Caught udm_exception in UML2XML: \"") + e.what() + "\"";
		GMEConsole::Console::SetupConsole(builder.GetProject());
		GMEConsole::Console::Error::WriteLine(err.c_str());
		GMEConsole::Console::ReleaseConsole();
		if (!(param & GME_SILENT_MODE))
			AfxMessageBox(err.c_str());
		else
			throw (HRESULT)E_FAIL;
	}
}

int CComponent::GatherPackageFolders(CBuilderFolder *folder,CPackageBuilderList &packages)
{
	int foldercount = 0;
	if(folder->GetKindName() == "RootFolder")
	{

		const CBuilderFolderList *subfolders = folder->GetSubFolders();
		if(subfolders)
		{
			POSITION pos = subfolders->GetHeadPosition();
			while(pos)
			{
				CBuilderFolder *subfolder = subfolders->GetNext(pos);
				foldercount += GatherPackageFolders(subfolder, packages);
			}
		}

		const CBuilderModelList *models = folder->GetRootModels();
		if(models)
		{	
			POSITION pos = models->GetHeadPosition();
			while(pos)
			{	
				CBuilderModel *model = models->GetNext(pos);
				if (model->IsKindOf(RUNTIME_CLASS(CPackageBuilder)))
				{
					packages.AddTail(BUILDER_CAST(CPackageBuilder, model));	
					foldercount = 1;
				}		
			}
		}
			
		return foldercount;
	}
	return foldercount;
}


///////////////////////////// CContainer ///////////////////////////////

CContainer::~CContainer()
{
	POSITION pos = compositeClasses.GetHeadPosition();
	while(pos)
		delete compositeClasses.GetNext(pos);
}

void CContainer::TraverseModels(void *pointer)
{
	if (pointer == NULL) return;
	CBuilderModelList *models = (CBuilderModelList *) pointer;
	POSITION pos = models->GetHeadPosition();
	while(pos)
	{
		CBuilderModel *model = models->GetNext(pos);
		if(model->GetKindName() == "ClassDiagram")
		{
			CClassDiagramBuilder *sheet = dynamic_cast<CClassDiagramBuilder *>(model);
			if(!sheet)
			{
				AfxMessageBox("Unexpected model kind found: " + sheet->GetName());
				continue;
			}
			sheet->Build();
			TraverseModels((void *) sheet->GetModels());
		}
		if(model->GetKindName() == "Namespace")
		{
			CNamespaceBuilder *ns = dynamic_cast<CNamespaceBuilder *>(model);
			namespaces.AddTail(ns);
			ns->Build();
		}
	}
}

void CContainer::AddCompositeClass(CCompositeClass *cls)
{
	compositeClasses.AddTail(cls);
}

void CContainer::AddComposition(CCompositionBuilder *comp)
{
	compositions.AddTail(comp);
}

void CContainer::AddAssociation(CAssociationBase *ass)
{
	associations.AddTail(ass);
}

void CContainer::BuildInheritance()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildInheritance();

	pos = compositeClasses.GetHeadPosition();
	while(pos)
		compositeClasses.GetNext(pos)->FindSubClasses();
}

bool CContainer::CheckInheritance()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		if(!namespaces.GetNext(pos)->CheckInheritance())
			return false;

	pos = compositeClasses.GetHeadPosition();
	while(pos) {
		CStringList trace;
		CCompositeClass *comp = compositeClasses.GetNext(pos);
		if(!comp->CheckInheritance(trace)) {
			ASSERT(!trace.IsEmpty());
			CString traceTxt;
			POSITION pos = trace.GetHeadPosition();
			while(pos)
				traceTxt += trace.GetNext(pos) + " -> ";
			traceTxt += trace.GetHead();
			AfxMessageBox("Loop detected in the inheritance hierarchy: \n" + traceTxt,MB_OK | MB_ICONSTOP);
			return false;
		}
	}
	return true;
}

void CContainer::BuildCompositions()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildCompositions();

	pos = compositeClasses.GetHeadPosition();
	while(pos)
		compositeClasses.GetNext(pos)->BuildCompositions();
}

void CContainer::BuildAssociations()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildAssociations();

	pos = compositeClasses.GetHeadPosition();
	while(pos)
		compositeClasses.GetNext(pos)->BuildAssociations();
}

void CContainer::BuildUMLClasses()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildUMLClasses();

	pos = compositeClasses.GetHeadPosition();
	while(pos)
		compositeClasses.GetNext(pos)->BuildUML();
}

void CContainer::BuildUMLInheritance()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildUMLInheritance();

	pos = compositeClasses.GetHeadPosition();
	while(pos)
		compositeClasses.GetNext(pos)->BuildUMLInheritance();
}

void CContainer::BuildUMLAssociations()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildUMLAssociations();

	pos = associations.GetHeadPosition();
	while(pos)
		associations.GetNext(pos)->BuildUML();
}

void CContainer::BuildUMLCompositions()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildUMLCompositions();

	pos = compositions.GetHeadPosition();
	while(pos)
		compositions.GetNext(pos)->BuildUML();
}

void CContainer::BuildCrossUMLClasses()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildCrossUMLClasses();

	pos = compositeClasses.GetHeadPosition();
	while(pos)
		compositeClasses.GetNext(pos)->BuildCrossUML();
}

void CContainer::BuildCrossUMLInheritance()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildCrossUMLInheritance();

	pos = compositeClasses.GetHeadPosition();
	while(pos)
		compositeClasses.GetNext(pos)->BuildCrossUMLInheritance();
}

void CContainer::BuildCrossUMLAssociations()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildCrossUMLAssociations();

	pos = associations.GetHeadPosition();
	while(pos)
		associations.GetNext(pos)->BuildCrossUML();
}

void CContainer::BuildCrossUMLCompositions()
{
	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildCrossUMLCompositions();

	pos = compositeClasses.GetHeadPosition();
	while(pos)
		compositeClasses.GetNext(pos)->BuildCrossUMLCompositions();
}

///////////////////////////// CPackage /////////////////////////////////

IMPLEMENT_CUSTOMMODEL(CPackageBuilder, CBuilderModel, "Package")

CString CPackageBuilder::GetVersion() const
{
	CString ret;
	GetAttribute("version", ret);
	return ret;
};

CString CPackageBuilder::GetNameorAlias() const
{
	CString ret;
	GetAttribute("alias", ret);
	if (ret.GetLength()) return ret;
	else return GetName();
};

void CPackageBuilder::Build()
{
	CContainer::TraverseModels((void *) GetModels());
};

void CPackageBuilder::BuildUML()
{
	uml_dgr.name() = (LPCTSTR) GetNameorAlias();
	uml_dgr.version() = (LPCTSTR) GetVersion();

	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildUML();
}

///////////////////////////// CNamespace ////////////////////////////////////

IMPLEMENT_CUSTOMMODEL(CNamespaceBuilder, CBuilderModel, "Namespace")

void CNamespaceBuilder::Build()
{
	CContainer::TraverseModels((void *) GetModels());
};

CPackageBuilder * CNamespaceBuilder::GetPackage() const
{
    CPackageBuilder * package;
	const CBuilderModel * parent = GetParent();
	while (parent && parent->GetKindName() != "Package")
		parent = parent->GetParent();
	package = BUILDER_CAST(CPackageBuilder, parent);
	ASSERT(package);
	return package;

};

CNamespaceBuilder * CNamespaceBuilder::GetNamespace() const
{
	CNamespaceBuilder * ns;
	const CBuilderModel *parent = GetParent();
	while (parent && parent->GetKindName() != "Namespace")
		parent = parent->GetParent();
	ns = BUILDER_CAST(CNamespaceBuilder, parent);
	return ns;
};

void CNamespaceBuilder::BuildUML()
{
	const CBuilderModel *parent = GetParent();
	if (parent->GetKindName() == "Namespace") {
		uml_ns = ::Uml::Namespace::Create(BUILDER_CAST(CNamespaceBuilder, parent)->GetUmlNamespace());
	} else if (parent->GetKindName() == "Package") {
		uml_ns = ::Uml::Namespace::Create(BUILDER_CAST(CPackageBuilder, parent)->GetUmlDiagram());
	}
	uml_ns.name() = (LPCTSTR) GetName();

	POSITION pos = namespaces.GetHeadPosition();
	while(pos)
		namespaces.GetNext(pos)->BuildUML();
}

///////////////////////////// CClassDiagram /////////////////////////////////

IMPLEMENT_CUSTOMMODEL(CClassDiagramBuilder, CBuilderModel, "ClassDiagram")

CNamespaceBuilder * CClassDiagramBuilder::GetNamespace() const
{
	CNamespaceBuilder * ns;
	const CBuilderModel *parent = GetParent();
	while (parent && parent->GetKindName() != "Namespace")
		parent = parent->GetParent();
	ns = BUILDER_CAST(CNamespaceBuilder, parent);
	return ns;
};

void CClassDiagramBuilder::Build()
{
	const CBuilderAtomList *classes = GetAtoms("Class");
	POSITION pos = classes->GetHeadPosition();
	while(pos) {
		CClassBuilder *cls = dynamic_cast<CClassBuilder *>(classes->GetNext(pos));
		ASSERT(cls);
		CBuilderObjectList copies;
		cls->GetReferencedBy(copies);

		CCompositeClass *comp = new CCompositeClass(cls,copies);
		CNamespaceBuilder *ns = GetNamespace();
		if (ns)
			ns->AddCompositeClass(comp);
		else
			GetPackage()->AddCompositeClass(comp);
	}
}

CPackageBuilder * CClassDiagramBuilder::GetPackage() const
{
    CPackageBuilder * package;
	const CBuilderModel * parent = GetParent();
	while (parent && parent->GetKindName() != "Package")
		parent = parent->GetParent();
	package = BUILDER_CAST(CPackageBuilder, parent);
	ASSERT(package);
	return package;

};

////////////////////////////// CCompositeClass ////////////////////////////////

CCompositeClass::CCompositeClass(CClassBuilder *c,CBuilderObjectList &copies) : cls(c), association(0), flag(false)
{
	cls->composite = this;
	parts.AddHead(cls);
	POSITION pos = copies.GetHeadPosition();
	while(pos) 
	{	CBuilderObject *cpy = copies.GetNext(pos);
		if(cpy->GetKindName() != "ClassCopy")
			continue;
		CClassCopyBuilder *copy = dynamic_cast<CClassCopyBuilder *>(cpy);
		copy->composite = this;
		ASSERT(copy);
		parts.AddTail(copy);
	}
}

void CCompositeClass::BuildUML()
{
	CNamespaceBuilder *ns = cls->GetClassDiagram()->GetNamespace();
	if (ns)
		uml_cls = ::Uml::Class::Create(ns->GetUmlNamespace());
	else
		uml_cls = ::Uml::Class::Create(cls->GetClassDiagram()->GetPackage()->GetUmlDiagram());

	uml_cls.name() = (LPCTSTR) GetName();
	if(!cls->stereotype.IsEmpty())
		uml_cls.stereotype() = (LPCTSTR) cls->stereotype;
	uml_cls.isAbstract() = IsAbstract();

	std::vector<AttributeObject>::iterator aoi = cls->attributes.begin();
	while (aoi != cls->attributes.end())
	{
		aoi->BuildUML(uml_cls);
		aoi++;
	}

	//Dump the constraints, if any
	const CBuilderConnectionList *  constraints = cls->GetInConnections("HasConstraint");

	POSITION mPos = constraints->GetHeadPosition();
	while (mPos)
		//this line should not fail. The paradigm allows only constraint source
		//for HasConstraint type connection
		BUILDER_CAST(CConstraintBuilder, constraints->GetNext(mPos)->GetSource())->BuildUML(uml_cls);

	//Dump the constraintDefinitions, if any
	const CBuilderConnectionList *  constraintdefs = cls->GetInConnections("HasDefinition");
	POSITION cdPos = constraintdefs->GetHeadPosition();
	while (cdPos)
		//this line should not fail. The paradigm allows only constraint source
		//for HasConstraint type connection
		BUILDER_CAST(CConstraintDefinitionBuilder, constraintdefs->GetNext(cdPos)->GetSource())->BuildUML(uml_cls);
}

void CCompositeClass::BuildUMLInheritance()
{
	if(!baseClasses.IsEmpty())
	{
		POSITION pos = baseClasses.GetHeadPosition();
		while(pos)
			uml_cls.baseTypes() += baseClasses.GetNext(pos)->GetUmlClass();
	}
	if(!subClasses.IsEmpty()) {
		POSITION pos = subClasses.GetHeadPosition();
		while(pos)
			uml_cls.subTypes() += subClasses.GetNext(pos)->GetUmlClass();
	}
}

void CCompositeClass::BuildCrossUML()
{
	if(!IsCrossClass()) return;

	CNamespaceBuilder * cls_ns = cls->GetClassDiagram()->GetNamespace();

	CString class_ph_name = GetName() + "_cross_ph_" + cls->GetPackage()->GetNameorAlias();
	CString from = cls->GetPackage()->GetNameorAlias();

	if(cls_ns)
	{
		// name of parent namespaces, from bottom to the top
		vector<CString> ns_names;
		while(cls_ns)
		{
			ns_names.push_back(cls_ns->GetName());
			cls_ns = cls_ns->GetNamespace();
		}
		for(vector<CString>::reverse_iterator i = ns_names.rbegin(); i != ns_names.rend(); i++)
		{
			class_ph_name += "_cross_ph_" + *i;
			from += ":" + *i;
		}
	}

	cross_uml_cls = ::Uml::Class::Create(CComponent::theInstance->GetCrossUmlDiagram());
	cross_uml_cls.name() = (LPCTSTR) class_ph_name;
	cross_uml_cls.isAbstract() = false;
	cross_uml_cls.from() = (LPCTSTR) from;

	if(!HasCrossBases())
	{
		AttributeObject rem_sysname("rem_sysname:String[1..1]");
		rem_sysname.BuildUML(cross_uml_cls);
		AttributeObject rem_id("rem_id:Integer[1..1]");
		rem_id.BuildUML(cross_uml_cls);
	}
}

bool CCompositeClass::HasCrossBases()
{
	bool has_cross_bases = false;

	if (!baseClasses.IsEmpty())
	{
		//we only care about baseclasses when this class actually inherits a cross-package association
		//so we simply check if any of the baseclasses is part of a cross-package association
		POSITION bc_pos = baseClasses.GetHeadPosition();
		while (!has_cross_bases && bc_pos)
		{
			CCompositeClass * cccl = baseClasses.GetNext(bc_pos);
			has_cross_bases = cccl->IsCrossClass();
		};
	}

	return has_cross_bases;
}

void CCompositeClass::BuildCrossUMLInheritance()
{
	if(!IsCrossClass()) return;

	if(HasCrossBases())
	{
		/*
			Multiple inheritence bug: Only base-classes which are CrossClasses (or inherited from cross-classes)
			should be listed here, because for the other ones no placeholders classes are generated.

			Although we sure that at least one such class will exist once HasCrossBases() is true ...
		*/
		POSITION pos = baseClasses.GetHeadPosition();
		while (pos)
		{
			CCompositeClass * cccl = baseClasses.GetNext(pos);
			if (cccl->IsCrossClass())
				cross_uml_cls.baseTypes() += cccl->GetCrossUmlClass();
		};
	}

	if(!subClasses.IsEmpty()) {
		POSITION pos = subClasses.GetHeadPosition();
		while(pos)
			cross_uml_cls.subTypes() += subClasses.GetNext(pos)->GetCrossUmlClass();
	}
}

void CCompositeClass::BuildCrossUMLCompositions()
{
	if(!IsCrossClass()) return;

	// if it's a derived class, no composition is generated - it uses the base's composition */
	if(HasCrossBases()) return;

	::Uml::Diagram cross_dgr = CComponent::theInstance->GetCrossUmlDiagram();
	::Uml::Class cont_class = ::Uml::classByName(cross_dgr, "_gen_cont");
	ASSERT(cont_class != ::Uml::Class(NULL));

	::Uml::Composition comp = ::Uml::Composition::Create(cross_dgr);
	::Uml::CompositionChildRole crole = ::Uml::CompositionChildRole::Create(comp);
	crole.target() = cross_uml_cls;
	crole.min() = 0;
	crole.max() = -1;
	crole.name() = static_cast<std::string>(cross_uml_cls.name()) + "_childrole";

	::Uml::CompositionParentRole prole = ::Uml::CompositionParentRole::Create(comp);
	prole.target() = cont_class;
	prole.name() = static_cast<std::string>(cross_uml_cls.name()) + "_parentrole";
}

CString CCompositeClass::GetName()
{
	return cls ? cls->GetName() : "";
}

void CCompositeClass::FindSubClasses(CBuilderObjectList &triangles)
{
	POSITION pos = triangles.GetHeadPosition();
	while(pos) {
		CBuilderObject *triangle = triangles.GetNext(pos);
		CBuilderObjectList subs;
		triangle->GetOutConnectedObjects("Sub",subs);
		RegisterSubClasses(subs);
	}
}

void CCompositeClass::FindSubClasses()
{
	POSITION pos = parts.GetHeadPosition();
	while(pos) {
		CClassBase *part = parts.GetNext(pos);
		CBuilderObjectList triangles;
		part->builder->GetOutConnectedObjects("Base",triangles);
		FindSubClasses(triangles);
	}
}

void CCompositeClass::RegisterSubClasses(CBuilderObjectList &subs)
{
	POSITION pos = subs.GetHeadPosition();
	while(pos) {
		CBuilderObject *obj = subs.GetNext(pos);
		CClassBase *subClass = dynamic_cast<CClassBase *>(obj);
		if(subClass == 0)
			AfxMessageBox("Unexpected subclass " + obj->GetName() + " of " + GetName(),MB_OK | MB_ICONSTOP);
		else {
			subClass->composite->AddBaseClass(this);
			AddSubClass(subClass->composite);
		}
	}
}

void CCompositeClass::AddSubClass(CCompositeClass *sub)
{
	if(!subClasses.Find(sub))
		subClasses.AddTail(sub);
}

void CCompositeClass::AddBaseClass(CCompositeClass *base)
{
	if(!baseClasses.Find(base))
		baseClasses.AddTail(base);
}

bool CCompositeClass::CheckInheritance(CStringList &trace)
{
	if(flag) {
		flag = false;
		return false;
	}
	flag = true;
	trace.AddTail(GetName());

	POSITION pos = subClasses.GetHeadPosition();
	while(pos) {
		if(!subClasses.GetNext(pos)->CheckInheritance(trace)) {
			flag = false;
			return false;
		}
	}

	trace.RemoveTail();
	flag = false;
	return true;
}

void CCompositeClass::DumpInheritance()
{
	POSITION pos = baseClasses.GetHeadPosition();
	while(pos)
		AfxMessageBox("Base class of " + GetName() + " is " + baseClasses.GetNext(pos)->GetName());
}

void CCompositeClass::BuildCompositions()
{
	POSITION pos = parts.GetHeadPosition();
	while(pos) {
		CClassBase *part = parts.GetNext(pos);
		const CBuilderConnectionList *conns= part->builder->GetInConnections("Composition");

		CString name = GetName();

		if(conns) {
			POSITION cpos = conns->GetHeadPosition();
			while(cpos) {
				CBuilderConnection *conn = conns->GetNext(cpos);
				CCompositionBuilder *composition = dynamic_cast<CCompositionBuilder *>(conn);
				ASSERT(composition);
				composition->parent.cls = this;
				CClassBase *src = dynamic_cast<CClassBase *>(composition->GetSource());
				ASSERT(src);
				composition->child.cls = src->composite;
				RegisterComposition(composition);
			}
		}
	}
}

void CCompositeClass::RegisterComposition(CCompositionBuilder *composition)
{
	POSITION pos = parentCompositions.GetHeadPosition();
	while(pos) {
		CCompositionBuilder *candidate = parentCompositions.GetNext(pos);
		if(candidate->child.cls == composition->child.cls &&
				candidate->child.name == composition->child.name)
			return;
	}

	parentCompositions.AddTail(composition);
	composition->child.cls->childCompositions.AddTail(composition);

	CNamespaceBuilder *comp_ns = composition->GetNamespace();
	if (comp_ns)
		comp_ns->AddComposition(composition);
	else
	{
		CPackageBuilder *comp_package = composition->GetPackage();
		comp_package->AddComposition(composition);
	}
}

void CCompositeClass::BuildAssociations()
{
	POSITION pos = parts.GetHeadPosition();
	while(pos) 
	{
		CClassBase *part = parts.GetNext(pos);
		CBuilderObjectList connectors;
		part->builder->GetOutConnectedObjects("Src",connectors);
		POSITION cpos = connectors.GetHeadPosition();
		while(cpos) 
		{
			CAssociationBuilder *connector = dynamic_cast<CAssociationBuilder *>(connectors.GetNext(cpos));
			CBuilderObjectList dstList;
			connector->GetOutConnectedObjects("Dst",dstList);
			if(dstList.IsEmpty())
				AfxMessageBox("Partially specified association for class " + GetName());
			else if(dstList.GetCount() > 1)
				AfxMessageBox("Overspecified association for class " + GetName());
			else 
			{
				CClassBase *dst = dynamic_cast<CClassBase *>(dstList.GetHead());

				ASSERT(dst && dst->composite);
				RegisterAssociation(connector,dst->composite);
			}
		}
		const CBuilderConnectionList *directs = part->builder->GetOutConnections("Association");
		if(directs) {
			cpos = directs->GetHeadPosition();
			while(cpos) {
				CDirectAssociationBuilder *direct = dynamic_cast<CDirectAssociationBuilder *>(directs->GetNext(cpos));
				ASSERT(direct);
				CClassBase *dst = dynamic_cast<CClassBase *>(direct->GetDestination());
				ASSERT(dst && dst->composite);
				if (!dst) {
					// UDM-47
					CComPtr<IMgaFCO> destFco;
					COMVERIFY(direct->GetIConnection()->get_Dst(&destFco));
					CBstr name;
					COMVERIFY(destFco->get_Name(name));
					throw udm_exception(static_cast<CString>(name) + " is a null reference");
				}
				RegisterAssociation(direct,dst->composite);
			}
		}
	}
}

void CCompositeClass::RegisterAssociation(CAssociationBase *assoc,CCompositeClass *dst)
{
	assoc->SetSourceAndDestination(this,dst);
	assoc->SetRolesAndCardinalities();
	POSITION pos = srcAssociations.GetHeadPosition();
	while(pos) {
		CAssociationBase *ass = srcAssociations.GetNext(pos);
		if(assoc->IsEquivalent(ass))
			return;
	}
	srcAssociations.AddTail(assoc);
	dst->dstAssociations.AddTail(assoc);

	CNamespaceBuilder *ass_ns = assoc->GetNamespace();
	if (ass_ns)
		ass_ns->AddAssociation(assoc);
	else
	{
        CPackageBuilder *ass_package = assoc->GetPackage();
		if (!ass_package)
		{
			throw int_exception(assoc->path + " must be contained in a Namespace or Package");
		}
		ass_package->AddAssociation(assoc);
	}
}

bool CCompositeClass::IsAbstract()
{
	POSITION pos = parts.GetHeadPosition();
	while(pos) {
		CBuilderObject *part = parts.GetNext(pos)->builder;
		bool abstract;
		part->GetAttribute("IsAbstract",abstract);
		if(abstract)
			return true;
	}
	return false;
}
bool CCompositeClass::IsCrossClass()
{
	bool cross_package = false;

	cross_package = association && (association->IsCrossPackage());

	POSITION pos = dstAssociations.GetHeadPosition();

	while (!cross_package && pos)
		cross_package = dstAssociations.GetNext(pos)->IsCrossPackage();

	pos = srcAssociations.GetHeadPosition();
	while (!cross_package && pos)
		cross_package = srcAssociations.GetNext(pos)->IsCrossPackage();


	pos = baseClasses.GetHeadPosition();
	while (!cross_package && pos)
		cross_package = baseClasses.GetNext(pos)->IsCrossClass();

	return cross_package;


};


////////////////////////////////// CRole //////////////////////////////////////

CRole::CRole() : cls(0), minc(1), maxc(1)
{
}

void CRole::ParseCardinality()
{
	//Invoke ANTLR to parse the cardinality string
	try
	{
		CardinalityObject c_obj((LPCTSTR)cardinality);
		minc = c_obj.getmin();
		maxc = c_obj.getmax();

	}
	catch(const CardinalityObjectException &e)
	{
		AfxMessageBox(CString("Parser error occurred while parsing cardinality string: ") + e.what());
	}

};


/////////////////////////////// CClassBuilder /////////////////////////////////

IMPLEMENT_CUSTOMATOM(CClassBuilder, CBuilderAtom, "Class")

void CClassBuilder::Initialize()
{
	CBuilderAtom::Initialize();
	builder = this;
	GetAttribute("Stereotype",stereotype);
	GetUMLAttributes();

}

void CClassBuilder::GetConstraints()
{

};

void CClassBuilder::GetConstraintDefinitions()
{

};

CPackageBuilder * CClassBuilder::GetPackage()
{
	CPackageBuilder *package = BUILDER_CAST(CClassDiagramBuilder, GetParent())->GetPackage();
	ASSERT(package);
	return package;

};

CClassDiagramBuilder * CClassBuilder::GetClassDiagram()
{
	CClassDiagramBuilder * cd = BUILDER_CAST(CClassDiagramBuilder, GetParent());
	ASSERT(cd);
	return cd;
};



void CClassBuilder::GetUMLAttributes()
{
	CString str_attributes;

	GetAttribute("Attributes",str_attributes);
	str_attributes.Trim();
	char buf[4096];
	char *string = buf;
	strcpy(string,(LPCSTR)str_attributes);
	const char * endbuf = buf + strlen(buf);
	char seps[16];
	sprintf(seps,"%c\n",13);
	char *token;

	token = strtok(string,seps);
	while(token)
	{

		try
		{
			//antlr will do the parsing
			AttributeObject curr_atto(token);
			attributes.push_back(curr_atto);
		}

		//catch any parsing exceptions
		catch (AttributeObjectException &e)
		{
			AfxMessageBox(CString("Attribute Object Exception: ") + e.what());
		}

		//step to the next token
		string = token + strlen(token) + 1;

		//(if there is one)
		if (string > endbuf) string = 0;

		//get the next token
		token = strtok(string,seps);
	}
}

///////////////////////////// CClassCopyBuilder ///////////////////////////////

IMPLEMENT_CUSTOMATOMREF(CClassCopyBuilder, CBuilderAtomReference, "ClassCopy")

void CClassCopyBuilder::Initialize()
{
	CBuilderAtomReference::Initialize();
	builder = this;
}

CPackageBuilder * CClassCopyBuilder::GetPackage()
{
	CPackageBuilder *package = BUILDER_CAST(CClassDiagramBuilder, GetParent())->GetPackage();
	ASSERT(package);
	return package;

};


///////////////////////////// CCompositionBuilder ///////////////////////////////

IMPLEMENT_CUSTOMCONNECTION(CCompositionBuilder, CBuilderConnection, "Composition")

void CCompositionBuilder::Initialize()
{
	CBuilderConnection::Initialize();
	GetAttribute("ChildRole",child.name);
	GetAttribute("ParentRole",parent.name);
	GetAttribute("Cardinality",child.cardinality);
	child.ParseCardinality();
}

CPackageBuilder* CCompositionBuilder::GetPackage() const
{
	CPackageBuilder *package = BUILDER_CAST(CClassDiagramBuilder, GetParent())->GetPackage();
	ASSERT(package);
	return package;
}

CNamespaceBuilder* CCompositionBuilder::GetNamespace() const
{
	return BUILDER_CAST(CClassDiagramBuilder, GetParent())->GetNamespace();
}

void CCompositionBuilder::BuildUML()
{
	CString childName = child.name;
	CString parentName = parent.name;
	CString nm = GetName();

	CNamespaceBuilder *ns = GetNamespace();
	if(ns)
		uml_comp = ::Uml::Composition::Create(ns->GetUmlNamespace());
	else
		uml_comp = ::Uml::Composition::Create(GetPackage()->GetUmlDiagram());

	if(!nm.IsEmpty())
		uml_comp.name() = (LPCTSTR) nm;

	::Uml::CompositionChildRole crole = ::Uml::CompositionChildRole::Create(uml_comp);
	if(!child.name.IsEmpty())
		crole.name() = (LPCTSTR) child.name;
	crole.min() = child.minc;
	crole.max() = child.maxc;
	crole.target() = child.cls->GetUmlClass();

	::Uml::CompositionParentRole prole = ::Uml::CompositionParentRole::Create(uml_comp);
	if(!parent.name.IsEmpty())
		prole.name() = (LPCTSTR) parent.name;
	prole.target() = parent.cls->GetUmlClass();
}


//////////////////////////////// CAssociationBase /////////////////////////////////

CAssociationBase::CAssociationBase() : associationClass(0)
{
}
bool CAssociationBase::IsCrossPackage()
{


	if (dest.cls->cls->GetPackage() != source.cls->cls->GetPackage()) return true;

	//at this time dest and source for sure are in the same package

	//return associationClass ? (associationClass->cls->GetPackage() != source.cls->cls->GetPackage()) : false;
	if (associationClass  && (associationClass->cls->GetPackage() != source.cls->cls->GetPackage()) ) return true;
	//at this time dest and source and assoc class are in the same package.
	//or there is no assoc, and dest and source are in the same package



	const CPackageBuilder * owner_package;
	if (associationClass)
	{
		//there is an assoc class,
		//so this is really  a CAssociationBuilder
		const CAssociationBuilder * cab =  (CAssociationBuilder *)this;
		owner_package = BUILDER_CAST(CClassDiagramBuilder, cab->GetParent())->GetPackage();
		
	}
	else
	{
		//there isn't an association  class
		//so this is a CDirectConnectionBuilder
		const CDirectAssociationBuilder * cdab =(CDirectAssociationBuilder *) this;
		owner_package = BUILDER_CAST(CClassDiagramBuilder, cdab->GetParent())->GetPackage();

		
	}

	ASSERT(owner_package);
	//we have the package where the connection was drawn.
	//if the connection was drawn in a different package, but the peers are in the same package,
	//the user meant to use the cross link machinery to create temporary links between the objects 
	//of the same datanetwork


	//source and dest  (and assoc class, if there is one) are in the same packagefor sure
	//no need to re-check them, just one.
	if (owner_package != source.cls->cls->GetPackage()) return true;
		



	return false;

};
bool CAssociationBase::IsEquivalent(CAssociationBase *ass)
{
	//if(source.cls != ass->source.cls || dest.cls != ass->dest.cls || association != ass->association)
	if(source.cls != ass->source.cls || dest.cls != ass->dest.cls || association.Compare(ass->association))
		return false;
	//if(source.name != ass->source.name || dest.name != ass->dest.name)
	if(source.name.Compare(ass->source.name ) || dest.name.Compare(ass->dest.name))
		return false;
	if(associationClass && ass->associationClass && associationClass != ass->associationClass)
		AfxMessageBox("Conflicting association classes: "
		+ associationClass->GetName() + " and " + ass->associationClass->GetName());
	return true;
}

void CAssociationBase::BuildUML()
{
	if (uml_ass == Udm::null && !IsCrossPackage())
	{
		CNamespaceBuilder *ns = GetNamespace();
		if(ns)
			uml_ass = ::Uml::Association::Create(ns->GetUmlNamespace());
		else
			uml_ass = ::Uml::Association::Create(GetPackage()->GetUmlDiagram());
		_BuildUML(uml_ass, false);
	}

}

void CAssociationBase::BuildCrossUML()
{
	if (IsCrossPackage())
	{
		cross_uml_ass = ::Uml::Association::Create(CComponent::theInstance->GetCrossUmlDiagram());
		_BuildUML(cross_uml_ass, true);
	}
}

void CAssociationBase::_BuildUML(::Uml::Association &ass, bool is_cross)
{
	CString srcName = source.name;
	bool src_isnavig = true;
	if(srcName.IsEmpty())
	{
		srcName = source.cls->GetName();
		src_isnavig = false;
	}

	CString dstName = dest.name;
	bool dst_isnavig = true;
	if(dstName.IsEmpty())
	{
		dstName = dest.cls->GetName();
		dst_isnavig = false;
	}

	CString nm = association;
	if(!nm.IsEmpty())
		ass.name() = (LPCTSTR) nm;

	if(associationClass)
	{
		Uml::Class assocClass = is_cross ? associationClass->GetCrossUmlClass() : associationClass->GetUmlClass();
		if (static_cast<Uml::Association>(assocClass.association()))
		{
			throw udm_exception(std::string("Error: ") + static_cast<const char*>(association) + " cannot have more than 1 association class. "
				"If two association classes are desired, use a superclass to define one.");
		}
		ass.assocClass() = assocClass;
	}

	::Uml::AssociationRole role = ::Uml::AssociationRole::Create(ass);
	role.name() = (LPCTSTR) srcName;
	role.min() = source.minc;
	role.max() = source.maxc;
	role.target() = is_cross ? source.cls->GetCrossUmlClass() : source.cls->GetUmlClass();
	role.isNavigable() = src_isnavig;
	//role.isPrimary() = true;

	::Uml::AssociationRole orole = ::Uml::AssociationRole::Create(ass);
	orole.name() = (LPCTSTR) dstName;
	orole.min() = dest.minc;
	orole.max() = dest.maxc;
	orole.target() = is_cross ? dest.cls->GetCrossUmlClass() : dest.cls->GetUmlClass();
	orole.isNavigable() = dst_isnavig;

	// set reference port helper relations by looking for simple associations of
	// the association class that have roles ending in "__rp_helper" or "__rp_container"
	if (associationClass) {
		POSITION cpos = associationClass->srcAssociations.GetHeadPosition();
		while (cpos) {
			CAssociationBase *assoc = dynamic_cast<CAssociationBase *>(associationClass->srcAssociations.GetNext(cpos));
			ASSERT(assoc);
			::Uml::Association uml_ass = assoc->GetUmlAssociation();
			if (uml_ass == Udm::null) {
				assoc->BuildUML();
				uml_ass = assoc->GetUmlAssociation();
			}
			set< ::Uml::AssociationRole> aroles = uml_ass.roles();
			for (set< ::Uml::AssociationRole>::const_iterator i = aroles.begin(); i != aroles.end(); i++) {
				string role_name = ::Uml::MakeRoleName(*i);
				if (role_name == string(srcName) + "__rp_helper" || role_name == string(srcName) + "__rp_container") {
					role.rp_helper() = *i;
				}
				if (role_name == string(dstName) + "__rp_helper" || role_name == string(dstName) + "__rp_container") {
					orole.rp_helper() = *i;
				}
			}
		}
	}
}


///////////////////////////// CDirectAssociationBuilder ///////////////////////////////

IMPLEMENT_CUSTOMCONNECTION(CDirectAssociationBuilder, CBuilderConnection, "Association")

void CDirectAssociationBuilder::Initialize()
{
	CBuilderConnection::Initialize();
	parent_classdgr = GetParent();
}

void CDirectAssociationBuilder::SetSourceAndDestination(CCompositeClass *s,CCompositeClass *d)
{
	GetExtendedName(path, "/", true);
	source.cls = s;
	dest.cls = d;
	associationClass = 0;
	association = name;
}

void CDirectAssociationBuilder::SetRolesAndCardinalities()
{
	GetAttribute("srcRolename",source.name);
	GetAttribute("srcCardinality",source.cardinality);
	source.ParseCardinality();
	GetAttribute("dstRolename",dest.name);
	GetAttribute("dstCardinality",dest.cardinality);
	dest.ParseCardinality();
}


/////////////////////////////// CAssociationBuilder ///////////////////////////////

IMPLEMENT_CUSTOMATOM(CAssociationBuilder, CBuilderAtom, "Connector")

void CAssociationBuilder::Initialize()
{
	CBuilderAtom::Initialize();
	parent_classdgr = GetParent();
}

void CAssociationBuilder::SetSourceAndDestination(CCompositeClass *s,CCompositeClass *d)
{
	source.cls = s;
	dest.cls = d;
	CBuilderObjectList assClasses;
	GetOutConnectedObjects("AssociationClass",assClasses);
	GetInConnectedObjects("AssociationClass",assClasses);
	if(assClasses.GetCount() > 1)
		AfxMessageBox("Multiple association classes defined for association!");
	else if(!assClasses.IsEmpty()) {
		CBuilderObject* assClass = assClasses.GetHead();
		CClassBase *cls = dynamic_cast<CClassBase *>(assClass);
		if (!cls)
		{
			CString path;
			assClass->GetExtendedName(path, "/", true);
			throw int_exception(path + " must be an association class");
		}
		ASSERT(cls);
		associationClass = cls->GetComposite();
		if (!associationClass)
		{
			CString path;
			assClass->GetExtendedName(path, "/", true);
			throw int_exception(path + " must be an association class"); // FIXME this error message is wrong
		}
		associationClass->SetAssociation(this);
		association = associationClass->GetName();
	}
}

void CAssociationBuilder::SetRolesAndCardinalities()
{
	const CBuilderConnectionList *srcs = GetInConnections("Src");
	if(srcs == 0 || srcs->GetCount() != 1)
		AfxMessageBox("Invalid source specification for association " + association);
	else {
		CBuilderObject *obj = srcs->GetHead();
		obj->GetAttribute("Cardinality",source.cardinality);
		source.ParseCardinality();
		obj->GetAttribute("srcRolename",source.name);
	}
	const CBuilderConnectionList *dsts = GetOutConnections("Dst");
	if(dsts == 0 || dsts->GetCount() != 1)
		AfxMessageBox("Invalid destination specification for association " + association);
	else {
		CBuilderObject *obj = dsts->GetHead();
		obj->GetAttribute("Cardinality",dest.cardinality);
		dest.ParseCardinality();
		obj->GetAttribute("dstRolename",dest.name);
	}
}


//////////////////////////////CConstraintBuilder//////////////////////////////

IMPLEMENT_CUSTOMATOM(CConstraintBuilder, CBuilderAtom, "Constraint")

void CConstraintBuilder::Initialize()
{
	CBuilderAtom::Initialize();
	GetAttribute("ConstraintDescription", desc);
	desc.Replace( "\"" , "\\\"" );
	GetAttribute("ConstraintEqn", expr);
	expr.Replace( "\n" , "\\n" );
	expr.Replace( "\r" , "\\r" );
	expr.Replace( "\"" , "\\\"" );
};

void CConstraintBuilder::BuildUML(::Uml::Class &uml_class)
{
	::Uml::Constraint c = ::Uml::Constraint::Create(uml_class);
	c.name() = (LPCTSTR) GetName();
	c.description() = (LPCTSTR) desc;
	c.expression() = (LPCTSTR) expr;
}


//////////////////////////////CConstraintDefinitionBuilder//////////////////////////////

IMPLEMENT_CUSTOMATOM(CConstraintDefinitionBuilder, CBuilderAtom, "ConstraintDefinition")

void CConstraintDefinitionBuilder::Initialize()
{
	CBuilderAtom::Initialize();

	GetAttribute( "DefinitionEqn", expr );
	expr.Replace( "\n" , "\\n" );
	expr.Replace( "\r" , "\\r" );
	expr.Replace( "\"" , "\\\"" );
	
	CString sStereo;
	GetAttribute( "DefinitionStereo", sStereo );
	stereo = sStereo == "method";

	GetAttribute( "DefinitionRetType", retType );

	GetAttribute( "DefinitionParamList", paramList );
	paramList.Replace( ",", ";" );
};

void CConstraintDefinitionBuilder::BuildUML(::Uml::Class &uml_class)
{
	::Uml::ConstraintDefinition c = ::Uml::ConstraintDefinition::Create(uml_class);
	c.name() = (LPCTSTR) GetName();
	c.expression() = (LPCTSTR) expr;
	c.stereotype() = ( stereo ) ? "method" : "attribute";
	c.returnType() = (LPCTSTR) retType;
	c.parameterList() = (LPCTSTR) paramList;
}


/////////////////////////////////////////////////////////////////////////////////////
// Writing back information to config
/////////////////////////////////////////////////////////////////////////////////////
CString CComponent::GetFilePath(CBuilder &builder, CBuilderObject *focus, char *FILE_EXT, char *UDM_FILTER)
{	
	CBuilderFolder *root = builder.GetRootFolder();
/*	IMgaProject *proj = builder.GetProject();
	CComPtr<IMgaMetaProject> mproj;
	COMVERIFY( proj->get_RootMeta(&mproj));
	CBstr pn;
	COMVERIFY( mproj->get_Name(pn) );
	CString paradigmName = pn;
	if(paradigmName  == "UMLModelTransformer")
	{	isUMT = true;
*/
	if(isUMT)
	{
		CBuilderFolder *root = builder.GetRootFolder();
		const CBuilderFolderList *subfolders = root->GetSubFolders();
		CBuilderModel *theCfg;
		if(focus != NULL && focus->GetKindName() == "Configuration")
			theCfg = BUILDER_CAST(CBuilderModel, focus);
		else
		{	POSITION spos = subfolders->GetHeadPosition();
			CBuilderModelList configs;
			while(spos)
			{
				CBuilderFolder *subfolder = subfolders->GetNext(spos);
				if(subfolder->GetKindName() != "Configurations")
					continue;
				
				const CBuilderModelList *cfgs = subfolder->GetRootModels();
				POSITION cmpos = cfgs->GetHeadPosition();
				while(cmpos)
				{
					CBuilderModel *curr_model = cfgs->GetNext(cmpos);
					if ( curr_model->GetKindName() == "Configuration")
						configs.AddTail( curr_model);
				}
			}
			if(configs.GetCount() == 0)
			{	throw int_exception("Uml2Xml Error: No Configuration model found. Use UMT Master Interpreter.");
			}
			else if(configs.GetCount() > 1)
			{	throw int_exception("Uml2Xml Error: More than one Configuration model found. Either Start interpretation from one or delete the others.");
			}
			else
			{	theCfg = configs.GetHead();
			}
		}	

		CBuilderAtom *theMetaIcon;
		const CBuilderAtomList *mis = theCfg->GetAtoms("MetaInformation");
		if(!mis || mis->GetCount() == 0)
			theMetaIcon = theCfg->CreateNewAtom("MetaInformation");
		else
			theMetaIcon = mis->GetHead();
		CString cfgFileName;
		CString cfgDirName;
		theCfg->GetAttribute("ConfigFile",cfgFileName);
		if(cfgFileName == "" || cfgFileName.GetLength() <3 || cfgFileName.GetAt(1) != ':')
		{	throw int_exception("Uml2Xml Error: Config file name is not absolute. Run UMT Master interpreter.");
		}
		
		int p = cfgFileName.ReverseFind('\\');
		cfgDirName = cfgFileName.Left(p+1);
		
		CString transFileName;
		theMetaIcon->GetAttribute("UdmProjectFile",transFileName);
		if(transFileName == "")
		{	throw int_exception("Uml2Xml Error: Udm file is blank. Run UMT Master interpreter.");
		}
		
		CString transFilePath = cfgDirName + transFileName;
		CString transDirName;
		
		p = transFilePath.ReverseFind('\\');
		transDirName = transFilePath.Left(p+1);

		if(CreateDirectory((LPCSTR)transDirName, NULL)==0)
		{	int le = GetLastError();
			if(le != 183)
			{	LPTSTR lpMsgBuf;
				FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								NULL, le, 0, (LPTSTR) &lpMsgBuf, 0, NULL );
				CString msg = "Uml2Xml Error: Unable to create directory " + transDirName + "\n";
				msg += (LPCSTR)lpMsgBuf;
				LocalFree(lpMsgBuf);
				throw int_exception(std::string((LPCSTR)msg));
			}
		}
		SetCurrentDirectory((LPCSTR)transDirName);
		return transFilePath;
	}
	else
	{	//static char UDM_FILTER[] = "XML files (*.xml)|*.xml";
		CFileDialog cfdlg(false, FILE_EXT, root->GetName(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, UDM_FILTER);	
		cfdlg.m_ofn.lpstrTitle = "UDM Meta File Location";
		TCHAR currentMgaPath[MAX_PATH];
		if (m_strConnection.length() > 4 && wcsnicmp(static_cast<const wchar_t*>(m_strConnection), L"MGA=", 4) == 0) {
			std::string zsConn = static_cast<const char*>(m_strConnection);
			TCHAR* filename;
			if (!GetFullPathName(zsConn.c_str() + 4 /* skip MGA= */, MAX_PATH, currentMgaPath, &filename) || filename == 0) {
			} else {
				*filename = _T('\0');
				cfdlg.GetOFN().lpstrInitialDir = currentMgaPath;
			}
		}
		if (cfdlg.DoModal() != IDOK) 
		{	throw int_exception("");
		}
		return cfdlg.GetPathName();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
