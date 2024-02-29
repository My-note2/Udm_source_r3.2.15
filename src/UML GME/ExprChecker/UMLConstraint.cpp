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

#include "UMLConstraint.h"

namespace OCLUML
{
	typedef Ocl::Constraint OclConstraint;

//##############################################################################################################################################
//
//	C L A S S : OCLUML::Constraint <<< + Ocl::Constraint, OclCommon::ReferenceCountable< Constraint >
//
//##############################################################################################################################################

	Constraint::Constraint( const std::string& strContextType, CComPtr<IMgaFCO> spFCO )
	{
		std::string strName = GME::GetObjectName( spFCO.p );
		Trim( strName );

		m_strExpression = GME::GetStringAttribute( spFCO, "ConstraintEqn" );
		Trim( m_strExpression );

		Define( strContextType + "::" + strName, "context " + strContextType + " inv " + strName + " :\r\n\r\n" + m_strExpression, true );
	}

	Constraint::~Constraint()
	{
	}

	std::string Constraint::GetExpression() const
	{
		return m_strExpression;
	}

//##############################################################################################################################################
//
//	C L A S S : OCLUML::ConstraintDef <<< + Ocl::Constraint, OclCommon::ReferenceCountable< ConstraintDef >
//
//##############################################################################################################################################

	ConstraintDef::ConstraintDef( const std::string& strContextType, CComPtr<IMgaFCO> spFCO )
	{
		std::string strName = GME::GetObjectName( spFCO.p );
		Trim( strName );

		m_strExpression = GME::GetStringAttribute( spFCO, "DefinitionEqn" );
		Trim( m_strExpression );

		m_strParameterList = GME::GetStringAttribute( spFCO, "DefinitionParamList" );
		Trim( m_strParameterList );

		m_strStereotype = GME::GetStringAttribute( spFCO, "DefinitionStereo" );

		std::string strReturnType = GME::GetStringAttribute( spFCO, "DefinitionRetType" );
		Trim( strReturnType );

		Define( strContextType + "::" + strName, "context " + strContextType + "::" + strName + "( " + m_strParameterList + " ) : " + strReturnType + " def" + m_strStereotype + " " + strName + " :\r\n\r\n" + m_strExpression );
	}

	ConstraintDef::~ConstraintDef()
	{
	}

	std::string ConstraintDef::GetExpression() const
	{
		return m_strExpression;
	}

	std::string ConstraintDef::GetParameterList() const
	{
		return m_strParameterList;
	}

	std::string ConstraintDef::GetStereotype() const
	{
		return m_strStereotype;
	}

}; // namespace OCLUML