
/*
CHANGELOG

  11/25/04	-	endre

		Added template parameter long to min<>(), max<>() templated functions

*/
//###############################################################################################################################################
//
//	Object Constraint Language Generic Manager
//	GMESyntacticSemanticDialog.cp
//
//###############################################################################################################################################

#include "StdAfx.h"
#include "GMESyntacticSemanticDialog.h"
#include "GMECommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

	struct SortStruct
	{
		CSyntacticSemanticDialog* 	pDialog;
		int								iColumn;
	};

//##############################################################################################################################################
//
//	C L A S S : CSyntacticSemanticDialog <<< + CDialog
//
//##############################################################################################################################################

CSyntacticSemanticDialog::CSyntacticSemanticDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSyntacticSemanticDialog::IDD, pParent), m_iSelected( 0 ), m_iSelectedError( 0 )
{
	//{{AFX_DATA_INIT(CSyntacticSemanticDialog)
	//}}AFX_DATA_INIT
}

void CSyntacticSemanticDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSyntacticSemanticDialog)
	DDX_Control(pDX, SSD_LSTERRORS, m_lstErrors);
	DDX_Control(pDX, SSD_LSTCONSTRAINTS, m_lstConstraints);
	DDX_Control(pDX, SSD_LSTEXPRESSION, m_lstExpression);
	DDX_Control(pDX, SSD_LBLGENERALMSG, m_lblGeneralMessage);
	DDX_Control(pDX, SSD_ICNLARGE, m_imgLarge);
	DDX_Control(pDX, SSD_BTNCLOSE, m_btnClose);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSyntacticSemanticDialog, CDialog)
	//{{AFX_MSG_MAP(CSyntacticSemanticDialog)
	ON_BN_CLICKED(SSD_BTNCLOSE, OnClickClose)
	ON_NOTIFY(LVN_KEYDOWN, SSD_LSTCONSTRAINTS, OnKeyDownConstraints)
	ON_NOTIFY(LVN_KEYDOWN, SSD_LSTERRORS, OnKeyDownErrors)
	ON_NOTIFY(NM_CLICK, SSD_LSTERRORS, OnClickErrors)
	ON_NOTIFY(LVN_COLUMNCLICK, SSD_LSTCONSTRAINTS, OnClickConstraintsColumn)
	ON_NOTIFY(NM_CLICK, SSD_LSTCONSTRAINTS, OnClickConstraints)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()





