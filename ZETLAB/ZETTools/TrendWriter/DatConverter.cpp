#include "stdafx.h"
#include "DatConverter.h"
#include <algorithm>
#include <limits>
#include <ZETFile/ZETFile.h>
#include <WaterMark/WaterMarkFloat.h>
#include <WaterMark/WaterMarkInt.h>
#include <pugixml/src/pugixml.hpp>

#include <mathZet.h>

#define MAX_MODEL_NUMBER 99				// сейчас максимальное значение 19
#define FIRSTUSEDWM 1
#define ONE_CHANNEL_SIZE (1024 * 1024)

const float minFloat = std::numeric_limits<float>::denorm_min();
const double minTime = 1e+6;


CDatConverter::CDatConverter()
	: m_sSignalDir(L"")
	, m_sCompresDir(L"")
	, m_channels(L"")
	, m_tStartTime(0)
	, m_tEndTime(0)
{
	ClearWriter();
}

CDatConverter::~CDatConverter()
{
}

long CDatConverter::ConvertDatBufferToTrend(char *dataBuf, unsigned long rbSize, bool isComing, time_t &fileTime)
{
	m_watermarks.clear();

	float floatVal = 0;              // текущее float значение отсчета
	long *lngBuf = (long*)dataBuf;   // приводим к набору лонговых отсчетов
    short* shrtBuf = (short*)dataBuf;   // приводим к набору шортовых отсчетов
	unsigned int countCh = m_parcer.m_devicedata.size();   // количество каналов
	if (countCh == 0)
		return 0;

	// Поправочный коэффициент, т.к. смещение в файле иногда оказывается не верным, 
	// и вместе с данными зачитывается часть хедера, или количество данных по каждому 
	// из каналов не одинаково. Вобщем это костыль для того, чтобы файлы были одного размера.
    long corrFactor = 0;
    if (m_parcer.m_devicedata[0].wordsize() == 1)   // Если данные хранятся в short-ах
        corrFactor = (rbSize / sizeof(short)) % countCh;
    else   // Если данные хранятся в long-ах
        corrFactor = (rbSize / sizeof(long)) % countCh;
	auto it = m_parcer.m_devicedata[0];
	// для устройств ZET017 и ZET048 читаем вотермарки
	if (it.type() == deviceparcer::zdtADC8_500 ||
		it.type() == deviceparcer::zdtZET240)
		ReadPPS(lngBuf, (rbSize / sizeof(long)), countCh, it.freq(), corrFactor);		// считываем ватермарки		

	// для остальных устройств записываем начиная от момента создания первого файла
	if (m_watermarks.empty())
	{
		auto it = m_parcer.m_devicedata[0];
		if (it.wordsize() == 1)  // Если данные хранятся в short-ах
			MakePPS((rbSize / sizeof(short)), fileTime, countCh, it.freq(), corrFactor);
        else   // Если данные хранятся в long-ах
			MakePPS((rbSize / sizeof(long)), fileTime, countCh, it.freq(), corrFactor);
	}

	CTime ttime1(m_watermarks.front().rtcTime);
	SYSTEMTIME stime1 = {0};
	ttime1.GetAsSystemTime(stime1);
	CTime ttime2(m_watermarks.back().rtcTime);
	SYSTEMTIME stime2 = {0};
	ttime2.GetAsSystemTime(stime2);
	//TRACE("Convert watermarks %02d:%02d:%02d - %02d:%02d:%02d\n", stime1.wHour, stime1.wMinute, stime1.wSecond, stime2.wHour, stime2.wMinute, stime2.wSecond);

	// количество отсчетов без синхронизации, не записываются в результирующие файлы. 
	int startAcpOffset = 0;

	if (!isComing)
	{
		startAcpOffset = CalcStartAcpOffset(countCh);	// проверяем корректность старта АЦП

		if (startAcpOffset)
			m_parcer.increment_second();				//корректировка времени старта файла
	}

	int index(0);
	long usedWM = isComing ? 0 : FIRSTUSEDWM;			// переменная используется для того, чтобы каждый раз не проверять весь массив ватермарок.
	long floatInLong(0);
	long maskedValue(0);
	float floatInFloat(0);
	double absvolt(0);
	double gain(0);
	double sense(0);
	double ampl(0);
	double dc(0);
	double dTime(0);

	std::vector<std::vector<float>> datas;
	datas.resize(m_parcer.m_devicedata.size());
	for (int i = 0, size = datas.size(); i < size; i++)
		auto it = m_parcer.m_devicedata[0];
	if (it.wordsize() == 1)  // Если данные хранятся в short-ах
		    datas.reserve(rbSize / datas.size() / sizeof(short));
        else   // Если данные хранятся в long-ах
            datas.reserve(rbSize / datas.size() / sizeof(long));

	// если в первой секунде оказалось отсчетов больше чем частота дискретизации, то отрезаем отсчеты сначала, а точнее. просто не записываем их в файл (startAdcOffset).
    for (int i = startAcpOffset, size = (rbSize / ((m_parcer.m_devicedata[0].wordsize() == 1) ? sizeof(short) : sizeof(long))) - corrFactor; i < size; i++)
	{
        auto itt = m_parcer.m_devicedata[0];
		if (itt.wordsize() == 1)  // Если данные хранятся в short-ах
            maskedValue = shrtBuf[i];
        else   // Если данные хранятся в long-ах
            maskedValue = lngBuf[i] & 0xffffff00;
		
		index = i % countCh;
		auto it = m_parcer.m_devicedata[index];
		absvolt = it.absvolt();
		gain = it.gain();
		sense = it.sense();
		ampl = it.ampl();
		dc = it.dc();
		floatVal = (float)((float)maskedValue * (absvolt / (gain * sense * ampl)) - dc);	// конвертируем лонг во флоат, последний байт не учитываем.
        // Затычка для не отображения NaN после конвертирования
        if (floatVal == 0.0f)
            floatVal = FLT_MIN;
		floatInLong = *(long*)&floatVal;													// из флоата в лонг для битовой операции
		floatInLong = floatInLong & (~0x1);													// обнуляем младший бит


		if ((UINT)usedWM < m_watermarks.size() && i / countCh >= (unsigned int)m_watermarks[usedWM].ppsSample)
		{
			if (i / countCh - m_watermarks[usedWM].ppsSample < m_watermarks[usedWM].timelabel.size())
			{
				floatInLong = floatInLong | (m_watermarks[usedWM].timelabel[i / countCh - m_watermarks[usedWM].ppsSample] & 0x1);	// добавляем ватермарку
				CTime time(m_watermarks[usedWM].rtcTime);
				dTime = double(CTimeSpan(time.GetTime()).GetTotalSeconds());
				if (i / countCh - m_watermarks[usedWM].ppsSample + 1 == m_watermarks[usedWM].timelabel.size())
					usedWM++;
			}
		}

		if (floatInLong == 0)
			floatInLong = *(long*)&minFloat;

		floatInFloat = *(float*)&floatInLong;
		datas[index].push_back(floatInFloat);
	}

	//long w = 0;
	// удаляем "плохие" вотермарки
	long delta = m_watermarks.size();
	if (m_watermarks.size() > 2)
	{
		while (true)
		{
			if (m_watermarks.size() > 2)
			{
				bool flag = true;
				time_t mint = m_watermarks[0].rtcTime;
				time_t maxt = m_watermarks[0].rtcTime;
				long mini = 0;
				long maxi = 0;
				for (long i = 1, sizei = m_watermarks.size(); i < sizei; i++)
				{
					if (mint > m_watermarks[i].rtcTime)
					{
						mint = m_watermarks[i].rtcTime;
						mini = i;
					}
					if (maxt < m_watermarks[i].rtcTime)
					{
						maxt = m_watermarks[i].rtcTime;
						maxi = i;
					}
				}
				time_t s = 0;
				for (long i = 1, sizei = m_watermarks.size(); i < sizei; i++)
					if (i != mini && i != maxi)
						s += m_watermarks[i].rtcTime;
				s /= m_watermarks.size() - 2;
				// проверяем отклонение от среднего минимума и максимума
				if (abs(mint - s) > delta)
					m_watermarks[mini].rtcTime = 0;
				if (abs(maxt - s) > delta)
					m_watermarks[maxi].rtcTime = 0;
				// удаляем обнулённые метки времени
				std::vector<WMInfo>::iterator iter;
				for (iter = m_watermarks.begin(); iter != m_watermarks.end();)
				{
					if (iter->rtcTime == 0)
					{
						iter = m_watermarks.erase(iter);
						m_stat.bad_pps++;
						flag = false;
					}
					else
						++iter;
				}
				if (flag)
					break;
			}
			else
				break;
		}
	}
	
	for (long i = 0, sizei = datas.size(); i < sizei; i++)
	{
		for (long w = 0, sizew = m_watermarks.size() - 1; w < sizew; w++)
		{
			double dtime1 = (double)m_watermarks[w].rtcTime;
			double dtime2 = (double)m_watermarks[w+1].rtcTime;
			long shift = m_watermarks[w].ppsSample;
			long size = m_watermarks[w + 1].ppsSample - m_watermarks[w].ppsSample;
			dtime2 = dtime1 + size / m_parcer.m_devicedata[index].freq();
			WRITER_ERROR_CODE werCode = m_writer[i].WriteData(datas[i].data() + shift, size, m_parcer.m_devicedata[index].freq(), dtime1, dtime2);

			if (werCode == wecSuccessful)
			{
				if (m_tStartTime == 0)
					m_tStartTime = m_watermarks[w].rtcTime;
				m_tEndTime = m_watermarks[w + 1].rtcTime;
				if (m_stat.first_rtc == 0)
					m_stat.first_rtc = m_watermarks[w].rtcTime;
				m_stat.last_rtc = m_watermarks[w + 1].rtcTime;
			}
		}
	}

	CString sDevice(m_adcInfo.DeviceName);
	sDevice.AppendFormat(L"_%u: ", m_adcInfo.SerialNumber);

	for (long w = 0, sizew = m_watermarks.size() - 1; w < sizew; w++)
	{
		// записываем качество данных
		if (TestDataQuality(m_watermarks[w].rtcTime, m_watermarks[w].ucQuality))
		{
			long size = m_qual.size();
			if (size > 1)
			{
				if (m_qual[size - 1].sync_qual != m_qual[size - 2].sync_qual)
					m_writer[0].WriteLog((double)m_qual[size - 1].rtc_time, sDevice + GetSynchroQualityMessage(m_qual[size - 1].sync_qual));
				if (m_qual[size - 1].powr_qual != m_qual[size - 2].powr_qual)
					m_writer[0].WriteLog((double)m_qual[size - 1].rtc_time, sDevice + GetPowerQualityMessage(m_qual[size - 1].powr_qual));
				if (m_qual[size - 1].data_qual != m_qual[size - 2].data_qual)
					m_writer[0].WriteLog((double)m_qual[size - 1].rtc_time, sDevice + GetDataQualityMessage(m_qual[size - 1].data_qual));
			}
		}
		m_stat.time_interval[m_watermarks[w + 1].ppsSample - m_watermarks[w].ppsSample] ++;
	}

	long rest = m_watermarks.back().ppsSample;

    long conv = 0;
    if (m_parcer.m_devicedata[0].wordsize() == 1)   // Если данные хранятся в short-ах
        conv = rest * datas.size() * sizeof(short);
    else   // Если данные хранятся в long-ах
        conv = rest * datas.size() * sizeof(long);

	fileTime = m_watermarks.back().rtcTime;
	m_watermarks.clear();

	return conv;
}

