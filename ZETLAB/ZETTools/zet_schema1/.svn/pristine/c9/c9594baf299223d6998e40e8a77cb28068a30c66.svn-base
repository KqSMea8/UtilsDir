/// @file zet_schema1.h
/// @brief Библиотека обработки схем версии 1
/// @author Yursen Kim <kyursen@gmail.com>
/// @copyright Copyright 2015 ZETLAB

/// @todo Работа с реальной структурой на основе схемы
/// @todo Набить тесты

#ifndef ZET_SCHEMA1_H
#define ZET_SCHEMA1_H

// Compiler check
#if __cplusplus <= 199711L
	#if !defined(_MSC_VER)
		#error C++11 support is required
	#elif _MSC_VER < 1800
		#error Visual Studio 2013 or higher is required
	#endif
#endif

#include <stdint.h>

#include <cstdint>
#include <cstdlib>

#include <string>
#include <sstream>
#include <iostream>

#include <list>
#include <map>
#include <vector>

#include <algorithm>

#include "zet_struct1.h"

#if defined(_MSC_VER)
	#define ZET_SCHEMA1_HAS_ALIGNED_UNION 1
	#define ZET_SCHEMA1_HAS_DEFAULTFLOAT 1
#endif

namespace zet {
namespace schema1 {

/// @defgroup constants Константы
/// @brief Различные константы.
/// @details
/// Большинство констант составлены с помощью генераторов.

/// @defgroup const_generators Генераторы констант
/// @brief Генераторы различных констант.
/// @details
/// Генераторы - это макрофункции, которые позволяют сгенерировать код на этапе препроцессинга.
/// При вызове генератора в параметре указывается некая заранее определенная макрофункция M.
/// Генератор вызывает макрофункцию M несколько раз с разными параметрами.
/// После вызова генератора макрофункцию рекомендуется #undef.

/// @ingroup const_generators
/// @brief Генератор строковых базовых типов.
/// @param ID код базового типа (число от 1 до 255).
/// @param ENUM название типа (идентификатор).
/// @param SNAME название типа в текстовом формате (строка).
/// @param CTYPE соответствующий тип на языке ANSI C.
#define ZET_SCHEMA1_GENERATE_STRING_TYPES(M) \
	\
	M(1,  CHAR, "char", char)

/// @ingroup const_generators
/// @brief Генератор целочисленных базовых типов.
/// @param ID код базового типа (число от 1 до 255).
/// @param ENUM название типа (идентификатор).
/// @param SNAME название типа в текстовом формате (строка).
/// @param CTYPE соответствующий тип на языке ANSI C.
#define ZET_SCHEMA1_GENERATE_INTEGER_TYPES(M) \
	\
	M(2,  U8,   "u8",   uint8_t) \
	M(3,  S8,   "s8",   int8_t) \
	M(4,  U16,  "u16",  uint16_t) \
	M(5,  S16,  "s16",  int16_t) \
	M(6,  U32,  "u32",  uint32_t) \
	M(7,  S32,  "s32",  int32_t) \
	M(8,  U64,  "u64",  uint64_t) \
	M(9,  S64,  "s64",  int64_t)

/// @ingroup const_generators
/// @brief Генератор базовых типов для чисел с плавающей запятой.
/// @param ID код базового типа (число от 1 до 255).
/// @param ENUM название типа (идентификатор).
/// @param SNAME название типа в текстовом формате (строка).
/// @param CTYPE соответствующий тип на языке ANSI C.
#define ZET_SCHEMA1_GENERATE_FLOAT_TYPES(M) \
	\
	M(10, F32,  "f32",  float) \
	M(11, F64,  "f64",  double)

/// @ingroup const_generators
/// @brief Генератор арифметических (числовых) базовых типов.
/// @param ID код базового типа (число от 1 до 255).
/// @param ENUM название типа (идентификатор).
/// @param SNAME название типа в текстовом формате (строка).
/// @param CTYPE соответствующий тип на языке ANSI C.
#define ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(M) \
	\
	ZET_SCHEMA1_GENERATE_INTEGER_TYPES(M) \
	ZET_SCHEMA1_GENERATE_FLOAT_TYPES(M)

/// @ingroup const_generators
/// @brief Генератор всех возможных базовых типов.
/// @param ID код базового типа (число от 1 до 255).
/// @param ENUM название типа (идентификатор).
/// @param SNAME название типа в текстовом формате (строка).
/// @param CTYPE соответствующий тип на языке ANSI C.
#define ZET_SCHEMA1_GENERATE_BASE_TYPES(M) \
	\
	ZET_SCHEMA1_GENERATE_STRING_TYPES(M) \
	ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(M)

/// @ingroup constants
/// @brief Базовые типы.
/// @details Значения генерируются с помощью #ZET_SCHEMA1_GENERATE_BASE_TYPES(M).
enum class BaseType : uint8_t {
	UNDEFINED = UINT8_MAX, ///< Базовый тип не определен
	METADATA = 0, ///< Метаданные (не базовый тип)

/// @cond
#define ZET_SCHEMA1_M(ID, ENUM, SNAME, CTYPE) ENUM = ID,
	ZET_SCHEMA1_GENERATE_BASE_TYPES(ZET_SCHEMA1_M)
#undef ZET_SCHEMA1_M
/// @endcond
};
static_assert(sizeof(BaseType) == sizeof(uint8_t), "Invalid BaseType size");

/// @ingroup const_generators
/// @brief Генератор типов значений в свойствах.
/// @param ID код типа (число).
/// @param ENUM название типа (идентификатор).
/// @param SNAME комментарий (строка).
#define ZET_SCHEMA1_GENERATE_PROPERTY_TYPES(M) \
	\
	M(0, NONE,        "none") \
	M(1, IDENTIFIER,  "identifier") \
	M(2, STRING,      "string") \
	M(3, STRING_LIST, "string[]") \
	M(4, BASE,        "base_type") \
	M(5, BASE_LIST,   "base_type[]") \
	M(6, BOOL,        "bool") \
	M(7, CONTROL,     "u32") \
	M(8, BASE_TYPE,   "u32") \
	M(9, USER_TYPE,   "u32")

/// @ingroup constants
/// @brief Типы значений свойств.
/// @details Значения генерируются с помощью #ZET_SCHEMA1_GENERATE_PROPERTY_TYPES(M).
enum class PropertyType : uint8_t {
	UNDEFINED = UINT8_MAX, ///< Тип свойства не определен

/// @cond
#define ZET_SCHEMA1_M(ID, ENUM, SNAME) ENUM = ID,
	ZET_SCHEMA1_GENERATE_PROPERTY_TYPES(ZET_SCHEMA1_M)
#undef ZET_SCHEMA1_M
/// @endcond
};
static_assert(sizeof(PropertyType) == sizeof(uint8_t), "Invalid PropertyType size");

/// @ingroup constants
enum class PropertyFlags : uint8_t {
	BASE = 0x01, ///< Свойство может быть указано внутри поля
	META = 0x02, ///< Свойство может быть указано для всей структуры (метаданные)
	BOTH = BASE | META, ///< Свойство может быть указано в любом месте
};

/// @ingroup const_generators
/// @brief Генератор свойств.
/// @param ID код свойства (число от 0 до 65535).
/// @param ENUM название свойства (идентификатор).
/// @param SNAME название свойства в текстовом формате (строка).
/// @param FLAGS флаги (PropertyFlags).
/// @param PTYPE тип значения (PropertyType).
#define ZET_SCHEMA1_GENERATE_PROPERTIES(M) \
	\
	M(0,  TITLE,       "title",       META, PropertyType::STRING) \
	M(1,  NAME,        "name",        BOTH, PropertyType::IDENTIFIER) \
	M(2,  DESCRIPTION, "description", BOTH, PropertyType::STRING) \
	M(3,  DEFAULT,     "default",     BASE, PropertyType::BASE) \
	M(4,  CONTROL,     "control",     BASE, PropertyType::CONTROL) \
	M(5,  CONSTANT,    "const",       META, PropertyType::BASE_TYPE) \
	M(6,  MINIMUM,     "minimum",     BASE, PropertyType::BASE) \
	M(7,  MAXIMUM,     "maximum",     BASE, PropertyType::BASE) \
	M(8,  LIST,        "list",        BOTH, PropertyType::BASE_LIST) \
	M(9,  STRING_LIST, "string_list", BASE, PropertyType::STRING_LIST) \
	M(10, EXCLUDE,     "exclude",     BASE, PropertyType::BASE_LIST) \
	M(11, CHECK,       "check",       BASE, PropertyType::STRING) \
	M(12, EXPRESSION,  "expression",  BASE, PropertyType::STRING) \
	M(13, HIDDEN,      "hidden",      BOTH, PropertyType::BOOL) \
	M(14, READONLY,    "readonly",    BOTH, PropertyType::BOOL) \
	M(15, GROUP,       "group",       META, PropertyType::STRING) \
	M(17, TYPE,        "type",        BASE, PropertyType::USER_TYPE) \
	M(18, GT,          "gt",          BASE, PropertyType::BASE) \
	M(19, LT,          "lt",          BASE, PropertyType::BASE) \
	M(20, UNIT,        "unit",        BASE, PropertyType::STRING) \
	M(21, RESOLUTION,  "resolution",  BASE, PropertyType::BASE) \
	M(22, REFERENCE,   "reference",   BASE, PropertyType::BASE) \
	M(23, INACCURACY,  "inaccuracy",  BASE, PropertyType::NONE) \
	// New properties to be added here

/// @ingroup constants
/// @brief Коды свойств.
/// @details Значения генерируются с помощью #ZET_SCHEMA1_GENERATE_PROPERTIES(M).
enum class PropertyId : uint16_t {
	UNDEFINED = UINT16_MAX, ///< Свойство не определено

/// @cond
#define ZET_SCHEMA1_M(ID, ENUM, SNAME, FLAGS, PTYPE) ENUM = ID,
	ZET_SCHEMA1_GENERATE_PROPERTIES(ZET_SCHEMA1_M)
#undef ZET_SCHEMA1_M
/// @endcond
};
static_assert(sizeof(PropertyId) == sizeof(uint16_t), "Invalid PropertyId size");

/// @ingroup const_generators
/// @brief Генератор пользовательских типов (значений свойства type).
/// @param ID код пользовательского типа (uint32_t).
/// @param ENUM название типа (идентификатор).
/// @param SNAME название типа в текстовом формате (строка).
/// @param MIN_SIZE минимальный размер поля (положительное число).
/// @param MAX_SIZE максимальный размер поля (положительное число).
#define ZET_SCHEMA1_GENERATE_USER_TYPES(M) \
	\
	M(1, TIME,      "time",      4,  8) \
	M(2, TIMESTAMP, "timestamp", 16, 16) \
	M(3, IP4,       "ip4",       4,  4) \
	M(4, IP6,       "ip6",       16, 16) \
	M(5, MAC,       "mac",       6,  8) \
	// New user types to be added here

/// @ingroup constants
/// @brief Пользовательские (расширенные) типы.
/// @details Значения генерируются с помощью #ZET_SCHEMA1_GENERATE_USER_TYPES(M).
enum class UserType : uint32_t {
	UNDEFINED = UINT32_MAX, ///< Расширенный тип не определен

/// @cond
#define ZET_SCHEMA1_M(ID, ENUM, SNAME, MIN_SIZE, MAX_SIZE) ENUM = ID,
	ZET_SCHEMA1_GENERATE_USER_TYPES(ZET_SCHEMA1_M)
#undef ZET_SCHEMA1_M
/// @endcond
};

/// @ingroup const_generators
/// @brief Генератор элементов пользовательского интерфейса (значения свойства control).
/// @param ID код элемента (uint32_t).
/// @param ENUM название элемента (идентификатор).
/// @param SNAME название элемента в текстовом формате (строка).
#define ZET_SCHEMA1_GENERATE_CONTROLS(M) \
	\
	M(1,  EDIT,           "edit") \
	M(2,  CHECKBOX,       "checkbox") \
	M(3,  COMBOBOX,       "combobox") \
	M(4,  STATIC,         "static") \
	M(6,  TIME,           "time") \
	M(7,  CHANNEL,        "channel") \
	M(8,  ROTATE,         "rotate") \
	M(9,  IP4,            "ip4") \
	M(10, IP6,            "ip6") \
	M(11, DEVICE_CHANNEL, "device_channel") \
	M(13, MULTIEDIT ,     "multiedit") \
	M(14, FILE,           "file") \
	M(15, COLOR,          "color")
	// New controls to be added here

/// @ingroup constants
/// @brief Элементы пользовательского интерфейса (контролы).
/// @details Значения генерируются с помощью #ZET_SCHEMA1_GENERATE_CONTROLS(M).
enum class ControlId : uint32_t {
	UNDEFINED = UINT32_MAX, ///< Элемент не определен

/// @cond
#define ZET_SCHEMA1_M(ID, ENUM, SNAME) ENUM = ID,
	ZET_SCHEMA1_GENERATE_CONTROLS(ZET_SCHEMA1_M)
#undef ZET_SCHEMA1_M
/// @endcond
};
static_assert(sizeof(ControlId) == sizeof(uint32_t), "Invalid ControlId size");

/// @defgroup model Схема
/// @brief Представление схемы в виде классов C++

/// @ingroup model
/// @brief Полный идентификатор (адрес) структуры.
struct StructId
{
	static uint64_t const DEVICE_MASK = 0xffffFFFFffffFFFFull;
	static uint32_t const MODULE_MASK = 0xffffFFFFul;
	static uint16_t const PROCESS_MASK = 0xffff;
	static uint16_t const CHANNEL_MASK = 0xffff;

