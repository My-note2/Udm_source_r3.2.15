///////////////////////////////////////////////////////////////////////////
// RawComponent.cpp, the main RAW COM component implementation file
// This is the file (along with its header RawComponent.h)
// that the component implementor is expected to modify in the first place
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <ComHelp.h>
#include <GMECOM.h>
#include "ComponentConfig.h"
#if _MSC_VER >= 1900 || !defined(__INTELLISENSE__)
// #import Meta, Mga, GME libs
#import "libid:0adeec71-d83a-11d3-b36b-005004d38590" no_implementation auto_search no_namespace no_search_namespace
#import "libid:270b4f86-b17c-11d3-9ad1-00aa00b6fe26" no_implementation auto_search no_namespace no_search_namespace
#import "libid:0adeec71-d83a-11d3-b36b-005004d38590" implementation_only auto_search no_namespace no_search_namespace
#import "libid:270b4f86-b17c-11d3-9ad1-00aa00b6fe26" implementation_only auto_search no_namespace no_search_namespace
#import "libid:0ADEEC71-D83A-11D3-B36B-005004CC8592" no_implementation auto_search no_namespace no_search_namespace
#import "libid:0ADEEC71-D83A-11D3-B36B-005004CC8592" implementation_only auto_search no_namespace no_search_namespace
#else
// IntelliSense has a known issue with the above lines.
//  http://connect.microsoft.com/VisualStudio/feedback/details/533526/vc-2010-intellisense-import-directive-using-libid-does-not-work
#ifdef _DEBUG
// If IntelliSense reports "Cannot open source file", compile then reopen the project
#include "Debug\Meta.tlh"
#include "Debug\Mga.tlh"
#else
#include "Release\Meta.tlh"
#include "Release\Mga.tlh"
#endif
#endif

#include "RawComponent.h"

#include "UdmConsole.h"

// Udm includes
#include "UdmBase.h"
#include "Uml.h"
#include "UmlExt.h"

using namespace std;

#ifdef _USE_DOM
	#include "UdmDOM.h"
#endif

#include "UdmGme.h"
#include "UdmStatic.h"
#include "UdmUtil.h"

#include "UdmApp.h"
#include "UdmConfig.h"

__declspec(noreturn) void ThrowComError(HRESULT hr, LPOLESTR err); // from ComHelp.cpp

// this method is called after all the generic initialization is done
// this should be empty, unless application-specific initialization is needed
STDMETHODIMP RawComponent::Initialize(struct IMgaProject *) {
	return S_OK;
}

// this is the obsolete component interface
// this present implementation either tries to call InvokeEx, or returns an error;
STDMETHODIMP RawComponent::Invoke(IMgaProject* gme, IMgaFCOs *models, long param) {
#ifdef SUPPORT_OLD_INVOKE
	CComPtr<IMgaFCO> focus;
	CComVariant parval = param;
	return InvokeEx(gme, focus, selected, parvar);
#else
	if(interactive) {
		AfxMessageBox(_T("This component does not support the obsolete invoke mechanism"));
	}
	return E_MGA_NOT_SUPPORTED;
#endif
}

#ifdef _DYNAMIC_META
			void dummy(void) {; } // Dummy function for UDM meta initialization
#endif


