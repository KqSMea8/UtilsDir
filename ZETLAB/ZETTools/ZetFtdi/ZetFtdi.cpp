#include "ZetFtdi.h"

CZetFtdi::CZetFtdi(unsigned int dev_idx) : CFTDIdev(dev_idx)
{
	BYTE buff[36];
	DWORD tmp;
	wchar_t w_str[21];

	ReadUserData(buff, 0, 36);
	
	memcpy_s(&tmp, sizeof(DWORD), buff, sizeof(DWORD));
	SetBaudrate((BAUD_RATE) (tmp & 0xFFFFFFFE));
	SetParity(tmp & 0x00000001);

	memcpy_s(&m_License.field_0, sizeof(DWORD), &buff[4], sizeof(DWORD));
	memcpy_s(&m_License.field_1, sizeof(DWORD), &buff[8], sizeof(DWORD));
	memcpy_s(&m_License.field_2, sizeof(DWORD), &buff[12], sizeof(DWORD));
	
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR) &buff[16], 20, w_str, 20);
	w_str[20] = 0x00;
	m_Comment = w_str;
}

CZetFtdi::~CZetFtdi()
{
}

void CZetFtdi::SetBaudrate(BAUD_RATE br)
{
	CFTDIdev::SetBaudrate(m_Baudrate = br);
}

void CZetFtdi::SetParity(bool parity)
{
	CFTDIdev::SetParity(parity);
	m_Parity = parity;
}

void CZetFtdi::GetLicense(DWORD lic[3])
{
	lic[0] = m_License.field_0;
	lic[1] = m_License.field_1;
	lic[2] = m_License.field_2;
}

void CZetFtdi::SetLicense(DWORD lic[3])
{
	m_License.field_0 = lic[0];
	m_License.field_1 = lic[1];
	m_License.field_2 = lic[2];
}

ZET7XXX_DEVICE_TYPE CZetFtdi::GetType(void)
{
	CString name(GetDevName());
	
	if(name == _T("ZET7070")) return ZET_7070;
	else if(name == _T("ZET7174")) return ZET_7174;
	else if(name == _T("ZETKey")) return ZET_KEY;
	else return UNKNOWN;
}

void CZetFtdi::SaveParams(void)
{
	BYTE buff[36];
	DWORD tmp;
	memset(buff, 0x00, 36);

	tmp = m_Baudrate;
	if(m_Parity)
		tmp |= 0x00000001;

	memcpy_s(buff, sizeof(DWORD), &tmp, sizeof(DWORD));

	memcpy_s(&buff[4], sizeof(DWORD), &m_License.field_0, sizeof(DWORD));
	memcpy_s(&buff[8], sizeof(DWORD), &m_License.field_1, sizeof(DWORD));
	memcpy_s(&buff[12], sizeof(DWORD), &m_License.field_2, sizeof(DWORD));
	
	WideCharToMultiByte(CP_ACP, 0, m_Comment.GetBuffer(), 20, (LPSTR) &buff[16], 20, NULL, NULL);
		
	WriteUserData(buff, 0, 36);

	CFTDIdev::SaveParams();
}