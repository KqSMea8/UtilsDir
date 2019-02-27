//------------------------------------------------------------------------------
#pragma once

 #define MinSumTimeReaddForOneKit	1.		// в сек
 #define MaxSumTimeReaddForOneKit	100.	// в сек
// --------------------------------------------------------------------------------
/*	Файл ThreadSRV_CollectionOfKits.h
Зеленоград, ООО "ЭТМС", Савостин Ю.М.
--------------------------------------------------------------------------------
	Шаблонный класс CThreadSRV_CollectionOfKits предназначен для потокового
 синхронного чтения данных с нескольких каналов и вызова функции их обработки.
 --------------------------------------------------------------------------------
	Каналы сервера, предназначенные для синхронной обработке данных, объединяются
в комплекты каналов. Кол-во каналов в комплекте произвольное. В каждом комплекте
своё время, свой интервал чтения и свой временной интервал данных. Каждый комплект
не обязан что-либо знать о других комплектах. Частота дискретизации каналов в
комплекте не обязана быть одинаковой.
	Класс работает с коллекцией комплектов. Коллекция всегда одна.
Кол-во комплектов в коллекции произвольное.
	Коллекция комплектов каналов реализована с помощью std::vector.
 --------------------------------------------------------------------------------
	Положительные моменты использования класса:
- выполнение необходимых проверок;
- вызов NumFileUsed;
- работа со временами каналов;
- наличие механизмов синхронизации потоков при:
  - обработке событий сервера;
  - отображении результатов обработки синхронных данных;
- при добавлении в коллекцию комплекта запрещаются обращения к коллекции;
- при изменении состава каналов в коллекции запрещаются обращения к этой коллекции;
- при изменении канала в коллекции запрещаются обращения к этому каналу;
- автоматическое задание размера буфера чтения каналов.
--------------------------------------------------------------------------------
	Любая программа предназначена для вычисления и отображения в окнах программы
каких-то параметров, например: массивов для отображения в виде графиков и/или
чисел для отображения в Edit и Static. Для использования в программе экземпляра
данного класса существует три ключевых момента:
	1. Должна быть определена структура, содержащая отображаемые параметры.
Например: STRUCT_CALC_AND_VIEW. Кроме отображаемых параметров структура может
содержать вспомогательные объекты, необходимые для расчета этих параметров,
которые целесообразно хранить в промежутках между расчетами параметров.
	2. Должна быть функция расчета значений отображаемых параметров по данным,
считанным с SRV. Например: Calculation. Основной параметр этой функции -
указатель на экземпляр структуры STRUCT_CALC_AND_VIEW. Т.к. эта функция будет
запускаться в рабочем потоке программы, то она должна быть глобальной.
	3. Из рабочего потока в окно интерфейса программы будут посылаться сообщения
WM_NEW_DATA_SRV. Это говорит о том, что рассчитаны очередные параметры. Параметр
WPARAM функции-обработчика этого сообщения будет содержать индекс, по которому
следует определять экземпляр структуры STRUCT_CALC_AND_VIEW, который содержит
новые параметры для отображения в интерфейсе программы.
--------------------------------------------------------------------------------
	Синхронное чтение данных и их обработка выполняется в рабочем потоке. Функция
этого потока - это первичная глобальная функция, которая не видна пользователю класса.
В этой функции выполняется код реализации положительных моментов (см. выше). После
чтения синхронных данных вызывается функция вторичная глобальнная функция, т.е.
функция Calculation, в которую передаётся указатель на соответствующую структуру
STRUCT_CALC_AND_VIEW.
	Адрес и параметры функции Calculation передаются в класс при его создании как
параметры функции Create. Прототип вторичных функций:
typedef long (*FunctionThread)(void *);
Возвращать вторичная функция должна экземпляр структуры CodeQuality, у которой есть
оператор преобразования к long. Если флаг AllData этой структуры равен
Quality_AllData_True, то рабочий поток посылает в поток GUI сообщение
WM_NEW_DATA_SRV, что говорит о наличии новой порции синхронных данных.
Следующее чтение данных этого комплета возможно только после копирования этих
данных в потоке GUI для их отображения в интерфейсе. Для этого используется
функция класса SetSynchro(indxKit).
	Если поток остановить, то сервер не будет получать NumFileUsed, и, как
следствие, потеряет процесс. Поэтому для временного прекращения чтения данных
лучше использовать свойство EnableReadData.
--------------------------------------------------------------------------------
	Не забываем о функциях базового класса:
virtual long End();				// асинхронная команда потоку "Конец"
virtual long EndAndWait();		// синхронная команда потоку "Конец"
long Start();					// запуск потока
virtual long Stop();			// асинхронная команда потоку "Стоп"
virtual long StopAndWait();		// синхронная команда потоку "Стоп"
--------------------------------------------------------------------------------
	Пример использование класса CThreadSRV_CollectionOfKits см. в программе .

	Реализация ключевых моментов использрвания класса.

1. Объявляем структуру данных STRUCT_CALC_AND_VIEW как наследник структуры
контейнера STRUCT_KIT_OF_CHANNEL. В неё включаем отображаемые параметры и
промежуточные данные для их расчёта. У этой структуры должны быть:
   - конструктор по умолчанию;
   - деструктор;
   - конструктор копирования;
   - переопределённый оператор присваивания.

2. Объявляем функцию расчета значений отображаемых параметров:
	long Calculation(PVOID pParam).
Через pParam в функцию передаётся указатель на структуру STRUCT_CALC_AND_VIEW,
т.е. на контейнер в котором выполнено синхронное чтение данных с сервера.
Прочитанные данные содержатся в соответствующих массивах родительской структуры
STRUCT_KIT_OF_CHANNEL. В функции выполняются необходимые расчеты и если готовы
отображаемые параметры, то функция возвражает определённый код, по которому из
первичной глобальной функии в интерфейс посылается сообщение WM_NEW_DATA_SRV.

3. Обработчик события WM_NEW_DATA_SRV предназначен для отображения в программе
результатов синхронной обработки данных комплекта.
	В WPARAM этой функции индекс комплекта (indxKit), в котором выполнено
успешное синхронное чтение. LPARAM  - возвращаемое значение Calculation.
   В этом обработчике:
   - проверяем указатель m_pThread на nullptr;
   - по indxKit определяем структуру STRUCT_CALC_AND_VIEW;
   - из этой структуры куда-то копируем значения отображаемых параметров;
   - разрешаем работу потока с данным комплектом, вызвав функцию
     CThreadSRV_CollectionOfKits::SetSynchro(indxKit);
   - отображаем в интерфейсе программы скопированные параметров.

   Другие действия.

1. Объявляем структуру Param2 для параметров функции Calculation и создаём
экземпляр этой структуры. В эту структуру следует поместить параметры, общие
для всех контейнеров.

2. Объявляем экземпляр класса, напримеп так
   CThreadSRV_CollectionOfKits<STRUCT_CALC_AND_VIEW> * m_pThread = nullptr;

6. Создаём экземпляр класса (т.е. приостановленный поток), например так:
   m_pThread = new CThreadSRV_CollectionOfKits<STRUCT_CALC_AND_VIEW>();
   m_pThread->Create(L"", m_hWnd, m_psrv, Calc, &Param);
При этом будет создана коллекция из одного контейнера, в котором будет
один канал с номером "-1".

7. Для изменения кол-ва контейнеров используем функции класса
SetNumberKits, AddKitToCollection и DeleteLastKit.

8. Для изменения состава каналов в контейнерах используем функции класса:
ChangeAllChannelsInKitCur и ChangeAllChannelsInKit.

9. Для замены одного канала в контейнере используем функции класса:
ChangeOneChannelInKitCur и ChangeOneChannelInKit.

10. В программе также необходим обработчик сообщения WM_END_JOB_SRV, в котором
завершаем работу программы. В CDialog_ZET обработка этого сообщения уже есть.
*/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <CodeQuality.h>				// код качества данных
#include "KitOfChannels.h"				// параметры комплекта каналов
#include <ZetThread\CustomThreadSRV.h>	// базовый класс
//------------------------------------------------------------------------------
// Параметры, передаваемые во вторичную глобальную функцию потока класса.
//
template <class T>	// T - это структура, наследуемая от STRUCT_KIT_OF_CHANNEL
struct ZetThreadParam2
{
	int m_indxKitRead;			// индекс комплекта, в котором выполнено синхронное чтение
	double * pTimeReadSumMax;	// максимальное время чтения данных одного контейнера
	T * m_pKitRead;				// параметры этого комплекта
	CDSRV * m_psrv;				// указатель на сервер
	CZetTime * m_pTimeStartSRV;		// Время начала работы сервера
	PVOID m_pParamFromDlg;			// параметры из Dlg