	uint64_t device; ///< Идентификатор устройства
	uint32_t module; ///< Идентификатор модуля
	uint16_t process; ///< Идентификатор процесса
	uint16_t channel; ///< Идентификатор канала

	StructId() : device(DEVICE_MASK), module(MODULE_MASK), process(PROCESS_MASK), channel(CHANNEL_MASK) {}
};
static_assert(sizeof(StructId) == 16, "Invalid StructId size");

/// @ingroup model
/// @brief Метка времени структуры.
struct StructTime
{
	uint64_t seconds; ///< Количество секунд, прошедших с 01.01.1970 00:00
	uint32_t nanoseconds; ///< Количество наносекунд между seconds и seconds + 1
	uint32_t ticks; ///< Количество тиков

	StructTime() : seconds(0), nanoseconds(0), ticks(0) {}
};
static_assert(sizeof(StructTime) == 16, "Invalid StructTime size");

/// @ingroup model
/// @brief Заголовок структуры.
struct Header
{
	static uint32_t const TYPE_MASK = 0x80000000ul;

	uint32_t size;
	uint32_t type;
	StructId id;
	StructTime time;

	Header() : size(ZETLAB_ZETVOID_SIZE), type(0) {}
};
static_assert(sizeof(Header) == ZETLAB_ZETHEADER_SIZE, "Invalid Header size");

/// @ingroup model
/// @brief Окончание структуры.
struct Footer
{
	uint32_t size;
	uint32_t crc;

