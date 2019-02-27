//------------------------------------------------------------------------------
// CustomMain.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Custom\CustomMain.h>
#include <Interface2012\Custom\Dialog_ZETi12.h>

//------------------------------------------------------------------------------

long CustomMainDrawThread(PVOID pParam)
{
	//ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	ZetThreadParamCustomMain *par1 = (ZetThreadParamCustomMain*)pParam;
	CCustomMain *pCMain = (CCustomMain*)(par1->m_pCustomMain);
	CWnd *pParent = par1->m_pParent;
	LARGE_INTEGER qpFreq, qpCount1, qpCount2, qpCount3, qpCount4;
	QueryPerformanceFrequency(&qpFreq);
	QueryPerformanceCounter(&qpCount1);
	if (pParent)
	{
		pCMain->m_pDMain->GetCurrentContext();
		if (pCMain->m_bNoMinimize)
		{
			//TRACE("Thread draw\n");
			pCMain->m_pDMain->OnDraw();			// в этом Bitmap рисую вначале себя
			QueryPerformanceCounter(&qpCount2);
			pCMain->OnContainerDraw();			// затем своих вассалов
			QueryPerformanceCounter(&qpCount3);
			if (pCMain->m_pDMain->GetSizeIcon() > 0)
			{// иконка программы
				DrawIconEx(wglGetCurrentDC(), pCMain->m_pDMain->GetPointIcon().x, pCMain->m_pDMain->GetPointIcon().y,
					((CDialog_ZETi12*)pParent)->m_hIcon, pCMain->m_pDMain->GetSizeIcon(), pCMain->m_pDMain->GetSizeIcon(),
					0, NULL, DI_NORMAL );
			}
			glFlush();
			//pCMain->OnMainDrawThreadEnd();
			SwapBuffers(wglGetCurrentDC());
			//TRACE(L"Thread draw swap buffer\n");
		}
		else
		{
			//pCMain->m_bNoMinimize = true;
			//SwapBuffers(wglGetCurrentDC());
		}
		QueryPerformanceCounter(&qpCount4);
		CTime time = CTime::GetCurrentTime();
		CString stmp(L"");
		stmp = time.Format(L"%H:%M:%S");
		//TRACE("Thread\t%s\t%f\t%f\t%f\n", (CStringA)stmp, (double)(qpCount2.LowPart - qpCount1.LowPart)/(double)qpFreq.LowPart
		//												, (double)(qpCount3.LowPart - qpCount2.LowPart)/(double)qpFreq.LowPart
		//												, (double)(qpCount4.LowPart - qpCount3.LowPart)/(double)qpFreq.LowPart);
		//pCMain->m_pDMain->FreeCurrentContext();
		//Sleep(5000);
	}
	return 0;
}