	ZetThreadParam2()
		: m_indxKitRead(0)
		, pTimeReadSumMax(nullptr)
		, m_pKitRead(nullptr)
		, m_psrv(nullptr)
		, m_pTimeStartSRV(nullptr)
		, m_pParamFromDlg(nullptr)
	{}

	~ZetThreadParam2() {}

	// Получение абсолютного времени чтения данных комплекта
	CZetTime GetAbsoluteTimeSRV()
	{
		return *m_pTimeStartSRV + m_pKitRead->m_zts_timeDataCur;
	}
};
//------------------------------------------------------------------------------
// Параметры класса CThreadSRV_CollectionOfKits. Они же передаются в
// первичную глобальную функцию потока класса.
//
template <class T>	// T - это структура, наследуемая от STRUCT_KIT_OF_CHANNEL
struct ZetThreadParam1
{// Параметры экземпляра класса,
// передаваемые в первичную глобальную функцию
	// поддрержка устройств с маленькой частотой дискретизации
	bool m_bSupport7000;
	// все каналы всех комплектов должны быть рабочими
	bool m_bChanAllOk;
	// разрешение на чтение данных
	bool m_bReadData;
	// параметры вторичной глобальной функции
	ZetThreadParam2<T> * m_pParam2;
	// Доступ к коллекции
	CRITICAL_SECTION m_crSectinCollec;
	// вектор комплектов
	std::vector<T> m_vectKits;
	// функция усреднения данных
	FunctionThread m_FunctionAver;

	ZetThreadParam1()
		: m_bSupport7000(true)
		, m_bChanAllOk(false)
		, m_bReadData(true)
		, m_pParam2(nullptr)
		, m_FunctionAver(nullptr)
	{
		InitializeCriticalSection(&m_crSectinCollec);
	}

	~ZetThreadParam1()
	{
		DeleteCriticalSection(&m_crSectinCollec);
	}
};
//------------------------------------------------------------------------------
template <class T>	// T - это структура, наследуемая от STRUCT_KIT_OF_CHANNEL
class CThreadSRV_CollectionOfKits : public CCustomThreadSRV
{
private:
	// минимальная частота дискретизации нормального канала
	DWORD m_indxKitCur;				// индекс текущего комплекта
	DWORD m_numberKits;				// кол-во контейнеров в коллекции
	DWORD m_numberKitsMax;			// максимальное кол-во контейнеров в коллекции
	T * m_pKitCur;					// параметры текущего комплекта
	double m_freqADCMin;			// значение минимальной частоты (50 Гц)
	double m_timeReadSumMax;
	CZetTime m_zt_StartSRV;			// абсолютное время старта сервера
	ZetThreadParam1<T> m_parSRV1;	// параметры, передаваемые в первичную функцию
	ZetThreadParam2<T> m_parSRV2;	// параметры, передаваемые во вторичную функцию
	CHANNEL_FOR_KIT m_chanTmp;		// TMP-данные для CDSRV::GetChannelParEx

	// Первоначальное задание параметров канала по его номеру
	bool SetFirstChanByNumber(CHANNEL_FOR_KIT * pchFK, long channel);
	// Ожидание завершения выполнения вторичной глобальной функции
	void WaitEndingFuncSecond();
	// Установка у всех контейнеров синхронизирующего события
	void SetSynchroForAllKits();

protected:

public:
	// 
	CThreadSRV_CollectionOfKits();
	virtual ~CThreadSRV_CollectionOfKits();

	// Создание экземпляра класса.
	// nameThread - имя экземпляра (можно L"");
	// hwndParent - дескриптер окна для посылки WM_NEW_DATA_SRV и WM_END_JOB_SRV;
	// psrv - указатель на сервер;
	// function - имя вторичной глобальной функции для совместной обработки данных;
	// paramFunction - параметр вторичной глобальной функции m_pParamFromDlg;
	// Перед вызовом этой функции необходимо выполнить psrv->Connect().
	virtual bool Create(
		CString nameThread,
		HWND hwndParent,
		CDSRV * psrv,
		FunctionThread function,
		PVOID paramFunction,
		FunctionThread functionAver = nullptr);

	// Свойства класса --------------------------------------------------------

	// Получение максимального кол-ва контейнеров
	DWORD GetNumberKitsMax();
	// Задание максимального кол-ва контейнеров
	void SetNumberKitsMax(const DWORD val);
	//
	// Получение величины минимально-возможной частоты дискретизации канала при
	// запрете работы с медленными каналами
	double GetMinFreqADC();
	// Задание минимально-возможной частоты дискретизации канала при
	// запрете работы с медленными каналами. По умолчанию = 50 Гц.
	void SetMinFreqADC(const double freq);

	// Получение флага разрешения работы с медленными каналами
	bool GetSupport7000();
	// Задание флага разрешения работы с медленными каналами
	void SetSupport7000(const bool val);
	
	// Получение индекса текущего контейнера каналов
	DWORD GetIndexKitCur();
	// Задание текущего контейнера каналов.
	// Возвращаемое значение true если index допустимый.
	bool SetIndexKitCur(const DWORD indxKit);

	// Получение флага разрешения чтения данных по всем контейнерам
	bool GetEnableReadData();
	// Задание флага разрешения чтения данных по всем контейнерам
	void SetEnableReadData(const bool val);

	// Получение флага разрешения чтения данных в текущем контейнере
	bool GetEnableReadDataInKitCur();
	// Задание флага разрешения чтения данных в текущем контейнере
	void SetEnableReadDataInKitCur(const bool val);

	// Получение кол-ва контейнеров в коллекции
	DWORD GetNumberKits();
	// Задание кол-ва контейнеров в коллекции.
	// Если новое кол-во больше текущего кол-ва, то добавляются контейнеры из
	// одного канала с номером "-1" (старые контейнеры не изменяются).
	// Иначе с конца будет удалено соответствующее число контейнеров (оставшиеся
	// не изменятся). Новое кол-во должно быть больше 0.
	// Функция потоко-безопасна.
	bool SetNumberKits(const DWORD newNum);

	// Функции изменения состава коллекции ----------------------------------------
	//
	// Добавление в коллекцию контейнера каналов, который будет текущим контейнером.
	// Добавляемый контейнер должен содержать как минимум один канал.
	// numChan - кол-во каналов в добавляемом контейнере;
	// chan1 - первый канал в добавляемом контейнере.
	// Каналы следует задавать значениями типа long.
	// Номер канала "-1" допустим.
	// Функция потоко-безопасна.
	// Функция возвращает индекс текущего канала.
	DWORD AddKitToCollection(const DWORD numChan, const long chan1, ...);

	// Добавление в коллекцию контейнера каналов, который будет текущим контейнером.
	// Номера каналов добавляемого контейнера будут взяты из вектора vectChannels.
	// vectChannels не может быть пустым.
	// Номер канала "-1" допустим.
	// Функция потоко-безопасна.
	// Функция возвращает индекс текущего канала.
	DWORD AddKitToCollection(const std::vector<long> & vectChannels);

	// Удаление из коллекции последнего контейнера
	// Функция потоко-безопасна.
	void DeleteLastKit();

	// Функции изменения состава каналов одного контейнера ---------------------------
	//
	// Замена всех каналов в текущем контейнере.
	// numChan - новое кол-во каналов в текущем контейнере;
	// chan1 - новый первый канал в текущем контейнере.
	// Каналы следует задавать значениями типа long.
	// Номер канала "-1" допустим.
	// Функция потоко-безопасна.
	void ChangeAllChannelsInKitCur(const DWORD numChan, const long chan1, ...);

	// Замена всех каналов в текущем контейнере.
	// Новые номера каналов текущего контейнера будут взяты из вектора vectChannels.
	// Функция потоко-безопасна.
	void ChangeAllChannelsInKitCur(const std::vector<long> & vectChannels);