	Footer() : size(ZETLAB_ZETVOID_SIZE), crc(0) {}
};
static_assert(sizeof(Footer) == ZETLAB_ZETFOOTER_SIZE, "Invalid Footer size");

//class Structure;
//class Field;
//class Property;
//class BaseVector;

typedef std::string string;
typedef std::vector<uint8_t> binary;

/// @ingroup model
/// @brief Вектор элементов базового типа.

/// Используется для хранения значений полей или свойств.
/// В зависимости от указанного базового типа, внутри создается соответствующий вектор элементов.
/// Вектор поддерживает конвертирование из строки и в строку для всех базовых типов.
class BaseVector
{
private:
	union Pointer {
		std::vector<string> *STRING;

/// @cond
#define ZET_SCHEMA1_M(ID, ENUM, SNAME, CTYPE) \
		std::vector<CTYPE> *ENUM;
	ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(ZET_SCHEMA1_M)
#undef ZET_SCHEMA1_M
/// @endcond
		Pointer() : STRING(nullptr) {}
	};

private:

#if defined(ZET_SCHEMA1_HAS_ALIGNED_UNION)
	std::aligned_union< sizeof(std::vector<string>), std::vector<string> >::type storage;
#else
	std::aligned_storage< sizeof(std::vector<string>), alignof(std::vector<string>) >::type storage;
	static_assert(alignof(std::vector<string>) <= alignof(std::vector<uint8_t>), "Invalid storage");
	static_assert(alignof(std::vector<string>) <= alignof(std::vector<uint64_t>), "Invalid storage");
	static_assert(alignof(std::vector<string>) <= alignof(std::vector<double>), "Invalid storage");
#endif

