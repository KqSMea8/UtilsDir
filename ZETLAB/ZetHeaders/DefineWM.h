//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
// ������� ���������
//------------------------------------------------------------------------------
#define WM_GET_HWND_OF_PANEL WM_USER + 100	//������� ���������� ���� ZET-������
#define WM_SEND_CHILD_HWND	WM_USER + 101	//������� � ZET-������ ��������� �� ��������/�������� ���. ����
#define WM_SEND_STRUCTURE	WM_USER + 102	//��������� � ZET-������ ���������
#define WM_SHOW_PROGRAM		WM_USER + 103	//��������� ZET- ���������� �� ��� ���� ��������� � ����

#define WM_SEND_ERROR		WM_USER + 113	//��������� ZET-������ �� ������
#define WM_SEND_WARNING		WM_USER + 114	//��������� ZET-������ � ��������������

#define WM_SEND_CHILD_END	WM_USER + 151	//ZET-������ - � ���������� ������
#define WM_UNIT_APP			WM_USER + 151	// UNIT
#define WM_UNIT2_APP		WM_USER + 152	// UNIT
#define WM_FILE_CFG_CHANGE	WM_USER + 153	// UNIT

#define WM_SEND_STARTSTOPDEBUG    WM_USER + 402

#define WM_NEW_DATA_SRV		WM_USER + 501	//������ ������ ������
#define WM_ERROR_READ_SRV	WM_USER + 502	//������ ��� ������ ������ ������� SRV
#define WM_ERROR_CALC_SRV	WM_USER + 503	//������ ��� �������� ������ ������� SRV
#define WM_END_JOB_SRV		WM_USER + 504	//������ ��� ������� ������
#define WM_ERROR_THREAD		WM_USER + 505	//������ �������� ������
#define WM_END_CALC			WM_USER + 506	//���������� ��������
#define WM_ERROR_THREAD_DTU WM_USER + 507
#define WM_WRITE_DTU		WM_USER + 508
#define WM_NEW_PARAM		WM_USER + 509	//����� ���������
#define WM_PROC_STATUS		WM_USER + 509	//������ ��������� (��������, ���, ���������, �������)
#define WM_NEW_MODE			WM_USER + 510	//����� �����

#define WM_FAULT			WM_USER + 511	//�������������
#define WM_EVENT_START		WM_USER + 511	//������ �������
#define WM_EVENT_END		WM_USER + 512	//���������� �������
#define WM_END_WAIT			WM_USER + 513	//���������� ��������
#define WM_PROGRAM_STOP		WM_USER + 513	//���������� ������ ���������
#define WM_SEND_HWND		WM_USER + 514	//������ �� �������� ������ HWND
// ��������� ������� �� ������ (� WPARAM ����� ������)
#define WM_STOP_TIME_CHANNEL	WM_USER + 515
//------------------------------------------------------------------------------