	// Замена всех каналов в заданном контейнере.
	// indxKit - индекс заданного контейнера;
	// numChan - новое кол-во каналов в заданном контейнере;
	// chan1 - новый первый канал в заданном контейнере.
	// Каналы следует задавать значениями типа long.
	// Номер канала "-1" допустим.
	// Возвращаемое значение true если indxKit допустимый.
	// Функция потоко-безопасна.
	bool ChangeAllChannelsInKit(const DWORD indxKit, const DWORD numChan, const long chan1, ...);
	// Замена всех каналов в заданном контейнере.
	// Новые номера каналов заданного контейнера будут взяты из вектора vectChannels.
	// vectChannels не может быть пустым.
	// Возвращаемое значение true если indxKit допустимый.
	// Функция потоко-безопасна.
	bool ChangeAllChannelsInKit(const DWORD indxKit, const std::vector<long> & vectChannels);

	// Замена одного канала в текущекм контейнере.
	// indxChanInKir - индекс канала в текущем контейнере;
	// chanNew - номер нового канала.
	// Номер канала "-1" допустим.
	// Возвращается true если indxChanInKir допустимый.
	// Функция потоко-безопасна.
	bool ChangeOneChannelInKitCur(const DWORD indxChanInKir, const long chanNew);

	// Замена одного канала в заданном контейнере.
	// indxKit - индекс контейнера в коллекции;
	// indxChanInKir - индекс канала в текущем контейнере;
	// chanNew - номер нового канала.
	// Возвращается true если оба индекса допустимые.
	// Функция потоко-безопасна.
	bool ChangeOneChannelInKit(const DWORD indxKit, const DWORD indxChanInKir, const long chanNew);

	// Функции получения доступа к контейнерам ---------------------------------------
	//
	// Получение указателя на структуру параметров текущего контейнера
	// Если в коллекции нет контейнеров возвращается nullptr
	T * GetPointerOfKitCur();
	// Получение указателя на структуру параметров заданного контейнера
	// nullptr возвращается, если index не допустимый.
	T * GetPointerOfKit(const DWORD indxKit);

	// Текущий контейнер становится следующим (по кругу в коллекции)
	void IncrementKitCur();

	// Синхронизация обработки и отображения -----------------------------------------
	//
	// Синхронизация обработки и отображения синхронных данных indxKit-ого контейнера.
	// Возвращаемое значение true если indxKit допустимый.
	bool SetSynchro(const DWORD indxKit);

	// Функции проверки параметров канала на предмет их изменения -------------------
	//
	// Обработка события сервера Modify.
	// По GUID каждый канал определяет ВСЕ свои параметры. Если какой-то параметр
	// изменился, то взводится соответствующий флаг. Если изменилась частота
	// дискретизации, то буферу канала делается соответствующий ReSize.
	// Если сервер перестартовал, то изменяется SRV-время чтения контейнеров.
	// Если что-то изменилось в параметрах каналов, то возвращаю true.
	bool Modify();

	// Проверка параметров каналов текущего контейнера.
	// Если что-то изменилось, то возвращаю true.
	bool TestChannelsInKitCur();

	// Функции задания первого времени чтения (и первого времени усреднения) --------
	//
	// Задание времени старта для текущего контейнера.
	// time - время старта.
	void SetTimeStartForKitCur(const double time);
	// Задание времени старта для текущего контейнера.
	// Время старта будет определено в классе времени данного контейнера.
	void SetTimeStartForKitCur();

	// Задание времени старта для заданного контейнера.
	// indxKit - индекс контейнера в коллекции;
	// time - время старта.
	// Возвращаемое значение true если indxKit допустимый.
	bool SetTimeStartForKit(const DWORD indxKit, const double time);
	// Задание времени старта для заданного контейнера.
	// indxKit - индекс контейнера в коллекции;
	// Время старта будет определено в классе времени данного контейнера.
	// Возвращаемое значение true если indxKit допустимый.
	bool SetTimeStartForKit(const DWORD indxKit);

	// Задание оптимального времени старт всем каналам
	// Возвращает false, если есть контейнеры без рабочих каналов
	bool SetTimeStart();

	// Функции задания временных интервалов ----------------------------------------
	//
	// Получение временных интервалов чтения и буфера текущего контейнера.
	void GetTimeParamForKitCur(
		CZetTimeSpan & timeInternalRead, CZetTimeSpan & timeInternalBuffer);
	// Задание временных интервалов чтения и буфера текущего контейнера.
	// При изменении интервала буфера соответственно изменяются
	// размеры буферов каналов текущего контейнера
	void SetTimeParamForKitCur(
		const CZetTimeSpan & timeInternalRead, const CZetTimeSpan & timeInternalBuffer = CZetTimeSpan());
	
	// Получение временных интервалов чтения и буфера контейнера.
	// Возвращаемое значение true если indxKit допустимый.
	bool GetTimeParamForKit(const DWORD indxKit,
		CZetTimeSpan & timeInternalRead, CZetTimeSpan & timeInternalBuffer);

	// Задание временных интервалов чтения и буфера контейнера.
	// При изменении интервала буфера соответственно изменяются
	// размеры буферов каналов контейнера
	// Возвращаемое значение true если indxKit допустимый.
	bool SetTimeParamForKit(const DWORD indxKit,
		const CZetTimeSpan & timeInternalRead, const CZetTimeSpan & timeInternalBuffer = CZetTimeSpan());

	// Получение интервала времени усреднения текущего контейнера
	void GetTimeIntervalAverForKitCur(CZetTimeSpan & timeInternalAver);

	// Задание интервала времени усреднения текущего контейнера
	void SetTimeIntervalAverForKitCur(const CZetTimeSpan & timeInternalAver);

	// Получение интервала времени усреднения заданного контейнера
	// Возвращаемое значение true если indxKit допустимый.
	bool GetTimeIntervalAverForKit(const DWORD indxKit, CZetTimeSpan & timeInternalAver);

	// Задание интервала времени усреднения заданного контейнера
	// Возвращаемое значение true если indxKit допустимый.
	bool SetTimeIntervalAverForKit(const DWORD indxKit, const CZetTimeSpan & timeInternalAver);

	// Другие функции ------------------------------------------------------------
	// Преобразование относительного времени сервера в абсолютное
	CZetTime ConvertTimeSRVtoAbsolute(const CZetTimeSpan & ztsSRV);

