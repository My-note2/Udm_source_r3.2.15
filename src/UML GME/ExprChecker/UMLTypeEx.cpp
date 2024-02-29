//###############################################################################################################################################
//
//	Object Constraint Language Expression Checker For UML
//	UMLTypeEx.cpp
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
// 	an action of contract, negligence or other tortious action,
//	arising out of or in connection with the use or performance of
//	this software.
//
//###############################################################################################################################################


/*
CHANGELOG

	12/11/04	-	endre

	Bugfix in void TypeFactory::GetPredefinedTypes, UDM BUG 20.

	(http://escher.isis.vanderbilt.edu/JIRA/browse/UDM-20)

*/
#include "UMLTypeEx.h"
#include <strstream>
#include "CardinalityObject.h"
#include "AttributeObject.h"
#include "Mga.h"


namespace OCLUML
{
	TypeSeq GetAssociationTypeSeq( const std::string& strMultiplicity, bool bOrdered, CComPtr<IMgaFCO> spFCO )
	{
		TypeSeq vecType;
		bool bIsSet = true;
		std::string& strType = GME::GetQualifiedName( spFCO.p );

		/*
		Multiplicity objMulti;
		objMulti::Parse( strMultiplicity, objMulti );
		bIsSet = objMulti.IsSet();
		*/

		try {
			CardinalityObject cardObject( strMultiplicity );
			bIsSet = cardObject.getmax() == -1 || cardObject.getmax() > 1;
		}
		catch( ... ) {
			// In case of ExpressionChecker we care only valid cardinalities, invalid cardinalities are not shown, user is not warned !
		}

		if ( bIsSet )
			vecType.push_back( ( bOrdered ) ? "ocl::Sequence" : "ocl::Set" );
		vecType.push_back( strType );
		return vecType;
	}

//##############################################################################################################################################
//
//	T Y P E   O F   udm::DataNetwork
//
//##############################################################################################################################################