long CDatConverter::ConvertLogBufferToTrend(char *dataBuf, unsigned long ldSize, bool bLocal)
{
	long pos(0);
	char* bracket1 = dataBuf;
	char* bracket2 = NULL;
	char* bracket3 = NULL;
	SYSTEMTIME sSystemTime = { 0 };
	SYSTEMTIME sLocalTime = { 0 };
	CTime tTime;
	CString sMessage(L"");

	while (true)
	{
		bracket1 = strstr(bracket1, "[");
		if (bracket1 == NULL)
			break;
		bracket2 = strstr(bracket1, "]");
		if (bracket2 == NULL)
			break;

		sSystemTime.wYear = atoi(bracket1 + 1);
		sSystemTime.wMonth = atoi(bracket1 + 6);
		sSystemTime.wDay = atoi(bracket1 + 9);
		sSystemTime.wHour = atoi(bracket1 + 12);
		sSystemTime.wMinute = atoi(bracket1 + 15);
		sSystemTime.wSecond = atoi(bracket1 + 18);

		if (bLocal)
		{
			if (0 != SystemTimeToTzSpecificLocalTime(NULL, &sSystemTime, &sLocalTime))
				tTime = (CTime)sLocalTime;
		}
		else
			tTime = (CTime)sSystemTime;

		bracket3 = strstr(bracket2, "[");
		if (bracket3 != NULL)
			sMessage = CString(bracket2 + 2, bracket3 - bracket2 - 4);
		else
			sMessage = CString(bracket2 + 2);

		CString sDevice(m_adcInfo.DeviceName);
		sDevice.AppendFormat(L"_%u", m_adcInfo.SerialNumber);
		sMessage = sDevice + L": " + sMessage;

		if (!m_writer.empty())
			m_writer[0].WriteLog((double)tTime.GetTime(), sMessage);
		
		if (bracket3 == NULL)
			break;

		bracket1 = bracket3;
	}

	return 0;
}