	// Максимальное суммарное время время прочитанных данных для одного контейнера
	double GetMaxSummTimeReadForOneKit();
	void SetMaxSummTimeReadForOneKit(const double val);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//   Глобальная функция SRV_ThreadMulti - это первичная функция потока.
// Её аргумент - это указатель на экземпляр структуры общих параметров,
// которая включает в себя указатель на структуру частных параметров.
//   В функции потока вначале проверяется время и наличие работающего сервера.
// У каждого канала своё время! По истечению времени считываются данные и для
// каждого канала вызывается вторичная глобальная функция, где должна выполняться
// обработка считанных данных канала. Эта функция возвращает код качества
// результата обработанки данных по каналу. Если бит AllData этого кода равен 0
// (все данные обработаны), то ожидается завершение отображения данных в потоке
// диалогово окна, и после этого генерится сообщение WM_NEW_DATA_SRV.
//------------------------------------------------------------------------------
//		При выполнении кода рабочего потока возможны следующие сообщения:
//	WM_END_JOB_SRV - нет сервера данных, сообщение без параметров;
//	WM_NEW_DATA_SRV - данные по одному из каналов считаны и обработаны,
//		передаётся номер канала и код качества результаов обработки.
//------------------------------------------------------------------------------
template <class T>
DWORD WINAPI Function_CollectionOfKits(PVOID pParam)
{// первичная глобальная функция рабочего потока

	// Полученные параметры -----------------------------------------
	// адрес структуры параметров класса-прародителя CCustomZetThread
	ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	par0->statusEx = running;

	// адрес структуры параметров класса-родителя CCustomThreadSRV
	ZetThreadParamSRV0 *parSRV0 = (ZetThreadParamSRV0*)par0->pParamNext;
	auto psrv = parSRV0->pSRV;	// сервер

	// адрес структуры параметров данного класса CThreadSRVmulti
	ZetThreadParam1<T> *parSRV1 = (ZetThreadParam1<T>*)parSRV0->pParamNext;
	// Параметры вторичной глобальной функции
	ZetThreadParam2<T> *parSRV2 = (ZetThreadParam2<T>*)parSRV1->m_pParam2;

	// НАЧИНАЕМ -------------------------------------------------------
	while (par0->FlagJob)
	{// цикл рабочего потока -------------------------------------------
		par0->FlagViewStop = false;

		// чтобы не было проблем с сервером ----------------------------
		par0->statusEx = numFileUse;
		if (psrv->NumFileUsed(0) == -1)
		{	// сервер выгрузился
			if (par0->hWndParent != NULL)
				::PostMessageW(par0->hWndParent, WM_END_JOB_SRV, 0, 0);
			break;	// выходим из while (par0->FlagJob)
		}

		par0->statusEx = waitObject;
		bool bFlag = par0->FlagJob && !par0->FlagStop && parSRV1->m_bReadData;

		// проход по коллекции -----------------------------------------
		if (bFlag)
		{
			EnterCriticalSection(&parSRV1->m_crSectinCollec);

			int indexKit = 0;
			// Цикл по контейнерам,it - это контейнер -------------------------------
			for (auto & it : parSRV1->m_vectKits)
			{	// эти флаги в процессе чтения и обработки данных могли поменяться
				EnterCriticalSection(&it.m_crSectinKit);
				par0->statusEx = working;

				bFlag = par0->FlagJob && !par0->FlagStop &&
					parSRV1->m_bReadData && it.GetFlag(Flag_Kit_Read);

				if (parSRV1->m_bChanAllOk)
					bFlag &= it.m_numberChannels == it.m_numberChannelsWork;
				//	bFlag &= it.m_numberChannels == it.GetNumChannelsWork();

				if (bFlag)
				{
					par0->statusEx = dataTime;
					BYTE whileCount = 0;
					parSRV2->m_indxKitRead = indexKit;
					parSRV2->m_pKitRead = &it;

					if (parSRV1->m_FunctionAver == nullptr)
					{
						while (it.ReadinessData(psrv, parSRV1->m_bSupport7000))
						{
							if (!par0->FlagJob || par0->FlagStop ||
								!parSRV1->m_bReadData ||
								!it.GetFlag(Flag_Kit_Read))
								break;	// выход из while (it.ReadinessData(psrv))

							par0->statusEx = waitSynshroGUI;

							if (it.m_hEventSynchro != NULL)
							{
								bFlag = WAIT_OBJECT_0 ==
									WaitForSingleObject(it.m_hEventSynchro, par0->TimeOutSend);
							}
							if (bFlag)
							{// если обработанные данные данного комплекта ушли в интерфейс
								if (!par0->FlagJob || par0->FlagStop ||
									!parSRV1->m_bReadData ||
									!it.GetFlag(Flag_Kit_Read))
									break;	// выход из while (it.ReadinessData(psrv))

								par0->statusEx = dataRead;
								// m_zts_timeDataCur
								if (it.GetData(psrv, parSRV1->m_bSupport7000))
								{
									par0->statusEx = dataProcessing;
									// Вызов вторичной глобальной функции, обработка
									// синхронных данных
									CodeQuality retFunction = (*par0->Function)(parSRV2);
									par0->statusEx = working;
									if (retFunction.IsAllData() && (par0->hWndParent != NULL))
									{// сообщаю о завершении обработки данных
										::PostMessageW(par0->hWndParent, WM_NEW_DATA_SRV,
											(WPARAM)indexKit, (LPARAM)retFunction);
									}
								}// if (it.GetData(psrv))
								it.ResetAllFlagChangeFreq();
							}// if (wait1 == WAIT_OBJECT_0)
							else// GUI не сработало
								break;	// выход из while (it.ReadinessData(psrv))

							if (((++whileCount) * (double)it.m_cTimeRead.GetIntervalRead()) >
								(*parSRV2->pTimeReadSumMax))
								//		if (++whileCount > WhileCountMax)
								break;	// выход из while (it.ReadinessData(psrv))
						}// while (it.ReadinessData(psrv))
					}
					else// if (parSRV1->m_FunctionAver == nullptr)
					{
						double timeCur = it.GetTimeMin(psrv, parSRV1->m_bSupport7000);
						if (timeCur != DBL_MAX)
						{
							bool bRead(true), bAver(true);
							double timeCur0 = timeCur;

							while (bRead || bAver)
							{
								if (!par0->FlagJob || par0->FlagStop ||
									!parSRV1->m_bReadData ||
									!it.GetFlag(Flag_Kit_Read))
									break;	// выход из while (bRead || bAver)

								bRead = timeCur >= it.m_cTimeRead.GetTime();
								bAver = timeCur >= (double)it.m_cTimeRead.GetTimeAver();
								if (bRead)
								{
									par0->statusEx = dataRead;
									if (it.GetData(psrv, parSRV1->m_bSupport7000))
									{	// Если в GetData было чтение, то в m_zts_timeDataCur записывается
										// время прочитанных данных и инкрементируется время чтения
										par0->statusEx = dataProcessing;
										// Вызов вторичной глобальной функции, обработка
										// синхронных данных
										(*par0->Function)(parSRV2);
										par0->statusEx = working;
									}
									it.ResetAllFlagChangeFreq();
								}
								if (bAver)
								{
									par0->statusEx = waitSynshroGUI;

									if (it.m_hEventSynchro != NULL)
									{
										bFlag = WAIT_OBJECT_0 ==
											WaitForSingleObject(it.m_hEventSynchro, par0->TimeOutSend);
									}
									if (bFlag)
									{
										par0->statusEx = dataProcessingAver;
										CodeQuality retFunction = (*parSRV1->m_FunctionAver)(parSRV2);
										par0->statusEx = working;
										if (retFunction.IsAllData() && (par0->hWndParent != NULL))
										{// сообщаю о завершении усреднения данных
											::PostMessageW(par0->hWndParent, WM_NEW_DATA_SRV,
												(WPARAM)indexKit, (LPARAM)retFunction);
										}
										it.m_cTimeRead.IncrementTimeAver();
									}
									else// GUI не сработало
										break;	// выход из while (bRead || bAver)
								}
								if ((timeCur - timeCur0) > (*parSRV2->pTimeReadSumMax))
									//		if (++whileCount > WhileCountMax)
									break;	// выход из while (bRead || bAver)
							}// while (bRead || bAver)
						}//if (timeCur != DBL_MAX)
					}// else// if (parSRV1->m_FunctionAver == nullptr)
				}//if (bFlag)
				// проверка флагов движения времени по каналам контейнера
				for (auto & it_chan : it.m_vectChannels)
				{// Цикл по каналам контейнера
					if (it_chan.GetFlag(Flag_Chan_Stop_Time) && (par0->hWndParent != NULL))
						::PostMessageW(par0->hWndParent, WM_STOP_TIME_CHANNEL, (WPARAM)it_chan.channel, 0);
				}
				LeaveCriticalSection(&it.m_crSectinKit);
				++indexKit;
			}// for (auto & it : *pVectKits)
			LeaveCriticalSection(&parSRV1->m_crSectinCollec);
		}// if (parSRV1->m_bReadData)

		// завершение цикла while ----------------------------------------------
		if (!par0->FlagJob)		// эта проверка должна быть первой
			break;

		if (!par0->FlagStop)
		{	// надо отдать процессор
			par0->statusEx = sleeping;
			Sleep(par0->TimeSleep);
		}
		else
		{// надо остановить поток
			par0->FlagViewStop = true;
			par0->statusEx = suspend;
			SuspendThread(par0->hThread);	// последняя команда в цикле while
		}
	}// while ( par0->FlagJob )
	par0->statusEx = ending;
	return 0;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <class T>
CThreadSRV_CollectionOfKits<T>::CThreadSRV_CollectionOfKits()
	: CCustomThreadSRV()
	, m_indxKitCur(0)
	, m_numberKits(0)
	, m_numberKitsMax(200)
	, m_freqADCMin(50.f)
	, m_timeReadSumMax(20.)
	, m_pKitCur(nullptr)
{
	m_parSRV1.m_pParam2 = &m_parSRV2;
	parSRV0.pParamNext = &m_parSRV1;
}
//------------------------------------------------------------------------------
template <class T>
CThreadSRV_CollectionOfKits<T>::~CThreadSRV_CollectionOfKits()
{// Необходимо вначале синхронно остановить поток, а затем удалить данные,
	// иначе возможна ситуация, когда поток обратится к уже удалённым объектам
	SynchroFinishThread();
}
//------------------------------------------------------------------------------
template <class T>
bool CThreadSRV_CollectionOfKits<T>::Create(
	CString nameThread,			// имя потока
	HWND hwnd,					// HWND родительского окна
	CDSRV *psrv, 				// указатель на сервер	
	FunctionThread function,	// адрес вторичной функции
	PVOID paramFunction,		// параметры вторичной функции,
	FunctionThread functionAver)// адрес функции усреднения
{
	bool ret = (function != nullptr) && (psrv != nullptr);
	if (ret)
	{
		m_zt_StartSRV = (__time64_t)psrv->GetStartTime();
		m_parSRV2.pTimeReadSumMax = &m_timeReadSumMax;
		m_parSRV2.m_psrv = psrv;
		m_parSRV2.m_pTimeStartSRV = &m_zt_StartSRV;
		m_parSRV2.m_pParamFromDlg = paramFunction;

		bool ret = CCustomThreadSRV::Create(nameThread, hwnd, psrv, 1,
			function, paramFunction);
		if (ret)
		{
			ret = CreateZetThread(&Function_CollectionOfKits<T>);
			if (ret)
			{
				m_parSRV1.m_FunctionAver = functionAver;
				m_parSRV1.m_vectKits.reserve(1);
				m_parSRV1.m_vectKits.resize(1);
				m_pKitCur = m_parSRV1.m_vectKits.data();
				m_numberKits = 1;
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Ожидание завершения вторичной функция потока (ВФП). Функция предназначена для
// вызова из потока GUI. Если ВФП зависла, то завешивается всё!
void CThreadSRV_CollectionOfKits<T>::WaitEndingFuncSecond()
{
	while (true)
	{
		if ((zParam0.statusEx == dataProcessing) ||
			(zParam0.statusEx == dataProcessingAver))
			Sleep(zParam0.TimeSleep);
		else
			break;
	}
}
//------------------------------------------------------------------------------
template <class T>
// Установка у всех контейнеров синхронизирующего события
void CThreadSRV_CollectionOfKits<T>::SetSynchroForAllKits()
{
	if (!m_parSRV1.m_vectKits.empty())
	{
		for (auto & it : m_parSRV1.m_vectKits)
			it.SetSynchro();
	}
}
//------------------------------------------------------------------------------
template <class T>
// Получение максимального кол-ва контейнеров
DWORD CThreadSRV_CollectionOfKits<T>::GetNumberKitsMax()
{
	return m_numberKitsMax;
}

template <class T>
// Задание максимального кол-ва контейнеров
void CThreadSRV_CollectionOfKits<T>::SetNumberKitsMax(const DWORD val)
{
	if (val > 0)
		m_numberKitsMax = val;
}
//------------------------------------------------------------------------------
template <class T>
// Получение величины минимально-возможной частоты дискретитзации канала при
// запрете работы с медленными каналами
double CThreadSRV_CollectionOfKits<T>::GetMinFreqADC()
{
	return m_freqADCMin;
};
//------------------------------------------------------------------------------
template <class T>
// Задание минимально-возможной частоты дискретитзации канала при
// запрете работы с медленными каналами
void CThreadSRV_CollectionOfKits<T>::SetMinFreqADC(const double freq)
{
	if (freq > 0.f)
		m_freqADCMin = freq;
}
//------------------------------------------------------------------------------
template <class T>
// Получение индекса текущего контейнера каналов
DWORD CThreadSRV_CollectionOfKits<T>::GetIndexKitCur()
{
	return m_indxKitCur;
}
//------------------------------------------------------------------------------
template <class T>
// Задание текущего контейнера каналов
// Возвращаемое значение true если index допустимый.
bool CThreadSRV_CollectionOfKits<T>::SetIndexKitCur(const DWORD indxKit)
{
	bool ret = indxKit < m_numberKits;
	if (ret)
	{
		m_indxKitCur = indxKit;
		m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Текущий контейнер становится следующим (по кругу в коллекции)
void CThreadSRV_CollectionOfKits<T>::IncrementKitCur()
{
	if (m_numberKits > 1)
	{
		++m_indxKitCur;
		if (m_indxKitCur == m_numberKits)
			m_indxKitCur = 0;
		m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;
	}
}
//------------------------------------------------------------------------------
template <class T>
// Получение флага разрешения работы с медленными каналами
bool CThreadSRV_CollectionOfKits<T>::GetSupport7000()
{
	return m_parSRV1.m_bSupport7000;
}
//------------------------------------------------------------------------------
template <class T>
// Задание флага разрешения работы с медленными каналами
void CThreadSRV_CollectionOfKits<T>::SetSupport7000(const bool val)
{
	m_parSRV1.m_bSupport7000 = val;
}
//------------------------------------------------------------------------------
template <class T>
// Получение флага разрешения чтения данных
bool CThreadSRV_CollectionOfKits<T>::GetEnableReadData()
{
	return m_parSRV1.m_bReadData;
}
//------------------------------------------------------------------------------
template <class T>
// Задание флага разрешения чтения данных
void CThreadSRV_CollectionOfKits<T>::SetEnableReadData(const bool val)
{
	m_parSRV1.m_bReadData = val;
	if (val)
		SetSynchroForAllKits();
	else
		WaitEndingFuncSecond();
}
//------------------------------------------------------------------------------
template <class T>
T * CThreadSRV_CollectionOfKits<T>::GetPointerOfKitCur()
{
	return m_pKitCur;
}
//------------------------------------------------------------------------------
template <class T>
// Получение флага разрешения чтения данных в текущем контейнере
bool CThreadSRV_CollectionOfKits<T>::GetEnableReadDataInKitCur()
{
	return (m_pKitCur != nullptr) ? m_pKitCur->GetFlag(Flag_Kit_Read) : false;
}

template <class T>
// Задание флага разрешения чтения данных в текущем контейнере
void CThreadSRV_CollectionOfKits<T>::SetEnableReadDataInKitCur(const bool val)
{
	if (m_pKitCur != nullptr)
	{
		if (val)
			m_pKitCur->SetSynchro();
		else
			WaitEndingFuncSecond();
		m_pKitCur->SetFlag(Flag_Kit_Read, val);
	}
}
//------------------------------------------------------------------------------
template <class T>
T * CThreadSRV_CollectionOfKits<T>::GetPointerOfKit(const DWORD indxKit)
{
	T * ret(nullptr);
	if ((!m_parSRV1.m_vectKits.empty()) && (indxKit < m_parSRV1.m_vectKits.size()))
		ret = m_parSRV1.m_vectKits.data() + indxKit;
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CThreadSRV_CollectionOfKits<T>::SetFirstChanByNumber(
	CHANNEL_FOR_KIT * pchFK, long channel)
{
	bool ret = pchFK != nullptr;
	if (ret)
	{
		pchFK->InitFlags();
		pchFK->PlaceFlag(Flag_Chan_OnOff);

		if (channel >= 0)
		{
			ret = 0 == parSRV0.pSRV->GetChannelParEx(channel, pchFK);
			pchFK->channel = channel;
			pchFK->PlaceFlag(Flag_Chan_Exist);
			pchFK->DoChangeStatus();
		//	pchFK->m_numKadrMax = parSRV0.pSRV->DecadeBufferSize(pchFK->m_decade) / 2;
			pchFK->m_numKadrMax = (DWORD)(int)(parSRV0.pSRV->DecadeBufferSize(pchFK->m_decade) * 0.8f);
			pchFK->SetFlag(Flag_Chan_7000, pchFK->frequency < m_freqADCMin);
		}
		else
		{
			pchFK->channel = -1;
			pchFK->ResetFlag(Flag_Chan_Exist);

			pchFK->name[0] = 0;
			pchFK->measure[0] = 0;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Получение кол-ва контейнеров в коллекции
DWORD CThreadSRV_CollectionOfKits<T>::GetNumberKits()
{
	return m_numberKits;
}
//------------------------------------------------------------------------------
template <class T>
// Задание кол-ва контейнеров в коллекции.
// Если новое кол-во больше текущего кол-ва, то добавляются контейнеры,
// старые контейнеры не изменяются. Кол-во каналов в новых контейнерах
// определяется в конструкторе структуры Т.
// Иначе с конца будет удалено соответствующее число контейнеров (оставшиеся
// не изменятся). Новое кол-во должно быть больше 0.
// Функция потоко-безопасна.
// Ограничения сверху на кол-во в классе нет, т.к. это ограничение в каждой
// программе своё!!!
bool CThreadSRV_CollectionOfKits<T>::SetNumberKits(const DWORD newNum)
{
	bool ret = newNum <= m_numberKitsMax;
	if (ret)
	{
		DWORD number = (newNum > 0) ? newNum : 1;
		if (m_numberKits != number)
		{	// Надо запретить обращаться к коллекции из потока
			bool bEnableReadData = GetEnableReadData();
			SetEnableReadData(false);
			EnterCriticalSection(&m_parSRV1.m_crSectinCollec);

			// если выполняется вторичная функция, то надо ждать
			WaitEndingFuncSecond();

			m_parSRV1.m_vectKits.resize(number);

			m_numberKits = number;
			if (m_indxKitCur >= m_numberKits)
				m_indxKitCur = m_numberKits - 1;
			m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;

			if (newNum == 0)
				SetFirstChanByNumber(m_pKitCur->m_vectChannels.data(), -1);

			SetSynchroForAllKits();
			LeaveCriticalSection(&m_parSRV1.m_crSectinCollec);
			SetEnableReadData(bEnableReadData);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Добавление в коллекцию контейнера каналов, который будет текущим контейнером.
// Добавляемый контейнер должен содержать как минимум один канал.
// numChan - кол-во каналов в добавляемом контейнере;
// chan1 - первый канал в добавляемом контейнере.
// Каналы следует задавать значениями типа long.
// Номер канала "-1" допустим.
// Функция потоко-безопасна.
// Функция возвращает индекс текущего канала.
DWORD CThreadSRV_CollectionOfKits<T>::AddKitToCollection(const DWORD numChan, const long chan1, ...)
{	// Надо запретить обращаться к коллекции из потока
	if (m_numberKits < m_numberKitsMax)
	{
		bool bEnableReadData = GetEnableReadData();
		SetEnableReadData(false);
		EnterCriticalSection(&m_parSRV1.m_crSectinCollec);

		// если выполняется вторичная функция, то надо ждать
		WaitEndingFuncSecond();

		m_indxKitCur = m_numberKits;
		++m_numberKits;
		m_parSRV1.m_vectKits.reserve(m_numberKits);
		m_parSRV1.m_vectKits.resize(m_numberKits);
		m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;

		auto pvectChan = &m_pKitCur->m_vectChannels;
		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);
		auto pCur = pvectChan->data();
		SetFirstChanByNumber(pCur, chan1);

		if (numChan > 1)
		{
			va_list arguments;
			va_start(arguments, chan1);

			for (DWORD i = 1; i < numChan; ++i)
			{
				long ch = va_arg(arguments, long);
				SetFirstChanByNumber(++pCur, ch);
			}

			va_end(arguments);
		}
		m_pKitCur->m_numberChannels = numChan;
		m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);

		SetSynchroForAllKits();
		LeaveCriticalSection(&m_parSRV1.m_crSectinCollec);
		SetEnableReadData(bEnableReadData);
	}
	return m_indxKitCur;
}
//------------------------------------------------------------------------------
template <class T>
// Добавление в коллекцию контейнера каналов, который будет текущим контейнером.
// Номера каналов добавляемого контейнера будут взяты из вектора vectChannels.
// Номер канала "-1" допустим.
// Функция потоко-безопасна.
// Функция возвращает индекс текущего канала.
DWORD CThreadSRV_CollectionOfKits<T>::AddKitToCollection(const std::vector<long> & vectChannels)
{
	if ((m_numberKits < m_numberKitsMax) && (!vectChannels.empty()))
	{	// Надо запретить обращаться к коллекции из потока
		bool bEnableReadData = GetEnableReadData();
		SetEnableReadData(false);
		EnterCriticalSection(&m_parSRV1.m_crSectinCollec);

		// если выполняется вторичная функция, то надо ждать
		WaitEndingFuncSecond();

		m_indxKitCur = m_numberKits;
		++m_numberKits;
		m_parSRV1.m_vectKits.reserve(m_numberKits);
		m_parSRV1.m_vectKits.resize(m_numberKits);
		m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;

		auto pvectChan = &m_pKitCur->m_vectChannels;
		UINT numChan = vectChannels.size();
		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);
		auto pCur = pvectChan->data();

		for (long ch : vectChannels)
			SetFirstChanByNumber(pCur++, ch);

		m_pKitCur->m_numberChannels = numChan;
		m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);

		SetSynchroForAllKits();
		LeaveCriticalSection(&m_parSRV1.m_crSectinCollec);
		SetEnableReadData(bEnableReadData);
	}
	return m_indxKitCur;
}
//------------------------------------------------------------------------------
template <class T>
// Удаление из коллекции последнего контейнера. Последний не удаляется, но его
// каналы в -1. Функция потоко-безопасна.
void CThreadSRV_CollectionOfKits<T>::DeleteLastKit()
{	// Надо запретить обращаться к коллекции из потока
	bool bEnableReadData = GetEnableReadData();
	SetEnableReadData(false);
	EnterCriticalSection(&m_parSRV1.m_crSectinCollec);

	// если выполняется вторичная функция, то надо ждать
	WaitEndingFuncSecond();

	if (m_numberKits > 1)
	{// В коллекции более одного контейнера, удаление
		m_parSRV1.m_vectKits.pop_back();		
		--m_numberKits;

		if (m_indxKitCur >= m_numberKits)
			m_indxKitCur = m_numberKits - 1;
		m_pKitCur = m_parSRV1.m_vectKits.data() + m_indxKitCur;

	}
	else
	{// В коллекции один контейнер, все его каналы в -1
		for (auto * it : m_pKitCur->m_vectChannels)
			SetFirstChanByNumber(it, -1);
	}

	// Возобновляю работу коллекции
	SetSynchroForAllKits();
	LeaveCriticalSection(&m_parSRV1.m_crSectinCollec);
	SetEnableReadData(bEnableReadData);
}
//------------------------------------------------------------------------------
template <class T>
// Замена всех каналов в текущем контейнере.
// numChan - новое кол-во каналов в текущем контейнере;
// chan1 - новый первый канал в текущем контейнере.
// Каналы следует задавать значениями типа long.
// Номер канала "-1" допустим.
// Функция потоко-безопасна.
void CThreadSRV_CollectionOfKits<T>::ChangeAllChannelsInKitCur(const DWORD numChan, const long chan1, ...)
{
	if ((m_pKitCur != nullptr) && (numChan > 0))
	{
		bool bEnableReadData = m_pKitCur->GetFlag(Flag_Kit_Read);
		m_pKitCur->ResetFlag(Flag_Kit_Read);
		EnterCriticalSection(&m_pKitCur->m_crSectinKit);

		// если выполняется вторичная функция, то надо ждать
		WaitEndingFuncSecond();

		auto pvectChan = &m_pKitCur->m_vectChannels;

		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);

		auto pCur = pvectChan->data();
		SetFirstChanByNumber(pCur, chan1);

		if (numChan > 1)
		{
			va_list arguments;
			va_start(arguments, chan1);

			for (DWORD i = 1; i < numChan; ++i)
			{
				long ch = va_arg(arguments, long);
				SetFirstChanByNumber(++pCur, ch);
			}

			va_end(arguments);
		}
		m_pKitCur->m_numberChannels = numChan;
		m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
		m_pKitCur->SetSynchro();
		LeaveCriticalSection(&m_pKitCur->m_crSectinKit);
		m_pKitCur->SetFlag(Flag_Kit_Read, bEnableReadData);
	}
}
//------------------------------------------------------------------------------
template <class T>
// Замена всех каналов в текущем контейнере.
// Новые номера каналов текущего контейнера будут взяты из вектора vectChannels.
// Функция потоко-безопасна.
void CThreadSRV_CollectionOfKits<T>::ChangeAllChannelsInKitCur(const std::vector<long> & vectChannels)
{
	if ((m_pKitCur != nullptr) && !vectChannels.empty())
	{
		bool bEnableReadData = m_pKitCur->GetFlag(Flag_Kit_Read);
		m_pKitCur->ResetFlag(Flag_Kit_Read);
		EnterCriticalSection(&m_pKitCur->m_crSectinKit);

		// если выполняется вторичная функция, то надо ждать
		WaitEndingFuncSecond();

		auto pvectChan = &m_pKitCur->m_vectChannels;

		UINT numChan = vectChannels.size();
		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);

		auto pCur = pvectChan->data();

		for (long ch : vectChannels)
			SetFirstChanByNumber(pCur++, ch);

		m_pKitCur->m_numberChannels = numChan;
		m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);

		m_pKitCur->SetSynchro();
		LeaveCriticalSection(&m_pKitCur->m_crSectinKit);
		m_pKitCur->SetFlag(Flag_Kit_Read, bEnableReadData);
	}
}
//------------------------------------------------------------------------------
template <class T>
// Замена всех каналов в заданном контейнере.
// indxKit - индекс заданного контейнера;
// numChan - новое кол-во каналов в заданном контейнере;
// chan1 - новый первый канал в заданном контейнере.
// Каналы следует задавать значениями типа long.
// Номер канала "-1" допустим.
// Возвращаемое значение true если indxKit допустимый.
// Функция потоко-безопасна.
bool CThreadSRV_CollectionOfKits<T>::ChangeAllChannelsInKit(const DWORD indxKit, const DWORD numChan, const long chan1, ...)
{
	bool ret = (indxKit < m_parSRV1.m_vectKits.size()) && (numChan > 0);
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;

		bool bEnableReadData = pKit->GetFlag(Flag_Kit_Read);
		pKit->ResetFlag(Flag_Kit_Read);
		EnterCriticalSection(&pKit->m_crSectinKit);

		// если выполняется вторичная функция, то надо ждать
		WaitEndingFuncSecond();

		auto pvectChan = &pKit->m_vectChannels;

		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);

		auto pCur = pvectChan->data();
		SetFirstChanByNumber(pCur, chan1);

		if (numChan > 1)
		{
			va_list arguments;
			va_start(arguments, chan1);

			for (DWORD i = 1; i < numChan; ++i)
			{
				long ch = va_arg(arguments, long);
				SetFirstChanByNumber(++pCur, ch);
			}

			va_end(arguments);
		}

		pKit->m_numberChannels = numChan;
		pKit->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);

		pKit->SetSynchro();
		LeaveCriticalSection(&pKit->m_crSectinKit);
		pKit->SetFlag(Flag_Kit_Read, bEnableReadData);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Замена всех каналов в заданном контейнере.
// Новые номера каналов заданного контейнера будут взяты из вектора vectChannels.
// Возвращаемое значение true если indxKit допустимый.
// Функция потоко-безопасна.
bool CThreadSRV_CollectionOfKits<T>::ChangeAllChannelsInKit(const DWORD indxKit, const std::vector<long> & vectChannels)
{
	bool ret = (indxKit < m_parSRV1.m_vectKits.size()) && !vectChannels.empty();
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;

		bool bEnableReadData = pKit->GetFlag(Flag_Kit_Read);
		pKit->ResetFlag(Flag_Kit_Read);
		EnterCriticalSection(&pKit->m_crSectinKit);

		// если выполняется вторичная функция, то надо ждать
		WaitEndingFuncSecond();

		auto pvectChan = &pKit->m_vectChannels;

		UINT numChan = vectChannels.size();
		pvectChan->reserve(numChan);
		pvectChan->resize(numChan);

		auto pCur = pvectChan->data();

		for (long ch : vectChannels)
			SetFirstChanByNumber(pCur++, ch);

		pKit->m_numberChannels = numChan;
		pKit->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);

		m_pKit->SetSynchro();
		LeaveCriticalSection(&pKit->m_crSectinKit);
		pKit->SetFlag(Flag_Kit_Read, bEnableReadData);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Замена одного канала в текущекм контейнере.
// indxChanInKir - индекс канала в текущем контейнере;
// chanNew - номер нового канала.
// Номер канала "-1" допустим.
// Возвращается true если indxChanInKir допустимый.
// Функция потоко-безопасна.
bool CThreadSRV_CollectionOfKits<T>::ChangeOneChannelInKitCur(const DWORD indxChanInKir, const long chanNew)
{
	bool ret = m_pKitCur != nullptr;
	if (ret)
	{
		auto pvectChan = &m_pKitCur->m_vectChannels;
		ret = indxChanInKir < pvectChan->size();
		if (ret)
		{
			auto pCur = pvectChan->data() + indxChanInKir;
			if (pCur->channel != chanNew)
			{
				EnterCriticalSection(&pCur->m_crSectionChan);
				SetFirstChanByNumber(pCur, chanNew);
				m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
				LeaveCriticalSection(&pCur->m_crSectionChan);
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Замена одного канала в заданном контейнере.
// indxKit - индекс контейнера в коллекции;
// indxChanInKir - индекс канала в текущем контейнере;
// chanNew - номер нового канала.
// Возвращается true если оба индекса допустимые.
// Функция потоко-безопасна.
bool CThreadSRV_CollectionOfKits<T>::ChangeOneChannelInKit(const DWORD indxKit, const DWORD indxChanInKir, const long chanNew)
{
	bool ret = indxKit < m_parSRV1.m_vectKits.size();
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;
		auto pvectChan = &pKit->m_vectChannels;
		ret = indxChanInKir < pvectChan->size();
		if (ret)
		{
			auto pCur = pvectChan->data() + indxChanInKir;
			if (pCur->channel != chanNew)
			{
				EnterCriticalSection(&pCur->m_crSectionChan);
				SetFirstChanByNumber(pCur, chanNew);
				pKit->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
				LeaveCriticalSection(&pCur->m_crSectionChan);
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Синхронизация обработки и отображения синхронных данных indxKit-ого контейнера.
// Возвращаемое значение true если indxKit допустимый.
bool CThreadSRV_CollectionOfKits<T>::SetSynchro(const DWORD indxKit)
{
	bool ret = indxKit < m_parSRV1.m_vectKits.size();
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;
		pKit->SetSynchro();
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Задание времени старта для текущего контейнера.
// val - время старта.
void CThreadSRV_CollectionOfKits<T>::SetTimeStartForKitCur(const double time)
{
	if (m_pKitCur)
		m_pKitCur->SetTimeStart(time);
}
//------------------------------------------------------------------------------
template <class T>
// Задание времени старта для текущего контейнера.
// Время старта будет определено в классе времени данного контейнера.
void CThreadSRV_CollectionOfKits<T>::SetTimeStartForKitCur()
{
	if (m_pKitCur)
		m_pKitCur->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
}
//------------------------------------------------------------------------------
template <class T>
// Задание времени старта для заданного контейнера.
// indxKit - индекс контейнера в коллекции;
// val - время старта.
// Возвращаемое значение true если indxKit допустимый.
bool CThreadSRV_CollectionOfKits<T>::SetTimeStartForKit(const DWORD indxKit, const double time)
{
	bool ret = indxKit < m_parSRV1.m_vectKits.size();
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;
		pKit->SetTimeStart(time);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Задание времени старта для заданного контейнера.
// indxKit - индекс контейнера в коллекции;
// Время старта будет определено в классе времени данного контейнера.
// Возвращаемое значение true если indxKit допустимый.
bool CThreadSRV_CollectionOfKits<T>::SetTimeStartForKit(const DWORD indxKit)
{
	bool ret = indxKit < m_parSRV1.m_vectKits.size();
	if (ret)
	{
		auto pKit = m_parSRV1.m_vectKits.data() + indxKit;
		pKit->SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CThreadSRV_CollectionOfKits<T>::SetTimeStart()
{
	bool ret = !m_parSRV1.m_vectKits.empty();
	if (ret)
	{
		for (auto & it : m_parSRV1.m_vectKits)
		{
			bool b = it.SetTimeStart(parSRV0.pSRV, m_parSRV1.m_bSupport7000);
			ret &= b;	// только так!!!
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CThreadSRV_CollectionOfKits<T>::Modify()
{	// Проверка на ReStart сервера
	CZetTime ztSRV = (__time64_t)parSRV0.pSRV->GetStartTime();
	CZetTimeSpan dztStartSRV = ztSRV - m_zt_StartSRV;
	bool bReStart = (0 != dztStartSRV.GetSeconds()) &&
		(0 != dztStartSRV.GetNanoseconds());	// был ReStart сервера
	bool ret = !m_parSRV1.m_vectKits.empty();
	if (ret)
	{
		ret = false;
		for (auto & it : m_parSRV1.m_vectKits)
		{
			bool bEnableReadData = it.GetFlag(Flag_Kit_Read);
			it.ResetFlag(Flag_Kit_Read);

			EnterCriticalSection(&it.m_crSectinKit);
			WaitEndingFuncSecond();

			if (it.Modify(parSRV0.pSRV, m_chanTmp, m_freqADCMin))
				ret = true;
			if (bReStart)
			{	// был ReStart сервера, надо изменить времена чтения и усреднения,
				// т.е. сделать меньше чтобы не было пропусков данных
				// отнимаем разницу времён старта SRV
				it.m_cTimeRead.ReStartSRV(dztStartSRV);
			}

			it.SetSynchro();
			LeaveCriticalSection(&it.m_crSectinKit);

			it.SetFlag(Flag_Kit_Read, bEnableReadData);
			it.DefineFlagsOfKit();
		}
	}
	if (bReStart)
		m_zt_StartSRV = ztSRV;
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Проверка параметров каналов текущего контейнера.
// Если что-то изменилось, то возвращаю true.
bool CThreadSRV_CollectionOfKits<T>::TestChannelsInKitCur()
{
	bool ret = (m_pKitCur != nullptr) &&
		!m_pKitCur->m_vectChannels.empty();
	if (ret)
	{
		ret = false;
		for (auto & it : m_pKitCur->m_vectChannels)
		{	// обновление данных если канал задан
			if (it.GetFlag(Flag_Chan_OnOff))
			{	// канал мог не существовать или
				// номер канала мог поменяться
				EnterCriticalSection(&it.m_crSectionChan);
				it.channel = parSRV0.pSRV->detectChanNum(it.id);
				it.SetFlag(Flag_Chan_Exist, it.channel >= 0);

				if (it.GetFlag(Flag_Chan_Exist))
				{
					parSRV0.pSRV->GetChannelParEx(it.channel, &m_chanTmp);
					it.SetFlag(Flag_Chan_7000, m_chanTmp.frequency < m_freqADCMin);
					if (it.TestChangeStruct(m_chanTmp))
						ret = true;
					it.DoChangeStatus();	// статус мог поменяться
				}

				it = m_chanTmp;		// копирую новые значения
				LeaveCriticalSection(&it.m_crSectionChan);
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// Получение временных интервалов чтения и буфера текущего контейнера.
template <class T>
void CThreadSRV_CollectionOfKits<T>::GetTimeParamForKitCur(
	CZetTimeSpan & timeInternalRead, CZetTimeSpan & timeInternalBuffer)
{
	if (m_pKitCur)
	{
		timeInternalRead = m_pKitCur->m_cTimeRead.GetIntervalRead();
		timeInternalBuffer = m_pKitCur->m_cTimeRead.GetIntervalBuffer();
	}
	else
	{
		timeInternalRead = timeInternalBuffer = CZetTimeSpan();
	}
}
//------------------------------------------------------------------------------
template <class T>
void CThreadSRV_CollectionOfKits<T>::SetTimeParamForKitCur(
	const CZetTimeSpan & timeInternalRead, const CZetTimeSpan & timeInternalBuffer)
{
	if (m_pKitCur)
	{//  именно в таком порядке
		CZetTimeSpan timeBuffer = ((CZetTimeSpan)timeInternalBuffer != CZetTimeSpan()) ?
									timeInternalBuffer : timeInternalRead;
		m_pKitCur->SetAllFlagChangeFreq(m_pKitCur->m_cTimeRead.GetIntervalBuffer() != timeBuffer);
		m_pKitCur->SetTimeParam(timeInternalRead, timeBuffer);
	}
}
//------------------------------------------------------------------------------
template <class T>
// Получение временных интервалов чтения и буфера контейнера.
// Возвращаемое значение true если indxKit допустимый.
bool CThreadSRV_CollectionOfKits<T>::GetTimeParamForKit(const DWORD indxKit,
	CZetTimeSpan & timeInternalRead, CZetTimeSpan & timeInternalBuffer)
{
	auto pKit = GetPointerOfKit(indxKit);
	bool ret = pKit != nullptr;
	if (ret)
	{
		timeInternalRead = pKit->m_cTimeRead.GetIntervalRead();
		timeInternalBuffer = pKit->m_cTimeRead.GetIntervalBuffer();
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
bool CThreadSRV_CollectionOfKits<T>::SetTimeParamForKit(const DWORD indxKit,
	const CZetTimeSpan & timeInternalRead, const CZetTimeSpan & timeInternalBuffer)
{
	auto pKit = GetPointerOfKit(indxKit);
	bool ret = pKit != nullptr;
	if (ret)
	{//  именно в таком порядке
		CZetTimeSpan timeBuffer = ((CZetTimeSpan)timeInternalBuffer != CZetTimeSpan()) ?
									timeInternalBuffer : timeInternalRead;
		pKit->SetAllFlagChangeFreq(pKit->m_cTimeRead.GetIntervalBuffer() != timeBuffer);
		pKit->SetTimeParam(timeInternalRead, timeBuffer);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Получение интервала времени усреднения текущего контейнера
void CThreadSRV_CollectionOfKits<T>::GetTimeIntervalAverForKitCur(CZetTimeSpan & timeInternalAver)
{
	if (m_pKitCur)
	{
		timeInternalAver = m_pKitCur->m_cTimeRead.GetIntervalAver();
	}
}
//------------------------------------------------------------------------------
template <class T>
// Задание интервала времени усреднения текущего контейнера
void CThreadSRV_CollectionOfKits<T>::SetTimeIntervalAverForKitCur(const CZetTimeSpan & timeInternalAver)
{
	if (m_pKitCur)
	{
		m_pKitCur->m_cTimeRead.SetIntervalAver(timeInternalAver);
	}
}
//------------------------------------------------------------------------------
template <class T>
// Получение интервала времени усреднения текущего контейнера
// Возвращаемое значение true если indxKit допустимый.
bool CThreadSRV_CollectionOfKits<T>::GetTimeIntervalAverForKit(const DWORD indxKit, CZetTimeSpan & timeInternalAver)
{
	auto pKit = GetPointerOfKit(indxKit);
	bool ret = pKit != nullptr;
	if (ret)
	{
		timeInternalAver = m_pKit->m_cTimeRead.GetIntervalAver();
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Задание интервала времени усреднения текущего контейнера
// Возвращаемое значение true если indxKit допустимый.
bool CThreadSRV_CollectionOfKits<T>::SetTimeIntervalAverForKit(const DWORD indxKit, const CZetTimeSpan & timeInternalAver)
{
	auto pKit = GetPointerOfKit(indxKit);
	bool ret = pKit != nullptr;
	if (ret)
	{
		m_pKit->m_cTimeRead.SetIntervalAver(timeInternalAver);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// Преобразование относительного времени сервера в абсолютное
CZetTime CThreadSRV_CollectionOfKits<T>::ConvertTimeSRVtoAbsolute(const CZetTimeSpan & ztsSRV)
{
	return m_zt_StartSRV + ztsSRV;
}
//------------------------------------------------------------------------------
// Максимальное суммарное время время прочитанных данных для одного контейнера
template <class T>
double CThreadSRV_CollectionOfKits<T>::GetMaxSummTimeReadForOneKit()
{
	return m_timeReadSumMax;
}

template <class T>
void CThreadSRV_CollectionOfKits<T>::SetMaxSummTimeReadForOneKit(const double val)
{
	if (val < MinSumTimeReaddForOneKit)
		m_timeReadSumMax = MinSumTimeReaddForOneKit;
	else if (val > MaxSumTimeReaddForOneKit)
		m_timeReadSumMax = MaxSumTimeReaddForOneKit;
	else
		m_timeReadSumMax = val;
}
//------------------------------------------------------------------------------