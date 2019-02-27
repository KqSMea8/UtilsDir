#pragma once

template< typename T, UINT A >
class CAccelerated : public T
{
public:
    CAccelerated()
    {
        m_hAccel = LoadAccelerators(
            AfxGetInstanceHandle(),
            MAKEINTRESOURCE(A)
        );
    };
    CAccelerated( UINT nIDTemplate, CWnd* pParent ) : T(nIDTemplate, pParent)
    {
        m_hAccel = LoadAccelerators(
            AfxGetInstanceHandle(),
            MAKEINTRESOURCE(A)
        );
    };

    virtual ~CAccelerated()
    {
    };

protected:
    virtual BOOL PreTranslateMessage( MSG* pMsg )
    {
        if (m_hAccel)
        {
            // CWinApp patch
            #ifdef __APP__
                HWND m_hWnd = m_pMainWnd->m_hWnd;
            #endif

            if (::TranslateAccelerator( m_hWnd, m_hAccel, pMsg )) 
                return TRUE;
        }

        return T::PreTranslateMessage( pMsg );
    };

protected:
    HACCEL m_hAccel;
};
