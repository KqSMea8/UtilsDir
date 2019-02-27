#include "stdafx.h"
#include "DeviceInfoParcer.h"
#include <algorithm>


#define TIME_LOCATION_OFFSET 16


namespace deviceparcer
{

	deviceparcer::deviceparcer()
	{}

	deviceparcer::~deviceparcer()
	{}


	void deviceparcer::load(const ADC_INFO_USB &adcInfo, const CALIBR &calibr, FILE* fcal)
	{
		unsigned short countCh = adcInfo.WorkChADC;

		for(int i = 0; i < countCh; i++)
		{
			channeldata currCD;

			GetTime(currCD.m_time, adcInfo);

			currCD.m_type = (devicetype)adcInfo.ModelNumber;

			GetComment(currCD.m_comment, fcal, calibr);

			GetGain(currCD.m_gain, currCD.type(), adcInfo.CodAmplify[i]);

			GetAbsvolt(currCD.m_absvolt, currCD.type(), adcInfo.DigitalResolutionLowADC, adcInfo.DigitalResolChanADC[i]);

			currCD.m_freq = GetFrq( GetOporFrq(currCD.type(), &adcInfo), &adcInfo, countCh, currCD.type(), currCD.m_frh);

			GetWordSize(currCD.m_wordsize, currCD.type());

			currCD.m_channel = i + 1;

			GetSerial(currCD.m_serial, adcInfo);

			GetConvert(currCD.m_convert, fcal, calibr);

			GetSense(currCD.m_sense, fcal, calibr);

			GetAmpl(currCD.m_ampl, currCD.type(), adcInfo, i);

			GetDc(currCD.m_dc, fcal, calibr);	

			GetRefer(currCD.m_refer, fcal, calibr);
							
			GetTmi(currCD.m_tmi);

			GetFrl(currCD.m_frl);

			GetAfch(currCD.m_afch);

			if(!currCD.m_ampl) currCD.m_ampl = 1;
			if(!currCD.m_absvolt) currCD.m_absvolt = 1;
			if(!currCD.m_gain) currCD.m_gain = 1;
			if(!currCD.m_sense) currCD.m_sense = 1;

			GetMaxLevel(currCD.m_maxlevel, currCD.type(), currCD.absvolt(), currCD.sense());
							
			m_devicedata.push_back(currCD);
						
			if(fcal != NULL)	//  так было в регистраторе, на всякий случай оставил.
			{
				char chbuf[300];
				fgets(chbuf, 300, fcal);
			}
		}
	}


	void deviceparcer::load(const ADC_INFO_USB &adcInfo, const std::vector<CALIBR> &calibr)
	{
		unsigned short countCh = adcInfo.WorkChADC;

		if (!m_devicedata.empty())
			m_devicedata.clear();

		for (int i = 0; i < countCh; i++)
		{
			channeldata currCD;
			GetTime(currCD.m_time, adcInfo);
			currCD.m_type = (devicetype)adcInfo.ModelNumber;
			//GetComment(currCD.m_comment, fcal, calibr);
			GetGain(currCD.m_gain, currCD.type(), adcInfo.CodAmplify[i]);
			GetAbsvolt(currCD.m_absvolt, currCD.type(), adcInfo.DigitalResolutionLowADC, adcInfo.DigitalResolChanADC[i]);
			currCD.m_freq = GetFrq(GetOporFrq(currCD.type(), &adcInfo), &adcInfo, countCh, currCD.type(), currCD.m_frh);
			GetWordSize(currCD.m_wordsize, currCD.type());
			currCD.m_channel = i + 1;
			GetSerial(currCD.m_serial, adcInfo);
			//GetConvert(currCD.m_convert, fcal, calibr);
			//GetSense(currCD.m_sense, fcal, calibr);
			GetAmpl(currCD.m_ampl, currCD.type(), adcInfo, i);
			//GetDc(currCD.m_dc, fcal, calibr);
			//GetRefer(currCD.m_refer, fcal, calibr);
			GetTmi(currCD.m_tmi);
			GetFrl(currCD.m_frl);
			GetAfch(currCD.m_afch);

			if (i < (int)calibr.size())
			{
				currCD.m_comment.assign(calibr[i].name);
				currCD.m_convert.assign(calibr[i].conv);
				currCD.m_sense = calibr[i].sense;
				currCD.m_dc = calibr[i].DC;
				currCD.m_refer = calibr[i].refer;
			}

			if (!currCD.m_ampl) currCD.m_ampl = 1;
			if (!currCD.m_absvolt) currCD.m_absvolt = 1;
			if (!currCD.m_gain) currCD.m_gain = 1;
			if (!currCD.m_sense) currCD.m_sense = 1;

			GetMaxLevel(currCD.m_maxlevel, currCD.type(), currCD.absvolt(), currCD.sense());

			m_devicedata.push_back(currCD);
		}
	}



