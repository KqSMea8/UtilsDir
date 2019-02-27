#pragma once

#include <iostream>

class CSMTPClient
{
public:
	CSMTPClient(std::string    SMTPServer, 
				bool           UseSSL,
				unsigned short SMTPPort, 
				std::string    SMTPUser, 
				std::string    SMTPPass, 
				std::string    SMTPFrom,
				std::string    SMTPTo);
	~CSMTPClient(void); 

	void SetSMTPServer(std::string SMTPServer);
	void SetUseSSL(bool UseSSL);
	void SetSMTPPort(unsigned short SMTPPort);
	void SetSMTPUser(std::string SMTPUser);
	void SetSMTPPass(std::string SMTPPass);
	void SetSMTPFrom(std::string SMTPFrom);
	void SetSMTPTo(std::string SMTPTo);

	int sendEmail();
	void clearMessageString();
	void addSubject(std::string addSring);
	std::string getMessageString();
	void addMessageString(std::string addSring);
	std::string GetLastError();
private:
	
	std::string    m_SMTPServer; //адрес smtp сервера, например smtp.timeweb.ru (на момент 04.10.2012)
	bool           m_UseSSL;     //использование SSL 
	unsigned short m_SMTPPort;   //порт соединения, например 25 (для ZETLab.ru в timeweb на момент 04.10.2012)
						 
	std::string    m_SMTPUser;   //имя пользователя, например ErrorMessage@zetlab.ru
	std::string    m_SMTPPass;   //пароль, например zgfST83V (на момент 04.10.2012)
	std::string    m_SMTPFrom;   //совпадает с SMTPUser
	std::string    m_SMTPTo;     //адрес получателя сообщения, например ProgrammSupport@zetlab.ru

	std::string    m_messageString; //строка, содержащая основное послание
	std::string    m_messageSubject;//строка, содержащая заголовок письма
	std::string    m_sLastError;	//последняя ошибка
	inline std::wstring to_wstring(const std::string& str);
};