	Pointer pointer;
	BaseType type;

public:
	BaseVector() : type(BaseType::UNDEFINED) {}
	BaseVector(BaseType base_type) : type(BaseType::UNDEFINED) { SetType(base_type); }
	BaseVector(const BaseVector &init) : type(BaseType::UNDEFINED) { operator =(init); }
	~BaseVector() { Reset(); }

	void SetType(BaseType base_type);
	void Reset();
	BaseVector &operator = (const BaseVector &init);

	BaseType GetType() const { return this->type; }
	size_t GetCount() const;
	string GetString(size_t index = 0) const;

	const std::vector<string> *STRING() const { return (this->type == BaseType::CHAR) ? this->pointer.STRING : nullptr; }
	std::vector<string> *STRING() { return (this->type == BaseType::CHAR) ? this->pointer.STRING : nullptr; }

/// @cond
#define ZET_SCHEMA1_M(ID, ENUM, SNAME, CTYPE) \
	const std::vector<CTYPE> *ENUM() const { return (this->type == BaseType::ENUM) ? this->pointer.ENUM : nullptr; } \
	std::vector<CTYPE> *ENUM() { return (this->type == BaseType::ENUM) ? this->pointer.ENUM : nullptr; }
	ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(ZET_SCHEMA1_M)
#undef ZET_SCHEMA1_M
/// @endcond

	template<typename... Args>
	bool Assign(const Args &... args)
	{
		switch (this->type) {
		case BaseType::UNDEFINED:
		case BaseType::METADATA:
			return false;
		case BaseType::CHAR:
			return AssignTo(this->pointer.STRING, args...);

/// @cond
#define ZET_SCHEMA1_M(ID, ENUM, SNAME, CTYPE) \
		case BaseType::ENUM: \
			return AssignTo(this->pointer.ENUM, args...);
		ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(ZET_SCHEMA1_M)
#undef ZET_SCHEMA1_M
/// @endcond
		}
		return false;
	}

	template<typename... Args>
	bool Append(const Args &... args)
	{
		switch (this->type) {
		case BaseType::UNDEFINED:
		case BaseType::METADATA:
			return false;
		case BaseType::CHAR:
			return AppendTo(this->pointer.STRING, args...);

/// @cond
#define ZET_SCHEMA1_M(ID, ENUM, SNAME, CTYPE) \
		case BaseType::ENUM: \
			return AppendTo(this->pointer.ENUM, args...);
		ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(ZET_SCHEMA1_M)
#undef ZET_SCHEMA1_M
/// @endcond
		}
		return false;
	}

private:
	template<typename T, typename ... Args>
	static bool AssignTo(std::vector<T> *v, const Args & ... args)
	{
		v->clear();
		try {
			AppendValue(v, args...);
//			std::cerr << "ok" << std::endl;
			return true;
		} catch (const std::exception &) {
//			std::cerr << "exception" << std::endl;
			v->clear();
		}
		return false;
	}

