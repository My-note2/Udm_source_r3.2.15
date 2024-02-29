//###############################################################################################################################################
//
//	Object Constraint Language Implementation For UDM
//	UDMOCLType.cpp
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

#include "UdmOclType.h"
#include "OCLObjectExBasic.h"
#include "UdmOclObject.h"





#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#include "Uml.h"
#include "UmlExt.h"
#include "UdmOcl.h"

using namespace std;

namespace UmlOcl
{

//##############################################################################################################################################
//
//	T Y P E   O F   uml::DataNetwork
//
//==============================================================================================================================================
//
//	D E S C R I P T I O N :
//
//##############################################################################################################################################

	class TDataNetworkDerived_Classes
		: public OclImplementation::Association
	{
		private :
			const ::Uml::Class m_class;

		public :
			TDataNetworkDerived_Classes( const ::Uml::Class& inClass )
				: m_class( inClass )
			{
			}

			void operator()()
			{
				DECL_UDMDATANETWORK( objThis, GetThis() );
				if ( ! objThis )
					ThrowException( "Object is null." );
				OclMeta::ObjectVector vecOut;
				Collect( vecOut, objThis->GetRootObject() );
				SetResult( CREATE_SET( GetTypeManager(), vecOut ) );
			}

			void Collect( OclMeta::ObjectVector& vecResult, Udm::Object object )
			{
				if ( ::Uml::IsDerivedFrom(object.type(),m_class) )
					vecResult.push_back( CREATE_UDMOBJECT( GetTypeManager(), object ) );
				set<Udm::Object> setChildren = object.GetChildObjects();
				for ( set<Udm::Object>::iterator it = setChildren.begin() ; it != setChildren.end() ; it++ )
					Collect( vecResult, *it );
			}
	};