void CDatConverter::ReadPPS(long *lngBuf, long sizeBuf, const unsigned int &countCh, const long &frq, const long &corrFactor)
{
	long lengthTrace;
	long rtc_time;
	long diff_pps;
	unsigned char synch_lev;
	unsigned char pow_lev;
	unsigned char dat_lev;
	unsigned char reserve_lev;

	for (int i = 0; i < sizeBuf - corrFactor; i++)
	{
		if (WMI_GetSamplePPS(lngBuf[i]))		// сначала ищем ППС ))
		{
			WMInfo wmInfo;		// если нашли ппс, то ищем временную метку в текущей секунде.

			wmInfo.ppsSample = i / countCh;

			lengthTrace = (i + frq * countCh) > (DWORD)sizeBuf ? sizeBuf - i : frq * countCh;	

			if ((WMI_GetLastTimeLabel(&lngBuf[i], lengthTrace, &rtc_time, &diff_pps, &synch_lev, &pow_lev, &dat_lev, &reserve_lev)) != -1)	// read watermark int
			{
				unsigned char ucQuality = dat_lev;
				ucQuality <<= SIZEOFQUALITY;
				ucQuality += pow_lev;
				ucQuality <<= SIZEOFQUALITY;
				ucQuality += synch_lev;
				WMF_PutWatermark((float*)wmInfo.timelabel.data(), wmInfo.timelabel.size(), rtc_time, ucQuality, 0);	// write watermark float
				wmInfo.rtcTime = rtc_time;
				wmInfo.ucQuality = ucQuality;
				m_watermarks.push_back(wmInfo);
			}

			i += (countCh - 1); // сдвигаемся на количество каналов, чтобы не ловить pps на них.
		}
	}
}

