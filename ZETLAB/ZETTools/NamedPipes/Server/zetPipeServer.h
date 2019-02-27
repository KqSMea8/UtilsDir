// ����� ��� ������ ������� ����� ����������� ����� ����������� ������ (������)
// ������ 2.1.1 @ 2017/04/13
//
// ����� ��������� ������ �� �������� �� ����������� ������� � ������������ �� � ������� ������������ CallBack �������
// ������� ��������: ��� ������������� ��������� ���� ���������� (IOCompletionPort), � ���� ����� ��� ��������� Overlapped �� ���� ������� �� ��������� ����������� �������.
//                   ��������� ���� ���������� �����. ��� ����������� � ����, ��������� ��� ���� ����� �����. ��� ���������� �� ������������ ������ ��������� ���������.
//
// �������������:
// - ������������������� ��������� ������: 
//   CZetPipeServer pipeComm(*��� ������������ ������*, 
//                               *������� ���������� ���������� ������� �� ������������ ������*,
//                               *�������� ���������*);
//   CZetPipeServer pipeComm(L"\\\\.\\pipe\\testMyPipes", f, arg);

// - ������� ������� ��� ������� ������ ������������ ������:           Init()

// - ��� ���������� ������ � ��������� �� ������������ ������ �������: Close()

#ifndef zetpipecommand_server
#define zetpipecommand_server

#include <Windows.h>
#include <string>
#include <list>
#include <future>

typedef struct
{
    OVERLAPPED   oOverlap;        // ��������� OVERLAPPED ��� ������ � ������������ ��������
    HANDLE       hPipeInst;       // ����� ������������ ������
    LPVOID       pInBuff;         // ����� � ��������� �� ������������ ������ �������
    DWORD        dwInBuffSz;      // ������ �������� ������ �� ������������ ������
    LPVOID       pOutBuff;        // ����� � ��������� ������� �� ������������ ������
    DWORD        dwOutBuffSz;     // ������ ������������ ������ �� ������������ ������
    DWORD        dwState;         // ������� ��������� ������������ ������
    BOOL         bPendingIO;      // � ������ �������� �����������
} PIPE_CLIENT_STRUCT, *LPIPE_CLIENT_STRUCT;

class CZetPipeServer
{
// ���������
private:
    static const DWORD PIPE_CLIENT_DEFAULT_IO_BUFFER_SIZE = 4096;   // ������������ ������ �� ��������� ������ ��� ������ �������� �� ������������ ������
    static const DWORD PIPE_CLIENT_DEFAULT_FREE_COPY = 1;           // ������������ ���������� ��������� ����������� ����������� �������

    static const DWORD PIPE_CLIENT_STATE_CONNECTING = 0;   // ��������� ������������ ������ � ������ �������
    static const DWORD PIPE_CLIENT_STATE_READING = 1;      // ��������� ������������ ������ � ������ ������
    static const DWORD PIPE_CLIENT_STATE_WRITING = 2;      // ��������� ������������ ������ � ������ ������

// ������
private:
    HANDLE m_hIOCompletionPort = INVALID_HANDLE_VALUE;   // ���������� ����� ���������� (������� ������� Overlapped)
    DWORD  m_dwMaxPipeClientInBuffSz  = 0;               // ������ ������ ��� �������� ������
    DWORD  m_dwMaxPipeClientOutBuffSz = 0;               // ������ ������ ��� ��������� ������
    DWORD  m_dwMaxPipeClientFreeCopys = 0;               // ������������ ���������� ����������������� ��������� ����������� ����������� �������
    std::list<LPIPE_CLIENT_STRUCT> m_lPipeClients;       // ������ ����������� ��������� ����������� �������

protected:
    LPTSTR m_lpszPipeName = L"";      // ��� ������������ ������ ��� �������� ������

    LPVOID       m_lpArg = nullptr;   // ��������� �������� ������������ � ������� ����������
    // ���������� ���������� �������
    void(*m_fPipeClientHandler)(LPVOID lpInCommData_, DWORD dwInCommDataSz_, LPVOID lpOutCommData_, DWORD* pdwOutCommDataSz_, LPVOID lpArg_);

    std::thread* m_pthPipeClientThread = nullptr;   // ����� ������������ ������� �� ������ � ����������� �������

// �������
private:
    bool createPipeClient();                                  // ������� ����� ��������� ������������ ������ ��� �������� ����������� �������
    void deletePipeClient(LPIPE_CLIENT_STRUCT pipeClient_);   // ������� ��������� ������������ ������

public:
    CZetPipeServer();

    CZetPipeServer(
        LPTSTR pipeName_,                                   // ��� ������������ ������ ��� ������ � ���������
        void(*pF_)(LPVOID lpInData_, DWORD lpInDataSz_,   // ������� ���������� ���������� ������ �� ������������ ������
                    LPVOID lpOutData_, DWORD* lpOutDataSz_, LPVOID lpArg_),
        LPVOID lpArg_,                                        // �������� ������������ � ������� ����������
        DWORD  dwMaxPipeClientInBuffSz_      = PIPE_CLIENT_DEFAULT_IO_BUFFER_SIZE,   // ������ �������� ������
        DWORD  dwMaxPipeClientOutBuffSz_     = PIPE_CLIENT_DEFAULT_IO_BUFFER_SIZE,   // ������ ��������� ������
        DWORD  dwMaxPipeClientFreeExemplyar_ = PIPE_CLIENT_DEFAULT_FREE_COPY);       // ���������� ����������������� ��������� ����������� ����������� �������

    ~CZetPipeServer();

    void Init();    // ������� ������������� ������ ��� ������ � ������������ ��������
    void Close();   // ������� ��������������� ������ ��� ������ � ������������ ��������

protected:
    static UINT IOCompletionThread(LPVOID pParam);   // ��������� ������� ��������� ������� �� ������� ����� ����������
};

#endif
