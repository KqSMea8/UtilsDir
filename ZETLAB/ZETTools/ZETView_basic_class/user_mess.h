#pragma once
//маркеры качества OPC
// #define    OPC_QUALITY_BAD             0x00
// #define    OPC_QUALITY_UNCERTAIN       0x40
// #define    OPC_QUALITY_GOOD            0xC0

//максимальный размер двумерного массива
#define XMAX 256
#define YMAX 256

#define WINDOW_WIDTH 6500

//определение типов передаваемых данных для функций doRefreshData и RefreshData
#define TYPE_BOOL				28
#define TYPE_LONG				0
#define TYPE_LONG_LONG			29
#define TYPE_FLOAT				1
#define TYPE_PARAM				2
#define TYPE_MANAGE				3
#define TYPE_AUTOFORMAT			100
#define TYPE_POINTER			101
#define TYPE_POINTER_STRING		104
#define TYPE_STRING				4
#define TYPE_STRING_STD			30
#define TYPE_CHANNEL			5
#define TYPE_COLORSTRING		6
//флоат с маркером качества
#define TYPE_FLOAT_BAD			7
#define TYPE_FLOAT_UNCERTAIN	8
#define TYPE_FLOAT_GOOD			1
//интегер с маркером качества
#define TYPE_LONG_BAD			10
#define TYPE_LONG_UNCERTAIN		11
#define TYPE_LONG_GOOD			12
//Указатель на интерфейс
#define TYPE_INTERFACE			13
// double
#define TYPE_DOUBLE				14
#define TYPE_DOUBLE_GOOD		14
#define TYPE_DOUBLE_BAD			15
#define TYPE_DOUBLE_UNCERTAIN	16
// Широкий результат числа
#define TYPE_WIDE_RESULT_LONG	17
#define TYPE_WIDE_RESULT_FLOAT	18
#define TYPE_WIDE_RESULT_DOUBLE	19
// Массив в виде вектора
#define TYPE_ARRAY_VECTOR_LONG		20
#define TYPE_ARRAY_VECTOR_FLOAT		21
#define TYPE_ARRAY_VECTOR_DOUBLE	22
#define TYPE_ARRAY_VECTOR_STRING	27
#define TYPE_ARRAY_VECTOR_STRING_STD	31
// Широкий результат вектора
#define TYPE_WIDE_RESULT_VECTOR_LONG	23
#define TYPE_WIDE_RESULT_VECTOR_FLOAT	24
#define TYPE_WIDE_RESULT_VECTOR_DOUBLE	25
// Произвольная структура
#define TYPE_ARBITRARY_STRUCTURE		26

#define DETECTVERSION	MAKELONG(_wVerMinor, _wVerMajor)

#define SLEEPTIME	5

//константы, используемые компонентами для работы с массивами
#define MAXGRAPH 60
#define MAXPOINTS 500000
//-----------------------------------------------------------

#define WM_USER_OK_CHANGE_PROPERTIES		(WM_USER + 100)
#define IDM_RECALC_SIZE						(WM_USER + 106)
#define WM_USER_ALL_MESS					(WM_USER + 107)

#define WM_USER_CLICKCONNECTION				(WM_USER + 199)
#define WM_USER_ONE_OF_COMPONENTS_CHANGE_DIM (WM_USER + 201)

//Это сообщение посылает контейнер, говоря о том, что мышь контейнера
//находится над компонентом ActiveX
#define WM_MOUSE_ONCOMPONENT				(WM_USER + 202)
//сообщение о нахождении мыши над контактом
#define WM_MOUSE_ONCONTACT					(WM_USER + 203)
//пользователь щелкает на появивщуюся контактную площадку и дает сообщение
//"хозяину" выделенного пина, о том что надо создать проводник
#define WM_USER_WANT_PLACE_CONNECTOR		(WM_USER + 204)
//сообщение о подтверждении соединения по проводнику
#define WM_USER_SECOND_POINT_CONNECTED	    (WM_USER + 205)
//посылается соединительной линии, при удалении компонента
#define WM_USER_IS_BEING_DELETING			(WM_USER + 206)
//посылается от соединительной линии компонентам - запрос положения точек соединения
#define WM_CONNLINE_REQUEST_POINTS_POS		(WM_USER + 207)
//посылается компонентом в ответ на запрос 
#define WM_CONNLINE_ANSWER_POINTS_POS		(WM_USER + 208)
//тестовой сообщение
#define WM_USER_TEST						(WM_USER + 209)
//ActiveX дает соединительной линии знать, что он активен и надо подсветить линию
//#define WM_COMPONENT_CHANGELINECOLOR		(WM_USER + 210)
//необходимо обновить компонент
#define WM_USER_UPDATECOMP					(WM_USER + 211)
//сообщение посылается компоненту, говорит о том, какой вид ему принять, в зависимости от того, в каком окне он находится
#define WM_USER_CHANGE_FORM					(WM_USER + 212)
//сообщение от соединительной линии для удаления самой себя
#define WM_DELET_HIMSELF					(WM_USER + 213)