void CDatConverter::MakePPS(long sizeBuf, time_t rtc, unsigned int countCh, long frq, long corrFactor)
{
	for (long shift = corrFactor; shift <= sizeBuf;)
	{
		WMInfo wmInfo;
		wmInfo.ppsSample = shift / countCh;
		wmInfo.rtcTime = rtc;
		wmInfo.ucQuality = 0;
		m_watermarks.push_back(wmInfo);
		shift += countCh * frq;
		rtc++;
	}
}

int CDatConverter::CalcStartAcpOffset(const unsigned int &countCh)
{
	if (m_watermarks.size() > 1)
	{
		return m_watermarks[FIRSTUSEDWM].ppsSample * countCh;
	}

	return 0;
}

CString CDatConverter::RtcToString(time_t time)
{
	CTime timec = CTime(time);
	CString strtime;
	strtime.AppendFormat(L"[%04d-%02d-%02d  %02d:%02d:%02d] ", timec.GetYear(),
		timec.GetMonth(), timec.GetDay(), timec.GetHour(), timec.GetMinute(), timec.GetSecond());
	return strtime;
}

void CDatConverter::PrepareWriter()
{
	using namespace deviceparcer;

	if (!m_writer.empty())
	{
		for (long i = 0, size = m_writer.size(); i < size; i++)
			m_writer[i].StopWriter();
		m_writer.clear();
	}
	m_channels.Empty();

	m_parcer.load(m_adcInfo, m_Calibr);

	m_writer.resize(m_parcer.m_devicedata.size());
	std::vector<CSingleTrendWriter>::iterator witer = m_writer.begin();

	for (auto it = m_parcer.m_devicedata.begin(); it != m_parcer.m_devicedata.end(); ++it)
	{
		GUID gChannelID = { 0 };
		long lTypeADC = it->type();
		long lSerial = it->serial();
		long lChannel = it - m_parcer.m_devicedata.begin();
		*(long*)((long)&gChannelID + 12) = (lTypeADC << 8) + (lSerial << 16) + lChannel;
		CTime tStart(it->time());
		CString sName(it->comment());
		CString sConv(it->convert());

		if (!sName.IsEmpty())
			if (CheckStandartName(sName))
			{
				CString sTemp;
				sTemp.Format(L"%04u_", m_adcInfo.SerialNumber);
				sName = sTemp + sName;
			}
		if (sName.IsEmpty())
			sName.Format(L"%s_sig__%d_%04d", CString(m_adcInfo.DeviceName).GetString(), m_adcInfo.SerialNumber, lChannel + 1);
		if (sConv.IsEmpty())
			sConv = L"mV";
		m_channels += sName + L";";

		witer->SetSignalDirectory(m_sSignalDir);
		witer->SetCompressedDirectory(m_sCompresDir);
		CChannel *pChannelInfo = witer->GetChannelInfo();
		pChannelInfo->SetName(sName);// anpReader.GetComment());
		pChannelInfo->SetComment(L"");
		pChannelInfo->SetFrequency(it->freq());// anpReader.GetFrequency());
		pChannelInfo->SetMinLevel((float)it->absvolt());// anpReader.GetMinLevel());
		pChannelInfo->SetMaxLevel(it->maxlevel());// anpReader.GetMaxLevel());
		pChannelInfo->SetSense(it->sense());// anpReader.GetSense());
		pChannelInfo->SetReference(it->refer());// anpReader.GetReferense());
		pChannelInfo->SetShift(it->dc());// anpReader.GetShift());
		pChannelInfo->SetConversion(sConv);// anpReader.GetConversion());
		pChannelInfo->SetNumber(it->channel());// anpReader.GetNumber());
		pChannelInfo->SetTypeADC(lTypeADC);// lTypeADC);
		pChannelInfo->SetNumberDSP(0);// anpReader.GetNumberDsp());
		pChannelInfo->SetID(gChannelID);
		pChannelInfo->SetType(-1);
		pChannelInfo->SetGroupName(L"");// anpReader.GetGroupName());
		witer->SetDriveChecker(&m_checker);
		witer->SetDescriptorFileHelper(&m_helper);
		m_checker.SetSumFrequency(it->freq() * m_parcer.m_devicedata.size());
		witer->StartWriter(tStart);
		++witer;
	}
}

