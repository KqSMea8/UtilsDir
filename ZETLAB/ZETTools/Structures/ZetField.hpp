//------------------------------------------------------------------------------
#pragma once //TODO: ������� ������ ��� MSVC
#include <exception>
#include <limits>
#include <set>
#include <stdint.h>
#include <vector>
#include <array>
#include <cassert>
#include <functional>
#include <type_traits>
//------------------------------------------------------------------------------
#undef max
//------------------------------------------------------------------------------
namespace zet
{

namespace configuration
{
namespace schema
{
//------------------------------------------------------------------------------
typedef uint32_t StructModule;
const StructModule DefaultModuleValue = 0xFFFFFFFF;
typedef uint16_t StructProcess;
const StructProcess DefaultProcessValue = 0xFFFF;
typedef uint16_t StructChannel;
const StructChannel DefaultChannelValue = 0xFFFF;
typedef uint64_t StructDevice;
const StructDevice InvalidDeviceValue = 0xFFFFFFFFFFFFFFFF;
typedef uint16_t StructValidationType;
typedef uint32_t StructType;
typedef std::exception ZetException; // TODO: �������� �� zet::Exception
//------------------------------------------------------------------------------
template<class FieldClass, const size_t fieldSize>
class FieldArray : public std::array<FieldClass, fieldSize>
{
	// const size_t m_maxIndex = fieldSize;
public:
	/*
	FieldClass & operator [](int index) const
	{
		if (index < 0 || index >= m_maxIndex) throw ZetException("Index out of range");
		// return *(this->[index]);
		return std::array::operator[index];
	}
	*/
};
//------------------------------------------------------------------------------
template<class FieldType>
void TestValueDefault(
	// ������� ���������
	const FieldType newValue, const FieldType mini, const FieldType maxi,
	// �������� ��������
	FieldType *pValue)
{
	if (pValue)
	{
		if (newValue < mini)
			*pValue = mini;
		else if (newValue > maxi)
			*pValue = maxi;
		else
			*pValue = newValue;
	}
}
//------------------------------------------------------------------------------
template<class FieldType>
void TestValueDoNothing(
	// ������� ���������
	const FieldType newValue, const FieldType mini, const FieldType maxi,
	// �������� ��������
	FieldType *pValue)
{
	if (pValue)
	{
		if ((mini <= newValue) && (newValue <= maxi))
			*pValue = newValue;
		//	else ������ �� �����
	}
}
//------------------------------------------------------------------------------
template<class FieldType>
void TestSetDefault(
	// ������� ���������
	const FieldType newValue, const std::set<FieldType> &set,
	// �������� ��������
	FieldType *pValue)
{
	if (pValue)
	{
		if (set.find(newValue) != set.end())
		{
			*pValue = newValue;
		}
		else
		{
			FieldType fronf = *set.begin();
			FieldType back = *set.rbegin();
			if (newValue < fronf)
				*pValue = fronf;
			else if (newValue > back)
				*pValue = back;
			else
			{// ��������� �������� �� ������
				auto it = set.upper_bound(newValue);
				FieldType x1 = *it--;
				FieldType dx1 = (newValue > x1) ? (newValue - x1) : (x1 - newValue);
				FieldType x2 = *it;
				FieldType dx2 = (newValue > x2) ? (newValue - x2) : (x2 - newValue);
				*pValue = (dx2 < dx1) ? x2 : x1;
			}
		}
	}
}
//------------------------------------------------------------------------------
// ����� �������� ���������� ���� FieldType
template<typename FieldType>
class NumericField
{
	typedef std::set<FieldType> ValueSet;

private:	//----------------------------------------------		
	void TestValuesInDesigner()
	{
		if (minimum > maximum)
			throw ZetException("Invalid parameters! Maximum value less than mimimum");
	
		// �������� �������� default ---------------------------------
		FieldType *pDefault = const_cast<FieldType*>(&default);
		if (default == std::numeric_limits<FieldType>::max())
		{// default �� �����, �����
			if (set.empty())
			{// ��������
				*pDefault = ((minimum <= (FieldType)0) && ((FieldType)0 <= maximum)) ?
								(FieldType)0 : minimum;
			}
			else
			{// ������
				*pDefault = *set.begin();
			}
		}
		else
		{// �������� ��������� default
			if (set.empty())
			{// ��������
				if (default < minimum)
					*pDefault = minimum;
				else if (default > maximum)
					*pDefault = maximum;
			}
			else
			{// ������
				TestSetDefault(default, set, pDefault);
			}
		}
		m_value = default;
	}

protected:	//----------------------------------------------
	FieldType m_value;

public:		//----------------------------------------------
	const FieldType minimum;
	const FieldType maximum;
	const FieldType default;
	const std::string description;
	const ValueSet set;
public:
	// ����������� ��� �������� �� ����� ����������
	NumericField(
		const FieldType mininum_ = std::numeric_limits<FieldType>::lowest(),
		const FieldType maximun_ = std::numeric_limits<FieldType>::max(),
		const FieldType default_ = std::numeric_limits<FieldType>::max(),
		const ValueSet &set_ = ValueSet())
		: minimum(mininum_)
		, maximum(maximun_)
		, default(default_)
		, set(set_)
	{
		TestValuesInDesigner();			
	}

