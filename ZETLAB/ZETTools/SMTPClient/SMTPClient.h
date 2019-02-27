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
	
	std::string    m_SMTPServer; //����� smtp �������, �������� smtp.timeweb.ru (�� ������ 04.10.2012)
	bool           m_UseSSL;     //������������� SSL 
	unsigned short m_SMTPPort;   //���� ����������, �������� 25 (��� ZETLab.ru � timeweb �� ������ 04.10.2012)
						 
	std::string    m_SMTPUser;   //��� ������������, �������� ErrorMessage@zetlab.ru
	std::string    m_SMTPPass;   //������, �������� zgfST83V (�� ������ 04.10.2012)
	std::string    m_SMTPFrom;   //��������� � SMTPUser
	std::string    m_SMTPTo;     //����� ���������� ���������, �������� ProgrammSupport@zetlab.ru

	std::string    m_messageString; //������, ���������� �������� ��������
	std::string    m_messageSubject;//������, ���������� ��������� ������
	std::string    m_sLastError;	//��������� ������
	inline std::wstring to_wstring(const std::string& str);
};

