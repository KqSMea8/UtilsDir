// ��� ����������� ������� ����� � ������� ���������������� ��������� new � delete. 
// ��������� ����� ����� �������������� ����� ��������� ������� ������ ������ ��� ����

#ifndef ZET_VIEW_LEAK_DETECTOR
#define ZET_VIEW_LEAK_DETECTOR

extern volatile LONG gCurrComponentHeapBl;
extern volatile LONG gCurrComponentHeapSz;

// ���������������� ������� new
void *myNewFunc(size_t size_, const char* file_, int line_)
{
	void *_p = malloc(size_);	// �������� ����������� ������

	if (_p == NULL)
		throw std::bad_alloc();
	else
	{
		// ���� ��� ��� ��������� �� ������ �������������������� ����������
		memset(_p, 0x99, size_);

		::InterlockedIncrement(&gCurrComponentHeapBl);
		::InterlockedAdd(&gCurrComponentHeapSz, size_);
	}

	return _p;
}

// ���������������� ������� delete
void myDeleteFunc(void* pointer_)
{
	if (pointer_ != nullptr)
	{
		LONG sz = _msize(pointer_); sz = -sz;
		::InterlockedDecrement(&gCurrComponentHeapBl);
		::InterlockedAdd(&gCurrComponentHeapSz, sz);

		free(pointer_);
	}
}

// �������������� ��������� ������� new ��� ������ ��������
#pragma warning(suppress: 28251)
void* operator new(size_t size_)
{
	return myNewFunc(size_, "", 0);
}

void* operator new(size_t size_, const char* file_, int line_)
{
	return myNewFunc(size_, file_, line_);
}

// �������������� ��������� ������� new ��� ��������
#pragma warning(suppress: 28251)
void* operator new[](size_t size_)
{
	return myNewFunc(size_, "", 0);
}

void* operator new[](size_t size_, const char* file_, int line_)
{
	return myNewFunc(size_, file_, line_);
}

// �������������� ��������� ������� delete ��� ������ ��������
void operator delete(void* p_)
{
	myDeleteFunc(p_);
}

// �������������� ��������� ������� delete ��� �������
void operator delete[](void* p_)
{
	myDeleteFunc(p_);
}

//#define new new(__FILE__, __LINE__)   // ������ ��������� new ��� ��������� ���������� � ����� ����, � ������� ���� ��������� � ������

#endif	// endif zet_view_leak_detector