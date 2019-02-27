Attribute VB_Name = "ADCINFOModule"
Option Explicit

Public Type ADC_INFO
    ' Аппаратная конфигурация DSP
    x_numberDSP               As Integer  'Порядковый номер DSP на плате
    x_typeADC                 As Integer  'Режим работы DSP: 0 - ADC, 1 - DAC
    x_EnableADC               As Integer  'Наличие АЦП
    x_EnableDAC               As Integer  'Наличие ЦАП
    x_EnableExtDevice         As Integer  '0 - ничего нет, 1 - PRUS, 2 - HCP
    
    ' Запуск DSP
    x_Start                   As Integer  'состояние ввода данных 1- ввод

    ' Конфигурация каналов
    x_Channel                 As Long     'битовая комбинация вводимых каналов
    x_HCPChannel              As Long     'битовая комбинация включенных каналов HCP
    x_OverloadChannel         As Long     'битовая комбинация перегруженных каналов
    x_WorkChADC               As Integer  'количество работающих каналов АЦП
    x_WorkChDAC               As Integer  'количество работающих каналов ЦАП

    ' Конфигурация коэф. усиления
    x_KodAmplify(31)          As Integer  'коды усиления на АЦП
    x_TypeConnection          As Integer  'тип интерфейса (1 - PCI, 2 - USB, 3 - Ethernet) (был выравниванием!!!)
    x_Amplitude(31)           As Double   'коэффициент усиления усилителя АЦП
    x_PRUS(31)                As Integer  'коды усиления усилителя заряда / вкл. (выкл.) HCP
    x_PreAmplitude(31)        As Double   'коэффициент усиления предусилителя
    x_Atten(3)                As Integer  'аттенюатора ЦАП
    x_Reduction(3)            As Double   'коэффициент передачи аттенюатора ЦАП

    ' Конфигурация частот
    x_EnaExtStart             As Integer  'разрешение внешнего старта 0-внутр. 1-внеш
    x_EnaExtFreq              As Integer  'разрешение внешней частоты 0-внутр. 1-внеш
    x_EnaCycle                As Integer  'разрешение циклического накопления 0-однократно, 1-циклически
    x_ExtCycle                As Integer  'MasterSynchr: 1 - Задатчик синхронизации, 0 - ведомое устройство в синхронизации
    x_ModaADC                 As Integer  'мода работы АЦП 0-1, 1-2, 2-3, 3-4, 4-5, 5-8, 6-10 прореживание
    
    x_EnaExtStartDAC          As Integer  'разрешение внешнего старта ЦАП 0-внутр. 1-внеш (был выравниванием!!!)
    x_EnaExtFreqDAC           As Integer  'разрешение внешней частоты ЦАП 0-внутр. 1-внеш (был выравниванием!!!)
x_Reserv3       As Integer

    x_Freq                    As Double   'текущая частота преобразования
    x_OporFreq                As Double   'Опорная частота
    x_ExtOporFreq             As Double   'внешняя опорная частота
    x_Rate                    As Long     'коэффициент деления опорной частоты Fqwartc/Rate

    ' Конфигурация буферов
    x_sizeInterrupt           As Long     'размер для перекачки данных при каждом прерывании АЦП
    x_AddressBuffer           As Long     'Адрес буфера в системной области
    x_AddressBufferDAC        As Long     'Адрес буфера в системной области
    x_AddressUserBuffer       As Long     'Адреса буфера в пользовательской области
    x_AddressUserBufferDAC    As Long 'Адрес буфера в пользовательской области
    x_SizeBuffer              As Long     'Размер буфера ввода не более 1 000 000 отсчетов
    x_SizeBufferDAC           As Long     'Размер буфера ввода не более 1 000 000 отсчетов
    x_Pointer                 As Long     'индекс текущего накопления в буфере АЦП
    x_PointerDAC              As Long     'индекс текущего накопления в буфере ЦАП
    x_PointerDSP              As Long     'индекс прошедшего накопления в DSP

    ' Аппаратные параметры АЦП (ЦАП)
    x_Bits                    As Integer  'количество бит в АЦП преобразователе
    x_BitsDAC                 As Integer  'количество бит в ЦАП
    x_Words                   As Integer  'количество 16-разрядных слов в одном отсчете
    x_WordsDAC                As Integer  'количество 16-разрядных слов в одном отсчете
    x_QuantityDigitalChannel  As Integer  'количество линий цифрового порта
    x_DigitalResolution       As Double   'вес младшего разряда АЦП
    x_DigitalResolutionDAC    As Double   'вес младшего разряда ЦАП
    x_MaxFreq                 As Double   'Максимальная частота преобразования
    x_QuantityChannelADC      As Integer  'Максимальное количество каналов в модуле АЦП
    x_QuantityChannelDAC      As Integer  'Максимальное количество каналов в модуле ЦАП

    ' Переменные отслеживающие временные параметры
