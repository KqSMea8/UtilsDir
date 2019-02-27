//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
// Оконные сообщения
//------------------------------------------------------------------------------
#define WM_GET_HWND_OF_PANEL WM_USER + 100	//получен дескриптор окна ZET-панели
#define WM_SEND_CHILD_HWND	WM_USER + 101	//посылка в ZET-панель сообщения об открытии/закрытии доп. окна
#define WM_SEND_STRUCTURE	WM_USER + 102	//отправить в ZET-панель структуру
#define WM_SHOW_PROGRAM		WM_USER + 103	//сообщение ZET- отобразить ту или иную программу в меню

#define WM_SEND_ERROR		WM_USER + 113	//сообщение ZET-панели об ошибке
#define WM_SEND_WARNING		WM_USER + 114	//сообщение ZET-панели о предупреждении

#define WM_SEND_CHILD_END	WM_USER + 151	//ZET-панели - о завершении работы
#define WM_UNIT_APP			WM_USER + 151	// UNIT
#define WM_UNIT2_APP		WM_USER + 152	// UNIT
#define WM_FILE_CFG_CHANGE	WM_USER + 153	// UNIT

#define WM_SEND_STARTSTOPDEBUG    WM_USER + 402

#define WM_NEW_DATA_SRV		WM_USER + 501	//сервер считал данные
#define WM_ERROR_READ_SRV	WM_USER + 502	//ошибка при чтении данных сервера SRV
#define WM_ERROR_CALC_SRV	WM_USER + 503	//ошибка при обрабоке данных сервера SRV
#define WM_END_JOB_SRV		WM_USER + 504	//ошибка нет сервера данных
#define WM_ERROR_THREAD		WM_USER + 505	//ошибка рабочего потока
#define WM_END_CALC			WM_USER + 506	//завершение расчётов
#define WM_ERROR_THREAD_DTU WM_USER + 507
#define WM_WRITE_DTU		WM_USER + 508
#define WM_NEW_PARAM		WM_USER + 509	//новые параметры
#define WM_PROC_STATUS		WM_USER + 509	//Статус программы (запущена, нет, выгружена, зависла)
#define WM_NEW_MODE			WM_USER + 510	//новый режим

#define WM_FAULT			WM_USER + 511	//неисправность
#define WM_EVENT_START		WM_USER + 511	//начало события
#define WM_EVENT_END		WM_USER + 512	//завершение события
#define WM_END_WAIT			WM_USER + 513	//завершение ожидания
#define WM_PROGRAM_STOP		WM_USER + 513	//остановить работу программы
#define WM_SEND_HWND		WM_USER + 514	//запрос на передачц своего HWND
// остановка времени по каналу (в WPARAM номер канала)
#define WM_STOP_TIME_CHANNEL	WM_USER + 515
//------------------------------------------------------------------------------
