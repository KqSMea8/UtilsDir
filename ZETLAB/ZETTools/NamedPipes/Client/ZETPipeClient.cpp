#include <Windows.h>
#include "NamedPipes\Client\zetPipeClient.h"

CZetPipeClient::CZetPipeClient()
{

}

CZetPipeClient::CZetPipeClient(_In_ LPTSTR lpszPipeName_)
{
	m_lpszPipeCommPipeName = lpszPipeName_;
}

CZetPipeClient::~CZetPipeClient()
{
    Disconnect();
}

BOOL CZetPipeClient::Connect(_In_ DWORD dwMaxTimeOut_ /*= DEFAULT_CONNECTION_TIMEOUT*/)
{
    BOOL  bPipeConnectedResult = FALSE;   // ��������� ����������� � ������������ ������
    DWORD dwPipeOpenMode = PIPE_READMODE_MESSAGE;   // ���� �������� ������������ ������ �� ������ � ������, � ������ ������ �����������

    // �������� ������� ����������� �����
    m_hPipe = CreateFile(m_lpszPipeCommPipeName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    // � ������ ���� ����������� ����� �� ���������� ������� �� ������� ���� ��� �� �����, ������� ��� �������� ���������� �������
    if (m_hPipe == INVALID_HANDLE_VALUE && GetLastError() == ERROR_PIPE_BUSY)
    {
        WaitNamedPipe(m_lpszPipeCommPipeName, dwMaxTimeOut_);
        m_hPipe = CreateFile(m_lpszPipeCommPipeName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);
    }

    // ���� ����� ������� �������
    if (m_hPipe != INVALID_HANDLE_VALUE)
    {
        bPipeConnectedResult = SetNamedPipeHandleState(
            m_hPipe,
            &dwPipeOpenMode,
            NULL,
            NULL);
    }

    return bPipeConnectedResult;
}

BOOL CZetPipeClient::Disconnect()
{
    BOOL bDisconnectResult = FALSE;

    bDisconnectResult = CloseHandle(m_hPipe);

    return bDisconnectResult;
}

BOOL CZetPipeClient::Reconnect(_In_ DWORD dwMaxTimeOut_ /*= DEFAULT_CONNECTION_TIMEOUT*/)
{
    BOOL bReconnectResult = FALSE;

    bReconnectResult = Disconnect();
    bReconnectResult = Connect(dwMaxTimeOut_);

    return bReconnectResult;
}

BOOL CZetPipeClient::SendData(_In_ LPVOID lpInData_, _In_ DWORD dwInDataSz_, _Out_opt_ DWORD* pdwSentDataSz_ /*= nullptr*/)
{
    BOOL bWriteResult = FALSE;   // ��������� ������ ������ � �����
    DWORD dwSentDataSz = 0;     // ���������� ���������� ����

    bWriteResult = WriteFile(m_hPipe, lpInData_, dwInDataSz_, &dwSentDataSz, NULL);
    if (dwInDataSz_ != dwSentDataSz)   // ���� ���������� ���������� ���� ���������� �� ����������, ������ ������ ������
        bWriteResult = FALSE;

    if (pdwSentDataSz_ != nullptr)   // ���� ������������ ����� ������ ������� ���� ����������
        *pdwSentDataSz_ = dwSentDataSz;

    return bWriteResult;
}

BOOL CZetPipeClient::RequestData(_In_ LPVOID lpInData_, _In_ DWORD dwInDataSz_, _In_ LPVOID lpOutData_, _In_ DWORD dwOutDataSz_, _Out_ DWORD* pdwReadDataSz_ /*= nullptr*/)
{
    BOOL  bRequestResult = FALSE;   // ��������� �� ������ ������
    DWORD dwSentDataSz = 0;         // ������ ���������� ������
    DWORD dwReadDataSz = 0;         // ������ ��������� ������

    // ���������� ����� ��� ������� ������
    bRequestResult = WriteFile(m_hPipe, lpInData_, dwInDataSz_, &dwSentDataSz, NULL);

    // ������� ����� �� ����� �������
    if (bRequestResult && (dwInDataSz_ == dwSentDataSz))
        bRequestResult = ReadFile(m_hPipe, lpOutData_, dwOutDataSz_, &dwReadDataSz, NULL);
    else
        bRequestResult = FALSE;

    if (pdwReadDataSz_ != nullptr)   // ���� ������������ ����� ������ ������� ������ ���� �������� � �����
        *pdwReadDataSz_ = dwReadDataSz;

    return bRequestResult;
}