void CDatConverter::PrepareLogWriter()
{
	using namespace deviceparcer;

	if (!m_writer.empty())
	{
		for (long i = 0, size = m_writer.size(); i < size; i++)
			m_writer[i].StopWriter();
		m_writer.clear();
	}
	m_channels.Empty();
	m_writer.resize(m_Calibr.size());

	for (long i = 0, size = m_writer.size(); i < size; i++)
	{
		m_writer[i].SetSignalDirectory(m_sSignalDir);
		m_writer[i].SetCompressedDirectory(m_sCompresDir);
		m_writer[i].SetDriveChecker(&m_checker);
		m_writer[i].SetDescriptorFileHelper(&m_helper);
	}
}

void CDatConverter::ClearWriter()
{
	ZeroMemory(&m_adcInfo, sizeof(ADC_INFO_USB));
	if (!m_Calibr.empty())
		m_Calibr.clear();
	if (!m_parcer.m_devicedata.empty())
		m_parcer.m_devicedata.clear();
}

void CDatConverter::ClearStatistic()
{
	m_stat.Clear();
}

void CDatConverter::PrintStatistic()
{
	CString sTemp(L"");
	CTime tFirst(m_stat.first_rtc);
	CTime tLast(m_stat.last_rtc);
	CString sTimeFirst = tFirst.Format(L"[%Y-%m-%d  %H:%M:%S]");
	CString sTimeLast = tLast.Format(L"[%Y-%m-%d  %H:%M:%S]");
	sTemp.Format(L"Converted %d bytes\n", m_stat.conv_size);
	sTemp.Append(L"Time from " + sTimeFirst + L" to " + sTimeLast + "\n");
	sTemp.AppendFormat(L"Number of bad watermarks %d\n", m_stat.bad_pps);
	sTemp.AppendFormat(L"Number of bad writes %d\n", m_stat.bad_writes);
	std::map<long, long>::iterator iter;
	for (iter = m_stat.time_interval.begin(); iter != m_stat.time_interval.end(); ++iter)
		sTemp.AppendFormat(L"Interval numbers %8d   counts %8d\n", iter->first, iter->second);
	sTemp.Append(L"\n");
	OutputDebugString(sTemp);
}