// This is the main component method for interpereters and plugins.
// May als be used in case of invokeable addons
STDMETHODIMP RawComponent::InvokeEx( IMgaProject *project, IMgaFCO *currentobj,
									IMgaFCOs *selectedobjs, long param)
{
	// Calling the user's initialization function
	if(CUdmApp::Initialize())
	{
		return S_FALSE;
	}

	COMTRY {
		CComPtr<IMgaProject>ccpProject(project);

		try
		{
			// Setting up the console
			GMEConsole::Console::SetupConsole(ccpProject);

			CComBSTR projname;
			CComBSTR focusname = L"<nothing>";
			CComPtr<IMgaTerritory> terr;

			// Setting up Udm
	#ifdef _DYNAMIC_META
		#ifdef _DYNAMIC_META_DOM
				// Loading the meta for the project
				UdmDom::DomDataNetwork ddnMeta(Uml::diagram);
				Uml::Diagram theUmlDiagram;

				// Opening the XML meta of the project
				ddnMeta.OpenExisting(META_PATH, "uml.dtd", Udm::CHANGES_LOST_DEFAULT);

				// Casting the DataNetwork to diagram
				theUmlDiagram = Uml::Diagram::Cast(ddnMeta.GetRootObject());

				// Creating the UDM diagram
				Udm::UdmDiagram udmDataDiagram;
				udmDataDiagram.dgr = &theUmlDiagram;
				udmDataDiagram.init = dummy;

		#elif defined _DYNAMIC_META_STATIC
				// Loading the meta for the project
				Udm::SmartDataNetwork  dnsMeta(Uml::diagram);
				Uml::Diagram theUmlDiagram;

				// Opening the static meta of the project
				dnsMeta.OpenExisting(META_PATH, "", Udm::CHANGES_LOST_DEFAULT);

				// Casting the DataNetwork to diagram
				theUmlDiagram = Uml::Diagram::Cast(dnsMeta.GetRootObject());

				// Creating the UDM diagram
				Udm::UdmDiagram udmDataDiagram;
				udmDataDiagram.dgr = &theUmlDiagram;
				udmDataDiagram.init = dummy;

		#else
				#error "Neither _DYNAMIC_META_DOM or _DYNAMIC_META_STATIC defined for dynamic loading"
		#endif
				// Loading the project
				UdmGme::GmeDataNetwork dngBackend(udmDataDiagram);

	#else
			using namespace META_NAMESPACE;

			// Loading the project
			UdmGme::GmeDataNetwork dngBackend(META_NAMESPACE::diagram);

	#endif
			try
			{
				bool invokeExStartedATransaction = false;
				if (!(ccpProject->ProjectStatus & 8))
				{
					terr = ccpProject->BeginTransactionInNewTerr(TRANSACTION_NON_NESTED); // could also use TRANSACTION_GENERAL
					invokeExStartedATransaction = true;
				}
				try
				{
					// Opening backend
					dngBackend.OpenExisting(ccpProject, Udm::CHANGES_LOST_DEFAULT, true);


					CComPtr<IMgaFCO> ccpFocus(currentobj);
					Udm::Object currentObject;
					if(ccpFocus)
					{
						currentObject=dngBackend.Gme2Udm(ccpFocus);
					}

					std::set<Udm::Object> selectedObjects;

					if (selectedobjs) {
						CComPtr<IMgaFCOs> ccpSelObject(selectedobjs);

						MGACOLL_ITERATE(IMgaFCO,ccpSelObject){
							Udm::Object currObj;
							if(MGACOLL_ITER)
							{
								currObj=dngBackend.Gme2Udm(MGACOLL_ITER);
							}
						 selectedObjects.insert(currObj);
						}MGACOLL_ITERATE_END;
					}

					// Calling the main entry point
					CUdmApp::UdmMain(&dngBackend,currentObject,selectedObjects,param);
					// Closing backend
					dngBackend.CloseWithUpdate();
					if (invokeExStartedATransaction)
						ccpProject->CommitTransaction();
				}
				catch (...)
				{
					if (invokeExStartedATransaction)
						ccpProject->raw_AbortTransaction();
					throw;
				}
			}
			catch(udm_exception &exc)
			{
				// Close GME Backend (we may close it twice, but GmeDataNetwork handles it)
				dngBackend.CloseNoUpdate();

				ThrowComError(E_FAIL, GMEConsole::BSTRFromUTF8(exc.what()));
			}
		}
		catch (udm_exception& e)
		{
			GMEConsole::Console::gmeoleapp = 0;
			ThrowComError(E_FAIL, GMEConsole::BSTRFromUTF8(e.what()));
		}
		catch (_com_error& e)
		{
			GMEConsole::Console::gmeoleapp = 0;
			throw;
		}
		catch(...)
		{
			GMEConsole::Console::gmeoleapp = 0;
			ThrowComError(E_FAIL, _bstr_t(L"An unexpected error has occurred during the interpretation process."));
		}
	} COMCATCH(GMEConsole::Console::gmeoleapp = 0;)
	return S_OK;

}

// GME currently does not use this function
// you only need to implement it if other invokation mechanisms are used
STDMETHODIMP RawComponent::ObjectsInvokeEx(IMgaProject *project,  IMgaObject *currentobj, IMgaObjects *selectedobjs, long param) {
	if(interactive) {
		AfxMessageBox(_T("The ObjectsInvoke method is not implemented"));
	}
	return E_MGA_NOT_SUPPORTED;
}


// implement application specific parameter-mechanism in these functions:
STDMETHODIMP RawComponent::get_ComponentParameter(BSTR name, VARIANT *pVal) {
	return S_OK;
}

STDMETHODIMP RawComponent::put_ComponentParameter(BSTR name, VARIANT newVal) {
	return S_OK;
}


#ifdef GME_ADDON

// these two functions are the main
STDMETHODIMP RawComponent::GlobalEvent(globalevent_enum event) {
	if(event == GLOBALEVENT_UNDO) {
		AfxMessageBox(_T("Undo"));
	}
	return S_OK;
}

STDMETHODIMP RawComponent::ObjectEvent(IMgaObject * obj, unsigned long eventmask, VARIANT v) {
	if(eventmask & OBJEVENT_CREATED) {
		AfxMessageBox(_T("Object created: ObjID=") + obj->ID);
	}
	return S_OK;
}

#endif