	void deviceparcer::increment_second()
	{
		for(auto currCD = m_devicedata.begin(), endIt = m_devicedata.end(); currCD != endIt; ++currCD)
		{
			CTime ctime(currCD->m_time);	// увеличиваем на 1 сек.

			ctime += 1;
			
			ctime.GetAsSystemTime(currCD->m_time);
		}
	}



// 	template <class fFunc>
// 	void deviceparcer::for_each(fFunc func)
// 	{
// 		std::for_each(m_devicedata.begin(), m_devicedata.end(), func);
// 	}



	int deviceparcer::GetFrq(double oporFreq, const ADC_INFO_USB* AdcInfo, const int &numchan, const devicetype &type, int &frh)
	{
		int frq = 0;

		switch(type)
		{
			case zdtACPB:	//ACPB
				frq = GetFrqAPCB(oporFreq, AdcInfo);
				break;

			case zdtSigma_R4:
			case zdtSigma_R5:
			case zdtSigma:	//Sigma
				frq = GetFrqSigma(oporFreq, AdcInfo, numchan);
				break;

			case zdtDelta:	//Delta
				frq = GetFrqDelta(oporFreq, AdcInfo);
				break;

			case zdtZET017U8_32:
			case zdtADC8_500_R:
			case zdtADC8_500:	//ADC8-500
				frq = GetFrqADC8_500(AdcInfo);
				break;

			case zdtACPB_2_500://15)	//ACPB 2-500
				frq = GetFrqACPB_2_500(oporFreq, AdcInfo);
				break;

			case zdtOmega:	//ZET 220
				frq = GetFrqOmega(oporFreq, AdcInfo);
				break;

			case zdtZET230:	//ZET 230
				frq = GetFrqZET230(AdcInfo);
				break;

			case zdtZET240:	//ZET 240
				frq = GetFrqZET240(AdcInfo);
		}

		if(!frq) 
			frq = 1;

		frh = frq / 2;

		return frq;
	}



	int deviceparcer::GetFrqAPCB(double &oporFreq, const ADC_INFO_USB* AdcInfo)
	{
		return int(oporFreq / AdcInfo->Rate);
	}

	int deviceparcer::GetFrqSigma(double &oporFreq, const ADC_INFO_USB* AdcInfo, const int &numchan)
	{
		return int(oporFreq / AdcInfo->Rate / numchan);
	}

	int deviceparcer::GetFrqDelta(double &oporFreq, const ADC_INFO_USB* AdcInfo)
	{
		if((AdcInfo->ModaADC & 0xF0) == 0)		// Старая версия
		{
			switch (AdcInfo->ModaADC)
			{
			case 0x00:
				return int(oporFreq / AdcInfo->Rate);	//высокочастотная мода
				break;

			case 0x01:
				return 50000;
				break;

			case 0x02:
				return 25000;
				break;

			case 0x03:
				return 5000;
				break;

			case 0x04:
				return 2500;
				break;;
			}
		}
		else
		{
			struct myPair{int key; int val;};
			myPair  newDeltaFrq[] = 
			{
				{0x10,	50000},	// Шумомер
				{0x20,	5000},	// Виброметр
				{0x21, 2500},
				{0x22, 500},
				{0x30, 500000},		// Регистратор
				{0x31, 400000},
				{0x32, 250000},
				{0x33, 200000},
				{0x34, 100000},
				{0x35, 50000},
				{0x36, 25000},
				{0x37, 5000},
				{0x38, 2500},
				{0x39, 500},
				{0, NULL}
			};

			// новая версия
			for (int j = 0; newDeltaFrq[j].key != 0; ++j)
			{
				if (AdcInfo->ModaADC != newDeltaFrq[j].key)
					continue;

				return newDeltaFrq[j].val;
			}
		}

		return 0;
	}


	int deviceparcer::GetFrqADC8_500(const ADC_INFO_USB* AdcInfo)
	{
		int adcs[] = { 50000, 25000, 5000, 2500 };

		return adcs[AdcInfo->ModaADC-1];
	}



