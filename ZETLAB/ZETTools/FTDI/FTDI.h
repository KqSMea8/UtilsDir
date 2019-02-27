#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <list>
#include <afx.h>

#include <Exception/Exception.h>
#include <ZETModbus/CZETModbus.h>
#include <Zet7xxxDefinitions.h>

#include <ftd2xx.h>

void GetFTDIDevsInfo(std::list<FT_DEVICE_LIST_INFO_NODE>& info_list);		//Получить информацию об устройствах FTDI, подключенных к ПК
void RegisterFTDINotification(HANDLE hWnd);									//Регистрация обработчика автоопределения добавления/удаления устройства

#define MANUFACTERER_LEN		7			//Количество символов под имя производителя (включая нулевой символ конца строки)
#define DESCRIPTION_LEN			9			//Количество символов под название устройства (включая нулевой символ конца строки)
#define SERIAL_LEN				16			//Количество символов под серийный номер (включая нулевой символ конца строки)

#define UA_SIZE					36			//Размер пользовательской области памяти (зависит от MANUFACTERER_LEN, DESCRIPTION_LEN и SERIAL_LEN)

class CFTDIdev : public CZETModbus
{
public:
	CFTDIdev(unsigned int dev_idx);
	virtual ~CFTDIdev();
	

	BYTE			GetIdx() {return m_Idx;};			//Определение индекса устройства
	DWORD			GetChipID() {return m_ChipID;};		//Определение уникального ChipID
	WORD			GetVID() {return m_VID;};												
	WORD			GetPID() {return m_PID;};	
	CString			GetVendor() {return m_Vendor;};
	CString			GetDevName() {return m_DevName;};
	CString			GetSerial() {return m_Serial;};

	void			SetVID(WORD vid) {m_VID = vid;};												
	void			SetPID(WORD pid) {m_PID = pid;};												
	void			SetVendor(CString& vendor) {m_Vendor = vendor;};												
	void			SetDevName(CString& dev_name) {m_DevName = dev_name;};												
	void			SetSerial(CString& serial);	

	virtual void	SaveParams(void);
	bool			IsAvailable(void);			//Определяем доступно ли устройство в данный момент

	void			FreeBuff() {FT_Purge(m_Handle, FT_PURGE_RX | FT_PURGE_TX);};

protected:
	void		 	ReadUserData(void *pBuff, unsigned int start_byte, unsigned int num_bytes);
	void			WriteUserData(void *pBuff, unsigned int start_byte, unsigned int num_bytes);

	virtual void	Recv(BYTE* pBuff, unsigned int num_bytes);
	virtual void	Send(BYTE* pBuff, unsigned int num_bytes);

	void			SetBaudrate(BAUD_RATE br);
	void			SetParity(bool parity_flag);

private:
	BYTE			m_Idx;									//Индекс устройства
	FT_HANDLE		m_Handle;								//Хэндл текущего FTDI
	DWORD			m_ChipID;
	WORD			m_VID;
	WORD			m_PID;
	CString			m_Vendor;
	CString			m_DevName;
	CString			m_Serial;
	
	BYTE			m_UserData[UA_SIZE];

	void			ReadEEPROM();
	void			WriteEEPROM();
};

class FTDIException : public Exception
{
public:
	FTDIException(unsigned int err) : Exception(err) {};
}; 
