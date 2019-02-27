#pragma once

#ifndef deviceinfoparcer_h
#define deviceinfoparcer_h


#include <ADC_INFO_USB.h>
#include <CALIBR.h>
#include <NEW_CHREC.h>
#include <string>
#include <vector>

namespace deviceparcer
{
	//	Из-за того, что коды устройств в файле ADCINF.dat велась до какого-то момента не очень логично, то
	//	имеются устройства, коды которых не совпадают с указанными в документации.
	//	Для ZET210 имеется несколько ревизий для разных вариантов исполнения с кодами 7, 8, 10
	//	Для ZET017U8 имеется несколько вариантов исполнения, но во всех код 3 (есть тестовый с кодом 15)
	enum devicetype
	{
		zdtNone = 0
		,zdtADC8_500_R = 3	//ZET017U8
		,zdtZET017U8_32 = 4
		,zdtSigma_R4 = 7	//ZET210
		,zdtSigma_R5 = 8	//ZET210
		,zdtACPB = 9
		,zdtSigma = 10		//ZET210
		,zdtDelta = 12		//ZET110
		,zdtADC8_500 = 14	//ZET017U8
		,zdtACPB_2_500 = 15	//ZET017U8
		,zdtOmega = 16		//ZET220
		,zdtZET230 = 17		//ZET230
		,zdtZET240 = 19		//ZET240
	};


	class channeldata; // определяем класс, но не описываем его, для того, чтобы использовать его идентификатор при описании deviceparcer, а так же для того чтобы в channeldata объявить дружественную функцию из deviceparcer


	class deviceparcer
	{
	public:

		deviceparcer();
		~deviceparcer();

		void load(const ADC_INFO_USB &adcInfo, const CALIBR &calibr, FILE* fcal);
		void load(const ADC_INFO_USB &adcInfo, const std::vector<CALIBR> &calibr);
		void increment_second();

// 		template <class fFunc>
// 		void for_each(fFunc func);
		
		std::vector<channeldata> m_devicedata;

	private:

				

		void GetComment(std::string &stroka, FILE* fcal, const CALIBR &calibr);
		void GetGain(float &gain, const devicetype &type, const unsigned short &codAmplify);
		void GetAbsvolt(double &digres, const devicetype &type, const unsigned long &lowAdc, const unsigned long &chanAdc);
		void GetConvert(std::string &stroka, FILE* fcal, const CALIBR &calibr);
		void GetSense(float &sense, FILE* fcal, const CALIBR &calibr);
		void GetAmpl(float &ampl, const devicetype &type, const ADC_INFO_USB &adcInfo, const int &chNumber);
		void GetMaxLevel(float &maxlevel, const devicetype &type, const double &digres, const float &sense);
		void GetDc(float &dc, FILE* fcal, const CALIBR &mycal);
		void GetRefer(float &refer, FILE* fcal, const CALIBR &mycal);
		void GetWordSize(int &wordsize, const devicetype &type);
		void GetTmi(int &tmi);
		void GetFrl(float &frl);
		void GetAfch(int &afch);
		void GetTime(SYSTEMTIME &time, const ADC_INFO_USB &adcInfo);
		void GetSerial(unsigned long &serial, const ADC_INFO_USB &adcInfo);

		int GetFrq(double oporFreq, const ADC_INFO_USB* AdcInfo, const int &numchan, const devicetype &type, int &frh);
		int GetFrqAPCB(double &oporFreq, const ADC_INFO_USB* AdcInfo);
		int GetFrqSigma(double &oporFreq, const ADC_INFO_USB* AdcInfo, const int &numchan);
		int GetFrqDelta(double &oporFreq, const ADC_INFO_USB* AdcInfo);
		int GetFrqADC8_500(const ADC_INFO_USB* AdcInfo);
		int GetFrqACPB_2_500(double &oporFreq, const ADC_INFO_USB* AdcInfo);
		int GetFrqOmega(double &oporFreq, const ADC_INFO_USB* AdcInfo);
		int GetFrqZET230(const ADC_INFO_USB* AdcInfo);
		int GetFrqZET240(const ADC_INFO_USB* AdcInfo);

		double GetOporFrq(const devicetype &type, const ADC_INFO_USB* AdcInfo);

	};




	class channeldata
	{
	public:

		channeldata::channeldata()
		 : m_frh(0)
		 , m_tmi(0)
		 , m_frl(0.0f)
		 , m_afch(0)
		 , m_gain(0)
		 , m_absvolt(0)
		 , m_maxlevel(0)
		 , m_ampl(0)
		 , m_dc(0)
		 , m_sense(0)
		 , m_freq(0)
		 , m_channel(0)
		 , m_serial(0)
		 , m_refer(0.f)
		 , m_wordsize(25000)
		{
			m_time = { 0 };
		}

		channeldata::~channeldata()
		{}

		friend void deviceparcer::load(const ADC_INFO_USB &adcInfo, const CALIBR &calibr, FILE* fcal);	
		friend void deviceparcer::load(const ADC_INFO_USB &adcInfo, const std::vector<CALIBR> &calibr);
		friend void deviceparcer::increment_second();

		const char* comment() {return m_comment.data();}
		float gain() {return m_gain;}
		double absvolt() {return m_absvolt;}
		int freq() {return m_freq;}
		devicetype type() {return m_type;}
		int frh() {return m_frh;}
		int tmi() {return m_tmi;}
		float frl() {return m_frl;}
		int channel() {return m_channel;}
		unsigned long serial() {return m_serial;}
		const char* convert() {return m_convert.data();}
		float sense() {return m_sense;}
		float ampl() {return m_ampl;}
		float maxlevel() {return m_maxlevel;}
		float dc() {return m_dc;}
		float refer() {return m_refer;}
		int afch() {return m_afch;}
		int wordsize() {return m_wordsize;}
		SYSTEMTIME time() {return m_time;}


	private:

		std::string m_comment;
		float m_gain;
		double m_absvolt;
		int m_freq;
		devicetype m_type;
		int m_frh;
		int m_tmi;
		float m_frl;
		int m_channel;
		unsigned long m_serial;
		std::string m_convert;
		float m_sense;
		float m_ampl;
		float m_maxlevel;
		float m_dc;
		float m_refer;
		int m_afch;
		int m_wordsize;
		SYSTEMTIME m_time;
	};


}


#endif