#define WM_USER_ANIMATE_FRAME				(WM_USER + 214)

//сообщение для всех ActiveX-в, требование запустить/остановить все сервера и юниты
#define WM_USER_START_ALL					(WM_USER + 215)

#define WM_USER_START_ALL_INSIDE			(WM_USER + 216)
#define WM_USER_DEBUG_MODE					(WM_USER + 217)

//сообщение от соединительной линии контейнеру, содержит идентификатор uniqueIdent
//#define WM_USER_OVER_CONNECTOR				(WM_USER + 217)

#define WM_USER_EXPLORER_OPEN				(WM_USER + 218)

//сообщения обена данными
#define WM_USER_Refresh_SOURCE				(WM_USER + 200)
#define WM_USER_SWITCH_ONOFF				(WM_USER + 219)

//сообщение от ActiveX - подсоединение\отсоединение от server
//#define WM_USER_CONNECTTOSERVER				(WM_USER + 220)

//сообщение для ActiveX об обновлении сервера
//#define WM_USER_SERVER_MODIFY				(WM_USER + 221)
//запрос количества каналов сервера
//#define WM_USER_REQUEST_CHANLIST			(WM_USER + 222)

#define WM_USER_RESIZE_ALL					(WM_USER + 224)

//сообщение об обновлении PropertySheet для ActiveXа
#define WM_USER_PROPERTY_MODIFY				(WM_USER + 225)

//сообщение от контейнера о редактировании свойств компонента
#define WM_USER_SHOW_PROPS					(WM_USER + 226)

#define WM_USER_SECOND_POINT_DISCONNECTED	(WM_USER + 227)

//пользователь меняет название компонента
#define WM_USER_CHANGE_ACTIVEX_NAME			(WM_USER + 228)

#define WM_USER_Refresh_PDISP_LIST			(WM_USER + 229)
//показать строку редактора где в процессе интерпритации произошла ошибка
#define WM_USER_SHOWERROR_LINE				(WM_USER + 230)

//базовый класс обновляет производный, сервер modify
#define WM_USER_SERVER_UPDATE_FROMBASIC		(WM_USER + 231)

//Пользователь изменил название компонента прямо их хаголовка
#define WM_USER_CHANGELABELNAME				(WM_USER + 232)
//Пользователь щелкает на рабочий стол SCADA и надо спрятать CRichEdit метки
#define WM_USER_HIDE_RICH					(WM_USER + 233)
#define WM_USER_SHOW_ERRORS_INSCRIPT		(WM_USER + 234)
#define WM_USER_WANT_CHANGECONNTOLABLES     (WM_USER + 235) 
#define WM_USER_UPD							(WM_USER + 236) 

//запрос чкастоты дискретизации
#define WM_USER_REQUEST_FREQ				(WM_USER + 237)

//получаем хэндл сервера
#define WM_USER_GETSERVER_HANDLE			(WM_USER + 238)

//получаем хэндл сервера
//#define WM_USER_SET_COMP_LINKHANDLE			(WM_USER + 239)