	int deviceparcer::GetFrqACPB_2_500(double &oporFreq, const ADC_INFO_USB* AdcInfo)
	{
//		int freq; не используемая переменная
		if(AdcInfo->ModaADC == 0)
			return 1000000 / AdcInfo->Rate;
		else
			return 500000 / AdcInfo->Rate;

		return 0;
	}



	int deviceparcer::GetFrqOmega(double &oporFreq, const ADC_INFO_USB* AdcInfo)
	{
		USHORT WorkChADC;
		int frq = 0;
		unsigned short rate = AdcInfo->Rate;

		if(rate < 1)
			rate = 1;

		if(rate < 1)
			rate = 1;

		if(AdcInfo->ModaADC == 0)
		{
			frq = (int)((float)oporFreq / 256 / rate);
		}
		else
		{
			WorkChADC = AdcInfo->WorkChADC;

			if(WorkChADC < 1)
				WorkChADC = 1;

			if(WorkChADC == 3)
				WorkChADC = 4;

			if(WorkChADC > 4 && WorkChADC < 8)
				WorkChADC = 8;

			if(WorkChADC > 8)
				WorkChADC = 16;

			frq = (int)((float)oporFreq / 1860.f / rate / WorkChADC);
		}

		return (frq > 500000) ? 500 : frq;
	}



	int deviceparcer::GetFrqZET230(const ADC_INFO_USB* AdcInfo)
	{
		if(AdcInfo->ModaADC < 2)
			return 100000 / AdcInfo->Rate;
		else
			return 96000 / AdcInfo->Rate;

		return 0;
	}



	int deviceparcer::GetFrqZET240(const ADC_INFO_USB* AdcInfo)
	{
		int freq[7] = { 2500, 1000, 500, 250, 100, 50, 2500 };
		if (AdcInfo->Rate < 7)
			return freq[AdcInfo->Rate];
		else
			return freq[6];

// 		int freq[7]= {	2500, 1000,	500, 250, 100, 50, 2500	};
// 		if (AdcInfo->Rate > sizeof(freq) / sizeof(freq[0]) )
// 			return freq[6];
// 		else
// 			return freq[AdcInfo->Rate];

		return 0;
	}



	double deviceparcer::GetOporFrq(const devicetype &type, const ADC_INFO_USB* AdcInfo)
	{
		if (type <= 14)
		{
			return 80e+6;
		}

		if (type == 15)
		{
			if (AdcInfo->ModaADC == 0)
				return 1e+6;
			else
				return 5e+5;
		}

		if (type == 16)
		{
			if (AdcInfo->ModaADC == 0)
				return 12.8e+6;
			else
				return 14.88e+6;
		}

		if (type == 17)
		{
			if (AdcInfo->ModaADC < 2)
				return 12.8e+6;
			else
				return 12.288e+6;
		}

		return 0.0;
	}



	void deviceparcer::GetComment(std::string &stroka, FILE* fcal, const CALIBR &calibr)
	{
		stroka.resize(300);

		if(fcal != NULL)
		{
			if (fgets(&stroka[0], 300, fcal))
			{
				//int len = strlen(stroka.data());
				int len = stroka.length();
				if ((UINT)len > stroka.size())
					len = stroka.size() - 1;
				if(len > 2)
					stroka[len - 2] = 0;
			}
			else
				stroka[0] = 0;
		}
		else
			if(&calibr)
				strcpy_s(&stroka[0], 300, calibr.name);
	}



	void deviceparcer::GetGain(float &gain, const devicetype &type, const unsigned short &codAmplify)
	{
		gain = 1;
        if (type == zdtDelta || type == zdtACPB || type == zdtADC8_500 || type == zdtACPB_2_500 || type == zdtADC8_500_R)	//Delta or ACPB
		{
			if (codAmplify != 0)
				gain = pow(10.f, (int)codAmplify);
		}
	}



	void deviceparcer::GetAbsvolt(double &digres, const devicetype &type, const unsigned long &lowAdc, const unsigned long &chanAdc)
	{
		if(type == zdtACPB_2_500)	//	временный костыль.
		{
			digres = *(float*)&lowAdc;
		}
		else
			if(chanAdc != 0)
			{
				digres = *(float*)&chanAdc;	
			}	

		if(!digres)
			digres = *(float*)&lowAdc;
	}



	void deviceparcer::GetConvert(std::string &stroka, FILE* fcal, const CALIBR &calibr)
	{
		stroka.resize(300);

		if(fcal != NULL)
		{
			if (fgets(&stroka[0], 300, fcal))
			{
				//int len = strlen(stroka.data());
				int len = stroka.length();

				if ((UINT)len > stroka.size())
					len = stroka.size() - 1;

				if(len > 2)
					stroka[len - 2] = 0;					
			}
			else
				stroka[0] = 0;
		}
		else
			if(&calibr)
				strcpy_s(&stroka[0], 300, calibr.conv);
	}



