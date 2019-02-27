#ifndef localizator_h__
#define localizator_h__

#include <Dialog_ZET/Translate.h>

#define DECLARE_LOC_SUPPORT() \
	CString csLocalization;

#ifdef _DEBUG
#define LOCALIZED_WINDOW( IDD ) \
	csLocalization = TranslateResourse( IDS_##IDD ); \
	csLocalization = L"!!!DEBUG!!! " + csLocalization; \
	SetWindowText( csLocalization );
#else
#define LOCALIZED_WINDOW( IDD ) \
	csLocalization = TranslateResourse( IDS_##IDD ); \
	SetWindowText( csLocalization );
#endif

#define LOCALIZED_CONTROL( IDC ) \
	csLocalization = TranslateResourse( IDS_##IDC ); \
	GetDlgItem( IDC )->SetWindowText( csLocalization );

#define LOCALIZED_MENU_POPUP( MENU, ID, IDS ) \
	csLocalization = TranslateResourse( IDS ); \
	MENU->ModifyMenu( ID, MF_BYPOSITION | MF_STRING, ID, csLocalization );

#define LOCALIZED_MENU_ITEM( MENU, ID ) \
	csLocalization = TranslateResourse( IDS_##ID ); \
	MENU->ModifyMenu( ID, MF_BYCOMMAND | MF_STRING, ID, csLocalization );

#define TranslateResourse( iD ) \
	CString( TranslateResourse(iD) )

#define MAIN_MENU   GetMenu()

#endif // localizator_h__
