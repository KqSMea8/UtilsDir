//------------------------------------------------------------------------------
// ��������� ���������� ��� Spectre_Base : header file
//------------------------------------------------------------------------------
#pragma once

#include "afxwin.h"

#include <NaNsupport.h>
#include <Struct_grid.h>
#include <ZetThread\ThreadSRVmultiSelf.h>
#include <ZETView_basic_class\CDSRV.h>
#include <Dialog_ZET\CDZetPath.h>
#include <Intel\ipp\ipps.h>
#include <CustomSaveStruct.h>
#include <WideResult/WideResult.h>

//------------------------------------------------------------------------------
struct AnalysisParams
{
	long decade;		// ������ ������.
	int integral;		// 0 - dif2, 1 - dif, 2 - not, 3 -int, 4 - int2
	int typeusred;		// ��� ���������� 0 - ��������, 1-exp
	int linlog;			// ��� ������������� 0-�������� 1- � ��
	int analis;			// ��� ������� - 0-��� ��� 1-Z-transform
	int spm;			// 0-���, 1-���, 2-��, 3-���
	int	clean;			// ������� ������� ��������� �������� (0-���� 1-���)
	int wind;			// ��� ������������ ���� (������ ������� �������)
	float usred;		// ���������� � ��������
	double interval;	// �������� ������� �������������� ��������

	long size;			// ������ �����������
	int sizeforz;		// ������ ��� Z-��������������

	int showMaxGrf;		// ������ ������������� �������
	int showSredGrf;	// ������ �������� �������

	AnalysisParams()
		: decade(0)
		, integral(0)
		, typeusred(0)
		, linlog(0)
		, analis(0)
		, spm(0)
		, clean(0)
		, wind(0)
		, usred(0.F)
		, interval(0)

		, size(0)
		, sizeforz(0)
		, showMaxGrf(0)
		, showSredGrf(0)
	{

	}
};
//------------------------------------------------------------------------------
struct LoggerStruct
{
	int logSpectr;			// ���������� � ������ ������� ������
	float centralFreq;		// ����������� ������� ������������� ���������

	LoggerStruct()
	{
		logSpectr = 0;
		centralFreq = 0;
	}
};
//------------------------------------------------------------------------------
struct ShowParams
{
	long channel;		// ����� ������
	long IdChan;		// ������������� ������
	
	int showGrammaWindow;		// ����������� ���� ��������������
	int showTDGrammaWindow;		// ����������� ���� ��������������
	int showKepstrWindow;		// ����������� ���� �������
	int showProhodWindow;		// ����������� ���� ���������� ��������������

	int showNormaGrf;	// ����������� ������� �����	
	int grammaStart;	// ����������� �������������
	int tdGrammaStart;	// ����������� �������������
	int kepstrStart;    // ����������� �������
	int prohStart;		// ����������� ���������
	 
	int prohsize;		// ������ ���������

	int colorgrm;		// 1 - �������   0 - �����-����� ����������
	int sechgrammax;	// ������� ���������� �� X
	int sechgrammay;	// ������� ���������� �� Y

	double maxFreq;
	float maxLevel;

	LoggerStruct logger;			// ��������� ������������

	WINDOWPLACEMENT placement[6];	// ��������� ���������� ���. ����

	CHANNEL_STRUCT ChanStruct;	// ��������� ���������� ������

	wchar_t myComment[200];	// ����������� ��� ������
	wchar_t normaFile[400];	// ���� �����

	ShowParams()
		: channel(0)
		, IdChan(0)
		, showGrammaWindow(0)
		, showTDGrammaWindow(0)
		, showKepstrWindow(0)
		, showProhodWindow(0)
		, showNormaGrf(0)
		, grammaStart(0)
		, tdGrammaStart(0)
		, kepstrStart(0)
		, prohStart(0)
		, prohsize(0)
		, colorgrm(0)
		, sechgrammax(0)
		, sechgrammay(0)
		, maxFreq(0.)
		, maxLevel(0.f)
	{

		ZeroMemory(&ChanStruct, SIZE_CHANNEL_STRUCT);
		ZeroMemory(myComment, 200 * sizeof(wchar_t));
		ZeroMemory(normaFile, 400 * sizeof(wchar_t));
		ZeroMemory(placement, sizeof(placement));
		for (auto & it : placement)
			it.length = sizeof(WINDOWPLACEMENT);
	}
};
//------------------------------------------------------------------------------
struct ResultStatus
{
	bool spectreSuccess;
	bool maxSpectreSuccess;
	bool sredSpectreSuccess;
	void ResetStatus()
	{
		spectreSuccess = maxSpectreSuccess = sredSpectreSuccess = false;
	}
};
//------------------------------------------------------------------------------
struct MultiParamSelf : AnalysisParams
{// ��������� ���������� ������ � �������������� ������ ������, ���������������
	//	��� ���������
	long typeValUnit;	// ��� ������ ��� �������� � Unit