	void deviceparcer::GetSense(float &sense, FILE* fcal, const CALIBR &calibr)
	{
		if(fcal != NULL)
			fscanf_s(fcal,"%g", &sense);	//если файл калибровки открылся, то считываем чувствительность из него
		else
			if(&calibr)
				sense = calibr.sense;		//иначе, присваиваем значение mycal.sense
	
		if (!sense)
			sense = 0.0001f;				//заплатка, если вдруг в файле калибровки закисан 0.000000
	}



	void deviceparcer::GetAmpl(float &ampl, const devicetype &type, const ADC_INFO_USB &adcInfo, const int &chNumber)
	{
		if(chNumber < 16 && type != zdtOmega && type != zdtSigma && type != zdtZET230 && type != zdtZET240)
			ampl = *(float*)&adcInfo.DigitalResolChanADC[16 + chNumber];
		else
			ampl = 1.f;
	}



	void deviceparcer::GetMaxLevel(float &maxlevel, const devicetype &type, const double &digres, const float &sense)
	{
		long bitQuan = 16;
		switch (type)
		{
		case zdtDelta:  bitQuan = 16;	//16.08.12
			break;
		case zdtOmega:  bitQuan = 32;	//ZET220
			break;
		case zdtZET230: bitQuan = 32;	//ZET230
			break;
		case zdtZET240: bitQuan = 32;	//ZET240
			break;
		default: bitQuan = 16;
			break;
		};

		if(type == zdtDelta)
			maxlevel = float( digres * pow(2.0, bitQuan) * 4 ) / sense;	// 16.08.12 Серега Кашников сказал, что для шумомера формула должна быть такая.
		else
			maxlevel = float( digres / sense * pow(2.0, bitQuan) / 2 );
	}



	void deviceparcer::GetDc(float &dc, FILE* fcal, const CALIBR &calibr)
	{
		if(fcal != NULL)
			fscanf_s(fcal,"%g",&dc);
		else
			if(&calibr)
				dc = calibr.DC;
	}



	void deviceparcer::GetRefer(float &refer, FILE* fcal, const CALIBR &calibr)
	{
		if(fcal != NULL)
			fscanf_s(fcal,"%g",&refer);
		else
			if(&calibr)
				refer = calibr.refer;
	}



	void deviceparcer::GetWordSize(int &wordsize, const devicetype &type)
	{
		if ((type == zdtOmega) ||		// ZET220
			(type == zdtZET230) ||		// ZET230
			(type == zdtZET240) ||		// ZET240
			(type == zdtZET017U8_32))	// ZET017 32bit
			wordsize = 2;
		else
			wordsize = 1;
	}



	void deviceparcer::GetTmi(int &tmi)
	{

	}



	void deviceparcer::GetFrl(float &frl)
	{

	}



	void deviceparcer::GetAfch(int &afch)
	{

	}



	void deviceparcer::GetTime(SYSTEMTIME &time, const ADC_INFO_USB &adcInfo)
	{
		__time64_t time64 = 0;

		memcpy_s((void*)&time64, 2 * sizeof(unsigned short), (void*)&adcInfo.CodAmplify[TIME_LOCATION_OFFSET], 2 * sizeof(unsigned short));
		if (time64 != 0)
		{
			tm tm_time;
			_localtime64_s(&tm_time, &time64);
			_mktime64(&tm_time);
			
			time.wHour = tm_time.tm_hour;
			time.wMinute = tm_time.tm_min;
			time.wSecond = tm_time.tm_sec;
			time.wYear = tm_time.tm_year + 1900;
			time.wMonth = tm_time.tm_mon + 1;
			time.wDay = tm_time.tm_mday;
		}
		else
			GetLocalTime(&time);


		long msTime = 0;
		if ((devicetype)adcInfo.ModelNumber == zdtZET240)
			memcpy_s((void*)&msTime, 2 * sizeof(unsigned short), (void*)&adcInfo.CodAmplify[TIME_LOCATION_OFFSET + 2], 2 * sizeof(unsigned short));

		time.wMilliseconds = (WORD)msTime;
	}

	void deviceparcer::GetSerial(unsigned long &serial, const ADC_INFO_USB &adcInfo)
	{
		serial = adcInfo.SerialNumber;
	}


} // namespace deviceparcer