	// ����������� � ��������� � default-���������
	NumericField(
		const std::string &description_,
		const FieldType mininum_ = std::numeric_limits<FieldType>::lowest(),
		const FieldType maximun_ = std::numeric_limits<FieldType>::max(),
		const FieldType default_ = std::numeric_limits<FieldType>::max(),
		const ValueSet &set_ = ValueSet())
		: description(description_)
		, minimum(mininum_)
		, maximum(maximun_)
		, default(default_)
		, set(set_)
	{
		TestValuesInDesigner();
	}

	// ����������� � ���������, �� ��� default-��������
	NumericField(
		const std::string &description_,
		const FieldType mininum_ = std::numeric_limits<FieldType>::lowest(),
		const FieldType maximun_ = std::numeric_limits<FieldType>::max(),
		const ValueSet &set_ = ValueSet())
		: description(description_)
		, minimum(mininum_)
		, maximum(maximun_)
		, default((FieldType)0)
		, set(set_)
	{
		TestValuesInDesigner();
	}				

	~NumericField(){}

	//---------------------------------------------------------------------
	// ���������� ������� ���� ����� --------------------------------------
	typedef std::function < void(
		const FieldType,	// newValue
		const FieldType,	// minimum
		const FieldType,	// maximum
		const FieldType,	// default
		FieldType*) >	// Value for transformation
		TransformationValue;

	// ��������� �� ���������. �������� �� ��� � �������������, ���� �����
	TransformationValue TransformatorValue_Default = [](
		const FieldType newValue,
		const FieldType mini,
		const FieldType maxi,
		const FieldType defa,
		FieldType *pValue) -> void
	{
		if (std::numeric_limits<FieldType>::is_integer)
		{// int ...
			TestValueDefault(newValue, mini, maxi, pValue);
		} 
		else
		{// float ..
			if (_isnan(newValue) == 0)
			{// �����
				TestValueDefault(newValue, mini, maxi, pValue);
			}
			else
			{// NaN
				*pValue = defa;
			}
		}
	};

	// �������������� ���������. �������� �� ��� � ��� �������������, ���� �����
	TransformationValue TransformatorValue_DoNothing = [](
		const FieldType newValue,
		const FieldType mini,
		const FieldType maxi,
		const FieldType defa,
		FieldType *pValue) -> void
	{
		if (std::numeric_limits<FieldType>::is_integer)
		{// int ...
			TestValueDoNothing(newValue, mini, maxi, pValue);
		}
		else
		{// float ...
			if (_isnan(newValue) == 0)
			{// �����
				TestValueDoNothing(newValue, mini, maxi, pValue);
			}
		}
	};

	//---------------------------------------------------------------------
	// ���������� ������� ���� ����� ����� --------------------------------
	typedef std::function < void(
		const FieldType,	// newValue
		const FieldType,	// default
		const ValueSet&,	// ������ set
		FieldType *pFieldType) >	// Value for transformation
	TransformationSet;

	// ��������� �� ���������. �������� � �������������, ���� �����
	TransformationSet TransformatorSet_Default = [](
		const FieldType newValue,
		const FieldType defa,
		const ValueSet& set,
		FieldType *pFieldType) -> void
	{
		if (std::numeric_limits<FieldType>::is_integer)
		{// int ...
			TestSetDefault(newValue, set, pFieldType);
		}
		else
		{// float ..
			if (_isnan(newValue) == 0)
			{// �����
				TestSetDefault(newValue, set, pFieldType);
			}
			else
			{// NaN
				*pFieldType = defa;
			}
		}
	};

	// �������������� ���������. �������� � ��� �������������, ���� �����
	TransformationSet TransformatorSet_DoNothing = [](
		const FieldType newValue,
		const FieldType defa,
		const ValueSet& set,
		FieldType *pFieldType) -> void
	{// int ...
		if (std::numeric_limits<FieldType>::is_integer)
		{
			if (set.find(newValue) != set.cend())
				*pFieldType = newValue;
		} 
		else
		{// float ...
			if (_isnan(newValue) == 0)
			{// �����
				if (set.find(newValue) != set.cend())
					*pFieldType = newValue;
			}
		}
	};

