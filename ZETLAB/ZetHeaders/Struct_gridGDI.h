//#define MaxNumber 80
#ifdef MaxNumber
#error Error: "MaxNumber" defined in some other file.
#endif

struct grid 
{
	long yAxisShiftSize;				// ������ ������ ������� �� ��� Y
	long yAxisSize;				// ������ ������ ������� �� ��� Y
	static const int MaxNumber = 20;
	long sizeofgrid;				//������ grid-� � ������
	int lx,					//������� �����
	lenx,
	ly,						//             ���� ����
	legy,
	nstr,					//���������� ����� � �����
	dx,						//������
	dy,						//      ������ ����
	tpgr[MaxNumber],		//��� ������� 0-�������������, 1-����� ����������
	tpln[MaxNumber],		//0-������ ���. �������, //1-��������, ���. �������, //2-�������, //3-������������ ������� //4-�����������-8 ��������
	valid[MaxNumber],		
	needdraw[MaxNumber],
	//		 px,		//�������
	py,		//       ������� �� x
	dl,		// �������� (������) ������� (���. ���. �� y)
	posy,		//������� ������� �� Y
	msx,		//������� ����
	msy,		//������� ����
	ncx,		//������ �������
	tos,		//��� ���� 0- �����, 1- ��� �� x,4-��� ���� �� y, 2 - ��� ���� �� , 8 - ��� �������� �� �, 16 - ��� �������� �� �
	lin,		//������� ������ 0- ��������, 1- ��������.
	ind,		//������� ������ ������ ������ (2-� ������ �������)
	number,	//���-�� �����
	size,
	prznap,	//������� ������� 2-� ������� ������
	init,		//������� �������������
	timer,		//������� ��������� �������
	//		 ism,		// ��������� ������ ��� ����������� 0-������ ���, 1-����
	update;//,	//���������� �������
//		 otobr;		//0- ������ �� ������������, 1- ������������
	wchar_t  frm1[200],  //������ �������
			frm2[MaxNumber][200],
			stt[200],  //������������� ����� ������ ������
			caption[200],
			text_x_axis[200],//������� ����
			text_y_axis[200],//
			text_t_axis[200],//
			text_z_axis[200];//
	CPoint fmtpt[MaxNumber]; //����� ������ ����� �������
	int fmtnew;		//������� ��������� �������
	unsigned int handle;  //����� � HIMEM
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