//------------------------------------------------------------------------------
CCustomMain::CCustomMain(CWnd* parent, CRect rect, CString sFolderPNG)
	: CCustomContainer(NULL)
	, m_pParent(parent)
	, m_pDMain(NULL)
	, m_sFolderPNG(sFolderPNG)
	, m_bIsChange(false)
	, m_bNoMinimize(true)
{

	m_pCOwner = NULL;
	m_pOwner = NULL;
	QueryPerformanceFrequency(&qpFreq);

#ifdef _GDIPLUS
	m_pCurBitmap = NULL;
	m_pGraphics = NULL;
#endif

	_imageBufferSize.cx = _imageBufferSize.cy = 0;
	_imageWasUpdated = false;

	zParam1.m_pCustomMain = this;
	zParam1.m_pParent = m_pParent;
	zParam1.m_Context = true;
	m_ThreadCalc.Create(L"Draw", m_pParent->m_hWnd, CustomMainDrawThread, &zParam1 );

	InitializeCriticalSection(&_bufferCrSection);
}
//------------------------------------------------------------------------------
CCustomMain::~CCustomMain()
{
#ifdef _GDIPLUS
	DeleteGdiPlusObject(m_pCurBitmap);
	DeleteGdiPlusObject(m_pGraphics);
#endif	
	//if (m_pDMain)
	//	delete m_pDMain;
	m_ThreadCalc.End();
	TRACE("Thread must die!\n");
	DeleteCriticalSection(&_bufferCrSection);
}
//------------------------------------------------------------------------------
double CCustomMain::GetTime()
{
	double ret(0.0), *padd = GetPointerToTime();
	if (padd)
		ret = *padd;
	else
		ret = NULL;
	return ret;
}
//------------------------------------------------------------------------------
double* CCustomMain::GetPointerToTime()
{
	double *pret(nullptr);
	if (m_pParent)
    {
#ifdef _ACTIVEX2012
        pret = NULL;
#else
        pret = ((CCustom_ZETi12*)m_pParent)->GetPointerToTime();
#endif
    }
	else
		pret = NULL;
	return pret;
}
//------------------------------------------------------------------------------
void CCustomMain::MessageToParent(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pParent)
	{
		//Если сообщением является команда выхода, то необходимо остановить
		//поток рисования до начала выполнения удаления объектов класса элементов
		if (wParam == SC_CLOSE)
			m_ThreadCalc.StopAndWait();

		::PostMessageW(m_pParent->m_hWnd, msg, wParam, lParam);
	}
}
//------------------------------------------------------------------------------
void CCustomMain::OnTimer()
{
	CRect rect0(0, 0, 0, 0);
	if (m_pParent)
		m_pParent->GetClientRect(rect0);
	m_pDClass->DoProcess();
	DoProcess();
	if (m_bIsChange && m_pParent)
	{
		m_pParent->Invalidate(FALSE);
		QueryPerformanceCounter(&qpCount1);
		OnDraw();
		QueryPerformanceCounter(&qpCount2);
		//CTime time = CTime::GetCurrentTime();
		//CString stmp = time.Format(L"%H:%M:%S");
		//TRACE("%s   %f\n", (CStringA)stmp, (double)(qpCount2.LowPart - qpCount1.LowPart)/(double)qpFreq.LowPart);
		m_bIsChange = false;
	}
}
//------------------------------------------------------------------------------
void CCustomMain::OnDraw()
{
	/*if (m_pParent)
	{
		CPaintDC dc(m_pParent);
#ifdef _GDIPLUS
		Gdiplus::Graphics graphics(dc.m_hDC);
		if (m_bNoMinimize)
		{	// промежуточные Bitmap и Graphics для буферизации и восстановления
			if (m_pCurBitmap == NULL)
			{
				m_pCurBitmap = new Gdiplus::Bitmap( m_pDMain->GetRect().right,
					m_pDMain->GetRect().bottom);
				m_pGraphics = new Gdiplus::Graphics(m_pCurBitmap);
				m_pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
				m_pGraphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
			}
			m_pDMain->OnDraw();			// в этом Bitmap рисую вначале себя
			CCustomContainer::OnDraw();	// затем своих вассалов
			if (m_pDMain->GetSizeIcon() > 0)
			{// иконка программы
				HDC hdc = m_pGraphics->GetHDC();
				DrawIconEx( hdc, m_pDMain->GetPointIcon().x, m_pDMain->GetPointIcon().y,
					((CDialog_ZETi12*)m_pParent)->m_hIcon,
					m_pDMain->GetSizeIcon(), m_pDMain->GetSizeIcon(),
					0, NULL, DI_NORMAL );
				m_pGraphics->ReleaseHDC(hdc);	// иначе иконка не прорисовывается
			}
		}
		else
			m_bNoMinimize = true;
		// прорисовка промежуточного Bitmap-а в диалоговом окне
		graphics.DrawImage(m_pCurBitmap, 0, 0, m_pDMain->GetRect().right,
			m_pDMain->GetRect().bottom);
#else
		//Gdiplus::Graphics graphics(dc.m_hDC);
		LARGE_INTEGER qpFreq, qpCount1, qpCount2, qpCount3, qpCount4;
		QueryPerformanceFrequency(&qpFreq);
		QueryPerformanceCounter(&qpCount1);
		if (m_bNoMinimize)
		{
			m_pDMain->OnDraw();			// в этом Bitmap рисую вначале себя
			QueryPerformanceCounter(&qpCount2);
			CCustomContainer::OnDraw();	// затем своих вассалов
			QueryPerformanceCounter(&qpCount3);
			if (m_pDMain->GetSizeIcon() > 0)
			{// иконка программы
				DrawIconEx(wglGetCurrentDC(), m_pDMain->GetPointIcon().x, m_pDMain->GetPointIcon().y,
					((CDialog_ZETi12*)m_pParent)->m_hIcon,
					m_pDMain->GetSizeIcon(), m_pDMain->GetSizeIcon(),
					0, NULL, DI_NORMAL );
				//ReleaseDC(*m_pParent, wglGetCurrentDC());
				//ReleaseHDC(wglGetCurrentDC);
			}

			glFlush();

			updateImageBuffer();

			SwapBuffers(wglGetCurrentDC());
			TRACE(L"swap buffer!");
		}
		else
		{
			m_bNoMinimize = true;
			SwapBuffers(wglGetCurrentDC());
		}
		QueryPerformanceCounter(&qpCount4);
		CTime time = CTime::GetCurrentTime();
		CString stmp = time.Format(L"%H:%M:%S");
		TRACE("OnDraw\t%s\t%f\t%f\t%f\n", (CStringA)stmp, (double)(qpCount2.LowPart - qpCount1.LowPart)/(double)qpFreq.LowPart
														, (double)(qpCount3.LowPart - qpCount2.LowPart)/(double)qpFreq.LowPart
														, (double)(qpCount4.LowPart - qpCount3.LowPart)/(double)qpFreq.LowPart);
#endif
	}*/
	m_pDMain->FreeCurrentContext();
	//	создание контекста очень нужно, без него не рисуется ничего
	CPaintDC dc(m_pParent);
	m_ThreadCalc.Start();
} 
//------------------------------------------------------------------------------
void CCustomMain::OnContainerDraw()
{
	//	явно вызываем функцию рисования вассалов
	CCustomContainer::OnDraw();
}
//------------------------------------------------------------------------------
void CCustomMain::OnMainDrawThreadEnd()
{
	updateImageBuffer();
}
//------------------------------------------------------------------------------
void CCustomMain::MainDrawThreadWait()
{
	m_ThreadCalc.StopAndWait();
}
//------------------------------------------------------------------------------
void CCustomMain::MainDrawThreadStart()
{
	m_ThreadCalc.Start();
}
//------------------------------------------------------------------------------
HWND CCustomMain::GetWindow()
{
	return NULL;
}
//------------------------------------------------------------------------------
SIZE& CCustomMain::GetBufferSize()
{
	return _imageBufferSize;
}
//------------------------------------------------------------------------------
void CCustomMain::updateImageBuffer()
{
	EnterCriticalSection(&_bufferCrSection);

	int glViewPortRect[4];
	glGetIntegerv(GL_VIEWPORT, glViewPortRect);
	GLenum error_enum = glGetError();
	DWORD error_dword = GetLastError();
	_imageBufferSize.cx = glViewPortRect[2];
	_imageBufferSize.cy = glViewPortRect[3];
	_imageBuffer.SetSize(_imageBufferSize.cx * _imageBufferSize.cy * 3);
	// копирование цветного изображения из буфера изображения в обычную оперативную память
	glReadPixels(0, 0, _imageBufferSize.cx, _imageBufferSize.cy, GL_BGR_EXT, GL_UNSIGNED_BYTE, _imageBuffer.GetData());

	_imageWasUpdated = true;

	LeaveCriticalSection(&_bufferCrSection);
}
//------------------------------------------------------------------------------
bool CCustomMain::GetImageBuffer(CByteArray *buffer, SIZE &size, bool force)
{
	bool result = false;
	EnterCriticalSection(&_bufferCrSection);

	if(_imageWasUpdated || force)
	{
		buffer->Copy(_imageBuffer);
		_imageWasUpdated = false;
		result = true;
	}

	LeaveCriticalSection(&_bufferCrSection);

	return result;
}
//------------------------------------------------------------------------------