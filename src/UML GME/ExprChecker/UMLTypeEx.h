//###############################################################################################################################################
//
//	Object Constraint Language Expression Checker For UML
//	UMLTypeEx.h
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

#ifndef UMLTypeEx_h
#define UMLTypeEx_h

#include "GMECommon.h"
#include "OCLTypeExBasic.h"

namespace OCLUML
{

//##############################################################################################################################################
//
//	G L O B A L   F A C T O R I E S
//
//##############################################################################################################################################

	class TypeFactory
		: public OclBasic::TypeFactory
	{
		private :
		 	CComPtr<IMgaProject>	m_spProject;

		 public :
		 					TypeFactory( CComPtr<IMgaProject> spProject );

			virtual  void 	GetTypes( const std::string& strName, std::vector<OclMeta::Type*>& vecTypes );
		private :
					void 	GetDynamicTypes( const std::string& strName, std::vector<OclMeta::Type*>& vecTypes );
					void 	GetPredefinedTypes( const std::string& strName, std::vector<OclMeta::Type*>& vecTypes );
	};

}; // namespace OCLUML

#endif // UMLTypeEx_h