BOOL CSyntacticSemanticDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Load ImageLists

	m_lstConstraintImages.Create( 16, 16, ILC_COLOR24 | ILC_MASK, 10, 10 );
	CBitmap bmpTemp1;
	bmpTemp1.LoadBitmap( IDB_CONSTRAINTICONS );
	m_lstConstraintImages.Add( &bmpTemp1, RGB( 255, 0, 255 ) );

	m_lstErrorImages.Create( 16, 16, ILC_COLOR24 | ILC_MASK, 4, 4 );
	CBitmap bmpTemp2;
	bmpTemp2.LoadBitmap( IDB_ERRORICONS );
	m_lstErrorImages.Add( &bmpTemp2, RGB( 255, 0, 255 ) );

	m_lstLineImages.Create( 16, 16, ILC_COLOR24 | ILC_MASK, 2, 2 );
	CBitmap bmpTemp3;
	bmpTemp3.LoadBitmap( IDB_LINEICONS );
	m_lstLineImages.Add( &bmpTemp3, RGB( 255, 0, 255 ) );

	// Set required properties of ListControls

	m_lstConstraints.SetImageList( &m_lstConstraintImages, LVSIL_SMALL );

	int iLength = 3 * m_lstConstraints.GetStringWidth("Type") / 2;
	m_lstConstraints.InsertColumn( 0, "Type", LVCFMT_LEFT, iLength, -1 );
	iLength = 3 * m_lstConstraints.GetStringWidth("Context") / 2;
	m_lstConstraints.InsertColumn( 1, "Context", LVCFMT_LEFT, iLength, 1 );
	iLength = 3 * m_lstConstraints.GetStringWidth("Constraint(Definition)") / 2;
	m_lstConstraints.InsertColumn( 2, "Name", LVCFMT_LEFT, iLength, 2 );

	m_lstConstraints.SetExtendedStyle( LVS_EX_FULLROWSELECT );

	m_lstErrors.SetImageList( &m_lstErrorImages, LVSIL_SMALL );

	iLength = 3 * m_lstErrors.GetStringWidth("Code") / 2;
	m_lstErrors.InsertColumn( 0, "Code", LVCFMT_LEFT, iLength, -1 );
	iLength = 3 * m_lstErrors.GetStringWidth("Line") / 2;
	m_lstErrors.InsertColumn( 1, "Ln.", LVCFMT_LEFT, iLength, 1 );
	/*
	// This column is commented out because this features is not implemented yet
	// Be careful with uncommenting -> column numbers

	iLength = 3 * m_lstErrors.GetStringWidth("Column") / 2;
	m_lstErrors.InsertColumn( 2, "Col.", LVCFMT_LEFT, iLength, 2 );
	*/

	iLength = 3 * m_lstErrors.GetStringWidth("Message") / 2;
	m_lstErrors.InsertColumn( 2, "Message", LVCFMT_LEFT, iLength, 2 );

	m_lstErrors.SetExtendedStyle( LVS_EX_FULLROWSELECT );

	m_lstExpression.SetImageList( &m_lstLineImages, LVSIL_SMALL );

	iLength = 3 * m_lstExpression.GetStringWidth("Line") / 2;
	m_lstExpression.InsertColumn( 0, "Line", LVCFMT_RIGHT, iLength, -1 );
	iLength = 3 * m_lstExpression.GetStringWidth("Expression") / 2;
	m_lstExpression.InsertColumn( 1, "Expression", LVCFMT_LEFT, iLength, 1 );

	m_lstExpression.SetExtendedStyle( LVS_EX_FULLROWSELECT );

	// Fill Constraint List

	int iDefinitions = m_vecConstraintDefinitions.size();
	int i;
	for ( i = 0 ; i < iDefinitions ; i++ )
		AddConstraintRow( i );

	for ( i = 0 ; i < m_vecConstraints.size() ; i++ )
		AddConstraintRow( i + iDefinitions );

	m_lstConstraints.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_lstConstraints.SetColumnWidth( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_lstConstraints.SetColumnWidth( 2, LVSCW_AUTOSIZE_USEHEADER );

	// Display First Constraint

	SortConstraints( 2 );
	SortConstraints( 1 );
	SortConstraints( 0 );
	DisplayConstraintItem();

	return TRUE;
}

void CSyntacticSemanticDialog::OnClickClose()
{
	CDialog::OnOK();
}

void CSyntacticSemanticDialog::OnClickConstraints(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_iSelected = std::min<long>( std::max<long>( m_lstConstraints.GetSelectionMark(), 0 ), m_lstConstraints.GetItemCount() - 1 ); 
	DisplayConstraintItem();
	*pResult = 0;
}

void CSyntacticSemanticDialog::OnKeyDownConstraints(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)pNMHDR;
	if ( pLVKeyDown->wVKey == VK_UP )
		SelectConstraint( false );
	else
		if ( pLVKeyDown->wVKey == VK_DOWN )
			SelectConstraint( true );
	*pResult = 0;
}

void CSyntacticSemanticDialog::OnClickErrors(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_iSelectedError = std::min<long>( std::max<long>( m_lstErrors.GetSelectionMark(), 0 ), m_lstErrors.GetItemCount() - 1 );
	DisplayErrorItem();
	*pResult = 0;
}


void CSyntacticSemanticDialog::OnKeyDownErrors(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)pNMHDR;
	if ( pLVKeyDown->wVKey == VK_UP )
		SelectError( false );
	else
		if ( pLVKeyDown->wVKey == VK_DOWN )
			SelectError( true );
	*pResult = 0;
}

