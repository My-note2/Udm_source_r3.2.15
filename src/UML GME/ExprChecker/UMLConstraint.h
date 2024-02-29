//###############################################################################################################################################
//
//	Object Constraint Language Expression Checker For UML
//	UMLConstraint.h
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

#ifndef UMLConstraint_h
#define UMLConstraint_h

#include "GMECommon.h"
#include "OCLConstraint.h"
#include "OCLRefCount.h"

// Predeclarations

namespace OCLUML
{
	class Constraint;

	typedef OclCommon::RCSmart<Constraint> 	SpConstraint;
	typedef std::vector<SpConstraint>				ConstraintVector;

	class ConstraintDef;

	typedef OclCommon::RCSmart<ConstraintDef> 	SpConstraintDef;
	typedef std::vector<SpConstraintDef>				ConstraintDefVector;


// Declarations

//##############################################################################################################################################
//
//	C L A S S : OCLUML::Constraint <<< + Ocl::Constraint, OclCommon::ReferenceCountable< Constraint >
//
//==============================================================================================================================================
//
//	D E S C R I P T I O N :
//
//##############################################################################################################################################

	class Constraint
		: public Ocl::Constraint, public OclCommon::ReferenceCountable< Constraint >
	{
		private :
			std::string	m_strExpression;

		public :
										Constraint( const std::string& strContextType, CComPtr<IMgaFCO> spFCO );
				virtual					~Constraint();
						std::string			GetExpression() const;
	};

//##############################################################################################################################################
//
//	C L A S S : OCLUML::ConstraintDef <<< + Ocl::Constraint, OclCommon::ReferenceCountable< ConstraintDef >
//
//==============================================================================================================================================
//
//	D E S C R I P T I O N :
//
//##############################################################################################################################################

	class ConstraintDef
		: public Ocl::Constraint, public OclCommon::ReferenceCountable< ConstraintDef >
	{
		private :
			std::string	m_strExpression;
			std::string	m_strParameterList;
			std::string	m_strStereotype;

		public :
										ConstraintDef( const std::string& strContextType, CComPtr<IMgaFCO> spFCO );
				virtual					~ConstraintDef();
						std::string			GetExpression() const;
						std::string 			GetParameterList() const;
						std::string			GetStereotype() const;
	};


}; // namespace OCLUML

#endif // UMLConstraint_h