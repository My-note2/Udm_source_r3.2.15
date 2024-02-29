//###############################################################################################################################################
//
//	Object Constraint Language Expression Checker For UML
//	UMLFacade.cpp
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

#include "UMLFacade.h"

namespace OCLUML
{

//##############################################################################################################################################
//
//	C L A S S : OCLUML::ConstraintDefinitionFactory <<< + OclImplementation::ConstraintDefinitionFactory
//
//==============================================================================================================================================
//
//	D E S C R I P T I O N :
//
//##############################################################################################################################################

	class ConstraintDefinitionFactory
		: public OclImplementation::ConstraintDefinitionFactory
	{
		public :
			CComPtr<IMgaProject> 	m_spProject;
			OclTree::TreeManager* 	m_pTreeManager;

			virtual void GetFeatures( const OclSignature::Attribute& signature, OclMeta::AttributeVector& vecFeatures )
			{
				ConstraintDefVector vecDefs;
				GetConstraintDefinitions( vecDefs );
				for ( int i = 0 ; i < vecDefs.size() ; i++ ) {
					if ( m_pTreeManager->GetTypeManager()->IsTypeA( signature.GetTypeName(), vecDefs[ i ]->GetContextType() ) >= 0 )
						if ( 	vecDefs[ i ]->GetName() == signature.GetName() && vecDefs[ i ]->GetStereotype() == "attribute" )
							vecFeatures.push_back( new OclMeta::Attribute( signature.GetName(), CreateReturnType( vecDefs[ i ]->GetReturnType() ), NULL, true ) );
				}
			}

			virtual void GetFeatures( const OclSignature::Method& signature, OclMeta::MethodVector& vecFeatures )
			{
				ConstraintDefVector vecDefs;
				GetConstraintDefinitions( vecDefs );
				for ( int i = 0 ; i < vecDefs.size() ; i++ ) {
					if ( m_pTreeManager->GetTypeManager()->IsTypeA( signature.GetTypeName(), vecDefs[ i ]->GetContextType() ) >= 0 ) {
						OclCommon::FormalParameterVector vecParamsIn = vecDefs[ i ]->GetFormalParameters();
						if ( vecDefs[ i ]->GetName() == signature.GetName() && vecDefs[ i ]->GetStereotype() == "method" && vecParamsIn.size() == signature.GetParameterCount() ) {
							OclCommon::FormalParameterVector vecParams;
							for ( int j = 0 ; j < vecParamsIn.size() ; j++ ) {
								TypeSeq vecType;
								OclCommon::Convert( vecParamsIn[ j ].GetTypeName(), vecType );
								vecParams.push_back( OclCommon::FormalParameter( vecParamsIn[ j ].GetName(), vecType[ 0 ], true ) );
							}
							vecFeatures.push_back( new OclMeta::Method( signature.GetName(), vecDefs[ i ]->GetContextType(), vecParams, CreateReturnType( vecDefs[ i ]->GetReturnType() ), NULL, true ) );
						}
					}
				}
			}

		private :
			void GetConstraintDefinitions( ConstraintDefVector& vecDefs )
			{
				CComPtr<IMgaFilter> spFilter;
				COMTHROW( m_spProject->CreateFilter( &spFilter ) );
				COMTHROW( spFilter->put_Kind( CComBSTR( "ConstraintDefinition" ) ) );

				CComPtr<IMgaFCOs> spFCOs;
				COMTHROW( m_spProject->AllFCOs( spFilter, &spFCOs ) );

				MGACOLL_ITERATE( IMgaFCO, spFCOs ) {

					GME::FCOVector vecFCOs;
					GME::GetAssociationEnds( MGACOLL_ITER, "src", "HasDefinition", vecFCOs );

					for ( int i = 0 ; i < vecFCOs.size() ; i++ ) {
						std::string strType = GME::GetObjectName( vecFCOs[ i ].p );
						if ( ! strType.empty() ) {
							strType = GME::GetQualifiedName( vecFCOs[ i ].p );
							SpConstraintDef spConstraintDef( new ConstraintDef( strType, MGACOLL_ITER ) );
							spConstraintDef->Register( m_pTreeManager );
							Ocl::Constraint::State eState = spConstraintDef->ParseContext();
							if ( eState == Ocl::Constraint::CS_CTX_PARSE_SUCCEEDED ) {
								eState = spConstraintDef->CheckContext();
								if ( eState == Ocl::Constraint::CS_CTX_CHECK_SUCCEEDED )
									vecDefs.push_back( spConstraintDef );
							}
						}
					}
				} MGACOLL_ITERATE_END;
			}

			TypeSeq CreateReturnType( const std::string& strType )
			{
				TypeSeq vecType;
				OclCommon::Convert( strType, vecType );
				return vecType;
			}
	};

//##############################################################################################################################################
//
//	C L A S S : OCLUML::Facade
//
//##############################################################################################################################################

	Facade::Facade()
		: m_pTreeManager( NULL ), m_spProject( NULL )
	{
	}

	Facade::~Facade()
	{
		Finalize();
		if ( m_pTreeManager )
			delete m_pTreeManager;
	}

	CComPtr<IMgaProject> Facade::GetProject() const
	{
		return m_spProject;
	}

	OclTree::TreeManager* Facade::GetTreeManager() const
	{
		return m_pTreeManager;
	}