void CDatConverter::SetCalibr(char* pBuffer, long size)
{
	if (pBuffer == nullptr)
		return;

	long offset(0);
	char* pos = pBuffer;
	char* cursor = pBuffer;
	CALIBR tempCalibr;
	while (pos != NULL || (cursor - pBuffer) >= size)
	{
		// имя канала
		pos = strstr(cursor, "\r\n");
		if (!pos)
			return;
		ZeroMemory(tempCalibr.name, sizeof(tempCalibr.name));
		memcpy_s(tempCalibr.name, (pos - cursor)*sizeof(char), cursor, (pos - cursor)*sizeof(char));
		cursor = pos + 2;
		// единица измерения канала
		pos = strstr(cursor, "\r\n");
		if (!pos)
			return;
		ZeroMemory(tempCalibr.conv, sizeof(tempCalibr.conv));
		memcpy_s(tempCalibr.conv, (pos - cursor)*sizeof(char), cursor, (pos - cursor)*sizeof(char));
		cursor = pos + 2;
		// коэффициент преобразования
		pos = strstr(cursor, "\r\n");
		if (!pos)
			return;
		tempCalibr.sense = (float)atof(cursor);
		cursor = pos + 2;
		// смещение постоянной составляющей
		pos = strstr(cursor, "\r\n");
		if (!pos)
			return;
		tempCalibr.DC = (float)atof(cursor);
		cursor = pos + 2;
		// опорный уровень
		pos = strstr(cursor, "\r\n");
		if (!pos)
			return;
		tempCalibr.refer = (float)atof(cursor);
		cursor = pos + 2;
		m_Calibr.push_back(tempCalibr);
	}
}

void CDatConverter::SetDevices(char* fileName)
{
	// Открываем и загружаем конфигурационный файл
	pugi::xml_document xmlDoc; pugi::xml_node xmlNode;
	pugi::xml_parse_result xmlDocParseResult = xmlDoc.load_file(fileName);

	if (!xmlDoc.empty())
	{
		xmlNode = xmlDoc.child(L"Config");
		xmlNode = xmlNode.child(L"Device");
		xmlNode = xmlNode.child(L"Channels");

		for (auto it = xmlNode.begin(), end = xmlNode.end(); it != end; ++it)
		{   // Проходим по всем каналам
			CALIBR tmpCALIBR;	// Временная структура для хранения структуры типа CALIBR

			CStringA channelName = CStringA(it->attribute(L"name").value());
			memcpy_s(tmpCALIBR.name, sizeof(tmpCALIBR.name), channelName.GetBuffer(), channelName.GetLength());

			CStringA channelConv = CStringA(it->attribute(L"units").value());
			memcpy_s(tmpCALIBR.conv, sizeof(tmpCALIBR.conv), channelConv.GetBuffer(), channelConv.GetLength());

			swscanf_s(it->child(L"Sense").first_child().value(), L"%f", &tmpCALIBR.sense);
			swscanf_s(it->child(L"Shift").first_child().value(), L"%f", &tmpCALIBR.DC);
			swscanf_s(it->child(L"Reference").first_child().value(), L"%f", &tmpCALIBR.refer);

			m_Calibr.push_back(tmpCALIBR);
		}
	}
}

