//###############################################################################################################################################
//
//	Object Constraint Language Expression Checker For UML
//	RawComponent.cpp
//
//===============================================================================================================================================
//
//	Copyright (c) Vanderbilt University, 2009
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

///////////////////////////////////////////////////////////////////////////
// RawComponent.cpp, the main RAW COM component implementation file
// This is the file (along with its header RawComponent.h)
// that the component implementor is expected to modify in the first place
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <ComHelp.h>
#include <GMECOM.h>
#include <Console.h>
#include "ComponentConfig.h"
#include "RawComponent.h"

#include "GMESyntacticSemanticDialog.h"

// this method is called after all the generic initialization is done
// this should be empty, unless application-specific initialization is needed
STDMETHODIMP RawComponent::Initialize(struct IMgaProject *p) {

	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

	// initialization will also set m_Facade.m_bEnabled and
	// m_Facade.m_bEnabledInteractions to true, although those members are not used
	m_Facade.Initialize( p );

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
		AfxMessageBox("This component does not support the obsolete invoke mechanism");
	}
	return E_MGA_NOT_SUPPORTED;
#endif
}


// This is the main component method for interpereters and plugins. 
// May als be used in case of invokeable addons
STDMETHODIMP RawComponent::InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  
									IMgaFCOs *selectedobjs,  long param) {
	return S_OK;
}

// GME currently does not use this function
// you only need to implement it if other invokation mechanisms are used
STDMETHODIMP RawComponent::ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param) {
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

	if ( ! m_Facade.m_bEnabled )
		return S_OK;

	COMTRY {

		CWaitCursor crsrWait;

		OCLUML::ConstraintVector vecConstraints;
		m_Facade.CheckAllConstraints( vecConstraints );

		crsrWait.Restore();

		if ( ! vecConstraints.empty() ) {
			CSyntacticSemanticDialog dlgErrors( AfxGetApp()->m_pActiveWnd );
			for ( int i = 0 ; i < vecConstraints.size() ; i++ )
				dlgErrors.AddItem( vecConstraints[ i ] );
			dlgErrors.DoModal();
		}
		m_Facade.Finalize();

	} COMCATCH( ASSERT( 0 ); )
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
	return S_OK; 
}

STDMETHODIMP RawComponent::ObjectEvent(IMgaObject * obj, unsigned long eventmask, VARIANT v) {
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

	if ( ! m_Facade.m_bEnabled )
		return S_OK;

	CWnd* pMainWindow = AfxGetApp()->m_pActiveWnd;

	COMTRY {

		if( ( eventmask & OBJEVENT_ATTR ) || ( eventmask & OBJEVENT_CONNECTED ) ) {

			CComQIPtr<IMgaFCO> spFCO = obj;
			if( spFCO ) {
				std::string strKind = GME::GetObjectKind( spFCO.p );
				if ( strKind == "Constraint" ) {
					CWaitCursor crsrWait;
					OCLUML::ConstraintVector vecConstraints;
					m_Facade.CheckConstraint( spFCO.p, vecConstraints, false );
					if ( ! vecConstraints.empty() ) {
						CSyntacticSemanticDialog dlgErrors( pMainWindow );
						for ( int i = 0 ; i < vecConstraints.size() ; i++ )
							dlgErrors.AddItem( vecConstraints[ i ] );
						crsrWait.Restore();
						dlgErrors.DoModal();
					}
					else
						crsrWait.Restore();
				}
				if ( strKind == "ConstraintDefinition" ) {
					CWaitCursor crsrWait;
					OCLUML::ConstraintDefVector vecConstraintDefinitions;
					m_Facade.CheckConstraintDefinition( spFCO.p, vecConstraintDefinitions, false );
					if ( ! vecConstraintDefinitions.empty() ) {
						CSyntacticSemanticDialog dlgErrors( pMainWindow );
						for ( int i = 0 ; i < vecConstraintDefinitions.size() ; i++ )
							dlgErrors.AddItem( vecConstraintDefinitions[ i ] );
						crsrWait.Restore();
						dlgErrors.DoModal();
					}
					else
						crsrWait.Restore();
				}
				m_Facade.Finalize();
			}
		}

	} COMCATCH( ASSERT( 0 ); )
}

#endif
