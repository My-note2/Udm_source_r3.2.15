//###############################################################################################################################################
//
//	Object Constraint Language Expression Checker For UML
//	UMLFacade.h
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

#ifndef UMLFacade_h
#define UMLFacade_h

#include "GMECommon.h"
#include "OCLTree.h"
#include "UMLConstraint.h"
#include "UMLTypeEx.h"

namespace OCLUML
{

// Declarations

//##############################################################################################################################################
//
//	C L A S S : OCLUML::Facade
//
//==============================================================================================================================================
//
//	D E S C R I P T I O N :
//
//##############################################################################################################################################

	class Facade
	{
		private :
			CComPtr<IMgaProject>		m_spProject;
			OclTree::TreeManager*		m_pTreeManager;

		public :
			bool									m_bEnabled;
			bool									m_bEnabledEvents;
			bool									m_bEnabledInteractions;

		public :
												Facade();
												~Facade();
					void						Initialize( CComPtr<IMgaProject> spProject );
					void 						Finalize();

					CComPtr<IMgaProject>	GetProject() const;
					OclTree::TreeManager*	GetTreeManager() const;

					void 						CheckConstraint( CComPtr<IMgaFCO> spConstraint, ConstraintVector& vecFaileds, bool bFieldsMandatory );
					void 						CheckAllConstraints( ConstraintVector& vecFaileds );
					void 						CheckConstraintDefinition( CComPtr<IMgaFCO> spConstraintDef, ConstraintDefVector& vecFaileds, bool bFieldsMandatory );
					void 						CheckAllConstraintDefinitions( ConstraintDefVector& vecFaileds );

		private :
					bool 						IsConstraintDefined( CComPtr<IMgaFCO> spConstraint );
					bool						IsConstraintDefinitionDefined( CComPtr<IMgaFCO> spConstraintDef );
	};

}; // namespace OCLUML

#endif // UMLFacade_h