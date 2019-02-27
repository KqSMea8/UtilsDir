//------------------------------------------------------------------------------
// ���� DetectorShiftAmpl.cpp
// ������ �� 13.06.2012
// ��� "����", ����������, �������� �.�.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "Detector\DetectorShiftAmpl.h"
//------------------------------------------------------------------------------
CDetectorShiftAmpl::CDetectorShiftAmpl()
{
	FNumberSignals_1 = -1;	// ���-�� �������� ��������� - 1
	FNumberSignals =		// ���-�� �������� ���������
		NumPage = 0;			// ���-�� ������� � ������
	FThreshold = 0.f;	// ����� ��������������
	FFactorThreshold = 15.f;
	wmEvent = 0;			// wm �������
	hWnd = 0;				// ���������� ����
	FbAutoThreshold = true;	// ���� ����������
	bJob = true;
	pBufAmpl =				// ��������� ����� ���������
		pBufSummAmpl = 
		pBufSummAmpl2 = NULL;
	p2 = NULL;
	ZeroVariable();
}
//------------------------------------------------------------------------------
CDetectorShiftAmpl::~CDetectorShiftAmpl()
{
	if (pBufAmpl)
	{
		delete pBufAmpl;
		delete pBufSummAmpl;
		delete pBufSummAmpl2;
	}
	if (p2)
		delete [] p2;
}
//------------------------------------------------------------------------------
bool CDetectorShiftAmpl::Create(int numberSignal, int size, int sizePage,
				HWND hwnd, UINT wm_EVENT)
{
	pBufAmpl = new CShiftBuffer<float>();
	bool ret = pBufAmpl->Create(size, sizePage);
	if (ret)
	{
		FNumberSignals = FNumberSignals_1 = numberSignal;
		FNumberSignals_1--;
		NumPage = size / sizePage;
		wmEvent = wm_EVENT;
		hWnd = hwnd;
		pBufSummAmpl = new CShiftBuffer<float>();
		pBufSummAmpl->Create(NumPage, 1);
		pBufSummAmpl2 = new CShiftBuffer<float>();
		pBufSummAmpl2->Create(NumPage, 1);
	}
	return ret;
}
//------------------------------------------------------------------------------
void CDetectorShiftAmpl::CalcAutoThreshold()
{
	float mean = summAmpl / pBufAmpl->GetSize();
	float std = ( summAmpl2 - mean * mean * pBufAmpl->GetSize() ) /
		( pBufAmpl->GetSize() - 1 );
	FThreshold = mean + FFactorThreshold * std;
}
//------------------------------------------------------------------------------
void CDetectorShiftAmpl::ZeroVariable()
{
	summAmpl = summAmpl2 = 0.f;
	NumPageCur = -1;
	FbInit = false;
}
//------------------------------------------------------------------------------
bool CDetectorShiftAmpl::SetSize(int size)
{
	bool ret;
	if ( pBufAmpl->GetSize() != size )
	{
		NumPage = size / pBufAmpl->GetSizePage();
		ret = pBufAmpl->SetSize(size);
		pBufSummAmpl->SetSize(NumPage);
		pBufSummAmpl2->SetSize(NumPage);
		ZeroVariable();
	}
	else
		ret = true;
	return ret;
}
//------------------------------------------------------------------------------
void CDetectorShiftAmpl::SetSizePage(int val)
{
	if ( pBufAmpl->GetSizePage() != val )
	{
		pBufAmpl->SetSizePage(val);
		pBufSummAmpl->Clear();
		pBufSummAmpl2->Clear();
		ZeroVariable();
	}
}
//------------------------------------------------------------------------------
void CDetectorShiftAmpl::SetAutoThreshold(bool val)
{
	float *add, *addSumm, *addSumm2, *addTemp;
	if (FbAutoThreshold != val)
	{
		FbAutoThreshold = val;
		if (FbAutoThreshold)
		{
			add = pBufAmpl->GetGlobalAddres();
			addSumm  = pBufSummAmpl ->GetGlobalAddres();
			addSumm2 = pBufSummAmpl2->GetGlobalAddres();
			addTemp = new float [pBufAmpl->GetSizePage()];
			for(int i=0; i<NumPage; i++)
			{
				ippsSqr_32f( add, addTemp, pBufAmpl->GetSizePage() );

				ippsSum_32f( add,     pBufAmpl->GetSizePage(),
					addSumm++,  ippAlgHintFast );
				ippsSum_32f( addTemp, pBufAmpl->GetSizePage(),
					addSumm2++, ippAlgHintFast );

				add += pBufAmpl->GetSizePage();
			}
			delete [] addTemp;
			ippsSum_32f( pBufSummAmpl->GetGlobalAddres(),  NumPage,
				&summAmpl,  ippAlgHintFast );
			ippsSum_32f( pBufSummAmpl2->GetGlobalAddres(), NumPage,
				&summAmpl2, ippAlgHintFast );
		}
		else
			FbInit = true;
	}
}
//------------------------------------------------------------------------------
void CDetectorShiftAmpl::Calculation(float **ppData)
{	//	������� �������� �������� ��� ������ ���������� ������
	int i, n;
	float a, b, *add;
	static int sizePageOld = 0;
	if (bJob)
	{
		if (FbAutoThreshold)
		{	// ������ ��� ���������
			if ( sizePageOld != pBufAmpl->GetSizePage() )
			{
				sizePageOld = pBufAmpl->GetSizePage();
				if (p2)
					delete [] p2;
				p2 = new float [sizePageOld];
			}
			// ������� ������ ��������� ��������
			summAmpl  -= *pBufSummAmpl ->GetGlobalAddres();
			summAmpl2 -= *pBufSummAmpl2->GetGlobalAddres();
			// ������ ������� ������
			pBufAmpl->ShiftData();
			pBufSummAmpl->ShiftData();
			pBufSummAmpl2->ShiftData();
		}
		// ����������� ��������� ����� ������ � ��������� �� � �����
		add = pBufAmpl->GetPointerLastPage();
		if (FNumberSignals > 1)
		{
			for(n=0; n<pBufAmpl->GetSizePage(); n++ )
			{
				a = 0.f;
				for(i=0; i<FNumberSignals; i++)
				{
					b = ppData[i][n];
					a += b * b;
				}
				*add++ = sqrt(a);
			}
		}
		else
		{
			for(n=0; n<pBufAmpl->GetSizePage(); n++ )
				*add++ = abs( ppData[0][n] );
		}

		// �������� ����� ������ �� ���������� ������
		if (FbInit)
		{
			add = pBufAmpl->GetPointerLastPage();
			for(i=0; i<pBufAmpl->GetSizePage(); i++)
			{
				if ( ( *add++ > FThreshold ) && hWnd )
				{ // ����� ��������, ���� ���������� ��������
					bJob = false;
					// � �������� � ����������
					n = pBufAmpl->GetSize() - pBufAmpl->GetSizePage() + i;
					::PostMessageW( hWnd, wmEvent, n, 0 );
					break;	// ���������� ��������� ���� ���
				}
			}
		}
	
		// ���� ���������, �� ����������� ����� ��������
		if (FbAutoThreshold)
		{
			ippsSum_32f( pBufAmpl->GetPointerLastPage(),
				pBufAmpl->GetSizePage(), &a, ippAlgHintFast );
			summAmpl += a;
			pBufSummAmpl->CopyNewData(&a);

			ippsSqr_32f(pBufAmpl->GetPointerLastPage(), p2,
				pBufAmpl->GetSizePage() );
			ippsSum_32f( p2, pBufAmpl->GetSizePage(), &a, ippAlgHintFast );
			summAmpl2 += a;
			pBufSummAmpl2->CopyNewData(&a);

			if (FbInit)
				CalcAutoThreshold();
			else
			{
				if (++NumPageCur == NumPage)
					FbInit = true;
			}
		}
	}	// if (bJob)
}
//------------------------------------------------------------------------------
void CDetectorShiftAmpl::Reset()
{
	pBufAmpl->Clear();
	pBufSummAmpl->Clear();
	pBufSummAmpl2->Clear();
	ZeroVariable();
	bJob = true;
}
//------------------------------------------------------------------------------