//###############################################################################################################################################
//
//	Object Constraint Language Implementation For UDM
//	UDMOCLType.h
//
//###############################################################################################################################################

/*
	Copyright (c) Vanderbilt University, 2000-2001
	ALL RIGHTS RESERVED

	Vanderbilt University disclaims all warranties with regard to this
	software, including all implied warranties of merchantability
	and fitness.  In no event shall Vanderbilt University be liable for
	any special, indirect or consequential damages or any damages
	whatsoever resulting from loss of use, data or profits, whether
	in an action of contract, negligence or other tortious action,
	arising out of or in connection with the use or performance of
	this software.
*/

#ifndef UDMOCLType_h
#define UDMOCLType_h

#include "OCLTypeExBasic.h"

namespace Uml
{
	class Diagram;
	class Class;
};

namespace UmlOcl
{

//##############################################################################################################################################
//
//	G L O B A L   F A C T O R I E S
//
//==============================================================================================================================================
//
//	D E S C R I P T I O N :
//
//##############################################################################################################################################

	class TypeFactory
		: public OclBasic::TypeFactory
	{
		 public :
		 	const ::Uml::Diagram& m_diagram;

		 public :
		 					TypeFactory( const ::Uml::Diagram& diagram );

			virtual  void 	GetTypes( const std::string& strName, std::vector<OclMeta::Type*>& vecTypes );
		private :
					void 	GetDynamicTypes( const std::string& strName, std::vector<OclMeta::Type*>& vecTypes );
					void 	GetDynamicTypes( const std::vector< ::Uml::Class>& vecClasses, const std::string& strClassName, std::vector<OclMeta::Type*>& vecTypes );
					void 	GetPredefinedTypes( const std::string& strName, std::vector<OclMeta::Type*>& vecTypes );
	};

}; // namespace UmlOcl

#endif // UDMOCLType_h