	template<typename T, typename ... Args>
	static bool AppendTo(std::vector<T> *v, const Args & ... args)
	{
		try {
			AppendValue(v, args...);
//			std::cerr << "ok" << std::endl;
			return true;
		} catch (const std::exception &) {
//			std::cerr << "exception" << std::endl;
//			v->clear();
		}
		return false;
	}

	template<typename T>
	static void AppendValue(std::vector<T> *v)
	{
//		std::cerr << "end" << std::endl;
	}

	// T <- same type
	template<typename T, typename Arg, typename... Args>
	static typename std::enable_if<std::is_same<T, Arg>::value>::type
	AppendValue(std::vector<T> *v, const Arg &arg, const Args &... args)
	{
//		std::cerr << "T <- same type " << arg << std::endl;
		v->push_back(arg);
		AppendValue(v, args...);
	}

	// T <- const char *
	template<typename T, typename... Args>
	static void
	AppendValue(std::vector<T> *v, const char *arg, const Args &... args)
	{
//		std::cerr << "T <- const char * " << arg << std::endl;
		AppendValue(v, string(arg), args...);
	}

	// unsigned <- string
	template<typename T, typename... Args>
	static typename std::enable_if<std::is_unsigned<T>::value>::type
	AppendValue(std::vector<T> *v, const string &arg, const Args &... args)
	{
//		std::cerr << "unsigned <- string " << arg << std::endl;
		uint64_t num = std::stoull(arg, 0, 0);
//		if (num > static_cast<uint64_t>(std::numeric_limits<T>::max()))
		if (sizeof(T) == 1 && (num > UINT8_MAX) ||
			sizeof(T) == 2 && (num > UINT16_MAX) ||
			sizeof(T) == 4 && (num > UINT32_MAX))
			throw std::out_of_range("");
		v->push_back(static_cast<T>(num));
		AppendValue(v, args...);
	}

	// signed <- string
	template<typename T, typename... Args>
	static typename std::enable_if<std::is_signed<T>::value && !std::is_floating_point<T>::value>::type
	AppendValue(std::vector<T> *v, const string &arg, const Args &... args)
	{
//		std::cerr << "signed <- string " << arg << std::endl;
		int64_t num = std::stoll(arg, 0, 0);
//		if (num < static_cast<int64_t>(std::numeric_limits<T>::min()) || num > static_cast<int64_t>(std::numeric_limits<T>::max()))
		if (sizeof(T) == 1 && (num < INT8_MIN  || num > INT8_MAX) ||
			sizeof(T) == 2 && (num < INT16_MIN || num > INT16_MAX) ||
			sizeof(T) == 4 && (num < INT32_MIN || num > INT32_MAX))
			throw std::out_of_range("");
		v->push_back(static_cast<T>(num));
		AppendValue(v, args...);
	}

	// float <- string
	template<typename T, typename... Args>
	static typename std::enable_if<std::is_floating_point<T>::value>::type
	AppendValue(std::vector<T> *v, const string &arg, const Args &... args)
	{
//		std::cerr << "float <- string " << arg << std::endl;
		v->push_back(static_cast<T>(std::stod(arg, 0)));
		AppendValue(v, args...);
	}

	// string <- arithmetic
	template<typename T, typename Arg, typename... Args>
	static typename std::enable_if<std::is_same<T, string>::value && std::is_arithmetic<Arg>::value>::type
	AppendValue(std::vector<T> *v, const Arg &arg, const Args &... args)
	{
//		std::cerr << "string <- arithmetic " << arg << std::endl;
		std::ostringstream out;
#ifdef ZET_SCHEMA1_HAS_DEFAULTFLOAT
		out << std::defaultfloat << arg;
#else
		out << arg;
#endif
		v->push_back(out.str());
		AppendValue(v, args...);
	}

	// arithmetic <- another arithmetic
	template<typename T, typename Arg, typename... Args>
	static typename std::enable_if<!std::is_same<T, Arg>::value && std::is_arithmetic<T>::value && std::is_arithmetic<Arg>::value>::type
	AppendValue(std::vector<T> *v, const Arg &arg, const Args &... args)
	{
//		std::cerr << "arithmetic <- another arithmetic " << arg << std::endl;
		v->push_back(static_cast<T>(arg));
		AppendValue(v, args...);
	}

	// T <- std::vector<Arg>
	template<typename T, typename Arg, typename... Args>
	static void
	AppendValue(std::vector<T> *v, const std::vector<Arg> &arg, const Args &... args)
	{
		for (const auto &a : arg)
			AppendValue(v, a);
		AppendValue(v, args...);
	}
};

/// @ingroup model
/// @brief Информация об одном свойстве.
class Property
{
	friend class Reader;
	friend class TextReader;
	friend class Editor;

private:
	PropertyId id;
	BaseVector value;

public:
	Property() : id(PropertyId::UNDEFINED) {}

