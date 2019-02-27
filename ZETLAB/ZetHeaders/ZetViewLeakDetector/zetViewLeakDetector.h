// При подключении данного файла в проекте переопределяются операторы new и delete. 
// Благодаря этому можно контролировать какой компонент сколько забрал памяти под себя

#ifndef ZET_VIEW_LEAK_DETECTOR
#define ZET_VIEW_LEAK_DETECTOR

extern volatile LONG gCurrComponentHeapBl;
extern volatile LONG gCurrComponentHeapSz;

// Переопределенная функция new
void *myNewFunc(size_t size_, const char* file_, int line_)
{
	void *_p = malloc(size_);	// Выделяем необходимую память

	if (_p == NULL)
		throw std::bad_alloc();
	else
	{
		// Лайф хак для Маргариты по поиску неинициализированных переменных
		memset(_p, 0x99, size_);

		::InterlockedIncrement(&gCurrComponentHeapBl);
		::InterlockedAdd(&gCurrComponentHeapSz, size_);
	}

	return _p;
}

// Переопределенная функция delete
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

// Переопределяем глобально функцию new для одного элемента
#pragma warning(suppress: 28251)
void* operator new(size_t size_)
{
	return myNewFunc(size_, "", 0);
}

void* operator new(size_t size_, const char* file_, int line_)
{
	return myNewFunc(size_, file_, line_);
}

// Переопределяем глобально функцию new для массивов
#pragma warning(suppress: 28251)
void* operator new[](size_t size_)
{
	return myNewFunc(size_, "", 0);
}

void* operator new[](size_t size_, const char* file_, int line_)
{
	return myNewFunc(size_, file_, line_);
}

// Переопределяем глобально функцию delete для одного элемента
void operator delete(void* p_)
{
	myDeleteFunc(p_);
}

// Переопределяем глобально функцию delete для массива
void operator delete[](void* p_)
{
	myDeleteFunc(p_);
}

//#define new new(__FILE__, __LINE__)   // Макрос оператора new для получения информации о имени фала, в котором было выделение и строке

#endif	// endif zet_view_leak_detector