#define WM_USER_DELETE_LINK					(WM_USER + 239)
//#define WM_USER_QUEUE_ALLOW					(WM_USER + 240)
#define WM_USER_QUEUE_UPDATE				(WM_USER + 241)
//
#define WM_USER_ADD_CHANNELS				(WM_USER + 242)
#define WM_USER_REMOVE_CHANNELS				(WM_USER + 243)
#define WM_USER_UPDATE_NAME					(WM_USER + 244)
#define WM_USER_CLICKFOLSER					(WM_USER + 245)
#define WM_USER_RCLICKONBTN					(WM_USER + 246)
//-------------------------------------------
#define WM_USER_WANT_PLACE_CONNECTORDIRECT	(WM_USER + 247)
//сообщения от таймера класса компонента
#define WM_USER_CLASSTIMER_TICK				(WM_USER + 248)
#define WM_USER_Refresh_DOC_NAME			(WM_USER + 249)
#define WM_USER_ACCEPTCHANGES_PROPERTYGRID	(WM_USER + 250)
#define WM_USER_SHOWCOMPPROPERTIES			(WM_USER + 251)
#define WM_USER_ZETVIEWUPDATEBACKCOLOR		(WM_USER + 252)
#define WM_USER_RUN_FROM_BASIC				(WM_USER + 253)
#define WM_USER_COPY_FROM_BASIC				(WM_USER + 254)
#define WM_USER_PASTE_FROM_BASIC			(WM_USER + 255)
#define WM_USER_COMPONENT_CHANGE_PROPERTIES	(WM_USER + 256)
#define WM_USER_CLOSE_BY_AUTOMATION			(WM_USER + 257)
#define WM_USER_REPARE_OPC_CLIENT			(WM_USER + 258)
#define WM_USER_MINMAX						(WM_USER + 259)
#define WM_SYNCHRO_OPC						(WM_USER + 260)
#define WM_SEND_READ_PROPERTY				(WM_USER + 261)
#define WM_USER_ADD_DEVICES					(WM_USER + 262)
#define WM_USER_REMOVE_DEVICES				(WM_USER + 263)
#define WM_USER_UPDATECHANNELS_OPC_CLIENT	(WM_USER + 264)
#define WM_SEND_REINITCONNECTION			(WM_USER + 265) 
#define WM_SHOW_COMPONENT					(WM_USER + 266) 
#define WM_SORT_COMPONENTS					(WM_USER + 267) 
//пользователь меняет конфигурацию
#define WM_USER_CHANGE_CONFIGURATION		(WM_USER + 268)
#define WM_USER_SHOWUNIT_0_1				(WM_USER + 269)
#define WM_USER_ADD_COMPONENT				(WM_USER + 270)

#define WM_USER_CREATE_COMPONENT			(WM_USER + 271)
#define WM_USER_DELETE_COMPONENT			(WM_USER + 272)

#define WM_SYNCHRO_OPCUA					(WM_USER + 273)

//идёт обмен данными между компонентами
#define WM_USER_REFRESH_DATA_1				(WM_USER + 401)
//сообщение о регистрации пользователя
#define WM_USER_AUTHENTIC   				(WM_USER + 300)

//определение события - отключение или подключение канала
#define WPARAM_IDCHAN						1	//сообщение от канала
#define WPARAM_IDONOFF						2	//сообщение от выключателя
#define WPARAM_IDUPDATEDATA					3	
#define WPARAM_IDLEVEL					    4	//сообщение от селектора уровня
#define WPARAM_IDPOWER						5	//сообщение от силовоого устройства
//-------------------------------------------------------

//идентификаторы положения мыши на компоненте ActiveX
#define ActiveX_center						1	//перетаскиваем фигуру за центр
#define right_side							2
#define left_side							3
#define top_side							4	
#define bottom_side							5	
#define left_top							6
#define left_bottom							7
#define right_bottom						8
#define right_top							9


//типы контактов, возвращаемых от ActiveX-а и передаваемых в другой ActiveX
//для распознавания совместимости
#define OUT_SIGNAL							1   //Выходной сигнал
#define IN_SIGNAL							2	//Входной сигнал
#define OUT_BOOL							3	//Выходной Двоичный сигнал
#define IN_BOOL								4   //Входной двоичный сигнал
#define OUT_POLYSIGNAL						5	//Выходной сигнал 0...n
#define IN_POLYSIGNAL						6   //Входной сигнал 0...n
#define OUT_CONTROLS						7   //Выходной сигнал контроля состояний, например: lParam = 0 - медленно и т.д.
#define IN_CONTROLS							8   //Входной сигнал контроля состояний
#define OUT_POWER							9   //Выходной сигнал силового выхода 0.
#define IN_POWER						   10   //Входной сигнал для силового входа 1.
#define OUT_ALL							   11
#define IN_ALL							   12
#define OUT_STRING						   13
#define IN_STRING						   14
#define OUT_SCRIPT						   15
#define IN_SCRIPT						   16
#define OUT_PARRAY						   17
#define IN_PARRAY						   18
#define OUT_MANAGE						   19
#define IN_MANAGE						   20
#define OUT_PARRAY_STRING				   21
#define IN_PARRAY_STRING				   22
#define OUT_EXCEL_INTERFACE				   23
#define IN_EXCEL_INTERFACE				   24
#define OUT_DOUBLE						   25
#define IN_DOUBLE						   26
#define OUT_STRUCTURE					   27
#define IN_STRUCTURE					   28
#define FORBIDDEN							0