	void Facade::Initialize( CComPtr<IMgaProject> spProject )
	{
		if ( m_pTreeManager )
			delete m_pTreeManager;

		m_spProject = spProject;

		m_bEnabled = true;
		m_bEnabledEvents = true;
		m_bEnabledInteractions = true;

		ConstraintDefinitionFactory* pCDFactory = new ConstraintDefinitionFactory();
		pCDFactory->m_spProject = spProject;

		OclMeta::TypeManager* pTypeManager = new OclMeta::TypeManager( new TypeFactory( spProject ), new OclBasic::OperatorFactory(), new OclImplementation::FunctionFactory(), pCDFactory );

		m_pTreeManager = new OclTree::TreeManager( pTypeManager, new OclTree::ObjectNodeAdaptor(), new OclTree::CollectionNodeAdaptor() );
		pCDFactory->m_pTreeManager = m_pTreeManager;
	}

	void Facade::Finalize()
	{
		if ( m_pTreeManager )
			m_pTreeManager->GetTypeManager()->ClearDynamicTypes();
	}

	bool Facade::IsConstraintDefined( CComPtr<IMgaFCO> spConstraint )
	{
		return ! GME::GetStringAttribute( spConstraint, "ConstraintEqn" ).empty();
	}

	bool Facade::IsConstraintDefinitionDefined( CComPtr<IMgaFCO> spConstraintDef )
	{
		return ! GME::GetStringAttribute( spConstraintDef, "DefinitionEqn" ).empty() && ! GME::GetStringAttribute( spConstraintDef, "DefinitionRetType" ).empty();
	}

	void Facade::CheckConstraint( CComPtr<IMgaFCO> spConstraint, ConstraintVector& vecFaileds, bool bFieldsMandatory )
	{
		if ( ! IsConstraintDefined( spConstraint ) )
			return;

		GME::FCOVector vecFCOs;
		GME::GetAssociationEnds( spConstraint, "src", "HasConstraint", vecFCOs );

		for ( int i = 0 ; i < vecFCOs.size() ; i++ ) {
			std::string strType = GME::GetObjectName( vecFCOs[ i ].p );
			if ( strType.empty() ) {
				char chNum[ 100 ];
				sprintf( chNum, "%d", i );
				strType = "UntitledClass_" + std::string( chNum );
			} else {
				strType = GME::GetQualifiedName( vecFCOs[ i ].p );
			}
			SpConstraint pConstraint( new Constraint( strType, spConstraint ) );
			pConstraint->Register( m_pTreeManager );
			if ( pConstraint->GetState() == Ocl::Constraint::CS_DEFINED ) {
				Ocl::Constraint::State eState = pConstraint->Parse();
				if ( eState != Ocl::Constraint::CS_PARSE_SUCCEEDED )
					vecFaileds.push_back( pConstraint );
				else {
					OclTree::TypeContextStack context;
					context.AddVariable( "dn", TypeSeq( 1, "udm::DataNetwork" ) );
					eState = pConstraint->Check( context );
					if ( eState != Ocl::Constraint::CS_CHECK_SUCCEEDED )
						vecFaileds.push_back( pConstraint );
				}
			}
		}
	}

	void Facade::CheckConstraintDefinition( CComPtr<IMgaFCO> spConstraintDef, ConstraintDefVector& vecFaileds, bool bFieldsMandatory )
	{
		if ( ! IsConstraintDefinitionDefined( spConstraintDef ) )
			return;

		GME::FCOVector vecFCOs;
		GME::GetAssociationEnds( spConstraintDef, "src", "HasDefinition", vecFCOs );

		for ( int i = 0 ; i < vecFCOs.size() ; i++ ) {
			std::string strType = GME::GetObjectName( vecFCOs[ i ].p );
			if ( strType.empty() ) {
				char chNum[ 100 ];
				sprintf( chNum, "%d", i );
				strType = "UntitledClass_" + std::string( chNum );
			} else {
				strType = GME::GetQualifiedName( vecFCOs[ i ].p );
			}
			SpConstraintDef pConstraintDef( new ConstraintDef( strType, spConstraintDef ) );
			pConstraintDef->Register( m_pTreeManager );
			if ( pConstraintDef->GetState() == Ocl::Constraint::CS_DEFINED ) {
				Ocl::Constraint::State eState = pConstraintDef->Parse();
				if ( eState != Ocl::Constraint::CS_PARSE_SUCCEEDED )
					vecFaileds.push_back( pConstraintDef );
				else {
					OclTree::TypeContextStack context;
					context.AddVariable( "dn", TypeSeq( 1, "udm::DataNetwork" ) );
					eState = pConstraintDef->Check( context );
					if ( eState != Ocl::Constraint::CS_CHECK_SUCCEEDED )
						vecFaileds.push_back( pConstraintDef );
				}
			}
		}
	}

	void Facade::CheckAllConstraints( ConstraintVector& vecFaileds )
	{
		CComPtr<IMgaFilter> spFilter;
		COMTHROW( m_spProject->CreateFilter( &spFilter ) );
		COMTHROW( spFilter->put_Kind( CComBSTR( "Constraint" ) ) );

		CComPtr<IMgaFCOs> spFCOs;
		COMTHROW( m_spProject->AllFCOs( spFilter, &spFCOs ) );

		MGACOLL_ITERATE( IMgaFCO, spFCOs ) {
			CheckConstraint( MGACOLL_ITER, vecFaileds, true );
		} MGACOLL_ITERATE_END;
	}

	void Facade::CheckAllConstraintDefinitions( ConstraintDefVector& vecFaileds )
	{
		CComPtr<IMgaFilter> spFilter;
		COMTHROW( m_spProject->CreateFilter( &spFilter ) );
		COMTHROW( spFilter->put_Kind( CComBSTR( "ConstraintDefinition" ) ) );

		CComPtr<IMgaFCOs> spFCOs;
		COMTHROW( m_spProject->AllFCOs( spFilter, &spFCOs ) );

		MGACOLL_ITERATE( IMgaFCO, spFCOs ) {
			CheckConstraintDefinition( MGACOLL_ITER, vecFaileds, true );
		} MGACOLL_ITERATE_END;
	}

}; // namespace OCLUML