	Property(PropertyId property_id, BaseType field_type) : id(PropertyId::UNDEFINED) { SetId(property_id, field_type); }

	PropertyId GetId() const { return this->id; }
	PropertyType GetType() const;

	const BaseVector &GetValue() const { return this->value; }
	BaseType GetValueType() const { return this->value.GetType(); }
	size_t GetValueCount() const { return this->value.GetCount(); }
	string GetValueString(size_t index = 0) const { return this->value.GetString(index); }
	uint32_t GetValueU32() const;

	bool IsValid() const;

private:
	void SetId(PropertyId property_id, BaseType field_type);
	void Reset();
};

/// @ingroup model
/// @brief Информация об одном поле или метаданные структуры вместе со свойствами.
class Field
{
	friend class Structure;
	friend class Reader;
	friend class TextReader;
	friend class Editor;

public:
	static uint8_t const COUNT_POW2_MAX = 31;
	static uint8_t const COUNT_POW2_FLEXIBLE = 255;

private:
	BaseType type;
	uint8_t count_pow2;
	string name;

	std::map<PropertyId, Property> properties;
	uint32_t size;
	uint32_t offset;
	uint32_t index;

public:
	Field() : type(BaseType::UNDEFINED), count_pow2(0), size(0), offset(0), index(0) {}

	// Field info
	BaseType GetType() const { return this->type; }
	size_t GetTypeSize() const;
	string GetName() const;
	uint32_t GetCountPow2() const { return this->count_pow2; }
	uint32_t GetCount() const { return (this->count_pow2 <= COUNT_POW2_MAX ? (1UL << this->count_pow2) : 0); }
	bool IsBase() const { return (this->type != BaseType::METADATA); }
	bool IsFlexible() const { return (this->count_pow2 == COUNT_POW2_FLEXIBLE); }

	uint32_t GetSize() const { return this->size; }
	uint32_t GetOffset() const { return this->offset; }
	uint32_t GetIndex() const { return this->index; }

	bool ReadValue(const void *header, size_t size, BaseVector *value) const;
	bool SaveValue(void *header, size_t size, const BaseVector &value) const;
	bool SaveDefaultValue(void *header, size_t size) const;

public:
	// Field properties
	const std::map<PropertyId, Property> &GetProperties() const { return this->properties; }
	bool HasProperty(PropertyId property_id) const { return this->properties.find(property_id) != this->properties.end(); }
	string GetPropertyString(PropertyId property_id, size_t index = 0) const;

	string GetDescription() const { return GetPropertyString(PropertyId::DESCRIPTION); }

private:
	void SetType(BaseType type);
	void SetCountPow2(uint8_t pow2);
};

/// @ingroup model
/// @brief Информация о схеме одной структуры.
class Structure
{
	friend class Reader;
	friend class TextReader;
	friend class Editor;

public:
	struct FlexibleFields {
		size_t count;
		uint32_t size;
		uint32_t alignment;

		FlexibleFields() : count(0), size(0), alignment(1) {}
	};

private:
	Header header;
	string name;

	std::map<PropertyId, Property> properties; ///< Structure global properties

	std::vector<Field> fields; ///< All fields including metadata
	uint32_t base_fields_count; ///< Base fields are fields with type != BaseType::METADATA
	uint32_t size; ///< Size of structure (not including flexible fields)

	FlexibleFields flexible_fields;

public:
	Structure() : base_fields_count(0), size(sizeof(Header) + sizeof(Footer)) {}

	const Header &GetHeader() const { return this->header; }
	uint32_t GetType() const { return this->header.type; }
	string GetName() const;

	const std::map<PropertyId, Property> &GetProperties() const { return this->properties; }
	bool HasProperty(PropertyId property_id) const { return this->properties.find(property_id) != this->properties.end(); }

	const std::vector<Field> &GetFields() const { return this->fields; }
	uint32_t GetBaseFieldsCount() const { return this->base_fields_count; }
	uint32_t GetSize() const { return this->size; }

	const FlexibleFields &GetFlexibleFields() const { return this->flexible_fields; }

private:
	void RealignFields();
};

/// @ingroup model
/// @brief Информация о несколько схемах.
class File
{
	friend class Reader;
	friend class TextReader;
	friend class Editor;

private:
	std::list<Structure> structures;

public:
	File() {}

	const Structure &GetStructure(uint32_t type) const;
	const std::list<Structure> &GetStructures() const { return this->structures; }
};

/// @defgroup readers Чтение схем
/// @brief Чтение схем из различных форматов во внутреннее представление.

/// @defgroup writers Запись схем
/// @brief Запись схем из внутреннего представления в различные форматы.

/// @defgroup text_format Текстовый формат
/// @brief Текстовый формат схемы.

/// @defgroup binary_format Бинарный формат
/// @brief Бинарный формат схемы.

/// @ingroup readers
/// @ingroup binary_format
/// @brief Чтение схемы в бинарном формате и создание ее внутреннего представления.
class Reader
{
public:
	/// @brief Информация об ошибке.
	struct Error
	{
		string message; ///< Error message (english)
		size_t offset; ///< Position from buffer beginning (binary)

