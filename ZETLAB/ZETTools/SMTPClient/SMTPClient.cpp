#include "SMTPClient.h"

#include "WSockHelpers.h"
#include "SockByteStream.h"
#include "SSLContext.h"
#include "SSLByteStream.h"
#include "Smtp.h"

inline std::wstring CSMTPClient::to_wstring(const std::string& str)
{
	const std::string::value_type* s = str.c_str();

	int convert_size = ::MultiByteToWideChar(CP_ACP, 0, s, -1, 0, 0);
	std::wstring utf16_str(convert_size, ' ');
	convert_size = ::MultiByteToWideChar(CP_ACP, 0, s, -1, &utf16_str[0], utf16_str.size());
	utf16_str.resize(utf16_str.size()-1);
	return utf16_str;
};

CSMTPClient::CSMTPClient(std::string    SMTPServer, 
						 bool           UseSSL,
						 unsigned short SMTPPort, 
						 std::string    SMTPUser, 
						 std::string    SMTPPass, 
						 std::string    SMTPFrom,
						 std::string    SMTPTo)
						 : m_messageSubject("Empty subject")
						 , m_messageString("")
{
	m_SMTPServer = SMTPServer;
	m_UseSSL     = UseSSL;
	m_SMTPPort   = SMTPPort;
	m_SMTPUser   = SMTPUser;
	m_SMTPPass   = SMTPPass;
	m_SMTPFrom   = SMTPFrom;
	m_SMTPTo     = SMTPTo;
}

void CSMTPClient::SetSMTPServer(std::string SMTPServer)
{
	m_SMTPServer = m_SMTPServer;
}

void CSMTPClient::SetUseSSL(bool UseSSL)
{
	m_UseSSL     = m_UseSSL;
}
void CSMTPClient::SetSMTPPort(unsigned short SMTPPort)
{
	m_SMTPPort   = m_SMTPPort;
}
void CSMTPClient::SetSMTPUser(std::string SMTPUser)
{
	m_SMTPUser   = m_SMTPUser;
}
void CSMTPClient::SetSMTPPass(std::string SMTPPass)
{
	m_SMTPPass   = SMTPPass;
}
void CSMTPClient::SetSMTPFrom(std::string SMTPFrom)
{
	m_SMTPFrom   = SMTPFrom;
}
void CSMTPClient::SetSMTPTo(std::string SMTPTo)
{
	m_SMTPTo     = SMTPTo;
}

int CSMTPClient::sendEmail()
{
	//очищаем информацию об ошибке
	m_sLastError.clear();

	UseWinSock ws(2, 2);
	if( !ws )
	{
		m_sLastError = "0x1. Can not create UseWinSock";
		return -1;
	}

	u_long srvr_bin_addr = GetBinAddr(m_SMTPServer.c_str());
	if( srvr_bin_addr == INADDR_NONE) 
	{
		m_sLastError = "0x2. There is no network connection";
		return -1;
	}

	sockaddr_in ssock_addr;
	ssock_addr.sin_family = AF_INET;
	ssock_addr.sin_port = htons(m_SMTPPort);
	ssock_addr.sin_addr.s_addr = srvr_bin_addr;

	SOCKET hServerSock = socket(AF_INET, SOCK_STREAM, 0);
	if( INVALID_SOCKET == hServerSock )
	{
		m_sLastError = "0x3. INVALID_SOCKET";
		return -1;
	}
	AutoCloseSocket CloseSocket(hServerSock);

	int ws_ret = connect(hServerSock, (sockaddr *)&ssock_addr, sizeof(ssock_addr));
	if( SOCKET_ERROR != ws_ret ) 
	{
		AutoShutdownConnection ShutdownConnection(hServerSock, SD_BOTH);
		//std::cout<<"Connected to: "<<SMTPServer<<" port:"<<SMTPPort<<std::endl;

		try
		{
			LSockByteStream ss(hServerSock);
			LSSLContext SSLCtx;//in case we will need SSL
			LSSLByteStream ssls(&ss);//in case we will need SSL
			if( m_UseSSL ) 
			{
				SSLCtx.AcquireCredentials();
				SSLCtx.Handshake(&ss, to_wstring(m_SMTPServer).c_str() );
				ssls.SetSSLContext(&SSLCtx);
			}

			//SMTP protocol conversation
			LSmtp smtp( m_UseSSL ? (LByteStream*)&ssls : (LByteStream*)&ss );
			smtp.Greeting();
			//std::cout<<"Server greeting: "<<smtp.GetReply();
			std::vector<std::string> Extensions;
			smtp.EHLO("anonymous", &Extensions);
			if( smtp.AUTH_PLAIN(std::string(), m_SMTPUser, m_SMTPPass) ) 
			{
				std::string Message;
				Message += "from: "+m_SMTPFrom+"\r\n";
				Message += "to:"+m_SMTPTo+"\r\n";
				Message += "subject: ";
				Message += m_messageSubject;
				Message += "\r\n";
				Message += "Content-type: text/html; charset=windows-1251\r\n";
				Message += "<html><body>";
				Message += m_messageString;
				Message += "</body></html>";
				Message += "\r\n.\r\n";
				if( smtp.MAIL(m_SMTPFrom)&&
				    smtp.RCPT(m_SMTPTo)&&
				    smtp.DATA(&Message[0], Message.size()) )
				{
					//std::cout<<"Success!"<<std::endl;
				} else 
				{
					m_sLastError = "0x5. message failed";
					smtp.QUIT();
					return -1;
					//std::cout<<"Failed!"<<std::endl;
				}
			}
			smtp.QUIT();
		}
		catch(LSmtp::BAD_SMTP_PROTOCOL&)
		{
			m_sLastError = "0x6. some problem with SMTP";
			//some problem with SMTP
			return -1;
		}
		catch(LBS_EOF&)
		{   
			m_sLastError = "0x7. connection closed";
			//connection closed.
			return -1;
		}
		catch(LSSL_SEC_E&)
		{
			m_sLastError = "0x8. some problems with ssl";
			//some problems with ssl.
			return -1;
		}
		catch(int)
		{
			m_sLastError = "0x9. some problem with socket io";
			//some problem with socket io.
			return -1;
		}
	}
	else
	{
		m_sLastError = "0x4. SOCKET_ERROR";
		return -1;
	}

	return 0;
}

void CSMTPClient::clearMessageString()
{
	m_messageString.clear();
}

void CSMTPClient::addMessageString(std::string addSring)
{
	m_messageString += addSring;	//добавляем строку
	//m_messageString += "<br>";  //и сразуже ставим межстрочный разделитель
}

std::string CSMTPClient::getMessageString()
{
	return m_messageString;
}

void CSMTPClient::addSubject(std::string addSring)
{
	m_messageSubject = addSring;	//добавляем строку
}

std::string CSMTPClient::GetLastError()
{
	return m_sLastError;
}

CSMTPClient::~CSMTPClient(void)
{
}