	int indexsred;		// ������ ����������
	bool bStartInterval;	// ���� ������������� ��������� ������� (����� ��������� �������)
	float reference;
	float mnoj;
	double timer;		// ������ �� ����� ���������
	double swind;		// ������� ����
	double ewind;		// ������� ����
	double sdvig;		// ����� �� ��������� ����� ��� ������� ����������� �������
	float tempsred;		// ��������� ���������� �������� �������� ����������

	int linlog_popr;	// �������-��������������� �������� ��������
	float unis;			// ����������� �����

	double timedelay;
	double time;
	double dLastUnitTime;	// ����� ��������� ������ � ����

	double cycle;
	bool setInterval;
	bool bParamsChanged;

	WideResult<float*> wr;

	std::vector<float> mspectr;//[MaxSize/2+1];		// ������������ ������ (������������ ����� ����)
	std::vector<float> stspectr;//[MaxSize/2+1];	// ������� ������ (������������ ����� ����)
	std::vector<float> sspectr;//[MaxSize/2+1];		// ������� ������
	std::vector<float> pspectr;//[MaxSize/2+1];		// ���������� ������ (������������ ����� ����)
	std::vector<float> popspectr;//[MaxSize/2+1];	// ������ ��������
	std::vector<float> uspectr;//[MaxSize/2+1];		// ����������� ������ (����������)

	std::vector<Ipp32fc> prm2, prm3, prm4;
	std::vector<float> win, integr;
	//IppsFFTSpec_C_32fc* specSpectr;
	IppsFFTSpec_C_32fc* pFftSpec_32fc;
	std::vector<Ipp32fc> spectr;
	
	std::vector<double> dSignal;
	std::vector<double> dMaxSpectr;		// mspectr
	std::vector<double> dSrdSpectr;		// sspectr
	std::vector<double> dStsSpectr;		// stspectr
	std::vector<double> dPrmSpectr;		// pspectr
	std::vector<double> dPopSpectr;		// popspectr
	std::vector<double> dUsrSpectr;		// uspectr
	std::vector<double> dWin;
	std::vector<double> dIntegr;
	std::vector<Ipp64fc> dPrm2, dPrm3, dPrm4, dSpectr;
	//IppsFFTSpec_C_64fc* pdSpecSpectr;
	IppsFFTSpec_C_64fc* pFftSpecSpectr_64fc;
	IppsFFTSpec_C_64fc* pFftSpecSpectr_64fc2;
	Ipp8u* pFFTSpecSpectrBuf;
	Ipp8u* pFFTSpecSpectrBuf2;
	Ipp8u* pFFTWorkSpectrBuf;
	IppsFFTSpec_C_32fc* pFftSpecKepstr_32fc;
	IppsFFTSpec_C_32fc* pFftSpecKepstr_32fc2;
	Ipp8u* pFFTSpecKepstrBuf;
	Ipp8u* pFFTSpecKepstrBuf2;
	Ipp8u* pFFTWorkKepstrBuf;

	int oldOrder;
	int initialized;

	ResultStatus resultStatus;