		Error() : offset(0) {}
	};

public:
	Reader();
	~Reader();

	///
	bool ReadFromBinary(Structure *structure, const binary &input);

	const Error &GetError() const { return this->error; }

private:
	Error error;

private:
	bool SetError(const string &message, size_t offset)
	{
		this->error.message = message;
		this->error.offset = offset;
		return false;
	}
};

/// @ingroup writers
/// @ingroup binary_format
/// @brief Запись схемы в бинарный формат по ее внутреннему представлению.
class Writer
{
public:
	Writer();
	~Writer();

	bool WriteToBinary(const Structure &structure, binary *output) const;

private:
	bool WriteToBinary(const Field &field, binary *output) const;
	bool WriteToBinary(const Property &prop, binary *output) const;
};

/// @ingroup readers
/// @ingroup text_format
/// @brief Чтение схемы в текстовом формате и создание ее внутреннего представления.
class TextReader
{
public:
	/// @brief Информация об ошибке.
	struct Error
	{
		string message; ///< Сообщение (по-английски)
		size_t pos; ///< Указатель на место ошибки (смещение в байтах относительно начала исходного текста)
		size_t line_pos; ///< Указатель на начало ошибочной строки (смещение в байтах относительно исходного текста)
		size_t row; ///< Номер ошибочной строки, нумерация с нуля
		size_t column; ///< Номер столбца (номер символа в ошибочной строке)
	};

public:
	TextReader();
	~TextReader();

	// Comments will be lost
	bool ReadFromString(File *file, const string &input);

	const Error &GetError() const { return error; }

private:
	// Tokenizer (lexical analyzer) context
	string::const_iterator input_end;
	string::const_iterator input_pos;
	string::const_iterator input_next;
	size_t pos;
	size_t pos_next;
	size_t line_pos;
	size_t row;
	size_t column;
	size_t column_next;
	uint32_t token;
	string lexem;

	union {
		BaseType base_type;
		PropertyId property_id;
	} token_arg;

	uint32_t state;

	Error error;

	File file;
	Structure structure;
	Field field;
	bool field_started;
	Property property;

protected:
	bool NextToken();
	void NextLine();
	void NextChar();
	void PrevChar();

	bool SetState(uint32_t new_state);
	template<typename Arg, typename... Args>
	bool SetError(const Arg &arg, const Args &... args);
	bool SetError();

	bool ParseToken();

	bool FinishStructure();
	bool FinishField();
	bool FinishProperty();
	bool AddPropertyValue();
};

/// @ingroup writers
/// @ingroup text_format
/// @brief Запись схемы в текстовом формате по ее внутреннему представлению.
class TextWriter
{
public:
	TextWriter();
	~TextWriter();

	/// @brief Запись схемы в поток вывода.
	/// @param[in] file Файл схемы.
	/// @param[out] output Поток вывода.
	bool Write(const File &file, std::ostream &output) const;

	/// @brief Запись схемы одной структуры в поток вывода.
	/// @param[in] structure Схема одной структуры.
	/// @param[out] output Поток вывода.
	bool Write(const Structure &structure, std::ostream &output) const;

	/// @brief Запись схемы в текстовую строку.
	/// @param[in] file Файл схемы.
	/// @param[out] output Текстовая строка.
	bool WriteToString(const File &file, string *output) const;

	/// @brief Запись схемы одной структуры в текстовую строку.
	/// @param[in] structure Схема одной структуры.
	/// @param[out] output Текстовая строка.
	bool WriteToString(const Structure &structure, string *output) const;

	/// @brief Настройка отступов в генерируемом тексте.
	/// @param[in] spaces Количество пробелов в одном отступе. Если указан 0, то вместо пробелов будет использоваться символ табуляции \t.
	/// @details Настройка будет влиять на последующие вызовы фунции Write.
	void SetIndent(uint32_t spaces);

private:
	string indent;

	bool Write(const Field &field, std::ostream &output) const;
	bool Write(const Property &prop, std::ostream &output, const std::string &indent) const;
};

/// @ingroup writers
/// @brief Генерация заголовочного файла на языке ANSI C99.
class HeaderWriter
{
public:
	HeaderWriter();
	~HeaderWriter();

	bool Write(const File &file, std::ostream &output) const;
	bool Write(const Structure &structure, std::ostream &output) const;
	bool WriteToString(const File &file, string *output, bool append = false) const;
	bool WriteToString(const Structure &structure, string *output, bool append = false) const;