void CDatConverter::SetAdcInfo(char* pBuffer, long size)
{
	if (size < sizeof(ADC_INFO_USB))
		return;

	memcpy_s(&m_adcInfo, sizeof(ADC_INFO_USB), pBuffer, sizeof(ADC_INFO_USB));

	// не знаю, что это значит, так было в регистраторе.
	if ((m_adcInfo.TestCode[0] == 0) && (m_adcInfo.TestCode[1] == 4660) && (m_adcInfo.TestCode[2] == 22136) && (m_adcInfo.TestCode[3] == 39612) && (m_adcInfo.TestCode[4] == 57072)) 
	{
		memcpy_s(&m_adcInfo, sizeof(ADC_INFO_USB), pBuffer+sizeof(unsigned short), sizeof(ADC_INFO_USB));
	}
}

bool CDatConverter::CheckStandartName(CString chanName)
{
	if (chanName.Left(4) != L"Sig_")
		return false;
	long mid = chanName.Find(L"_", 4);
	if (mid == -1)
		return false;
	long num1 = _wtoi(chanName.Mid(4, mid - 4));
	long num2 = _wtoi(chanName.Right(chanName.GetLength() - mid - 1));
	if (num1 == 0 || num2 == 0)
		return false;
	CString stmp;
	stmp.Format(L"Sig_%d_%d", num1, num2);
	if (stmp != chanName)
		return false;
	return true;
}

bool CDatConverter::TestDataQuality(time_t lrtc, unsigned char quality)
{
	unsigned char sync = quality & 0x03;
	unsigned char powr = (quality & 0x0C) >> 2;
	unsigned char qual = (quality & 0x30) >> 4;
	bool bsync(false);
	bool bpowr(false);
	bool bqual(false);
	long ret(0);
	if (m_qual.empty())
	{
		m_qual.resize(1);
		auto it = m_qual.back();
		it.rtc_time = lrtc;
		it.sync_qual = sync;
		it.powr_qual = powr;
		it.data_qual = qual;
	}
	else
	{
		auto it = m_qual.back();
		if (it.sync_qual != sync)
			bsync = true;
		if (it.powr_qual != powr)
			bpowr = true;
		if (it.data_qual != qual)
			bqual = true;
		if (bsync || bpowr || bqual)
		{
			m_qual.push_back(m_qual.back());
			m_qual.back().rtc_time = lrtc;
		}
		if (bsync)
			m_qual.back().sync_qual = sync;
		if (bpowr)
			m_qual.back().powr_qual = powr;
		if (bqual)
			m_qual.back().data_qual = qual;
	}

	return (bsync || bpowr || bqual);
}

CString CDatConverter::GetSynchroQualityMessage(unsigned char quality)
{
	switch (quality)
	{
	case 0: return L"No syncronisation quality"; break;
	case 1: return L"Bad syncronisation quality"; break;
	case 2: return L"Good syncronisation quality"; break;
	case 3: return L"Excelent syncronisation quality"; break;
	}
	return L"";
}

CString CDatConverter::GetPowerQualityMessage(unsigned char quality)
{
	switch (quality)
	{
	case 0: return L"No power quality"; break;
	case 1: return L"Bad power quality"; break;
	case 2: return L"Good power quality"; break;
	case 3: return L"Excelent power quality"; break;
	}
	return L"";
}

CString CDatConverter::GetDataQualityMessage(unsigned char quality)
{
	switch (quality)
	{
	case 0: return L"No data quality"; break;
	case 1: return L"Bad data quality"; break;
	case 2: return L"Good data quality"; break;
	case 3: return L"Excelent data quality"; break;
	}
	return L"";
}

long CDatConverter::GetDataSizePerSec()
{
	// Данная функция необходима для расчета размер данных односекундной записи сигналов
    long retVal = 0;

    if (m_parcer.m_devicedata.size() > 0)
        retVal = m_parcer.m_devicedata[0].freq() * m_parcer.m_devicedata[0].wordsize() * sizeof(short) * m_parcer.m_devicedata.size();

    return retVal;
}