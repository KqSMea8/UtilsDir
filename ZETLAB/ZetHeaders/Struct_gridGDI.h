//#define MaxNumber 80
#ifdef MaxNumber
#error Error: "MaxNumber" defined in some other file.
#endif

struct grid 
{
	long yAxisShiftSize;				// размер сдвига графика по оси Y
	long yAxisSize;				// размер сдвига графика по оси Y
	static const int MaxNumber = 20;
	long sizeofgrid;				//размер grid-ы в байтах
	int lx,					//верхний левый
	lenx,
	ly,						//             угол окна
	legy,
	nstr,					//количество строк в шапке
	dx,						//размер
	dy,						//      размер окна
	tpgr[MaxNumber],		//тип графика 0-положительный, 1-знако переменный
	tpln[MaxNumber],		//0-график гор. лини€ми, //1-сплошной, гор. лини€ми, //2-лини€ми, //3-заполненными лини€ми //4-полутоновой-8 градаций
	valid[MaxNumber],		
	needdraw[MaxNumber],
	//		 px,		//позици€
	py,		//       курсора по x
	dl,		// параметр (ширина) курсора (поз. кур. по y)
	posy,		//позици€ курсора по Y
	msx,		//позици€ мыши
	msy,		//позици€ мыши
	ncx,		//начало графика
	tos,		//тип осей 0- норма, 1- лог по x,4-нет осей по y, 2 - нет осей по , 8 - нет надписей по ”, 16 - нет надписей по ’
	lin,		//признак данных 0- линейный, 1- децибелы.
	ind,		//текущий индекс строки данных (2-х мерна€ графика)
	number,	//кол-во строк
	size,
	prznap,	//признак полного 2-х мерного буфера
	init,		//признак инициализации
	timer,		//признак включени€ таймера
	//		 ism,		// изменение данных дл€ отображени€ 0-данных нет, 1-есть
	update;//,	//обновление графика
//		 otobr;		//0- график не отображаетс€, 1- отображаетс€
	wchar_t  frm1[200],  //формат надпись
			frm2[MaxNumber][200],
			stt[200],  //промежуточный буфер второй строки
			caption[200],
			text_x_axis[200],//ѕодпись осей
			text_y_axis[200],//
			text_t_axis[200],//
			text_z_axis[200];//
	CPoint fmtpt[MaxNumber]; //точки вывода строк формата
	int fmtnew;		//признак изменени€ формата
	unsigned int handle;  //буфер в HIMEM
	//	  CDC* pdc;
	HDC hwnd;
	HCURSOR move,
			up,
			down,
			vrsdv,
			vrrsd,
			left,
			right,
			hrsdv,
			hrrsd,
			norma;
	COLORREF clgrf[MaxNumber];
	CFont fontsetka,
			fontleg;
	CRect oldRect;
	CRect oldInv;
};