	void SetIndent(uint32_t spaces);

private:
	string indent;

private:
	void WritePadding(uint32_t size, uint32_t target, bool flexible, std::ostream &output) const;
};

/// @ingroup editors
/// @brief Редактирование внутреннего представления схемы.
class Editor
{
public:
	Editor();
	~Editor();

public: // File
	Structure *AppendStructure(File *file, const Structure &structure) const;
	bool RemoveLastStructure(File *file) const;

public: // Settings
	bool RemoveProperty(Structure *structure, PropertyId property_id) const;

public: // Editing struct properties
	bool SetId(Structure *structure, const StructId &id) const;
	bool SetName(Structure *structure, const string &name) const;
	bool SetType(Structure *structure, uint32_t type) const;

	bool SetTitle(Structure *structure, const string &title) const
	{ return SetProperty(structure, PropertyId::TITLE, title); }

	bool SetDescription(Structure *structure, const string &desc) const
	{ return SetProperty(structure, PropertyId::DESCRIPTION, desc); }

	bool SetHidden(Structure *structure, bool hidden) const
	{ return SetProperty(structure, PropertyId::HIDDEN, static_cast<uint32_t>(hidden)); }

	bool SetReadonly(Structure *structure, bool readonly) const
	{ return SetProperty(structure, PropertyId::READONLY, static_cast<uint32_t>(readonly)); }

public: // Editing fields
	Field *FindFieldByIndex(Structure *structure, size_t index) const;
	Field *AppendField(Structure *structure, const Field &field) const;
	Field *AppendField(Structure *structure, BaseType base_type, const string &name, uint32_t count_pow2 = 0) const;
	bool RemoveLastField(Structure *structure) const;

public: // Editing field properties
	bool SetName(Field *field, const string &name) const;
	bool SetType(Field *field, BaseType type) const;
	bool SetCountPow2(Field *field, uint8_t pow2) const;

	bool RemoveProperty(Field *field, PropertyId property_id) const;

	bool SetDescription(Field *field, const string &desc) const
	{ return SetProperty(field, PropertyId::DESCRIPTION, desc); }

	template<typename Arg>
	bool SetDefault(Field *field, const Arg &arg) const
	{ return SetProperty(field, PropertyId::DEFAULT, arg); }

	bool SetControl(Field *field, ControlId control_id) const
	{ return SetProperty(field, PropertyId::CONTROL, static_cast<uint32_t>(control_id)); }

	template<typename Arg>
	bool SetMinimum(Field *field, const Arg &arg) const
	{ return SetProperty(field, PropertyId::MINIMUM, arg); }

	template<typename Arg>
	bool SetMaximum(Field *field, const Arg &arg) const
	{ return SetProperty(field, PropertyId::MAXIMUM, arg); }

	template<typename... Args>
	bool SetList(Field *field, const Args &... args) const
	{ return SetProperty(field, PropertyId::LIST, args...); }

	template<typename... Args>
	bool SetStringList(Field *field, const Args &... args) const
	{ return SetProperty(field, PropertyId::STRING_LIST, args...); }

	template<typename... Args>
	bool SetExclude(Field *field, const Args &... args) const
	{ return SetProperty(field, PropertyId::EXCLUDE, args...); }

	bool SetHidden(Field *field, bool hidden) const
	{ return SetProperty(field, PropertyId::HIDDEN, static_cast<uint32_t>(hidden)); }

	bool SetReadonly(Field *field, bool readonly) const
	{ return SetProperty(field, PropertyId::READONLY, static_cast<uint32_t>(readonly)); }

	bool SetUserType(Field *field, UserType user_type) const
	{ return SetProperty(field, PropertyId::TYPE, static_cast<uint32_t>(user_type)); }

	template<typename Arg>
	bool SetGt(Field *field, const Arg &arg) const
	{ return SetProperty(field, PropertyId::GT, arg); }

	template<typename Arg>
	bool SetLt(Field *field, const Arg &arg) const
	{ return SetProperty(field, PropertyId::LT, arg); }

private:
	template<typename... Args>
	bool SetProperty(Structure *structure, PropertyId property_id, const Args &... args) const
	{
		if (structure && property_id != PropertyId::UNDEFINED) {
			Property p(property_id, BaseType::METADATA);
			if (p.value.Assign(args...)) {
				structure->properties[p.id] = p;
				return true;
			}
		}
		return false;
	}

	template<typename... Args>
	bool SetProperty(Field *field, PropertyId property_id, const Args &... args) const
	{
		if (field && property_id != PropertyId::UNDEFINED) {
			Property p(property_id, field->GetType());
			if (p.value.Assign(args...)) {
				field->properties[p.id] = p;
				return true;
			}
		}
		return false;
	}
};

/// Вычисление CRC по алгоритму, используемому в формате структур
uint32_t CalcCrc32(uint32_t crc32, const void *buf, size_t len);

/// Вычисление ближайшего к заданному числа, выровненного по base_size
uint32_t GetAlignedOffset(uint32_t offset, size_t base_size);

} // namespace schema1
} // namespace zet

#endif