//при прикреплении проводника говорит ActiveXу о том - создать проводник или поместить второй конец.
#define WANT_CREATE							1   //создать проводник
#define WANT_PLACE							2   //прикрепить проводник

//сторона, к которой прикрепляемся
#define LEFT_SIDE							1   //прикрепляемся к левой стороне
#define RIGHT_SIDE							2   //прикрепляемся к правой стороне
#define TOP_SIDE							5	//прикрепляемся к верхней стороне
#define BOTTOM_SIDE							6	//прикрепляемся к нижней стороне

#define	WPARAM_IDCHAN_ON					1	
#define WPARAM_IDCHAN_OFF					2

#define PIN_LENGTH	10

//ширина первого окна в пикселях
#define EXPLORER_WIDTH 220
#define BTNWIDTH 120
#define BTNHEIGH 22

#define IDD_LIST0                       20
#define IDD_LIST1                       21
#define IDD_LIST2                       22
#define IDD_LIST3                       23
#define IDD_LIST4                       24
#define IDD_LIST5                       25
#define IDD_LIST6                       26
#define IDD_LIST7                       27
#define IDD_LIST8                       28
#define IDD_LIST9                       29
#define IDD_LISTVIEW                    30

//идентификаторы таймера
#define TIMER1_ADDFRAME_DISIGN			1
#define TIMER2_ADDFRAME_OPERATE			2
#define TIMER3_PICTUREUPDATE			3
#define TIMER5_HIDE_HINTWINDOW			5
#define TIMER6_SHOWHINTDIALOG			6
#define TIMER8_UPDATEICONS				8
#define TIMER9_NUMFILEUSED				9
#define TIMER10_HIDEFOGDLG				10
#define TIMER11_BLINK					11
#define TIMER12_SHOWFRAME				12
#define TIMER13_UPDATENAME				13
#define TIMER16_UPDATESRV				16
#define TIMER17_UPDATESRV				16
#define TIMER18_TIMERAUTOSAVE			18
#define TIMER20_UPDATECOMP_AFTERSCROLL	20
#define TIMER21_UPDATEDATA_MODBUS		21
#define TIMER21_UPDATEDATA_OPC			22
#define TIMER22_OPENEXEPROJECT			23
#define TIMER23_RefreshConnections		24
#define TIMER25_RefreshConnections		25
#define TIMER26_RefreshConnections		26
#define TIMER27_SHOWPAGES				27
#define TIMER28_TIMER_TICK				28
#define TIMER29_SHOWWINDOW				29
#define TIMER30_UPDATEDATAINDEBUG		30
#define TIMER31_UPDATEPARENTPROCESS		31
#define TIMER32_MEMORYCONTROL           32
#define TIMER33_SERVER_MODIFY			33
#define TIMER34_DEMO_CHECK				34
#define BeginID			34000

#define libZet							0
#define keyword							1

//типы сортировок окон
#define SORT_0									0	//как есть 
#define SORT_VERT								1	//по вертикали
#define SORT_HORZ								2	//по горизонтали
#define SORT_ALL_WORKSPACE						3
#define SORT_TITLE								4	
#define SORT_CASCASE							5
#define SORT_MAXIMIZE							6

//константа, прибавляемая к передаваемому параметру
//например в интерфейсе OUT\IN BOOL 
//#define USER_PARAMS						1000

//выравнивание иконки
#define AL_NONE									0
#define AL_EXIST								1

#define PIN_1									1
#define PIN_2									2
#define PIN_3									3
#define PIN_4									4
#define PIN_5									5
#define PIN_6									6
#define PIN_7									7
#define PIN_8									8
#define PIN_9									9
#define PIN_10									10
#define PIN_11									11
#define PIN_12									12
#define PIN_13									13
#define PIN_14									14
#define PIN_15									15
#define PIN_16									16
#define PIN_17									17
#define PIN_18									18
#define PIN_19									19
#define PIN_20									20
#define PIN_21									21
#define PIN_22									22
#define PIN_23									23
#define PIN_24									24
#define PIN_25									25
#define PIN_26									26
#define PIN_27									27
#define PIN_28									28
#define PIN_29									29
#define PIN_30									30