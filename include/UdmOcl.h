//###############################################################################################################################################
//
//	Object Constraint Language Implementation For UDM
//	UDMOCL.h
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

//###############################################################################################################################################

#ifndef UDMOCL_h
#define UDMOCL_h
  

#include "UdmBase.h"
#include "Uml.h"


namespace Ocl
{
	using namespace std;

	//class UDM_DLL Evaluator;

//##############################################################################################################################################
//
// ENUMERATIONS, STRUCTS
//
//##############################################################################################################################################

	//  ENUMERATION which is returned by the Evaluator
	//  		CER_FALSE : Constraint is violated, evaluation result is false.
	//  		CER_TRUE : Constraint is satisfied, evaluation result is true.
	//  		CER_UNDEFINED : Constraint is invalid, during the evaluation process exception was thrown, or real undefined was returned.


	enum EEvaluationResult{
		CER_FALSE = 0,
		CER_TRUE = 1,
		CER_UNDEFINED = -1
	};

	//  ENUMERATION which is used to specify the kind of the error's (syntax, semantic errors, run-time exceptions) notification
	//  		ENT_NONE : Exception is NOT thrown
	//  		ENT_FIRST : Exception is thrown at the FIRST error
	//  		ENT_ALL : Exception is thrown after all errors are accumulated

	enum EThrowException {
		ENT_NONE = 0,
		ENT_FIRST,
		ENT_ALL
	};

	//  STRUCT which is used to set the error's notification kind
	//		bStdOutEnabled : FALSE : Errors are reported on the standard output
	//		eExceptionKind : See EThrowException

	struct SErrorNotification {
		bool 				bStdOutEnabled;
		EThrowException 	eExceptionKind;

		SErrorNotification()
			: bStdOutEnabled( true ), eExceptionKind( ENT_ALL )
		{
		}

		SErrorNotification( bool bSOE, EThrowException eTE )
			: bStdOutEnabled( bSOE ), eExceptionKind( eTE )
		{
		}
	};

	//  STRUCT which is used to alter the process of the evaluation
	// 		bLogicalShortCircuit : TRUE : Logical operators return immediely after the result is available
	// 		bIteratorShortCircuit : TRUE : Iterators return immediely after the result is available
	//		bTrackingEnabled : FALSE : The evaluation process cannot be followed in any way
	//		sErrorNotification : See SErrorNotification
	//		iContainmentDepth : -1 : Recursive evaluation for all object in a sub-tree defined by the context
	//		bSkipInvalids : FALSE : Unparsed and Failed constraints are not skipped during evaluation

	struct SEvaluationOptions {
		bool 				bLogicalShortCircuit;
		bool 				bIteratorShortCircuit;
		bool 				bTrackingEnabled;
		SErrorNotification	sErrorNotification;
		int					iContainmentDepth;
		bool 				bSkipInvalids;

		SEvaluationOptions()
			: bLogicalShortCircuit( true ), bIteratorShortCircuit( true ), bTrackingEnabled( true ), iContainmentDepth( -1 ), bSkipInvalids( true ), sErrorNotification( SErrorNotification() )
		{
		}

		/*SEvaluationOptions( bool bLSC, bool, bISC, bool bTE, const SErrorNotification& sEN, int iCD, bool bSI )
			: bLogicalShortCircuit( bLSC ), bIteratorShortCircuit( bISC), bTrackingEnabled( bTE ), iContainmentDepth( iCD ), sErrorNotification( sEN ), bSkipInvalids( bSI )
		{
		}*/
	};

//##############################################################################################################################################
//
// CLASSES
//
//##############################################################################################################################################

	// CLASS which is the key for constraint evaluation

	class UDM_DLL Evaluator
	{
		// INSTANCE MEMBERS
		private :
			Udm::Object m_objObject;
			set< ::Uml::Constraint> m_setConstraints;

		// CONSTRUCTION, DESTRUCTION, INITIALIZATION
		public :
			Evaluator( const Udm::Object& objObject, const set< ::Uml::Constraint>& setConstraints = set< ::Uml::Constraint>() );
			Evaluator( const Evaluator& evaluator );
			Evaluator& operator=( const Evaluator& evaluator );

		// GET'ERS, SET'ERS
		public :
			Udm::Object& object() const;
			set< ::Uml::Constraint>& constraints() const;

		// INSTANCE METHODS
		public :
			EEvaluationResult Check( const SEvaluationOptions& sOptions = SEvaluationOptions() ) const;
	};

//##############################################################################################################################################
//
// FUNCTIONS
//
//##############################################################################################################################################

	// FUNCTION which is used to compile the constraints defined by a MetaDataNetwork.
	// 	If the Meta was already initialized regarding Constraints, then it will be updated.
	//	This mechanism allows us to alter the meta, even the constraints run-time
	//	Attention: In case of modification, this Function has to be called in order to avoid Evaluation failures
	//		objDiagram : the Diagram as MetaDataNetwork's root object defining the constraints
	//		sErrorNotification : See SErrorNotification



	UDM_DLL bool Initialize( const ::Uml::Diagram& objDiagram, const SErrorNotification& sErrorNotification = SErrorNotification() );
	UDM_DLL void UnInitialize();
	UDM_DLL void UnInitialize( const ::Uml::Diagram& objDiagram );
	UDM_DLL bool ProcessPat( const ::Uml::Diagram& metaDiagram, const Udm::Object& objContext, const std::string& strExpression);

}; // namespace Ocl

namespace UdmPat {
	UDM_DLL bool ProcessPat( const ::Uml::Diagram& metaDiagram, const Udm::Object& dataNetwork , const std::string& strExpression);
}; //namespace UdmPat

namespace UdmOcl {
	std::string LowerFirst( const std::string& strValue );
	std::string GetQualifiedName(const ::Uml::Class &c, bool isAssocRole = false);
}; //namespace UdmOcl

#endif // UDMOCL_h