x_Reserv4   As Integer
x_Reserv5   As Integer
    x_Flag_Low                As Long     'количество прерываний
    x_Flag_High               As Long
    x_CurentCounter_Low       As Long     'Значение счетчика в момент опроса
    x_CurentCounter_High      As Long
    x_IrqCounter_Low          As Long     'Значение счетчика в текущее прерывание
    x_IrqCounter_High         As Long
    x_PeriodIrq_Low           As Long     'Текущий период прерываний
    x_PeriodIrq_High          As Long
    x_FreqIrqCounter_Low      As Long     'Частота счетчика
    x_FreqIrqCounter_High     As Long
    
    ' Прочие переменные
    x_MyError                 As Long     'ошибка ввода или задания параметров
    x_modify                  As Integer  'номер изменения, каждое существенное изменение параметров устройства
                                        'делает инкремент этого параметра
    x_AdrDSP                  As Long     'Adress in DSP
    x_AdrMem                  As Long     'Adress of Memory for DSP
    x_count                   As Long     'Количество слов для передачи
    x_numberCHNL              As Long     'Номер канала (служебная переменная)
    x_TestCode(5)             As Integer  'Тестовые проверки
    x_verDSP(63)              As Byte     'Версия DSP
    x_verDriver(63)           As Byte     'Версия драйвера
    x_sizeInterruptDAC        As Long     'размер для перекачки данных при каждом прерывании ЦАП
    x_StartDAC                As Long     'состояние ввода данных 1- ввод
    х_MaxSizeInterrupt        As Long     'Максимально возможный размер для перекачки данных при каждом прерывании
    x_MaxSizeInterruptDAC     As Long     'Максимально возможный размер для перекачки данных при каждом прерывании
    x_ChannelDAC              As Long     'битовая комбинация выводимых каналов


    x_EnaCycleDAC             As Long     'разрешение циклического накопления 0-однократно, 1-циклически
    x_ExtCycleDAC             As Long     '0 - генерация сигнала из внутренней памяти DSP, без подкачки из внешней памяти
                                            '1 - генерация сигнала из внешней памяти с подкчкой по прерываниям
    x_FreqDAC                 As Double   'текущая частота преобразования (FreqDAC = OporFreqDAC/RateDAC)
    x_OporFreqDAC             As Double   'опорная частота
    x_MaxFreqDAC              As Double   'Максимальная частота преобразования
    x_RateDAC                 As Long     'коэффициент деления опорной частоты
    x_DigitalInput            As Long     'Данные цифрового порта
    x_DigitalOutput           As Long     'Данные цифрового порта
    
    x_StopDevice              As Long     'Останов устройства
    x_NumVersionDriver        As Long     'Бинарная версия драйвера
    x_ChannelDiff             As Long     'битовая комбинация дифференциальных каналов
    x_DigitalOutEnable        As Long     'битовая комбинация выходов цифрового порта

    x_SizePacketADC           As Integer  'Кол-во слов данных в пакете АЦП
    x_SizePacketDAC           As Integer  'Кол-во слов данных в пакете ЦАП
    x_MaxSizePacketADC        As Integer  'Макс. кол-во слов данных в пакете АЦП
    x_MaxSizePacketDAC        As Integer  'Макс. кол-во слов данных в пакете ЦАП
    x_QuantityPacketsADC      As Integer  'Кол-во пакетов передаваемых за одно прерывание АЦП
    x_QuantityPacketsDAC      As Integer  'Кол-во пакетов передаваемых за одно прерывание ЦАП
    x_MaxQuantityPacketsADC   As Integer  'Макс. кол-во пакетов передаваемых за одно прерывание АЦП
    x_MaxQuantityPacketsDAC   As Integer  'Макс. кол-во пакетов передаваемых за одно прерывание ЦАП

    x_StartPWM                As Integer  'Запуск ШИМ (0x01 - старт 1 канала, 0x02 - старт 2 канала, 0x04 - старт 3 канала;
                                          '            0x100 - стоп 1 канала, 0x200 - стоп 2 канала, 0x400 - стоп 3 канала)
    x_RatePWM                 As Integer  'Коэф. деления опорной частоты для ШИМ
    x_PeriodPWM               As Integer  'Период ШИМ
    x_OnDutyPWM0              As Integer  'Скважность ШИМ (канал 0)
    x_OnDutyPWM1              As Integer  'Скважность ШИМ (канал 1)
    x_OnDutyPWM2              As Integer  'Скважность ШИМ (канал 2)
    x_ShiftPWM1               As Integer  'Сдвиг ШИМ (канал 0)
    x_ShiftPWM2               As Integer  'Сдвиг ШИМ (канал 1)

    x_Reserved(9)             As Integer  'Зарезервировано
End Type


Public Type COMMAND_INFO_USB
    x_Command       As Integer  'команда
    x_Error         As Integer  'дополнительный код или ошибка
    x_Size          As Long     'размер в байтах
    x_Data(251)     As Integer  'данные
End Type
