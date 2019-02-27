#ifdef ZETCONFIGURE_EXPORTS
#define ZETCONFIGURE_API __declspec(dllexport)
#else
#define ZETCONFIGURE_API __declspec(dllimport)
#endif

class ZETCONFIGURE_API CZETConfigure
{
public:
	CZETConfigure();
	~CZETConfigure();

	const bool isChanged();

protected:
	bool m_bChanged;
		
private:
	HANDLE m_hThread;
	HANDLE m_hEnd;
	
	static DWORD WINAPI watchThread(_In_ const LPVOID pParam);
	DWORD threadFunction();
};
