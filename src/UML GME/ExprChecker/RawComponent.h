//###############################################################################################################################################
//

#include "Mga.h"
//	Object Constraint Language Expression Checker For UML
//	RawComponent.h
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

#ifndef RAWCOMPONENT_H
#define RAWCOMPONENT_H


// Declaration of the main RAW COM component interface class


#ifdef BUILDER_OBJECT_NETWORK
#error   This file should only be included in the RAW COM configurations
#endif

#include "UMLFacade.h"


class RawComponent {
////////////////////
// Insert your application specific member and method definitions here
	OCLUML::Facade	m_Facade;
public:
	RawComponent() { ; }
private:
	
	
// Try not to modify the code below this line
////////////////////
public:	
#ifdef GME_ADDON
	CComPtr<IMgaProject> project;  // this is set before Initialize() is called
	CComPtr<IMgaAddOn> addon;      // this is set before Initialize() is called
#endif
	bool interactive;
	
	STDMETHODIMP Initialize(struct IMgaProject *);
	STDMETHODIMP Invoke(IMgaProject* gme, IMgaFCOs *models, long param);
	STDMETHODIMP InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  IMgaFCOs *selectedobjs,  long param);
	STDMETHODIMP ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param);
	STDMETHODIMP get_ComponentParameter(BSTR name, VARIANT *pVal);
	STDMETHODIMP put_ComponentParameter(BSTR name, VARIANT newVal);

#ifdef GME_ADDON
	STDMETHODIMP GlobalEvent(globalevent_enum event);
	STDMETHODIMP ObjectEvent(IMgaObject * obj, unsigned long eventmask, VARIANT v);
#endif
};


#endif //RAWCOMPONENT_H