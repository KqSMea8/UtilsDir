#include "stdafx.h"
#include "../../../../application/application.h"
#include "tag_properties_dlg.h"

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

CTagPropertiesDlg::CTagPropertiesDlg( CWnd* pParent ) :
    CAnchoredDialog(CTagPropertiesDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon( IDI_TAG_PROPERTIES );
    m_value = _T("<>");
    m_qualityIndex = 0;
}

void CTagPropertiesDlg::DoDataExchange( CDataExchange* pDX )
{
    CAnchoredDialog::DoDataExchange( pDX );

    DDX_Text   ( pDX, IDC_VALUE,   m_value );
    DDX_CBIndex( pDX, IDC_QUALITY, m_qualityIndex );
}

BEGIN_MESSAGE_MAP( CTagPropertiesDlg, CAnchoredDialog )
END_MESSAGE_MAP()

BEGIN_ANCHOR_MAP( CTagPropertiesDlg )
    ANCHOR_MAP_ENTRY( IDC_LABEL_VALUE,   ANF_TOPLEFT )
    ANCHOR_MAP_ENTRY( IDC_VALUE,         ANF_TOPLEFT | ANF_BOTTOMRIGHT )
    ANCHOR_MAP_ENTRY( IDC_LABEL_QUALITY, ANF_BOTTOMLEFT )
    ANCHOR_MAP_ENTRY( IDC_QUALITY,       ANF_LEFTRIGHT | ANF_BOTTOM )
    ANCHOR_MAP_ENTRY( IDOK,              ANF_BOTTOMRIGHT )
    ANCHOR_MAP_ENTRY( IDCANCEL,          ANF_BOTTOMRIGHT )
END_ANCHOR_MAP()

BOOL CTagPropertiesDlg::OnInitDialog()
{
    CAnchoredDialog::OnInitDialog();

    // Localization
    LOCALIZED_WINDOW ( IDD_TAG_PROPERTIES );
    LOCALIZED_CONTROL( IDC_LABEL_VALUE );
    LOCALIZED_CONTROL( IDC_LABEL_QUALITY );
    LOCALIZED_CONTROL( IDOK );
    LOCALIZED_CONTROL( IDCANCEL );

    // Icon
    SetIcon( m_hIcon, FALSE );

    InitValue();

    InitQuality();

    return TRUE;
}

INT_PTR CTagPropertiesDlg::DoModal( const item_list& tags )
{
    ASSERT( !tags.empty() );

    _tags = tags;

    return CAnchoredDialog::DoModal();
}

void CTagPropertiesDlg::InitValue()
{
    CEdit*  edit  = reinterpret_cast< CEdit* >(GetDlgItem( IDC_VALUE ));
    CString value = _tags[0]->getValueStr().c_str();

    for (auto it = _tags.begin(), end = _tags.end(); it != end; ++it)
    {
        if ((*it)->getValueStr().c_str() != value)
        {
            value = _T("<>");
            break;
        }
    }

    edit->SetWindowText( value );
}

void CTagPropertiesDlg::InitQuality()
{
    CComboBox* combo   = reinterpret_cast< CComboBox* >(GetDlgItem( IDC_QUALITY ));
    WORD       quality = _tags[0]->getQuality();

    for (auto it = _tags.begin(), end = _tags.end(); it != end; ++it)
    {
        if ((*it)->getQuality() != quality)
        {
            quality = -1;
            break;
        }
    }

    combo->SetCurSel( QualityToIndex( quality ) );
}

int CTagPropertiesDlg::QualityToIndex( const WORD& quality ) const
{
    for (int i = 0, s = sizeof(_qualities) / sizeof(_qualities[0]); i < s; ++i)
    {
        if (quality == _qualities[i])
            return i;
    }

    return -1;
}