	MultiParamSelf()
	{
		double freq=25000.;
		reference = 0.f;
		typeValUnit = 0;
		decade = 0;
		swind = 0.0;
		timedelay = 0.01;
		ewind = 0.0;
		linlog_popr = 0;
		size = 2000;
		sizeforz = 4096;
		unis = 0.f;
		time = 0.0;
		oldOrder = 0;
		analis=1;
		wind = 2;
		sdvig = freq*timedelay;
		spm = 0; 
		indexsred = 0;
		bStartInterval = false;
		bParamsChanged = false;
		integral=2;
		interval=100.;
		tempsred=0;
		usred=1.;
		typeusred = 0;
		linlog=1;
		clean = 0;
		mnoj = 1.f;
		cycle = 0;
		setInterval = false;

		timer = 0;
		dLastUnitTime = 0;

		pFftSpec_32fc = NULL;
		
		pFftSpecSpectr_64fc = nullptr;
		pFftSpecSpectr_64fc2 = nullptr;
		pFFTSpecSpectrBuf = nullptr;
		pFFTSpecSpectrBuf2 = nullptr;
		pFFTWorkSpectrBuf = nullptr;

		pFftSpecKepstr_32fc = nullptr;
		pFftSpecKepstr_32fc2 = nullptr;
		pFFTSpecKepstrBuf = nullptr;
		pFFTSpecKepstrBuf2 = nullptr;
		pFFTWorkKepstrBuf = nullptr;

		showMaxGrf = 0;
		showSredGrf = 0;

		resultStatus.ResetStatus();

		initialized = 0;
	}
	~MultiParamSelf()
	{

		if (pFftSpec_32fc != NULL)
		{
			ippsFree(pFftSpec_32fc);
			pFftSpec_32fc = nullptr;
		}
		mspectr.clear();
		stspectr.clear();
		sspectr.clear();
		pspectr.clear();
		popspectr.clear();
		uspectr.clear();
		prm2.clear();
		prm3.clear();
		prm4.clear();
		win.clear();
		integr.clear();
		spectr.clear();

		pFftSpecSpectr_64fc = nullptr;
		pFftSpecSpectr_64fc2 = nullptr;
		if (pFFTSpecSpectrBuf != nullptr)
		{
			ippsFree(pFFTSpecSpectrBuf);
			pFFTSpecSpectrBuf = nullptr;
		}	
		if (pFFTSpecSpectrBuf2 != nullptr)
		{
			ippsFree(pFFTSpecSpectrBuf2);
			pFFTSpecSpectrBuf2 = nullptr;
		}
		if (pFFTWorkSpectrBuf != nullptr)
		{
			ippsFree(pFFTWorkSpectrBuf);
			pFFTWorkSpectrBuf = nullptr;
		}

		pFftSpecKepstr_32fc = nullptr;
		pFftSpecKepstr_32fc2 = nullptr;
		if (pFFTSpecKepstrBuf != nullptr)
		{
			ippsFree(pFFTSpecKepstrBuf);
			pFFTSpecKepstrBuf = nullptr;
		}
		if (pFFTSpecKepstrBuf2 != nullptr)
		{
			ippsFree(pFFTSpecKepstrBuf2);
			pFFTSpecKepstrBuf2 = nullptr;
		}
		if (pFFTWorkKepstrBuf != nullptr)
		{
			ippsFree(pFFTWorkKepstrBuf);
			pFFTWorkKepstrBuf = nullptr;
		}

		dSignal.clear();
		dMaxSpectr.clear();
		dSrdSpectr.clear();
		dStsSpectr.clear();
		dPrmSpectr.clear();
		dPopSpectr.clear();
		dUsrSpectr.clear();
		dWin.clear();
		dIntegr.clear();
		dPrm2.clear();
		dPrm3.clear();
		dPrm4.clear();
		dSpectr.clear();
	}
	MultiParamSelf(const MultiParamSelf &val)
	{
		typeValUnit = val.typeValUnit;
		decade = val.decade;

		integral = val.integral;		//�������������� - ����������������� �������, 2-�����,
		typeusred = val.typeusred;		//��� ���������� 0 - ��������, 1-exp
		linlog = val.linlog;			//��� ������������� 0-�������� 1- � ��
		analis = val.analis;			//��� ������� - 0-��� ��� 1-Z-transform
		spm = val.spm;					//0-���, 1-���, 2-��, 3-���
		clean = val.clean;				//������� ������� ��������� �������� (0-���� 1-���)
		wind = val.wind;				//��� ������������ ���� (������ ������� �������)
		usred = val.usred;				// ���������� � ��������
		interval = val.interval;		//�������� �������
		indexsred = val.indexsred;		// ������ ����������

		bStartInterval = val.bStartInterval;	//����� ��������� �������
		reference = val.reference;
		mnoj = val.mnoj;
		timer = val.timer;				//������ �� ����� ���������
		swind = val.swind;				//������� ����
		ewind = val.ewind;				//������� ����
		sdvig = val.sdvig;				//����� �� ��������� ����� ��� ������� ����������� �������
		tempsred = val.tempsred;		//��������� ���������� �������� �������� ����������

		linlog_popr = val.linlog_popr;	//�������-��������������� �������� ��������
		unis = val.unis;

		timedelay = val.timedelay;
		time = val.time;
		cycle = val.cycle;
		setInterval = val.setInterval;

		size = val.size;				//������ �����������
		sizeforz = val.sizeforz;		//������ ��� Z-��������������

		win = val.win;
		integr = val.integr;
		spectr = val.spectr;
		oldOrder = val.oldOrder;
				
		if (val.pFftSpec_32fc != NULL)
		{ 
			Ipp8u* pFFTSpecBuf(nullptr), *pFFTInitBuf(nullptr), *pFFTWorkBuf(nullptr);
			int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
			//ippsFFTInitAlloc_C_32fc(&pFftSpec_32fc, specSpectrSize, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone);
			ippsFFTGetSize_C_32fc(size, IPP_DIV_INV_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
			pFFTSpecBuf = ippsMalloc_8u(sizeFFTSpec);
			pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
			pFFTWorkBuf = ippsMalloc_8u(sizeFFTWorkBuf);
			ippsFFTInit_C_32fc(&pFftSpec_32fc, size, IPP_DIV_INV_BY_N, ippAlgHintNone, pFFTSpecBuf, pFFTInitBuf);
			if (pFFTInitBuf)
				ippsFree(pFFTInitBuf);
			if (pFFTWorkBuf)
				ippsFree(pFFTWorkBuf);
			if (pFFTSpecBuf)
				ippsFree(pFFTSpecBuf);
		}
		else
			pFftSpec_32fc = NULL;
		
		dWin = val.dWin;
		dIntegr = val.dIntegr;
		dSpectr = val.dSpectr;

		pFftSpecSpectr_64fc = nullptr;
		pFftSpecSpectr_64fc2 = nullptr;
		pFFTSpecSpectrBuf = nullptr;
		pFFTSpecSpectrBuf2 = nullptr;
		pFFTWorkSpectrBuf = nullptr;

		pFftSpecKepstr_32fc = nullptr;
		pFftSpecKepstr_32fc2 = nullptr;
		pFFTSpecKepstrBuf = nullptr;
		pFFTSpecKepstrBuf2 = nullptr;
		pFFTWorkKepstrBuf = nullptr;
				
		showMaxGrf = val.showMaxGrf;
		showSredGrf = val.showSredGrf;
		TRACE("Spectr copy show sred param %d\n", showSredGrf);

		resultStatus = val.resultStatus;

		initialized = val.initialized;
	}

	void operator = (_In_ const MultiParamSelf &val)
	{
		typeValUnit = val.typeValUnit;
		decade = val.decade;

		integral = val.integral;		//�������������� - ����������������� �������, 2-�����,
		typeusred = val.typeusred;		//��� ���������� 0 - ��������, 1-exp
		linlog = val.linlog;			//��� ������������� 0-�������� 1- � ��
		analis = val.analis;			//��� ������� - 0-��� ��� 1-Z-transform
		spm = val.spm;					//0-���, 1-���, 2-��, 3-���
		clean = val.clean;				//������� ������� ��������� �������� (0-���� 1-���)
		wind = val.wind;				//��� ������������ ���� (������ ������� �������)
		usred = val.usred;				// ���������� � ��������
		interval = val.interval;		//�������� �������
		indexsred = val.indexsred;		// ������ ����������

		bStartInterval = val.bStartInterval;	//����� ��������� �������
		reference = val.reference;
		mnoj = val.mnoj;
		timer = val.timer;				//������ �� ����� ���������
		swind = val.swind;				//������� ����
		ewind = val.ewind;				//������� ����
		sdvig = val.sdvig;				//����� �� ��������� ����� ��� ������� ����������� �������
		tempsred = val.tempsred;		//��������� ���������� �������� �������� ����������

		linlog_popr = val.linlog_popr;	//�������-��������������� �������� ��������
		unis = val.unis;

		timedelay = val.timedelay;
		time = val.time;
		cycle = val.cycle;
		setInterval = val.setInterval;

		size = val.size;				//������ �����������
		sizeforz = val.sizeforz;		//������ ��� Z-��������������

		win = val.win;
		integr = val.integr;
		spectr = val.spectr;
		oldOrder = val.oldOrder;

		if (val.pFftSpec_32fc != NULL)
		{
			Ipp8u* pFFTSpecBuf(nullptr), *pFFTInitBuf(nullptr), *pFFTWorkBuf(nullptr);
			int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
			//ippsFFTInitAlloc_C_32fc(&pFftSpec_32fc, specSpectrSize, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone);
			ippsFFTGetSize_C_32fc(size, IPP_DIV_INV_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
			pFFTSpecBuf = ippsMalloc_8u(sizeFFTSpec);
			pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
			pFFTWorkBuf = ippsMalloc_8u(sizeFFTWorkBuf);
			ippsFFTInit_C_32fc(&pFftSpec_32fc, size, IPP_DIV_INV_BY_N, ippAlgHintNone, pFFTSpecBuf, pFFTInitBuf);
			if (pFFTInitBuf)
				ippsFree(pFFTInitBuf);
			if (pFFTWorkBuf)
				ippsFree(pFFTWorkBuf);
			if (pFFTSpecBuf)
				ippsFree(pFFTSpecBuf);
		}
		else
			pFftSpec_32fc = NULL;

		dWin = val.dWin;
		dIntegr = val.dIntegr;
		dSpectr = val.dSpectr;

		pFftSpecSpectr_64fc = nullptr;
		pFftSpecSpectr_64fc2 = nullptr;
		pFFTSpecSpectrBuf = nullptr;
		pFFTSpecSpectrBuf2 = nullptr;
		pFFTWorkSpectrBuf = nullptr;

		pFftSpecKepstr_32fc = nullptr;
		pFftSpecKepstr_32fc2 = nullptr;
		pFFTSpecKepstrBuf = nullptr;
		pFFTSpecKepstrBuf2 = nullptr;
		pFFTWorkKepstrBuf = nullptr;

		showMaxGrf = val.showMaxGrf;
		showSredGrf = val.showSredGrf;
		TRACE("Spectr copy show sred param %d\n", showSredGrf);

		resultStatus = val.resultStatus;

		initialized = val.initialized;
	}

	void ReSize(DWORD newSize)
	{
		size = newSize;

		if(analis != 0)
		{
			float por=(float)(log((float)size) / log((float)2.));
			por=(float)((int)por + 2);
			//	������ ������� ��� �������������� �������� �� ������� ����������, ����� �������� ��������� ������
			sizeforz=(int)(pow((float)2., por)+.5) * 2;
		}
		else
		{
			sizeforz = size;
		}

		float nan = ZETGetNaN();
		mspectr.resize(size, -1000);
		stspectr.resize(size, 0);
		sspectr.resize(size, 0);
		pspectr.resize(size, 0);
		popspectr.resize(size, 1.0f);
		uspectr.resize(size, 0);

		prm2.resize(sizeforz);
		prm3.resize(size);
		prm4.resize(sizeforz);;
		win.resize(size, 1.0f);
		integr.resize(size, 0.);
		if (pFftSpec_32fc != NULL)
		{
			ippsFree(pFftSpec_32fc);
			pFftSpec_32fc = nullptr;
		}		
		
		//ippsFFTInitAlloc_C_32fc(&specSpectr, specSpectrSize, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone );
		spectr.resize(size);

		dMaxSpectr.resize(size, -1000.);	// mspectr
		dPopSpectr.resize(size, 1.);		// popspectr
		dPrmSpectr.resize(size, 0);			// pspectr
		dSrdSpectr.resize(size, 0);			// sspectr
		dStsSpectr.resize(size, 0);			// stspectr
		dUsrSpectr.resize(size, 0);			// uspectr
		dPrm2.resize(sizeforz);
		dPrm4.resize(sizeforz);
		dPrm3.resize(size);
		dWin.resize(size, 1.);
		dIntegr.resize(size, 0.);
		dSignal.resize(size);
		dSpectr.resize(size);

		pFftSpecSpectr_64fc = nullptr;
		pFftSpecSpectr_64fc2 = nullptr;
		if (pFFTSpecSpectrBuf != nullptr)
		{
			ippsFree(pFFTSpecSpectrBuf);
			pFFTSpecSpectrBuf = nullptr;
		}
		if (pFFTSpecSpectrBuf2 != nullptr)
		{
			ippsFree(pFFTSpecSpectrBuf2);
			pFFTSpecSpectrBuf2 = nullptr;
		}
		if (pFFTWorkSpectrBuf != nullptr)
		{
			ippsFree(pFFTWorkSpectrBuf);
			pFFTWorkSpectrBuf = nullptr;
		}

		pFftSpecKepstr_32fc = nullptr;
		pFftSpecKepstr_32fc2 = nullptr;
		if (pFFTSpecKepstrBuf != nullptr)
		{
			ippsFree(pFFTSpecKepstrBuf);
			pFFTSpecKepstrBuf = nullptr;
		}
		if (pFFTSpecKepstrBuf2 != nullptr)
		{
			ippsFree(pFFTSpecKepstrBuf2);
			pFFTSpecKepstrBuf2 = nullptr;
		}
		if (pFFTWorkKepstrBuf!= nullptr)
		{
			ippsFree(pFFTWorkKepstrBuf);
			pFFTWorkKepstrBuf = nullptr;
		}
		initialized = size;
	}
};
//------------------------------------------------------------------------------
struct SaveStruct : public CustomSaveStructure
{
	AnalysisParams analysisParams;
	ShowParams showParams;
	grid safeGrid;
	
	SaveStruct() : CustomSaveStructure()
		, analysisParams()
		, showParams()
		, safeGrid()
	{}
};
//------------------------------------------------------------------------------
struct CalcDataStruct
{
	DWORD curSize;
	std::vector<float> nspectr;//[MaxSize/2+1];	//�����
	std::vector<float> kspectr;//[MaxSize/2+1];	//������
	std::vector<float> prohspectr;//[MaxSize];	//��������� ��������������

	std::vector<float> freqm;//[MaxSize];

	std::vector<float> massivImag; //[MaxSize]; //������������� ������
	std::vector<double> dMassivImag;

	std::vector<float> massivUnit; //[4*MaxSize]; //������ ��� �������� ������ � ����

	CalcDataStruct(): curSize(0) {}

	void Resize(DWORD val)
	{
		if(val > curSize)
		{
			curSize = val;

			float nan = ZETGetNaN();
			nspectr.resize(val, nan);
			kspectr.resize(val, nan);
			prohspectr.resize(val, 0);
			freqm.resize(val, 0);
			massivImag.resize(val, 0);
			dMassivImag.resize(val, 0);
		}
	}
	~CalcDataStruct()
	{
		nspectr.clear();
		kspectr.clear();
		prohspectr.clear();
		freqm.clear();
		massivImag.clear();
		massivUnit.clear();
		dMassivImag.clear();
	}
};
//------------------------------------------------------------------------------
struct WorkParams
{
	int prohindex;			// ������ ���������
	int prohReset;
	double prohfreq;		// ������� ��� ���������

	wchar_t linstr[200];	// �������� � �������� ��������
	wchar_t logstr[200];	// �������� � ��������������� ��������
	wchar_t legendstr[200];	// �������� � �������
	wchar_t lstry[200];		// �������� � �������
	int prohx;

	bool allnan;				// ���� ��� ������ NAN
	int GRAPH_UPOR;				//����������� �� �������

	WorkParams()
		: prohindex(0)
		, prohReset(0)
		, prohfreq(0)
		, prohx(0)
		, GRAPH_UPOR(1)
		, allnan(false)
	{

		ZeroMemory(linstr, sizeof(linstr));
		ZeroMemory(logstr, sizeof(logstr));
		ZeroMemory(legendstr, sizeof(legendstr));
		ZeroMemory(lstry, sizeof(lstry));

	}
};
//------------------------------------------------------------------------------
struct ParamStruct
{
	CDSRV *server;
	CDZetPath *zpath;
	AnalysisParams analysisParams;
	ShowParams showParams;
	//WorkParams workParams;
	ParamStruct()
		: server(nullptr)
		, zpath(nullptr)
	{

	}
};
//------------------------------------------------------------------------------
struct CommonPointers
{
	CDSRV *server;
	CDZetPath *zpath;

	MultiChannelSelf** ppChanCur;
	MultiParamSelf** ppParCur;
	//TODO:SV SaveStruct* saveParams;
	SaveStruct* saveParams;
	WorkParams* workParams;
	CalcDataStruct* calcData;
	CommonPointers()
		: server(nullptr)
		, zpath(nullptr)
		, ppChanCur(nullptr)
		, ppParCur(nullptr)
		, saveParams(nullptr)
		, workParams(nullptr)
		, calcData(nullptr)
	{
	}
};
//------------------------------------------------------------------------------
struct FileArrayStruct
{
	int size;
	float* pF;
	CString strGL;
	FileArrayStruct()
		:size(0)
		,pF(nullptr)
	{
	}
	~FileArrayStruct()
	{
		if (pF != NULL)
		{
			delete pF;
			pF = nullptr;
		}
	}
};