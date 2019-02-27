#pragma once

#include "../AnchoredDialog/AnchoredDialog.h"

#include <afxcview.h>

class CSplittedDialog : public CAnchoredDialog
{
public:
    CSplittedDialog( UINT nIDTemplate, CWnd* pParent = NULL ) :
        CAnchoredDialog(nIDTemplate, pParent) {};

    virtual ~CSplittedDialog()
    {
    };

public:
    BOOL SplitWindow( int nRows, int nCols )
    {
        return m_splitter.CreateStatic( m_frame, nRows, nCols );
    };

    BOOL CreateView( int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit )
    {
        return m_splitter.CreateView( row, col, pViewClass, sizeInit, &m_context );
    };

    void GetRowInfo( int row, int& cyCur, int& cyMin )
    {
        m_splitter.GetRowInfo( row, cyCur, cyMin );
    }

    void SetRowInfo( int row, int cyIdeal, int cyMin )
    {
        m_splitter.SetRowInfo( row, cyIdeal, cyMin );
        m_splitter.RecalcLayout();
    };

    void GetColumnInfo( int col, int& cxCur, int& cxMin )
    {
        m_splitter.GetColumnInfo( col, cxCur, cxMin );
    }

    void SetColumnInfo( int col, int cxIdeal, int cxMin )
    {
        m_splitter.SetColumnInfo( col, cxIdeal, cxMin );
        m_splitter.RecalcLayout();
    };

protected:
    PTM_WARNING_DISABLE
    virtual const AFX_MSGMAP* GetMessageMap() const
    {
        return GetThisMessageMap();
    }

    static const AFX_MSGMAP* PASCAL GetThisMessageMap()
    {
        typedef CSplittedDialog ThisClass;
        typedef CAnchoredDialog TheBaseClass;
        static const AFX_MSGMAP_ENTRY _messageEntries[] = {
            // Message map
            ON_WM_CREATE()
            ON_WM_SIZE()

            {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
        };

        static const AFX_MSGMAP messageMap = {
            &TheBaseClass::GetThisMessageMap, &_messageEntries[0]
        };

        return &messageMap;
    }
    PTM_WARNING_RESTORE

    virtual void DoDataExchange( CDataExchange* pDX )
    {
        CAnchoredDialog::DoDataExchange( pDX );
    }

    virtual BOOL OnInitDialog()
    {
        CAnchoredDialog::OnInitDialog();

        return TRUE;
    }

    afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct )
    {
        if (-1 == CAnchoredDialog::OnCreate( lpCreateStruct ))
            return -1;

        m_context.m_pNewViewClass   = NULL;
        m_context.m_pCurrentDoc     = NULL;
        m_context.m_pNewDocTemplate = NULL;
        m_context.m_pLastView       = NULL;
        m_context.m_pCurrentFrame   = NULL;

        CString frameClass = AfxRegisterWndClass(
            CS_VREDRAW | CS_HREDRAW,
            ::LoadCursor( NULL, IDC_ARROW ),
            ::GetSysColorBrush( COLOR_BTNFACE ),
            ::LoadIcon( NULL, IDI_APPLICATION )
        );

        m_frame = new CFrameWnd;
        m_frame->Create(
            frameClass,
            _T(""),
            WS_CHILD | WS_VISIBLE,
            CRect(),
            this,
            NULL,
            0,
            &m_context
        );

        return 0;
    };

    afx_msg void OnSize( UINT nType, int cx, int cy )
    {
        CAnchoredDialog::OnSize( nType, cx, cy );

        CRect rc;

        GetDlgItem( IDC_FRAME )->GetWindowRect( &rc );
        ScreenToClient( &rc );

        m_frame->SetWindowPos(
            GetDlgItem( IDC_FRAME ),
            rc.left,
            rc.top,
            rc.Width(),
            rc.Height(),
            SWP_NOACTIVATE
        );
    };

protected:
    CSplitterWndEx m_splitter;
    CCreateContext m_context;
    CFrameWnd*     m_frame;
};
