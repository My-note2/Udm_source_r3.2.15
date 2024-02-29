//###############################################################################################################################################
//
//	Object Constraint Language Implementation For UDM
//	UDMOCLObject.h
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

#ifndef UDMOCLObject_h
#define UDMOCLObject_h

#include "OCLObject.h"
#include "UdmBase.h"

#define CREATE_UDMDATANETWORK( pManager, object )	\
	OclMeta::Object( new UmlOcl::DataNetwork( pManager, object ) )

#define CREATE_UDMOBJECT( pManager, object ) 	\
	OclMeta::Object( new UmlOcl::Object( pManager, object ) )

#define DECL_UDMOBJECT( varName, var2Name )		\
	Udm::Object varName;					\
	if ( ! var2Name.IsUndefined() )						\
		( ( UmlOcl::Object* ) var2Name.GetImplementation() )->GetValue( varName )

#define DECL_UDMDATANETWORK( varName, var2Name ) 	\
	Udm::DataNetwork* varName = NULL;					\
	if ( ! var2Name.IsUndefined() )						\
		( ( UmlOcl::DataNetwork* ) var2Name.GetImplementation() )->GetValue( varName )

namespace UmlOcl
{
	typedef OclImplementation::Object Any;

//##############################################################################################################################################
//
//	C L A S S : UmlOcl::Object <<< + OclImplementation::Object
//
//==============================================================================================================================================
//
//	D E S C R I P T I O N :
//
//##############################################################################################################################################

	class Object
		: public Any
	{
		private :
			Udm::Object	m_Value;

		public :
							Object( OclMeta::TypeManager* pManager, Udm::Object value );
		private :
							void SetTypeName();
		public :
			virtual 	Any* 	Clone() const;

					void	GetValue( Udm::Object& value ) const;
					void 	SetValue( const Udm::Object& value );

			virtual 	bool 	Equals( const Any& object ) const;
					bool 	operator==( const Object& object ) const;
					bool 	operator!=( const Object& object ) const;

			virtual std::string 	Print() const;
	};

//##############################################################################################################################################
//
//	C L A S S : UmlOcl::DataNetwork <<< + OclImplementation::Object
//
//==============================================================================================================================================
//
//	D E S C R I P T I O N :
//
//##############################################################################################################################################

	class DataNetwork
		: public Any
	{
		private :
			Udm::DataNetwork*	m_Value;

		public :
							DataNetwork( OclMeta::TypeManager* pManager, Udm::DataNetwork* value );
			virtual 	Any* 	Clone() const;

					void	GetValue( Udm::DataNetwork*& value ) const;
					void	SetValue( Udm::DataNetwork* value );

			virtual 	bool 	Equals( const Any& object ) const;
					bool 	operator==( const DataNetwork& object ) const;
					bool 	operator!=( const DataNetwork& object ) const;

			virtual std::string 	Print() const;
	};

}; // namespace UmlOcl

#endif // UDMOCLObject_h
