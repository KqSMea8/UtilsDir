#pragma once

#include <FTDI/FTDI.h>
#include <Exception/Exception.h>
#include <ZETModbus/CZETModbus.h>
#include <Zet7xxxDefinitions.h>

class CZetFtdi : public CFTDIdev
{
public:
	CZetFtdi(unsigned int dev_idx);
	virtual ~CZetFtdi();

	BAUD_RATE		GetBaudRate() {return m_Baudrate;};
	void			SetBaudrate(BAUD_RATE br);

	bool			GetParity() {return m_Parity;};
	void			SetParity(bool parity);

	void			GetLicense(DWORD lic[3]);
	void			SetLicense(DWORD lic[3]);

	CString			GetComment() {return m_Comment;};
	void			SetComment(CString comment) {m_Comment = comment;};

	ZET7XXX_DEVICE_TYPE	GetType(void);

	virtual void	SaveParams(void);

private:
	BAUD_RATE 	m_Baudrate;					//Скорость передачи данных
	bool		m_Parity;					//Четность

	struct
	{
		DWORD field_0;
		DWORD field_1;
		DWORD field_2;
	}			m_License;					//Лицензионные ключи

	CString		m_Comment;					//
};

class СZetFtdiException : public Exception
{
public:
	СZetFtdiException(unsigned int err) : Exception(err) {};
}; 
