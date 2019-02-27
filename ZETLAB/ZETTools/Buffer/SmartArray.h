//------------------------------------------------------------------------------
/*	��������� ��������� ������������� ��� ���������� ����� ��������.
	��������� �����:
- ����������� �� ���������;
- ����������;
- ��������������� ����������, ����������� ����� �������� ������ �������;
- ����������� �����������;
- �������� ������������;
- �������� [] ��� ��������� � �������� ������� �� ������������ �������;
- �������� () ��� ��������� � �������� ������� �� �� ������������ �������;
- ����� ReSize ��� ��������� ������� �������;
- ������ ����������� �(��) ������(�);
- ������ ��� ���������� ���������� ������, � ������� ��� ������;
- ������ �������� ������...
//------------------------------------------------------------------------------
	��� ������������ ���������� ��������� ��������� ������ ������� �� ����,
� ���� ��������� ����������� ����������.
//------------------------------------------------------------------------------
	�� ��������� ����������� � �������� ���� ������� � ����� ������������
������� ���� - int, float, double, ...*/
//------------------------------------------------------------------------------
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
template <class T>
struct SmartArray
{
	DWORD size;		// ������ ���������� ������ � �������
	DWORD capacity;	// ������ �������� �������
	T *pData;		// ��������� �� ������
	DWORD sizeofT;	// ��������� ��� ������������ �� 8 ����
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
	template <class Tint>	// ����� ������������� ���
	// ��������� �������� ������� � ��������� �������
	T & operator [] (const Tint index)
	{// ������� ������ ��������� ������������� �������������
	// ��������� ����������� � SmartArray
		static const T _nan(std::numeric_limits<T>::quiet_NaN());
		static T ret(_nan);
		if ((0 <= index) && ((DWORD)(int)index < size))
		{
			return pData[(int)index];
		}
		else
		{	// ���� ��� ��������� ��� ������ ���������� ��������
			// ���������� ���� � ��� �� NaN
			if (memcmp(&ret, (T*)(&_nan), sizeofT) != 0)
				memcpy_s(&ret, sizeofT, &_nan, sizeofT);
			return ret;
		}
	}
	// -------------------------------------
	template <class Tint>	// ����� ������������� ���
	// ��������� �������� ������� ��� �������� �������.
	// ������������ ��� ������ �� ��������� �������, ���
	// ��������� ������ ��� ������.
	T & operator () (const Tint index)
	{
		return pData[(int)index];
	}
	// -------------------------------------
	// ��������� ����� �������
	void Zero()
	{
		if (capacity > 0)
			ZeroMemory(pData, capacity * sizeofT);
	}
	// -------------------------------------
	// ������� ����� ������� ���������
	void NaN()
	{
		if (size > 0)
			memset(pData, 0xFF, size * sizeofT);
	}
	// -------------------------------------
	// �������� �������
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
			{// ���� ����������� ������
				if (bSaveData)
				{// ��������� ������ ����
					if (pData != nullptr)
					{// ������ ����
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
							{// �������� ������ �����
								pData = pDataOld;
							}
						}
						catch (...)
						{// �������� ������ �����
							pData = pDataOld;
						}
					}
					else
					{// ������� ���
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
				{// ��������� ������ �� ����
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
			}// else // ���� ����������� ������
		}// if (!ret) ����� ������ != ������� �������
		return ret;
	}
	// -------------------------------------
	// ����������� number �������� � ������ SmartArray
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
	// ����������� number �������� �� ������� SmartArray
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
	// �������� ������� ����� � ��������������� ����������� �������.
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
	// �������� ������� ����� � ��������������� ����������� �������.
	bool DeleteLastPoint(DWORD number)
	{
		bool ret = size > number;
		if (ret && (number > 0))
			size -= number;
		return ret;
	}
	// -------------------------------------
	// ����� ������ �� number ����� �����. ���������� ���������� ������.
	// number ������� ����� ����� ��������. number ������� ����� ���������.
	// ������ �� ����������.
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
	// ��������������, ��� ������ ������ ������������� ��� ��������� �����
	// ��������� ���� �������� � ���� �����
	bool NewDataInShiftBuffer(T val)
	{
		bool ret = ShiftLeft(1);
		if (ret)
			pData[size - 1] = val;
		return ret;
	}
	// -------------------------------------
	// ��������������, ��� ������ ������ ������������� ��� ��������� �����
	// ��������� num �������� � ���� �����
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
		{// ����������� ��������� �������
			UINT sizeCopy = size * sizeofT;
			ret = 0 ==
				memcpy_s(pData, sizeCopy, pVal + (num - size), sizeCopy);
		}
		return ret;
	}
	// -------------------------------------
	// ��������������, ��� ������ ������ ������������� ��� ��������� �����
	// ��������� ������� ����� ������ � ����������� � ��� ������.
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