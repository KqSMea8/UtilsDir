//------------------------------------------------------------------------------
/*	Шаблонная структура предназначена для реализации умных массивов.
	Структура имеет:
- конструктор по умолчанию;
- деструктор;
- параметрический констуктор, позволяющий сразу задавать размер массива;
- конструктор копирования;
- оператор присваивания;
- оператор [] для обращения к элементу массива по проверяемому индексу;
- оператор () для обращения к элементу массива по не проверяемому индексу;
- метод ReSize для изменения размера массива;
- методы копирования в(из) массив(а);
- методы для реализации сдвигового буфера, в котором нет циклов;
- другие полезные методы...
//------------------------------------------------------------------------------
	Для статического экземпляра структуры созданный массив удалять не надо,
с этим прекрасно справляется деструктор.
//------------------------------------------------------------------------------
	Во избехания сюрпризизов в качестве типа шаблона Т лучше использовать
простые типы - int, float, double, ...*/
//------------------------------------------------------------------------------
//	Зеленоград, ООО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
template <class T>
struct SmartArray
{
	DWORD size;		// размер хранящихся данных в массиве
	DWORD capacity;	// размер текущего массива
	T *pData;		// указатель на массив
	DWORD sizeofT;	// добавлено для выравнивания на 8 байт
	// -------------------------------------
	SmartArray(const DWORD sizeNew = 0)
		: size(0)
		, capacity(0)
		, pData(nullptr)
		, sizeofT(sizeof(T))
	{
		if (sizeNew > 0)
		{
			ReSize(sizeNew);
			Zero();
		}
	}
	// -------------------------------------
	SmartArray(const SmartArray & val)
		: size(0)
		, capacity(0)
		, pData(nullptr)
		, sizeofT(sizeof(T))
	{
		*this = val;
	}
	// -------------------------------------
	~SmartArray()
	{
		if (capacity > 0)
		{
			delete[] pData;
		}
	}
	// -------------------------------------
	bool operator = (const SmartArray &val)
	{
		return Copy(val.pData, val.size);
	}
	// -------------------------------------
	template <class Tint>	// любой целочисленный тип
	// Получение элемента массива с проверкой индекса
	T & operator [] (const Tint index)
	{// Возврат ссылки позволяет реализовывать левостороннее
	// изменение содержимого в SmartArray
		static const T _nan(std::numeric_limits<T>::quiet_NaN());
		static T ret(_nan);
		if ((0 <= index) && ((DWORD)(int)index < size))
		{
			return pData[(int)index];
		}
		else
		{	// Этот код позволяет при разных невалидных индексах
			// возвращать один и тот же NaN
			if (memcmp(&ret, (T*)(&_nan), sizeofT) != 0)
				memcpy_s(&ret, sizeofT, &_nan, sizeofT);
			return ret;
		}
	}
	// -------------------------------------
	template <class Tint>	// любой целочисленный тип
	// Получение элемента массива без проверки индекса.
	// Предназначен для циклов по элементам массива, где
	// проверять индекс нет смысла.
	T & operator () (const Tint index)
	{
		return pData[(int)index];
	}
	// -------------------------------------
	// Обнуление всего массива
	void Zero()
	{
		if (capacity > 0)
			ZeroMemory(pData, capacity * sizeofT);
	}
	// -------------------------------------
	// Заливка всего массива единицами
	void NaN()
	{
		if (size > 0)
			memset(pData, 0xFF, size * sizeofT);
	}
	// -------------------------------------
	// Удаление массива
	void Clear()
	{
		if (capacity > 0)
		{
			delete[] pData;
			capacity = size = 0;
			pData = nullptr;
		}
	}
	// -------------------------------------
	bool TestSize(DWORD newSize)
	{
		return capacity >= newSize;
	}
	// -------------------------------------
	bool ReSize(DWORD newSize, bool bSaveData = false)
	{
		bool ret = size == newSize;
		if (!ret)
		{
			ret = TestSize(newSize);
			if (ret)
			{
				if (bSaveData)
				{
					ZeroMemory(pData + newSize,
						(capacity - newSize) * sizeofT);
				}
				size = newSize;
			}
			else
			{// надо увеличивать массив
				if (bSaveData)
				{// сохранять массив надо
					if (pData != nullptr)
					{// массив есть
						T * pDataOld = pData;
						try
						{
							pData = new T[newSize];
							if (pData != nullptr)
							{
								DWORD sizeInByte = size * sizeofT;
								memcpy_s(pData, sizeInByte, pDataOld, sizeInByte);
								sizeInByte = (newSize - size) * sizeofT;
								ZeroMemory(pData + size, sizeInByte);

								delete[] pDataOld;
								size = capacity = newSize;
								ret = true;
							}
							else
							{// оставляю старый буфер
								pData = pDataOld;
							}
						}
						catch (...)
						{// оставляю старый буфер
							pData = pDataOld;
						}
					}
					else
					{// массива нет
						try
						{
							pData = new T[newSize];
							if (pData != nullptr)
							{
								size = capacity = newSize;
								ZeroMemory(pData, size * sizeofT);
								ret = true;
							}
							else
							{
								size = capacity = 0;
							}
						}
						//	catch (CMemoryException*)
						catch (...)
						{
							pData = nullptr;
							size = capacity = 0;
						}
					}
				}
				else// if (bSaveData)
				{// сохранять массив не надо
					if (pData != nullptr)
					{
						delete[] pData;
						pData = nullptr;
					}
					try
					{
						pData = new T[newSize];
						if (pData != nullptr)
						{
							size = capacity = newSize;
							ret = true;
						}
						else
						{
							size = capacity = 0;
						}
					}
					catch (...)
					{
						pData = nullptr;
						size = capacity = 0;
					}
				}
			}// else // надо увеличивать массив
		}// if (!ret) новый размер != сторому размеру
		return ret;
	}
	// -------------------------------------
	// Копирование number значений в массив SmartArray
	bool Copy(const T *pointer, DWORD number)
	{
		bool ret(true);
		if ((pointer != nullptr) && (number > 0))
		{
			UINT sizeInByte = sizeofT * number;
			ret = ReSize(number) &&
				(memcpy_s(pData, sizeInByte, pointer, sizeInByte) == 0);
			size = ret ? number : 0;
		}
		else
			size = 0;
		return ret;
	}
	// -------------------------------------
	// Копирование number значений из массива SmartArray
	bool CopyOut(T *pointer, DWORD number = -1)
	{
		bool ret(false);
		if (pointer != nullptr)
		{
			UINT num = (number != -1) ? number : size;
			if (num <= size)
			{
				if (num > 0)
				{
					num *= sizeofT;
					ret = memcpy_s(pointer, num, pData, num) == NOERROR;
				}
				else
					ret = true;
			}
		}
		return ret;
	}
	// -------------------------------------
	// Удаление младших чисел с соответствующим уменьшением размера.
	bool DeleteFirstPoint(DWORD number)
	{
		bool ret = size > number;
		if (ret && (number > 0))
		{
			size -= number;
			UINT sizeCopy = size * sizeofT;
			ret = memmove_s(pData, sizeCopy, pData + number, sizeCopy) == 0;
		}
		return ret;
	}
	// -------------------------------------
	// Удаление старших чисел с соответствующим уменьшением размера.
	bool DeleteLastPoint(DWORD number)
	{
		bool ret = size > number;
		if (ret && (number > 0))
			size -= number;
		return ret;
	}
	// -------------------------------------
	// Сдвиг данных на number чисел влево. Реализация сдвигового буфера.
	// number младших чисел будет утрачено. number старших чисел останутся.
	// Размер не изменяется.
	bool ShiftLeft(DWORD number)
	{
		bool ret = size > number;
		if (ret && (number > 0))
		{
			UINT sizeCopy = (size - number) * sizeofT;
			ret = memmove_s(pData, sizeCopy, pData + number, sizeCopy) == 0;
		}
		return ret;
	}
	// -------------------------------------
	// Предполагается, что данный массив исполльзуется как сдвиговый буфер
	// Добавляем одно значение в этот буфер
	bool NewDataInShiftBuffer(T val)
	{
		bool ret = ShiftLeft(1);
		if (ret)
			pData[size - 1] = val;
		return ret;
	}
	// -------------------------------------
	// Предполагается, что данный массив исполльзуется как сдвиговый буфер
	// Добавляем num значение в этот буфер
	bool NewDataInShiftBuffer(T * pVal, DWORD num)
	{
		bool ret = num <= size;
		if (ret)
		{
			ret = ShiftLeft(num);
			if (ret)
			{
				UINT sizeCopy = num * sizeofT;
				ret = 0 ==
					memcpy_s(pData + (size - num), sizeCopy, pVal, sizeCopy);
			}
		}
		else
		{// копирование последних отчётов
			UINT sizeCopy = size * sizeofT;
			ret = 0 ==
				memcpy_s(pData, sizeCopy, pVal + (num - size), sizeCopy);
		}
		return ret;
	}
	// -------------------------------------
	// Предполагается, что данный массив исполльзуется как сдвиговый буфер
	// Изменение размера этого буфера с сохранением в нем данных.
	bool ReSizeShiftBuffer(DWORD newSize)
	{
		bool ret = size == newSize;
		if (!ret)
		{
			if (newSize > 0)
			{
				if (size > newSize)
				{
					ret = DeleteFirstPoint(size - newSize);
				}
				else
				{
					DWORD sizeOld = size * sizeofT;
					DWORD sizeNew = (newSize - size) * sizeofT;
					ret = ReSize(newSize, true);
					if (ret)
					{
						ret = (0 == memmove_s(((BYTE*)pData) + sizeNew, sizeOld, pData, sizeOld)) &&
							memset(pData, 0xFF, sizeNew);
					}
				}
			}
			else
				size = 0;
		}
		return ret;
	}
	// -------------------------------------
	bool AppendData(T *pointer, const DWORD number, const bool bReserve10Persent = false)
	{
		DWORD sizeOld = size;
		DWORD sizeNew = size + number;
		if (bReserve10Persent)
			sizeNew += sizeNew / 10;
		bool ret = ReSize(sizeNew, true);
		if (ret)
		{
			DWORD sizeCopy = number * sizeofT;
			ret = memcpy_s(pData + sizeOld, sizeCopy, pointer, sizeCopy) == 0;
			if (ret)
				size = sizeOld + number;
		}
		return ret;
	}
	// -------------------------------------
};
//------------------------------------------------------------------------------