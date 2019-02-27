//------------------------------------------------------------------------------
// CustomMain.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomContainer.h>
#include <Interface2012\Draw\DMain.h>
#include <Interface2012\Custom\IScreenBuffering.h>
//#include <Interface2012\Thread\ThreadMainDraw.h>
#include <ZETThread\ThreadCalc.h>

struct ZetThreadParamCustomMain
{
	PVOID m_pCustomMain;
	CWnd *m_pParent;
	bool m_Context;
};

//------------------------------------------------------------------------------
class CCustomMain : public CCustomContainer, public IScreenBuffering
{
public:
	CCustomMain(CWnd* parent, CRect rect, CString sFolderPNG);
	virtual ~CCustomMain();
	CDMain *m_pDMain;
private:

	CRITICAL_SECTION _bufferCrSection;
	bool _imageWasUpdated;
	CByteArray _imageBuffer;
	SIZE _imageBufferSize;

	void updateImageBuffer();

protected:
	CWnd *m_pParent;
	Gdiplus::Graphics *m_pGraphics;
#ifdef _GDIPLUS
	
	//CDMain *m_pDMain;
	Gdiplus::Bitmap *m_pCurBitmap;
#endif
	bool m_bIsChange;
	CString m_sFolderPNG;
//	GraphicsPath m_GraphPathRgn;
	CRgn m_RegionWnd;


	LARGE_INTEGER qpFreq, qpCount1, qpCount2;

	//CThreadMainDraw m_ThreadDraw;
	CThreadCalc m_ThreadCalc;
	ZetThreadParamCustomMain zParam1;

public:
	bool m_bNoMinimize;
	
	virtual CWnd* GetParent() { return m_pParent; }
	CString GetFolderPNG() { return m_sFolderPNG; }
#ifdef _GDIPLUS
	virtual Gdiplus::Graphics** GetPointerToGraphics() { return &m_pGraphics; }
#endif
	virtual double GetTime();
	virtual double* GetPointerToTime();
	bool* GetPointerToIsChange() { return &m_bIsChange; }

	CString GetTitle() { return m_pDMain->GetTitle(); }
	void SetTitle(CString val) { m_pDMain->SetTitle(val); }

	void OnDraw();
	void OnContainerDraw();
	void OnMainDrawThreadEnd();

	void MainDrawThreadWait();
	void MainDrawThreadStart();

	virtual void OnTimer();
	void IsChange() { m_bIsChange = true;}
	

	virtual void MessageToParent(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual HWND GetWindow() override;
	virtual SIZE& GetBufferSize() override;
	virtual bool GetImageBuffer(CByteArray *buffer, SIZE &size, bool force = false) override;
};
//------------------------------------------------------------------------------