void CSyntacticSemanticDialog::OnClickConstraintsColumn( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	SortConstraints( pNMListView->iSubItem );
	*pResult = 0;
}

	void CSyntacticSemanticDialog::AddItem( const OCLUML::SpConstraint& item )
	{
		m_vecConstraints.push_back( item );
	}

	void CSyntacticSemanticDialog::AddItem( const OCLUML::SpConstraintDef& item )
	{
		m_vecConstraintDefinitions.push_back( item );
	}

	void CSyntacticSemanticDialog::SelectConstraint( bool bNext )
	{
		if ( ! bNext ) {
			if ( m_iSelected != 0 ) {
				m_iSelected--;
				DisplayConstraintItem();
			}
		}
		else {
			if ( m_iSelected != m_lstConstraints.GetItemCount() - 1 ) {
				m_iSelected++;
				DisplayConstraintItem();
			}
		}
	}

	void CSyntacticSemanticDialog::SelectError( bool bNext )
	{
		if ( ! bNext ) {
			if ( m_iSelectedError != 0 ) {
				m_iSelectedError--;
				DisplayErrorItem();
			}
		}
		else {
			if ( m_iSelectedError != m_lstErrors.GetItemCount() - 1 ) {
				m_iSelectedError++;
				DisplayErrorItem();
			}
		}
	}

	void CSyntacticSemanticDialog::AddConstraintRow( int iPos )
	{
		CString strContext, strName; int eType;
		GetConstraintRow( iPos, eType, strContext, strName );

		LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE | LVIF_PARAM;;
		if ( iPos == 0 ) {
			lvItem.mask = lvItem.mask | LVIF_STATE;
			lvItem.state = LVIS_SELECTED;
			lvItem.stateMask = LVIS_SELECTED;
		}
		lvItem.iItem = m_lstConstraints.GetItemCount();
		lvItem.iSubItem = 0;
		lvItem.iImage = eType;
		lvItem.lParam = iPos;
		m_lstConstraints.InsertItem( &lvItem );

		lvItem.mask = LVIF_TEXT;
		lvItem.iSubItem = 1;
		lvItem.pszText = strContext.GetBuffer( strContext.GetLength() );
		m_lstConstraints.SetItem( &lvItem );
		strContext.ReleaseBuffer();

		lvItem.mask = LVIF_TEXT;
		lvItem.iSubItem = 2;
		lvItem.pszText = strName.GetBuffer( strName.GetLength() );
		m_lstConstraints.SetItem( &lvItem );
		strName.ReleaseBuffer();
	}

	void CSyntacticSemanticDialog::GetConstraintRow( int iPos, int& eType, CString& strContext, CString& strName )
	{
		int iDefinitions = m_vecConstraintDefinitions.size();
		std::string strDName = ( iPos >= iDefinitions ) ? m_vecConstraints[ iPos - iDefinitions ]->GetDefinedName() : m_vecConstraintDefinitions[ iPos ]->GetDefinedName();
		std::string::size_type iColonPos = strDName.rfind( "::" );

		if ( iColonPos == std::string::npos ) {
			strContext = "?";
			strName = GME::Convert( strDName );
		}

		strContext = GME::Convert( strDName.substr( 0, iColonPos ) );
		strName = GME::Convert( strDName.substr( iColonPos + 2 ) );

		eType = ( iPos >= iDefinitions ) ? 0 : ( m_vecConstraintDefinitions[ iPos ]->GetStereotype() == "attribute" ) ? 6 : 8;
	}

	void CSyntacticSemanticDialog::DisplayErrorItem()
	{
		int iCPos = m_lstConstraints.GetItemData( m_iSelected );
		int iDefinitions = m_vecConstraintDefinitions.size();
		m_lstErrors.SetItemState( m_iSelectedError, LVIS_SELECTED, LVIS_SELECTED );

		OclCommon::ExceptionPool poolExceptions = ( iCPos >= iDefinitions ) ? m_vecConstraints[ iCPos - iDefinitions ]->GetExceptions() : m_vecConstraintDefinitions[ iCPos ]->GetExceptions();
		int iLine = poolExceptions.GetAt( m_iSelectedError ).GetLine();
		for ( int i = 0 ; i < m_lstExpression.GetItemCount() ; i++ ) {
			LVITEM lvItem;
			lvItem.mask = LVIF_IMAGE | LVIF_STATE;
			lvItem.state = ( i + 1== iLine ) ? LVIS_SELECTED : 0;
			lvItem.stateMask = LVIS_SELECTED;
			lvItem.iItem = i;
			lvItem.iSubItem = 0;
			lvItem.iImage = ( i + 1== iLine ) ? 1 : 0;
			m_lstExpression.SetItem( &lvItem );
			if ( i + 1== iLine )
				m_lstExpression.EnsureVisible( i, false );
		}
	}

	void CSyntacticSemanticDialog::DisplayConstraintItem()
	{
		int iPos = m_lstConstraints.GetItemData( m_iSelected );
		int iDefinitions = m_vecConstraintDefinitions.size();
		m_lstConstraints.SetItemState( m_iSelected, LVIS_SELECTED, LVIS_SELECTED );

		// Set Constraint Text

		m_lstExpression.DeleteAllItems();

		CStringArray arrText;
		CString strExpression = GME::Convert( ( iPos >= iDefinitions ) ? m_vecConstraints[ iPos - iDefinitions ]->GetText() : m_vecConstraintDefinitions[ iPos ]->GetText() );
		strExpression.Replace( "\r", "" );
		strExpression.Replace( "\n", "\r\n" );
		strExpression.Replace( "\t", "    " );
		int iFPos = 0;
		do {
			iFPos = strExpression.Find( "\r\n" );
			if ( iFPos == -1 )
				arrText.Add( strExpression );
			else {
				arrText.Add( strExpression.Left( iFPos ) );
				strExpression = strExpression.Right( strExpression.GetLength() - iFPos - 2 );
			}
		} while ( iFPos != -1 );

		LVITEM lvItem;
		int i;
		for ( i = 0 ; i < arrText.GetSize() ; i++ ) {

			// Add Line

			lvItem.mask = LVIF_IMAGE | LVIF_NORECOMPUTE | LVIF_TEXT;
			lvItem.iItem = m_lstExpression.GetItemCount();
			lvItem.iSubItem = 0;
			lvItem.iImage = 0;
			lvItem.lParam = i;
			CString strTemp;
			strTemp.Format( "%d", i + 1 );
			lvItem.pszText = strTemp.GetBuffer( strTemp.GetLength() );
			m_lstExpression.InsertItem( &lvItem );

			strTemp.ReleaseBuffer();
			strTemp.Empty();

			// Add ExpressionPiece

			lvItem.mask = LVIF_TEXT;
			lvItem.iSubItem = 1;
			strTemp = arrText.GetAt( i );
			lvItem.pszText = strTemp.GetBuffer( strTemp.GetLength() );
			m_lstExpression.SetItem( &lvItem );

			strTemp.ReleaseBuffer();
		}

		m_lstExpression.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
		m_lstExpression.SetColumnWidth( 1, LVSCW_AUTOSIZE_USEHEADER );

		// Add Errors

		m_lstErrors.DeleteAllItems();

		OclCommon::ExceptionPool poolExceptions = ( iPos >= iDefinitions ) ? m_vecConstraints[ iPos - iDefinitions ]->GetExceptions() : m_vecConstraintDefinitions[ iPos ]->GetExceptions();
		for ( i = 0 ; i < poolExceptions.Size() ; i++ ) {
			OclCommon::Exception ex = poolExceptions.GetAt( i );

			// Add Code

			lvItem.mask = LVIF_IMAGE | LVIF_NORECOMPUTE | LVIF_TEXT;
			if ( i == 0 ) {
				lvItem.mask = lvItem.mask | LVIF_STATE;
				lvItem.state = LVIS_SELECTED;
			}
			lvItem.iItem = m_lstErrors.GetItemCount();
			lvItem.iSubItem = 0;
			lvItem.iImage = ex.GetType();
			lvItem.lParam = i;
			CString strTemp;
			if ( ex.GetCode() == -1 )
				strTemp = "None";
			else
				strTemp.Format( "%d", ex.GetCode() );
			lvItem.pszText = strTemp.GetBuffer( strTemp.GetLength() );
			m_lstErrors.InsertItem( &lvItem );

			strTemp.ReleaseBuffer();
			strTemp.Empty();

			// Add Line

			lvItem.mask = LVIF_TEXT;
			lvItem.iSubItem = 1;
			if ( ex.GetLine() < 0 )
				strTemp = "?";
			else
				strTemp.Format( "%d", ex.GetLine() );
			lvItem.pszText = strTemp.GetBuffer( strTemp.GetLength() );
			m_lstErrors.SetItem( &lvItem );

			strTemp.ReleaseBuffer();
			strTemp.Empty();

			// Add Column

			/*
			// This column is commented out because this features is not implemented yet
			// Be careful with uncommenting -> column and subitem numbers

			lvItem.iSubItem = 2;
			strTemp.Format( "%d", ex.GetColumn() );
			lvItem.pszText = strTemp.GetBuffer( strTemp.GetLength() );
			m_lstErrors.SetItem( &lvItem );

			strTemp.ReleaseBuffer();
			strTemp.Empty();
			*/

			// Add Message

			lvItem.iSubItem = 2;
			strTemp = GME::Convert( ex.GGetMessage() );
			lvItem.pszText = strTemp.GetBuffer( strTemp.GetLength() );
			m_lstErrors.SetItem( &lvItem );

			strTemp.ReleaseBuffer();
		}

		m_lstErrors.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
		m_lstErrors.SetColumnWidth( 1, LVSCW_AUTOSIZE_USEHEADER );
		m_lstErrors.SetColumnWidth( 2, LVSCW_AUTOSIZE_USEHEADER );

		/*
		// This column is commented out because this features is not implemented yet
		// Be careful with uncommenting -> column numbers

		m_lstErrors.SetColumnWidth( 3, LVSCW_AUTOSIZE_USEHEADER );
		*/

		if ( poolExceptions.IsEmpty() )
			ASSERT( 0 );
		else {
			m_iSelectedError = 0;
			DisplayErrorItem();
		}
	}

	void CSyntacticSemanticDialog::SortConstraints( int iColumn )
	{
		SortStruct sortInfo;
		sortInfo.pDialog = this;
		sortInfo.iColumn = iColumn;
		m_lstConstraints.SortItems( CSyntacticSemanticDialog::CompareConstraint, ( LPARAM ) ( &sortInfo ) );

		m_iSelected = std::min<long>( std::max<long>( m_lstConstraints.GetSelectionMark(), 0 ), m_lstConstraints.GetItemCount() - 1 );
	}

	int CALLBACK CSyntacticSemanticDialog::CompareConstraint( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
	{
		SortStruct* pSortInfo = ( SortStruct* ) lParamSort;

		CString strCtx1, strName1; int eType1;
		pSortInfo->pDialog->GetConstraintRow( lParam1, eType1, strCtx1, strName1 );

		CString strCtx2, strName2; int eType2;
		pSortInfo->pDialog->GetConstraintRow( lParam2, eType2, strCtx2, strName2 );

		if ( pSortInfo->iColumn == 0 ) {
			if ( eType1 < 4 )
				eType1 += 10;
			if ( eType2 < 4 )
				eType2 += 10;
			return ( eType1 < eType2 ) ? -1 : ( eType1 > eType2 ) ? 1 : 0;
		}
		if ( pSortInfo->iColumn == 1 )
			return strcmp( strCtx1, strCtx2 );
		return strcmp( strName1, strName2 );
	}