	class TProject_AssociationFactory
		: public OclImplementation::AssociationFactory
	{
		private :
		 	CComPtr<IMgaProject>	m_spProject;

		 public :
		 	TProject_AssociationFactory( CComPtr<IMgaProject> spProject )
		 		: m_spProject( spProject )
		 	{
		 	}

		 public :
			virtual void GetFeatures( const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
			{
				if ( ! signature.GetAcceptableTypeName().empty() )
					return;

				CComPtr<IMgaFilter> spFilter;
				COMTHROW( m_spProject->CreateFilter( &spFilter ) );
				COMTHROW( spFilter->put_Kind( CComBSTR( "Class" ) ) );
				COMTHROW( spFilter->put_ObjType( CComBSTR( "OBJTYPE_ATOM" ) ) );

				CComPtr<IMgaFCOs> spFCOs;
				COMTHROW( m_spProject->AllFCOs( spFilter, &spFCOs ) );
				MGACOLL_ITERATE( IMgaFCO, spFCOs ) {
					std::string strKind = GME::GetObjectName( MGACOLL_ITER.p );
					std::string strRole = GME::LowerFirst( strKind );
					std::string strRole_Q = GME::GetQualifiedName( MGACOLL_ITER.p, true );
					std::string strSig = signature.GetName();
					bool bIsQualified = strSig.find( "::" ) != std::string::npos;	// true if qualified type, false if local type

					if ( ( bIsQualified && ( strRole_Q == strSig ) ) || ( (!bIsQualified) && ( strRole == strSig ) ) ) {
						TypeSeq vecType( 1, "ocl::Set" );
						vecType.push_back( GME::GetQualifiedName( MGACOLL_ITER.p ) );
						vecFeatures.push_back( new OclMeta::Association( strRole, "", vecType, NULL, true ) );
					}
				} MGACOLL_ITERATE_END;
			}
	};

//##############################################################################################################################################
//
//	D E R I V E D   O F   udm::Object
//
//##############################################################################################################################################
	/*
	class TudmObject_AssociationFactory
		: public OclImplementation::AssociationFactory
	{
	public:
		TudmObject_AssociationFactory(){};
		virtual void GetFeatures( const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
		{
		//	std::string debug = signature.Print();
		//	std::string debug1 = signature.GetAcceptableTypeName();

		};
			
	};
	*/

	class TObjectDerived_AssociationFactory
		: public OclImplementation::AssociationFactory
	{
		private :
		 	CComPtr<IMgaFCO>	m_spFCO;

		 public :
		 	TObjectDerived_AssociationFactory( CComPtr<IMgaFCO> spFCO )
		 		: m_spFCO( spFCO )
		 	{
		 	}

		 public :
			virtual void GetFeatures( const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
			{
				GetAssociations( m_spFCO, signature, vecFeatures );
				CComPtr<IMgaFCOs> spReferences;
				COMTHROW( m_spFCO->get_ReferencedBy( &spReferences ) );
				MGACOLL_ITERATE( IMgaFCO, spReferences ) {
					GetAssociations( MGACOLL_ITER, signature, vecFeatures );
				} MGACOLL_ITERATE_END;
			}

		private :
			void GetConnections( CComPtr<IMgaFCO> spFCO, const std::string& strName, GME::FCOVector& vecFCOs, GME::FCOVector& vecConnections )
			{
				CComPtr<IMgaConnPoints> spStartCPs;
				COMTHROW( spFCO->get_PartOfConns( &spStartCPs ) );
				MGACOLL_ITERATE( IMgaConnPoint, spStartCPs ) {
					CComPtr<IMgaConnPoint> spStartCP = MGACOLL_ITER;
					CComPtr<IMgaConnection> spConnection;
					COMTHROW( spStartCP->get_Owner( &spConnection ) );
					if ( GME::GetObjectKind( spConnection.p ) == strName ) {
						CComPtr<IMgaConnPoints> spEndCPs;
						COMTHROW( spConnection->get_ConnPoints( &spEndCPs ) );
						MGACOLL_ITERATE( IMgaConnPoint, spEndCPs ) {
							CComPtr<IMgaConnPoint> spEndCP = MGACOLL_ITER;
							if ( spEndCP != spStartCP ) {
								CComPtr<IMgaFCO> spTarget;
								COMTHROW( spEndCP->get_Target( &spTarget ) );
								vecConnections.push_back( spConnection.p );
								vecFCOs.push_back( spTarget.p );
							}
						} MGACOLL_ITERATE_END;
					}
				} MGACOLL_ITERATE_END;
			}

			void GetConnections( CComPtr<IMgaFCO> spFCO, const std::string& strName, GME::FCOVector& vecFCOs )
			{
				GME::FCOVector vecConnections;
				GetConnections( spFCO, strName, vecFCOs, vecConnections );
			}

			CComPtr<IMgaFCO> GetEntity( CComPtr<IMgaFCO> spFCO )
			{
				CComQIPtr<IMgaReference> spReference = spFCO;
				if ( spReference.p ) {
					CComPtr<IMgaFCO> spResult;
					COMTHROW( spReference->get_Referred( &spResult ) );
					return spResult;
				}
				return spFCO;
			}

			void GetAssociations( CComPtr<IMgaFCO> spFCO, const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
			{
				
				GetCompositions( spFCO, signature, vecFeatures );
				GetAssociationsWOClass( spFCO, signature, vecFeatures );
				GetAssociationsWClass( spFCO, signature, vecFeatures, true );
				GetAssociationsWClass( spFCO, signature, vecFeatures, false );
				GetAssociationsToClass( spFCO, signature, vecFeatures, true );
				GetAssociationsToClass( spFCO, signature, vecFeatures, false );
				GetAssociationsFromClass( spFCO, signature, vecFeatures, true );
				GetAssociationsFromClass( spFCO, signature, vecFeatures, false );


			}

			void GetCompositions( CComPtr<IMgaFCO> spFCO, const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
			{
				std::string strSelf = GME::GetObjectName( spFCO.p );
				if ( ! signature.GetAcceptableTypeName().empty() )
					return;

				GME::FCOVector vecConnections;
				GME::FCOVector vecFCOs;
				GetConnections( spFCO, "Composition", vecFCOs, vecConnections );
				for ( int i = 0 ; i < vecFCOs.size() ; i++ ) {
					CComPtr<IMgaFCO> spAssocEnd = GetEntity( vecFCOs[ i ].p );
					if ( spAssocEnd.p ) {
						CComPtr<IMgaConnPoints> spCPs;
						CComQIPtr<IMgaConnection> spConnection = vecConnections[ i ].p;
						COMTHROW( spConnection->get_ConnPoints( &spCPs ) );
						MGACOLL_ITERATE( IMgaConnPoint, spCPs ) {
							CComPtr<IMgaFCO> spTarget;
							COMTHROW( MGACOLL_ITER->get_Target( &spTarget ) );
							if ( spTarget == vecFCOs[ i ] ) {
								std::string strCPRole = GME::GetConnPointRole( MGACOLL_ITER );
								std::string strRole = GME::GetStringAttribute( vecConnections[ i ].p, ( strCPRole == "src" ) ? "ChildRole" : "ParentRole" );
								std::string strKind = GME::GetObjectName( spAssocEnd.p );
								std::string strRoleKind = GME::LowerFirst( strKind );
								if ( ! strRole.empty() && strRole == signature.GetName() || strRole.empty() && strRoleKind == signature.GetName() ) {
									bool bOrdered = false; //// WARNING.... Where is ordered information????
									std::string strCardinality = ( strCPRole == "src" ) ? GME::GetStringAttribute( vecConnections[ i ].p, "Cardinality" ) : "1";
									TypeSeq vecType = GetAssociationTypeSeq( strCardinality, bOrdered, spAssocEnd.p );
									vecFeatures.push_back( new OclMeta::Association( signature.GetName(), "", vecType, NULL, true ) );
								}
							}
						} MGACOLL_ITERATE_END;
					}
				}
			}

			void GetAssociationsWOClass( CComPtr<IMgaFCO> spFCO, const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures )
			{
				if ( ! signature.GetAcceptableTypeName().empty() )
					return;

				GME::FCOVector vecConnections;
				GME::FCOVector vecFCOs;
				GetConnections( spFCO, "Association", vecFCOs, vecConnections );
				for ( int i = 0 ; i < vecFCOs.size() ; i++ ) {
					CComPtr<IMgaFCO> spAssocEnd = GetEntity( vecFCOs[ i ].p );
					if ( spAssocEnd.p ) {
						CComPtr<IMgaConnPoints> spCPs;
						CComQIPtr<IMgaConnection> spConnection = vecConnections[ i ].p;
						COMTHROW( spConnection->get_ConnPoints( &spCPs ) );
						MGACOLL_ITERATE( IMgaConnPoint, spCPs ) {
							CComPtr<IMgaFCO> spTarget;
							COMTHROW( MGACOLL_ITER->get_Target( &spTarget ) );
							if ( spTarget == vecFCOs[ i ] ) {
								std::string strCPRole = GME::GetConnPointRole( MGACOLL_ITER );
								std::string strRole = GME::GetStringAttribute( vecConnections[ i ].p, ( strCPRole == "src" ) ? "srcRolename" : "dstRolename" );
								std::string strKind = GME::GetObjectName( spAssocEnd.p );
								std::string strRoleKind = GME::LowerFirst( strKind );
								if ( ! strRole.empty() && strRole == signature.GetName() || strRole.empty() && strRoleKind == signature.GetName() ) {
									bool bOrdered = false; //// WARNING.... Where is ordered information????
									std::string strCardinality = GME::GetStringAttribute( vecConnections[ i ].p, ( strCPRole == "src" ) ? "srcCardinality" : "dstCardinality" );
									TypeSeq vecType = GetAssociationTypeSeq( strCardinality, bOrdered, spAssocEnd );
									vecFeatures.push_back( new OclMeta::Association( signature.GetName(), "", vecType, NULL, true ) );
								}
							}
						} MGACOLL_ITERATE_END;
					}
				}
			}

			void GetAssociationsWClass( CComPtr<IMgaFCO> spFCO, const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures, bool bSrcToDst )
			{
				if ( ! signature.GetAcceptableTypeName().empty() )
					return;

				GME::FCOVector vecConnections;
				GME::FCOVector vecConnectors;
				GME::FCOVector vecFCOs;
				GetConnections( spFCO, ( bSrcToDst ) ? "Src" : "Dst", vecConnectors );
				int i;
				for ( i = 0 ; i < vecConnectors.size() ; i++ )
					GetConnections( vecConnectors[ i ].p,  ( bSrcToDst ) ? "Dst" : "Src", vecFCOs, vecConnections );
				for ( i = 0 ; i < vecFCOs.size() ; i++ ) {
					CComPtr<IMgaFCO> spAssocEnd = GetEntity( vecFCOs[ i ].p );
					if ( spAssocEnd.p ) {
						std::string strRole = GME::GetStringAttribute( vecConnections[ i ].p, ( bSrcToDst ) ? "dstRolename" : "srcRolename" );
						std::string strKind = GME::GetObjectName( spAssocEnd.p );
						std::string strRoleKind = GME::LowerFirst( strKind );
						if ( ! strRole.empty() && strRole == signature.GetName() || strRole.empty() && strRoleKind == signature.GetName() ) {
							bool bOrdered = false; //// WARNING.... Where is ordered information????
							std::string strCardinality = GME::GetStringAttribute( vecConnections[ i ].p, "Cardinality" );
							TypeSeq vecType = GetAssociationTypeSeq( strCardinality, bOrdered, spAssocEnd );
							vecFeatures.push_back( new OclMeta::Association( signature.GetName(), "", vecType, NULL, true ) );
						}
					}
				}
			}

			void GetAssociationsToClass( CComPtr<IMgaFCO> spFCO, const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures, bool bSrcToDst )
			{
				GME::FCOVector vecConnectors;
				GetConnections( spFCO, ( bSrcToDst ) ? "Src" : "Dst", vecConnectors );
				for ( int i = 0 ; i < vecConnectors.size() ; i++ ) {
					GME::FCOVector vecFCOs;
					GetConnections( vecConnectors[ i ].p, "AssociationClass", vecFCOs );
					GME::FCOVector vecConnections;
					GME::FCOVector vecAssocEnds;
					GetConnections( vecConnectors[ i ].p, ( bSrcToDst ) ? "Dst" : "Src", vecAssocEnds, vecConnections );
					for ( int j = 0 ; j < vecFCOs.size() ; j++ ) {
						CComPtr<IMgaFCO> spAssocEnd = GetEntity( vecFCOs[ j ].p );
						if ( spAssocEnd.p ) {
							for ( int k = 0 ; k < vecConnections.size() ; k++ ) {
								std::string strRole = GME::GetStringAttribute( vecConnections[ k ].p, ( bSrcToDst ) ? "dstRolename" : "srcRolename" );
								std::string strKind = GME::GetObjectName( spAssocEnd.p );
								std::string strRoleKind = GME::LowerFirst( strKind );
								std::string strAcceptable = signature.GetAcceptableTypeName();
								if ( strAcceptable.empty() && strRoleKind == signature.GetName() || ! strAcceptable.empty() && ! strRole.empty() && strRole == signature.GetName() && strAcceptable == strRoleKind ) {
									bool bOrdered = false; //// WARNING.... Where is ordered information????
									std::string strCardinality = GME::GetStringAttribute( vecConnections[ k].p, "Cardinality" );
									TypeSeq vecType = GetAssociationTypeSeq( strCardinality, bOrdered, spAssocEnd );
									vecFeatures.push_back( new OclMeta::Association( signature.GetName(), strAcceptable, vecType, NULL, true ) );
								}
							}
						}
					}
				}
			}

			void GetAssociationsFromClass( CComPtr<IMgaFCO> spFCO, const OclSignature::Association& signature, OclMeta::AssociationVector& vecFeatures, bool bSrcToDst )
			{
				if ( ! signature.GetAcceptableTypeName().empty() )
					return;

				GME::FCOVector vecConnectors;
				GetConnections( spFCO, "AssociationClass", vecConnectors );
				GME::FCOVector vecFCOs;
				GME::FCOVector vecConnections;
				int i;
				for ( i = 0 ; i < vecConnectors.size() ; i++ )
					GetConnections( vecConnectors[ i ].p, ( bSrcToDst ) ? "Src" : "Dst", vecFCOs, vecConnections );
				for ( i = 0 ; i < vecFCOs.size() ; i++ ) {
					CComPtr<IMgaFCO> spAssocEnd = GetEntity( vecFCOs[ i ].p );
					if ( spAssocEnd.p ) {
						std::string strRole = GME::GetStringAttribute( vecConnections[ i ].p, ( bSrcToDst ) ? "srcRolename" : "dstRolename" );
						std::string strKind = GME::GetObjectName( spAssocEnd.p );
						std::string strRoleKind = GME::LowerFirst( strKind );
						if ( ! strRole.empty() && strRole == signature.GetName() || strRole.empty() && strRoleKind == signature.GetName() ) {
							bool bOrdered = false; //// WARNING.... Where is ordered information????
							TypeSeq vecType = GetAssociationTypeSeq( "1", bOrdered, spAssocEnd );
							vecFeatures.push_back( new OclMeta::Association( signature.GetName(), "", vecType, NULL, true ) );
						}
					}
				}
			}

	};

	class TObjectDerived_AttributeFactory
		: public OclImplementation::AttributeFactory
	{
		private :
		 	CComPtr<IMgaFCO>	m_spFCO;

		 public :
		 	TObjectDerived_AttributeFactory( CComPtr<IMgaFCO> spFCO )
		 		: m_spFCO( spFCO )
		 	{
		 	}

		 public :
			virtual void GetFeatures( const OclSignature::Attribute& signature, OclMeta::AttributeVector& vecFeatures )
			{
				std::string strAttributeDefs = GME::GetStringAttribute( m_spFCO, "Attributes" );
				Trim( strAttributeDefs );
				while ( ! strAttributeDefs.empty() ) {
					int iPos = strAttributeDefs.find( "\n" );
					std::string strAttribute = ( iPos != std::string::npos ) ? strAttributeDefs.substr( 0, iPos ) : strAttributeDefs;
					strAttributeDefs = ( iPos != std::string::npos ) ? strAttributeDefs.substr( iPos ) : "";
					Trim( strAttribute );
					Trim( strAttributeDefs );
					try {
						AttributeObject objAttribute( strAttribute );
						if ( objAttribute.GetName() == signature.GetName() ) {
							TypeSeq vecType;
							if ( objAttribute.IsSet() )
								vecType.push_back( ( objAttribute.IsOrdered() ) ? "ocl::Sequence" : "ocl::Set" );
							switch ( objAttribute.GetType() ) {
								case Integer : vecType.push_back( "ocl::Integer" ); break;
								case Real : vecType.push_back( "ocl::Real" ); break;
								case String : vecType.push_back( "ocl::String" ); break;
								default : vecType.push_back( "ocl::Boolean" );
							}
							vecFeatures.push_back( new OclMeta::Attribute( signature.GetName(), vecType, NULL, true ) );
						}
					}
					catch (...) {
						// In case of ExpressionChecker we care only valid attributes, invalid attributes are not shown, user is not warned !
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
//	T Y P E   F A C T O R Y
//
//##############################################################################################################################################

	TypeFactory::TypeFactory( CComPtr<IMgaProject> spProject )
		: m_spProject( spProject )
	{
	}

	void TypeFactory::GetTypes( const std::string& strName, std::vector<OclMeta::Type*>& vecTypes )
	{
		bool bIsQualified = strName.find( "::" ) != std::string::npos;	// true if qualified type, false if local type

		GetDynamicTypes( strName, vecTypes );	//get the types from metamodel
		if ( ! bIsQualified && ! vecTypes.empty() )	//if local type, and results not empty - return the results
			return;

		GetPredefinedTypes( strName, vecTypes );	//get the predefined types
		if ( ! bIsQualified && ! vecTypes.empty() )	//if local type, and results not empty - return the results
			return;

		// WARNING :
		// hack.... we have to call the base class' GetTypes, but because of different namespace I could solve this only in that way
		// if the base class has member variables this call may lead to errors as copy constructor does not exist
		( ( OclBasic::TypeFactory ) *this ).GetTypes( strName, vecTypes );
	}

	void TypeFactory::GetPredefinedTypes( const std::string& strName, std::vector<OclMeta::Type*>& vecTypes )
	{
		if ( strName == "udm::DataNetwork" || strName == "DataNetwork" ) {
			StringVector vecSupers;
			vecSupers.push_back( "ocl::Any" );
			vecTypes.push_back( new OclMeta::Type( "udm::DataNetwork", vecSupers, new OclImplementation::AttributeFactory(), new TProject_AssociationFactory( m_spProject ), new OclImplementation::MethodFactory(), true ) );
			return;
		}

		if ( strName == "udm::Object" || strName == "Object" ) {
			StringVector vecSupers;
			vecSupers.push_back( "ocl::Any" );
			/*
			Endre, 12/11/04
				Because of this call, udm::object has all the features udm::DataNetwork has, which I believe is an error
				and leads to ambigous calls in non-ambigous situations
				(like having an attribute and a class with the same name)

			*/
			//vecTypes.push_back( new OclMeta::Type( "udm::Object", vecSupers, new OclImplementation::AttributeFactory(), new TProject_AssociationFactory( m_spProject ), new TObjectDerived_MethodFactory(), true ) );
			vecTypes.push_back( new OclMeta::Type( "udm::Object", vecSupers, new OclImplementation::AttributeFactory(), new OclImplementation::AssociationFactory(), new TObjectDerived_MethodFactory(), true ) );
			return;
		}
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

		CComPtr<IMgaFilter> spFilter;
		COMTHROW( m_spProject->CreateFilter( &spFilter ) );
		COMTHROW( spFilter->put_Kind( CComBSTR( "Class" ) ) );
		COMTHROW( spFilter->put_ObjType( CComBSTR( "OBJTYPE_ATOM" ) ) );

		CComPtr<IMgaFCOs> spFCOs;
		COMTHROW( m_spProject->AllFCOs( spFilter, &spFCOs ) );
		MGACOLL_ITERATE( IMgaFCO, spFCOs ) {
			if ( GME::GetObjectName( MGACOLL_ITER.p ) == strClassName ) {

				std::string strIterNsPath = GME::GetNamespacePath( MGACOLL_ITER.p );
				std::string strIterDgrName = GME::GetDiagramName( MGACOLL_ITER.p );
				if ( (!bIsQualified) || ( strNsPath == strIterNsPath && strDgrName == strIterDgrName ) ) {
					StringVector vecSuperTypes;
					GME::FCOVector vecSuperFCOs;
					GME::GetInheritances( MGACOLL_ITER, true, vecSuperFCOs );
					for ( int i = 0 ; i < vecSuperFCOs.size() ; i++ ) {
						vecSuperTypes.push_back( GME::GetQualifiedName( vecSuperFCOs[ i ].p ) );
					}
					if ( vecSuperFCOs.empty() )
						vecSuperTypes.push_back( "udm::Object" );

					vecTypes.push_back( new OclMeta::Type( GME::GetQualifiedName( MGACOLL_ITER.p ), vecSuperTypes, new TObjectDerived_AttributeFactory( MGACOLL_ITER ), new TObjectDerived_AssociationFactory( MGACOLL_ITER ), new TObjectDerived_MethodFactory(), true ) );
				}
			}
		} MGACOLL_ITERATE_END;
	}

}; // namespace OCLUML
