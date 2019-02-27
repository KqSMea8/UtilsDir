//----------------------------------------------------------------------
// Файл Option.cpp
// Версия от 6.06.2012
// ЗАО "ЭТМС", Зеленоград, Антонов А.Ю.
//----------------------------------------------------------------------
#include "stdafx.h"
#include "Option\Option.h"
//----------------------------------------------------------------------
#define STD_QUANTITY										9
#define SOFTWARE_OPTION_QUANTITY							30
#define HARDWARE_OPTION_QUANTITY							11

#define ZETLAB_DEMO											0x3			
#define ZETLAB_ANALIZ										0x2f1f97
#define ZETLAB_VIBRO										0x2f9f9f	
#define ZETLAB_NOISE										0x1e93		
#define ZETLAB_TENZO										0x273e17	
#define ZETLAB_SEISMO										0x2f9f93	
#define ZETLAB_SEISMO_DAC									ZETLAB_SEISMO | 0x4
#define ZETLAB_BASE											0x7			
#define ZETLAB_SENSOR										0x1a60c03	
#define RESERVE_MASK										0x7f024060

//стандартные программные прошивки
const long lStdSoftwareMasks[STD_QUANTITY] =			   {ZETLAB_DEMO,
															ZETLAB_BASE,
															ZETLAB_NOISE,
															ZETLAB_SENSOR,
															ZETLAB_SEISMO,
															ZETLAB_SEISMO_DAC,
															ZETLAB_TENZO,
															ZETLAB_ANALIZ,
															ZETLAB_VIBRO};
//названия стандартных программных прошивок
const CString sStdSoftwareOptions[STD_QUANTITY] =		   {L"ZETLabDemo",
															L"ZETLabBase",
															L"ZETLabNoise",
															L"ZETLabSensor",
															L"ZETLabSeismo",
															L"ZETLabSeismo",
															L"ZETLabTenzo",
															L"ZETLabAnaliz",
															L"ZETLabVibro"};
//названия отдельных программных опций
const CString sSoftwareOptions[SOFTWARE_OPTION_QUANTITY] = {L"Базовые программы",
															L"Генераторы",
															L"СУВ",
															L"Тахометр, Торсиограф, Энкодер",
															L"",
															L"",
															L"Долеоктавные спектры, Виброметр, АЧХ",
															L"Взаимные кор. и узк. спектры",
															L"Синхр. нак., Мод. анализ, Анализ нел. иск.",
															L"Регистрация",
															L"Фильтрация",
															L"ZETLabStudio",
															L"Термометры, Тензометр",
															L"",
															L"ZETView",
															L"Регулятор, Формула",
															L"Передатчик сигналов",
															L"Подключение к передатчику",
															L"ZETView (exe)",
															L"Внешние приборы",
															L"Гистограмма, STA-LTA, Вейвлет",
															L"Метрологические настройки",
															L"Операторские настройки",
															L"Подключение к ZET7176",
															L"",
															L"",
															L"",
															L"",
															L"",
															L""};
//названия отдельных аппаратных опций
const CString sHardwareOptions[HARDWARE_OPTION_QUANTITY] = {L"PrUs",
															L"ICP",
															L"Atten",
															L"SD-card",
															L"Ethernet",
															L"Bluetooth",
															L"ШИМ",
															L"Логический анализатор",
															L"TEDs",
															L"GPS",
															L"PTP"};
//----------------------------------------------------------------------
COption::COption()
{
	
}
//----------------------------------------------------------------------
COption::~COption()
{
	
}
//----------------------------------------------------------------------
CString COption::GetSoftware(long lMask)
{
	CString sOption(L"");

	//убираем все резервные биты
	lMask &= (~RESERVE_MASK);
	
	for (int i = 0; i < STD_QUANTITY; ++i)
	{
		//стандартная опция
		if (((lMask & lStdSoftwareMasks[i]) == lStdSoftwareMasks[i]) && ((lMask & (~lStdSoftwareMasks[i])) == 0))
		{
			return sStdSoftwareOptions[i];
		}
	}
	
	for (int i = STD_QUANTITY - 1; i >= 0; --i)
	{
		//стандартная опция с чем-то
		if ((lMask & lStdSoftwareMasks[i]) == lStdSoftwareMasks[i])
		{
			sOption = sStdSoftwareOptions[i];
			long lOptions = lMask & (~lStdSoftwareMasks[i]);
			for (int j = 1; j < sizeof(long) * 8 - 1; ++j)
			{
				if ((lOptions >> j) & 0x1)
				{
					sOption.Append(L" + ");
					sOption.Append(sSoftwareOptions[j - 1]);
				}
			}
			return sOption;
		}
	}
	//набор отдельных опций
	for (int j = 1; j < SOFTWARE_OPTION_QUANTITY + 1; ++j)
	{
		if ((lMask >> j) & 0x1)
		{
			if(sOption.GetLength() > 0)
				sOption.Append(L" + ");
						
			sOption.Append(sSoftwareOptions[j - 1]);
		}
	}
	return sOption;
}
//----------------------------------------------------------------------
CString COption::GetHardware(long lMask)
{
	CString sOption(L"");
	//набор отдельных опций
	for (int i = 0; i < HARDWARE_OPTION_QUANTITY; ++i)
	{
		if ((lMask >> i) & 0x1)
		{
			if(sOption.GetLength() > 0)
				sOption.Append(L" + ");

			sOption.Append(sHardwareOptions[i]);
		}
	}
	return sOption;
}
//----------------------------------------------------------------------