	// ����������. ������� ������� �����
	TransformationValue m_transformatorValue = TransformatorValue_Default;
//	TransformationValue m_transformatorValue = TransformatorValue_DoNothing;

	TransformationSet m_transformatorSet = TransformatorSet_Default;
//	TransformationSet m_transformatorSet = TransformatorSet_DoNothing;

	void SetTransformationValue(TransformationValue transformator)
	{
		m_transformatorValue = transformator;
	}
	void ResetTransformationValue()
	{
		m_transformatorValue = TransformatorValue_Default;
	}
	void SetTransformationSet(TransformationSet transformator)
	{
		m_transformatorSet = transformator;
	}
	void ResetTransformationSet()
	{
		m_transformatorSet = TransformatorSet_Default;
	}

	// �������� ���������� � ������� ���������� ����
	operator FieldType() const
	{
		return m_value;
	}

	// �������� ��������� ����� ������� ����
	const bool operator == (const FieldType other) const
	{
		return !(m_value != other);
	}

	// �������� ��������� ����� �� ������� ����
	template < typename T >
	const bool operator == (const T tOther) const
	{
		return !(m_value != (FieldType)tOther);			
	}

	// �������� �� ��������� ����� ������� ����
	const bool operator != (const FieldType other) const
	{
		return !(m_value == other);
	}

	// �������� �� ��������� ����� �� ������� ����
	template < typename T >
	const bool operator != (const T tOther) const
	{
		return !(m_value == (FieldType)tOther);			
	}

	// �������� ������������ ����� ������� ����
	FieldType & operator = (const FieldType newValue)
	{
		if (set.empty())
			m_transformatorValue(newValue, minimum, maximum, default, &m_value);
		else
			m_transformatorSet(newValue, default, set, &m_value);
		
		return m_value;
	}

	// �������� ������������ ���������� NumericField
	NumericField & operator = (const NumericField & rNumField)
	{
		FieldType * pField = const_cast<FieldType*>(&minimum); _ASSERTE(pField);
		*pField = rNumField.minimum;

		pField = const_cast<FieldType*>(&maximum); _ASSERTE(pField);
		*pField = rNumField.maximum;

		pField = const_cast<FieldType*>(&default); _ASSERTE(pField);
		*pField = rNumField.default;

		std::string * pString = const_cast<std::string*>(&description); _ASSERTE(pString);
		*pString = rNumField.description;

		ValueSet * pSet = const_cast<ValueSet*>(&set); _ASSERTE(pSet);
		*pSet = rNumField.set;

		operator = (rNumField.m_value);

		return *this;
	}

	// �������� �� ��������� ���������� NumericField
	const bool operator != (const NumericField & rNumField) const
	{
		if (m_value != rNumField.m_value)
			return true;
		if (minimum != rNumField.minimum)
			return true;
		if (maximum != rNumField.maximum)
			return true;
		if (default != rNumField.default)
			return true;
		if (description != rNumField.description)
			return true;
		if (set != rNumField.set)
			return true;

		return false;
	}
};
//------------------------------------------------------------------------------
// template<typename FieldType = std::string, size_t maxSize = std::string::max_size()>
// typedef std::set<FieldType> ValueList;

//template<typename FieldType, size_t maxSize, class ValueList list>


template<typename FieldType, size_t maxSize>
class StringField
{
	typedef std::set<FieldType> ValueSet;

public:
	const static size_t maxlength = maxSize;

protected:
	FieldType m_value;

public: // TODO: ��� ����-��������� ��������� ������� � static-����.
	const FieldType default;
	const std::string description;
	const ValueSet set;

public:
	// �����������
	StringField(
		const FieldType &description_ = "",
		const FieldType &default_ = "",
		const ValueSet &set_ = ValueSet())
		: description(description_)
		, default(default_)
		, set(set_)
	{ 
		if (default.empty())
			return; // TODO: nullable emulation.
		operator = (default); 
	}

	~StringField(){}

	operator FieldType() const
	{
		return m_value;
	}				

	FieldType & operator = (const FieldType &newValue)// throw (ZetException)
	{
		if (!set.empty())
		{
			if (set.find(newValue) == set.cend())
				throw ZetException("New value is not member of allowed value set");
		}

		if (newValue.size() >= maxlength)
			throw ZetException("New value has invalid size");

		m_value = newValue;
		return m_value;
	}

	const bool operator != (const StringField & rStringField) const
	{
		if (m_value != rStringField.m_value)
			return true;
		if (maxlength != rStringField.maxlength)
			return true;
		if (set != rStringField.set)
			return true;
		if (description != rStringField.description)
			return true;
		if (default != rStringField.default)
			return true;

		return false; // ==
	}
};
//------------------------------------------------------------------------------
}
}
}
//------------------------------------------------------------------------------