	class TDataNetwork_AssociationFactory
		: public OclImplementation::AssociationFactory
	{
		private :
			const ::Uml::Diagram m_diagram;

			void GetFeatures( const std::vector< ::Uml::Class>& vecClasses, const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
			{
				for ( int i = 0 ; i < vecClasses.size() ; i++ ) 
				{
					std::string strClass = vecClasses[ i ].name();
					std::string strRole = ::UdmOcl::LowerFirst( strClass );
					std::string strRole_Q = ::UdmOcl::GetQualifiedName( vecClasses[ i ], true );
					std::string strSig = signature.GetName();
					bool bIsQualified = strSig.find( "::" ) != std::string::npos; // true if qualified type, false if local type
						
					if ((bIsQualified && ( strRole_Q == signature.GetName())) || ( (!bIsQualified) && (strRole == signature.GetName()) ) )
					{
						TypeSeq vecType( 1, "ocl::Set" );
						vecType.push_back( ::UdmOcl::GetQualifiedName( vecClasses[ i ] ) );
						vecFeatures.push_back( new OclMeta::Association( strRole, "", vecType, new TDataNetworkDerived_Classes( vecClasses[ i ] ), true ) );
					}
				}
			}

		 public :
			 TDataNetwork_AssociationFactory( const ::Uml::Diagram& diagram )
		 		: m_diagram( diagram )
		 	{
		 	}

			virtual void GetFeatures( const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
			{
				if ( ! signature.GetAcceptableTypeName().empty() )
					return;

				GetFeatures( m_diagram.classes(), signature, vecFeatures );

				::Uml::DiagramNamespaces nses(m_diagram);
				for( ::Uml::DiagramNamespaces::iterator i = nses.begin(); i != nses.end(); i++)
					GetFeatures( i->classes(), signature, vecFeatures );
			}
	};

//##############################################################################################################################################
//
//	D E R I V E D   O F   udm::Object
//
//==============================================================================================================================================
//
//	D E S C R I P T I O N :
//
//##############################################################################################################################################

	class TObjectDerived_Parent
		: public OclImplementation::Association
	{
		private :
			::Uml::CompositionParentRole m_parentRole;
			const ::Uml::Class m_class;

		public :
			TObjectDerived_Parent( const ::Uml::CompositionParentRole& parentRole, const ::Uml::Class& inClass )
				: m_class( inClass ), m_parentRole( parentRole )
			{
			}

			void operator()()
			{
				DECL_UDMOBJECT( objThis, GetThis() );
				if ( ! objThis )
					ThrowException( "Object is null." );
				if ( m_parentRole )
					SetResult( CREATE_UDMOBJECT( GetTypeManager(), Udm::Object ( objThis.__impl()->getParent( m_parentRole ) ) ) ); // Access by Role
				else {
					Udm::Object objParent = objThis.GetParent();	// Access by Class
					if ( ! objParent || ! ::Uml::IsDerivedFrom( objParent.type(), m_class ) )
						objParent = Udm::Object( NULL );
					SetResult( CREATE_UDMOBJECT( GetTypeManager(), objParent ) );
				}
			}
	};

	class TObjectDerived_Children
		: public OclImplementation::Association
	{
		private :
			Udm::Object::CompositionInfo m_info;
			const ::Uml::Class m_class;
			bool m_bCompound;

		public :
			TObjectDerived_Children( const Udm::Object::CompositionInfo& info, const ::Uml::Class& inClass, bool bCompound )
				: m_class( inClass ), m_info( info ), m_bCompound( bCompound )
			{
			}

			void operator()()
			{
				DECL_UDMOBJECT( objThis, GetThis() );
				if ( ! objThis )
					ThrowException( "Object is null." );
				set<Udm::Object> setOut;
				if ( m_info.strParentRoleName.empty() )  								// Access by Class
					setOut = objThis.GetChildObjects( m_class );
				else																		// Access by Role
					setOut = objThis.GetChildObjects( m_info, m_class );
				OclMeta::ObjectVector vecOut;
				for ( set<Udm::Object>::iterator it = setOut.begin() ; it != setOut.end() ; it++ )
					vecOut.push_back( CREATE_UDMOBJECT( GetTypeManager(), *it ) );
				if ( m_bCompound )
					SetResult( CREATE_SET( GetTypeManager(), vecOut ) );
				else
					SetResult( ( vecOut.empty() ) ? CREATE_UDMOBJECT( GetTypeManager(), Udm::Object( NULL ) ) : vecOut[ 0 ] );

			}
	};

	class TObjectDerived_Peers
		: public OclImplementation::Association
	{
		private :
			const ::Uml::AssociationRole m_assocRole;
			const ::Uml::Class m_class;
			bool m_bCompound;

		public :
			TObjectDerived_Peers( const ::Uml::AssociationRole& inAssocRole, const ::Uml::Class& inClass, bool bCompound )
				: m_class( inClass  ), m_assocRole( inAssocRole ), m_bCompound( bCompound )
			{
			}

			void operator()()
			{
				DECL_UDMOBJECT( objThis, GetThis() );
				if ( ! objThis )
					ThrowException( "Object is null." );
				multiset<Udm::Object> setOut;
				if ( ! m_class ) {
					std::vector<Udm::ObjectImpl*> vecImpls = objThis.__impl()->getAssociation( m_assocRole, Udm::TARGETFROMPEER );
					for ( int i = 0 ; i < vecImpls.size() ; i++ )
						setOut.insert( Udm::Object( vecImpls[ i ] ) );
				}
				else
					setOut = objThis.GetAdjacentObjects( m_class );
				OclMeta::ObjectVector vecOut;
				for ( multiset<Udm::Object>::iterator it = setOut.begin() ; it != setOut.end() ; it++ )
					vecOut.push_back( CREATE_UDMOBJECT( GetTypeManager(), *it ) );
				if ( m_bCompound )
					SetResult( CREATE_SET( GetTypeManager(), vecOut ) );
				else
					SetResult( ( vecOut.empty() ) ? CREATE_UDMOBJECT( GetTypeManager(), Udm::Object( NULL ) ) : vecOut[ 0 ] );
			}
	};

	class TObjectDerived_AssocClasses
		: public OclImplementation::Association
	{
		private :
			::Uml::AssociationRole m_assocRole;
			bool m_bCompound;

		public :
			TObjectDerived_AssocClasses( const ::Uml::AssociationRole& assocRole, bool bCompound )
				: m_assocRole( assocRole ), m_bCompound( bCompound )
			{
			}

			void operator()()
			{
				DECL_UDMOBJECT( objThis, GetThis() );
				if ( ! objThis )
					ThrowException( "Object is null." );
				std::vector<Udm::ObjectImpl*> vecImpls = objThis.__impl()->getAssociation( m_assocRole, Udm::CLASSFROMTARGET );
				OclMeta::ObjectVector vecOut;
				for ( int i = 0 ; i < vecImpls.size() ; i++ )
					vecOut.push_back( CREATE_UDMOBJECT( GetTypeManager(), Udm::Object( vecImpls[ i ] ) ) );
				if ( m_bCompound )
					SetResult( CREATE_SET( GetTypeManager(), vecOut ) );
				else
					SetResult( ( vecOut.empty() ) ? CREATE_UDMOBJECT( GetTypeManager(), Udm::Object( NULL ) ) : vecOut[ 0 ] );
			}
	};

	class TObjectDerived_Targets
		: public OclImplementation::Association
	{
		private :
			::Uml::AssociationRole m_assocRole;

		public :
			TObjectDerived_Targets( const ::Uml::AssociationRole& assocRole )
				: m_assocRole( assocRole )
			{
			}

			void operator()()
			{
				DECL_UDMOBJECT( objThis, GetThis() );
				if ( ! objThis )
					ThrowException( "Object is null." );
				std::vector<Udm::ObjectImpl*> vecImpls = objThis.__impl()->getAssociation( m_assocRole, Udm::TARGETFROMCLASS );
				SetResult( CREATE_UDMOBJECT( GetTypeManager(), Udm::Object( vecImpls[ 0 ] ) ) );
			}
	};

	class TObjectDerived_AssociationFactory
		: public OclImplementation::AssociationFactory
	{
		private :
		 	const ::Uml::Class m_class;

		 public :
		 	TObjectDerived_AssociationFactory( const ::Uml::Class& inclass )
		 		: m_class( inclass )
		 	{
		 	}

		 	virtual void GetFeatures( const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
		 	{
		 		GetParents( signature, vecFeatures );
		 		GetChildren( signature, vecFeatures );
		 		GetAssociationEnds( signature, vecFeatures );
		 		GetAssociationClasses( signature, vecFeatures );
		 		GetAssociationEndsFromClasses( signature, vecFeatures );
		 	}

		 private :
		 	void GetParents( const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
		 	{
		 		if ( ! signature.GetAcceptableTypeName().empty() )
					return;

				std::string strName = signature.GetName();
				set< ::Uml::Class> setAncestors = ::Uml::AncestorClasses( m_class );
				bool bClassAccessFound = false;
				for ( set< ::Uml::Class>::iterator itClass =  setAncestors.begin() ; itClass != setAncestors.end() ; itClass++ ) {
					set< ::Uml::CompositionChildRole> setChildRoles = (*itClass).childRoles();
					for ( set< ::Uml::CompositionChildRole>::iterator itChild = setChildRoles.begin() ; itChild != setChildRoles.end() ; itChild++ ) {
						::Uml::CompositionParentRole parentRole = ( (::Uml::Composition)(*itChild).parent() ).parentRole();
			 			std::string strRole = parentRole.name();
			 			::Uml::Class parent = parentRole.target();
			 			std::string strClass = parent.name();
			 			TypeSeq vecType( 1, ::UdmOcl::GetQualifiedName( parent ) );
			 			if ( ! strRole.empty() && strRole == strName )
			 				vecFeatures.push_back( new OclMeta::Association( strName, "", vecType, new TObjectDerived_Parent( parentRole, parent ), true ) );
			 			else if ( strName == ::UdmOcl::LowerFirst( strClass ) && ! bClassAccessFound && *itClass == m_class ) {
		 					bClassAccessFound = true;
							vecFeatures.push_back( new OclMeta::Association( strName, "", vecType, new TObjectDerived_Parent( ::Uml::CompositionParentRole( NULL ), parent ), true ) );
		 				}
			 		}
			 	}
		 	}

		 	void GetChildren( const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
		 	{
		 		if ( ! signature.GetAcceptableTypeName().empty() )
					return;

				std::string strName = signature.GetName();
				set< ::Uml::CompositionParentRole> setParentRoles = m_class.parentRoles();
				for ( set< ::Uml::CompositionParentRole>::iterator itParent = setParentRoles.begin() ; itParent != setParentRoles.end() ; itParent++ ) {
					::Uml::Composition composition = (*itParent).parent();
					::Uml::CompositionChildRole childRole = composition.childRole();
		 			std::string strRole = childRole.name();
		 			::Uml::Class child = childRole.target();
		 			std::string strClass = child.name();

					if ( (! strRole.empty() && strRole == strName) || (strName == ::UdmOcl::LowerFirst( strClass )) ) {
		 				TypeSeq vecType;
		 				if ( childRole.max() > 1 || childRole.max() == -1 )
		 					vecType.push_back( "ocl::Set" );
		 				vecType.push_back( ::UdmOcl::GetQualifiedName( child ) );
		 				Udm::Object::CompositionInfo info;
		 				if ( ! strRole.empty() && strRole == strName ) {
		 					info.strParentRoleName = (*itParent).name();
		 					info.strChildRoleName = strRole;
		 				}
		 				vecFeatures.push_back( new OclMeta::Association( strName, "", vecType, new TObjectDerived_Children( info, child, vecType.size() > 1 ), true ) );
		 			}
		 		}
		 	}

		 	void GetAssociationEnds( const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
		 	{
		 		if ( ! signature.GetAcceptableTypeName().empty() )
					return;

				std::string strName = signature.GetName();
				set< ::Uml::AssociationRole> setFromRoles = m_class.associationRoles();
				for ( set< ::Uml::AssociationRole>::iterator itFrom = setFromRoles.begin() ; itFrom != setFromRoles.end() ; itFrom++ ) {
					::Uml::Association association = (*itFrom).parent();
					set< ::Uml::AssociationRole> setToRoles = association.roles();
					for ( set< ::Uml::AssociationRole>::iterator itTo = setToRoles.begin() ; itTo != setToRoles.end() ; itTo++ ) {
						if ( *itTo != *itFrom ) {
							std::string strRole = (*itTo).name();
							::Uml::Class peer = (*itTo).target();
							std::string strClass = peer.name();
							if ( (! strRole.empty() && strRole == strName) || (strRole.empty() && strName == ::UdmOcl::LowerFirst( strClass )) ) {
								TypeSeq vecType;
		 						if ( (*itTo).max() > 1 ||  (*itTo).max() == -1 )
		 							vecType.push_back( "ocl::Set" );
		 						vecType.push_back( ::UdmOcl::GetQualifiedName( peer ) );
		 						TObjectDerived_Peers* pAssociation = new TObjectDerived_Peers( *itTo,  ( strRole.empty() && strName == ::UdmOcl::LowerFirst( strClass ) ) ? m_class : ::Uml::Class( NULL ), vecType.size() > 1 );
		 						vecFeatures.push_back( new OclMeta::Association( strName, "", vecType, pAssociation , true ) );
		 					}
		 				}
		 			}
		 		}
		 	}

		 	void GetAssociationClasses( const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
		 	{
				std::string strName = signature.GetName();
				std::string strAcceptable = signature.GetAcceptableTypeName();

				set< ::Uml::AssociationRole> setFromRoles = m_class.associationRoles();
				for ( set< ::Uml::AssociationRole>::iterator itFrom = setFromRoles.begin() ; itFrom != setFromRoles.end() ; itFrom++ ) {
					::Uml::Association association = (*itFrom).parent();
					::Uml::Class assocClass = association.assocClass();
					if ( assocClass ) {
						std::string strClass = assocClass.name();
						std::string strRole = (*itFrom).name();
						std::string strClassRole = ::UdmOcl::LowerFirst( strClass );
						if ( (strAcceptable.empty() && strClassRole == strName) || (! strAcceptable.empty() && ! strRole.empty() && strRole == strName && strAcceptable == strClassRole )) {
							set< ::Uml::AssociationRole> setToRoles = association.roles();
							TypeSeq vecType;
							set< ::Uml::AssociationRole>::iterator itTo;
							for (  itTo = setToRoles.begin() ; itTo != setToRoles.end() ; itTo++ ) {
								if ( *itTo != *itFrom ) {
									long lMax = (*itTo).max();
			 						if ( lMax > 1 || lMax == -1 ) {
			 							vecType.push_back( "ocl::Set" );
			 							break;
			 						}
			 					}
		 					}
		 					vecType.push_back( ::UdmOcl::GetQualifiedName( assocClass ) );
		 					vecFeatures.push_back( new OclMeta::Association( strName, strAcceptable, vecType, new TObjectDerived_AssocClasses( *itTo, vecType.size() > 1 ), true ) );
		 				}
		 			}
		 		}
		 	}

		 	void GetAssociationEndsFromClasses( const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
		 	{
		 		if ( ! signature.GetAcceptableTypeName().empty() )
					return;

		 		std::string strName = signature.GetName();
				::Uml::Association association = m_class.association();
		 		if ( association ) {
					set< ::Uml::AssociationRole> setRoles = association.roles();
					for ( set< ::Uml::AssociationRole>::iterator it = setRoles.begin() ; it != setRoles.end() ; it++ ) {
			 			::Uml::Class target = (*it).target();
			 			std::string strClass = target.name();
			 			std::string strRole = (*it).name();
			 			if ( (! strRole.empty() && strRole == strName) || (strRole.empty() && strName == ::UdmOcl::LowerFirst( strClass ) )) {
							TypeSeq vecType( 1, ::UdmOcl::GetQualifiedName( target ) );
			 				vecFeatures.push_back( new OclMeta::Association( strName, "", vecType, new TObjectDerived_Targets( *it ), true ) );
			 			}
			 		}
			 	}
		 	}
	};

	class TObjectDerived_Attribute
		: public OclImplementation::Attribute
	{
		private :
			int 						m_iCompound;
			const ::Uml::Attribute m_attribute;

		public :
			TObjectDerived_Attribute( const ::Uml::Attribute& attribute, int iCompound )
				: m_attribute( attribute ), m_iCompound( iCompound )
			{
			}

			void operator()()
			{
				DECL_UDMOBJECT( objThis, GetThis() );
				if ( ! objThis )
					ThrowException( "Object is null." );
				std::string strType = m_attribute.type();
				if ( strType == "ocl::Boolean" || strType == "Boolean" ) {
					if ( m_iCompound == 0 )
						SetResult( CREATE_BOOLEAN( GetTypeManager(), objThis.getBooleanAttr( m_attribute ) ) );
					else {
						std::vector<bool> vec = objThis.getBooleanAttrArr( m_attribute );
						OclMeta::ObjectVector vecOut;
						for ( int i = 0 ; i< vec.size() ; i++ )
							vecOut.push_back( CREATE_BOOLEAN( GetTypeManager(), vec[ i ] ) );
						if ( m_iCompound == 1 )
							SetResult( CREATE_SET( GetTypeManager(), vecOut ) );
						else
							SetResult( CREATE_SEQUENCE( GetTypeManager(), vecOut ) );
					}
				}
				else if ( strType == "ocl::Integer" || strType == "Integer" ) {
					if ( m_iCompound == 0 )
						SetResult( CREATE_INTEGER( GetTypeManager(), objThis.getIntegerAttr( m_attribute ) ) );
					else {
						std::vector<__int64> vec = objThis.getIntegerAttrArr( m_attribute );
						OclMeta::ObjectVector vecOut;
						for ( int i = 0 ; i< vec.size() ; i++ )
							vecOut.push_back( CREATE_INTEGER( GetTypeManager(), vec[ i ] ) );
						if ( m_iCompound == 1 )
							SetResult( CREATE_SET( GetTypeManager(), vecOut ) );
						else
							SetResult( CREATE_SEQUENCE( GetTypeManager(), vecOut ) );
					}
				}
				else if ( strType == "ocl::Real" || strType == "Real" ) {
					if ( m_iCompound == 0 )
						SetResult( CREATE_REAL( GetTypeManager(), objThis.getRealAttr( m_attribute ) ) );
					else {
						std::vector<double> vec = objThis.getRealAttrArr( m_attribute );
						OclMeta::ObjectVector vecOut;
						for ( int i = 0 ; i< vec.size() ; i++ )
							vecOut.push_back( CREATE_REAL( GetTypeManager(), vec[ i ] ) );
						if ( m_iCompound == 1 )
							SetResult( CREATE_SET( GetTypeManager(), vecOut ) );
						else
							SetResult( CREATE_SEQUENCE( GetTypeManager(), vecOut ) );
					}
				}
				else {
					if ( m_iCompound == 0 )
						SetResult( CREATE_STRING( GetTypeManager(), objThis.getStringAttr( m_attribute ) ) );
					else {
						std::vector<std::string> vec = objThis.getStringAttrArr( m_attribute );
						OclMeta::ObjectVector vecOut;
						for ( int i = 0 ; i< vec.size() ; i++ )
							vecOut.push_back( CREATE_STRING( GetTypeManager(), vec[ i ] ) );
						if ( m_iCompound == 1 )
							SetResult( CREATE_SET( GetTypeManager(), vecOut ) );
						else
							SetResult( CREATE_SEQUENCE( GetTypeManager(), vecOut ) );
					}
				}
			}
	};

	class TObjectDerived_AttributeFactory
		: public OclImplementation::AttributeFactory
	{
		private :
		 	const ::Uml::Class m_class;

		 public :
		 	TObjectDerived_AttributeFactory( const ::Uml::Class&  classin )
		 		: m_class( classin )
		 	{
		 	}

		 public :
			virtual void GetFeatures( const OclSignature::Attribute& signature, OclMeta::AttributeVector& vecFeatures )
			{
				std::string strName = signature.GetName();

				set< ::Uml::Attribute> setAttribs = m_class.attributes();
				for ( set< ::Uml::Attribute>::iterator it = setAttribs.begin() ; it != setAttribs.end() ; it++ ) {
					if ( strName == (std::string) (*it).name() ) {
						TypeSeq vecType;
			 			int iCompound = ( (*it).max() > 1 || (*it).max() == -1 ) ? 1 : 0;
						if ( iCompound ) {
							if ( (bool) (*it).ordered() )
								iCompound = 2;
							vecType.push_back( ( iCompound == 2 ) ? "ocl::Sequence" : "ocl::Set" );
						}
						std::string strType = (*it).type();
						if ( strType == "ocl::Boolean" || strType == "Boolean" )
							vecType.push_back( "ocl::Boolean" );
						else if ( strType == "ocl::Integer" || strType == "Integer" )
							vecType.push_back( "ocl::Integer" );
						else if ( strType == "ocl::Real" || strType == "Real" )
							vecType.push_back( "ocl::Real" );
						else if ( strType == "ocl::String" || strType == "String" )
							vecType.push_back( "ocl::String" );
						else
							continue;
						vecFeatures.push_back( new OclMeta::Attribute( strName, vecType, new TObjectDerived_Attribute( *it, iCompound ), true ) );
					}
				}
			}
	};

	class TObjectDerived_MethodFactory
		: public OclImplementation::MethodFactory
	{
		public :
			virtual void GetFeatures( const OclSignature::Method& signature, OclMeta::MethodVector& vecFeatures )
			{
				std::string strName = signature.GetName();
				int iCount = signature.GetParameterCount();
				TypeSeq vecType;
				OclCommon::FormalParameterVector vecParams;

				if ( ( strName == "isNull" ) && iCount == 0 ) {
					vecType.push_back( "ocl::Boolean" );
					vecFeatures.push_back( new OclMeta::Method( strName, vecParams, vecType, NULL, false ) );
					return;
				}
			}
	};


//##############################################################################################################################################
//
//	G L O B A L   F A C T O R I E S
//
//==============================================================================================================================================
//
//	D E S C R I P T I O N :
//
//##############################################################################################################################################

	TypeFactory::TypeFactory( const ::Uml::Diagram& diagram )
		: m_diagram( diagram )
	{
	}

	void TypeFactory::GetTypes( const std::string& strName, std::vector<OclMeta::Type*>& vecTypes )
	{
		bool bIsQualified = strName.find( "::" ) != std::string::npos; // true if qualified type, false if local type

		GetDynamicTypes( strName, vecTypes );				//get the types from UML meta
		if ( ! bIsQualified && ! vecTypes.empty() )		//if local type, and results not empty - return the results
			return;

		GetPredefinedTypes( strName, vecTypes );			//get the predefined types
		if ( ! bIsQualified && ! vecTypes.empty() )		//if local type, and resulst not empty - return the results
			return;

		// WARNING :
		// hack.... we have to call the base class' GetTypes, but because of different namespace I could solve this only in that way
		// if the base class has member variables this call may lead to errors as copy constructor does not exist
		( ( OclBasic::TypeFactory ) *this ).GetTypes( strName, vecTypes );	//search & return the OCL types
	}

	void TypeFactory::GetDynamicTypes( const std::string& strName, std::vector<OclMeta::Type*>& vecTypes )
	{
		bool bIsQualified = strName.find( "::" ) != std::string::npos; // true if qualified type, false if local type

		std::string strDgrName, strNsPath, strClassName;

		if (bIsQualified) 
		{
			std::string::size_type loc1 = strName.find("::");
			std::string::size_type loc2 = strName.rfind("::");
			if (loc1 != loc2)
			{
				strDgrName = strName.substr(0, loc1);
				strNsPath = strName.substr(loc1 + 2, loc2 - loc1 - 2);
				strClassName = strName.substr(loc2 + 2);
			}
			else
			{
				strDgrName = strName.substr(0, loc1);
				strClassName = strName.substr(loc1 + 2);
			}
		}
		else
			strClassName = strName;
/*
		std::string strRealName = strName;
		if ( strName.size() > 6 && strName.substr( 0, 6 ) == "meta::" )
			strRealName = strName.substr( 6 );
*/

		//if type is qualified, we search it only in the diagram or
		//the corresponding namespace
		//if type is not qualified, we search it in the diagram and
		//all namespaces 
		if ( (!bIsQualified) || ( strNsPath.empty() ) )
		{
			GetDynamicTypes( m_diagram.classes(), strClassName, vecTypes );
		}

		::Uml::DiagramNamespaces nses(m_diagram);
		for ( ::Uml::DiagramNamespaces::iterator j = nses.begin(); j != nses.end(); j++)
		{
			if ( (!bIsQualified) || ( strNsPath == j->getPath2("::", false) ) )
			{
				GetDynamicTypes( j->classes(), strClassName, vecTypes );
			}
		}
	}

	void TypeFactory::GetDynamicTypes( const std::vector< ::Uml::Class>& vecClasses, const std::string& strClassName, std::vector<OclMeta::Type*>& vecTypes )
	{
		for ( int i = 0 ; i < vecClasses.size() ; i++ ) 
		{
			if ( strClassName == (std::string) vecClasses[ i ].name() ) 
			{
				StringVector vecSuperTypes;
				set< ::Uml::Class> setBases = vecClasses[ i ].baseTypes();
				for ( set< ::Uml::Class>::iterator it = setBases.begin(); it != setBases.end() ; it++ )
					vecSuperTypes.push_back( ::UdmOcl::GetQualifiedName( *it ) );
				if ( setBases.empty() )
					vecSuperTypes.push_back( "udm::Object" );
				::Uml::Class theClass = vecClasses[ i ];
				vecTypes.push_back( new OclMeta::Type( ::UdmOcl::GetQualifiedName( theClass ), vecSuperTypes, new TObjectDerived_AttributeFactory( theClass ), new TObjectDerived_AssociationFactory( theClass ), new TObjectDerived_MethodFactory(), true ) );
			}
		}
	}

	void TypeFactory::GetPredefinedTypes( const std::string& strName, std::vector<OclMeta::Type*>& vecTypes )
	{
		if ( strName == "udm::DataNetwork" || strName == "DataNetwork" ) {
			StringVector vecSupers;
			vecSupers.push_back( "ocl::Any" );
			vecTypes.push_back( new OclMeta::Type( "udm::DataNetwork", vecSupers, new OclImplementation::AttributeFactory(), new TDataNetwork_AssociationFactory( m_diagram ), new OclImplementation::MethodFactory(), true ) );
			return;
		}

		if ( strName == "udm::Object" || strName == "Object" ) {
			StringVector vecSupers;
			vecSupers.push_back( "ocl::Any" );
			vecTypes.push_back( new OclMeta::Type( "udm::Object", vecSupers, new OclImplementation::AttributeFactory(), new OclImplementation::AssociationFactory(), new TObjectDerived_MethodFactory(), true ) );
			return;
		}
	}

}; // namespace UmlOcl
