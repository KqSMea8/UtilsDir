#ifndef ZET_CONFIGURATOR_SCHEMA_REVISION
#define ZET_CONFIGURATOR_SCHEMA_REVISION 9
#endif

#ifdef _WIN32
// MSVS specific stdlib routines
#define strtoull _strtoui64
#define strtoll _strtoi64
#define snprintf sprintf_s
#endif

#include "zet_schema1.h"

#include <cctype>
#include <cstring>
#include <functional>
#include <memory.h>
#include <iomanip>

#include <set>

namespace zet {
namespace schema1 {

namespace bin {

struct Block
{
	uint16_t size;
	BaseType type;
	uint8_t count_pow2;
};
static_assert(sizeof(Block) == 4, "Invalid packing");

struct Property
{
	uint16_t size;
	PropertyId id;
};
static_assert(sizeof(Property) == 4, "Invalid packing");

template<typename T>
static typename std::enable_if<std::is_arithmetic<T>::value, size_t>::type
Read(const binary &bin, size_t pos, std::vector<T> *data, size_t count)
{
	if (data) {
		size_t actual_count = (bin.size() - pos) / sizeof(T);
		if (actual_count > count)
			actual_count = count;
		size_t size = actual_count * sizeof(T);

		data->resize(count, 0);
		memcpy(data->data(), bin.data() + pos, size);
		return (pos + size);
	}
	return pos;
}

static size_t Read(const binary &bin, size_t pos, size_t end_pos, std::vector<string> *data)
{
	if (data && pos < end_pos) {
		data->clear();

		const auto end = bin.begin() + end_pos;
		auto left = bin.begin() + pos;
		while (true) {
			auto right = std::find(left, end, 0);
			if (right > left)
				data->push_back(string(left, right));
			if (right == end)
				return (right - bin.begin());
			left = right + 1;
		}
	}
	return pos;
}

template<typename T>
static size_t Read(const binary &bin, size_t pos, T *data)
{
	if (data) {
		if (pos + sizeof(T) <= bin.size()) {
			memcpy(data, bin.data() + pos, sizeof(T));
			return pos + sizeof(T);
		}
	}
	return pos;
}

template<typename T>
static void Append(binary *bin, const T &data)
{
	if (bin) {
		auto ptr = reinterpret_cast<const uint8_t *>(std::addressof(data));
		bin->insert(std::end(*bin), ptr, ptr + sizeof(T));
	}
}

static void Append(binary *bin, const binary &data)
{
	if (bin)
		bin->insert(std::end(*bin), data.begin(), data.end());
}

static void Append(binary *bin, const string &str)
{
	if (bin) {
		bin->insert(std::end(*bin), str.begin(), str.end());
		bin->push_back('\0');
	}
}

template<typename T>
static void Append(binary *bin, const std::vector<T> &value)
{
	for (const auto &x : value)
		Append(bin, x);
}

static void Append(binary *bin, const BaseVector &value)
{
	if (bin) {
		switch (value.GetType()) {
		case BaseType::CHAR:
			if (value.STRING())
				Append(bin, *value.STRING());
			break;
#define M(ID, ENUM, SNAME, CTYPE) \
		case BaseType::ENUM: \
			if (value.ENUM()) \
				Append(bin, *value.ENUM()); \
			break;
		ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(M)
#undef M
		}
	}
}

} // namespace bin

namespace crc32 {

static const uint32_t XORMASK = 0xfffffffful;
static const uint32_t RESIDUE = 0x2144df1cul;

static uint32_t Calculate(uint32_t crc, const void *buf, size_t len);

} // namespace crc32

namespace text {

static const uint32_t NOT_UTF8 = 0xFFFF;

static const char MACRO_PREFIX[] = "ZETLAB_";
static const char DEFAULT_INDENT[] = "\t";
static const char OPERATORS[] = ":{}[],=";

/// Token numbers must be higher than max Unicode character 0x10FFFF
#define ZET_SCHEMA1_GENERATE_TOKENS(M) \
	M(0x110000, ERROR,         "error") \
	M(0x110001, END_OF_FILE,   "end of file") \
	M(0x110002, STRUCT,        "struct") \
	M(0x110003, TYPE,          "base type") \
	M(0x110004, PROPERTY,      "property") \
	M(0x110005, USER_PROPERTY, "user property") \
	M(0x110006, IDENTIFIER,    "identifier") \
	M(0x110007, STRING,        "string") \
	M(0x110008, INTEGER,       "integer number") \
	M(0x110009, FLOAT,         "float number") \
	M(0x11000A, BINARY,        "binary string") \
	M(0x11000B, CONSTANT,      "const")

enum Token : uint32_t {
#define M(ID, ENUM, COMMENT) ENUM = ID,
	ZET_SCHEMA1_GENERATE_TOKENS(M)
#undef M
};
static_assert(sizeof(Token) == sizeof(uint32_t), "Invalid Token size");

#define M(ID, ENUM, COMMENT) static_assert(Token::ENUM <= 0xffffff, "Invalid " COMMENT " value");
ZET_SCHEMA1_GENERATE_TOKENS(M)
#undef M

namespace fsm {

#define ZET_SCHEMA1_GENERATE_SYNTAX_STATES(M) \
	M(0,  ERROR,               "error") \
	M(1,  END_OF_FILE,         "end of file") \
	M(2,  START,               "start") \
	M(10, STRUCT,              "struct") \
	M(11, STRUCT_NAME,         "struct name") \
	M(12, STRUCT_ID_START,     "struct id start") \
	M(13, STRUCT_ID,           "struct id") \
	M(14, STRUCT_START,        "struct start") \
	M(20, FIELD,               "field") \
	M(21, FIELD_NAME,          "field name") \
	M(22, FIELD_COUNT_START,   "field count start") \
	M(23, FIELD_COUNT_VALUE,   "field count value") \
	M(24, FIELD_COUNT_END,     "field count end") \
	M(25, FIELD_START,         "field start") \
	M(30, PROPERTY,            "property") \
	M(31, PROPERTY_VALUE,      "property value") \
	M(32, PROPERTY_NEXT,       "property next") \
	M(40, USER_PROPERTY,       "user property") \
	M(41, USER_PROPERTY_ID,    "user property id") \
	M(42, USER_PROPERTY_VALUE, "user property value") \
	M(50, CONSTANT,            "const") \
	M(51, CONSTANT_TYPE,       "const type") \
	M(52, CONSTANT_NAME,       "const name") \
	M(53, CONSTANT_START,      "const start") \
	M(54, CONSTANT_VALUE,      "const value") \
	M(55, CONSTANT_NEXT,       "const next")

enum State : uint32_t {
#define M(ID, ENUM, COMMENT) ENUM = ID,
	ZET_SCHEMA1_GENERATE_SYNTAX_STATES(M)
#undef M
};
static_assert(sizeof(State) == sizeof(uint32_t), "Invalid syntax::State size");

#define M(ID, ENUM, COMMENT) static_assert(State::ENUM <= 255, "Invalid " COMMENT " value");
ZET_SCHEMA1_GENERATE_SYNTAX_STATES(M)
#undef M

#define ZET_SCHEMA1_GENERATE_SYNTAX_TRANSITIONS(TOKEN, LITER) \
	TOKEN(START, END_OF_FILE, END_OF_FILE) \
	TOKEN(START, STRUCT, STRUCT) \
	TOKEN(STRUCT, IDENTIFIER, STRUCT_NAME) \
	TOKEN(STRUCT, PROPERTY, STRUCT_NAME) \
	TOKEN(STRUCT, USER_PROPERTY, STRUCT_NAME) \
	LITER(STRUCT_NAME, ':', STRUCT_ID_START) \
	TOKEN(STRUCT_ID_START, INTEGER, STRUCT_ID) \
	LITER(STRUCT_ID, '{', STRUCT_START) \
	TOKEN(STRUCT_START, TYPE, FIELD) \
	TOKEN(STRUCT_START, CONSTANT, CONSTANT) \
	TOKEN(STRUCT_START, PROPERTY, PROPERTY) \
	TOKEN(STRUCT_START, USER_PROPERTY, USER_PROPERTY) \
	LITER(STRUCT_START, '}', START) \
	TOKEN(FIELD, IDENTIFIER, FIELD_NAME) \
	TOKEN(FIELD, PROPERTY, FIELD_NAME) \
	TOKEN(FIELD, USER_PROPERTY, FIELD_NAME) \
	LITER(FIELD_NAME, '[', FIELD_COUNT_START) \
	LITER(FIELD_NAME, '}', START) \
	LITER(FIELD_NAME, '{', FIELD_START) \
	TOKEN(FIELD_NAME, TYPE, FIELD) \
	TOKEN(FIELD_NAME, CONSTANT, CONSTANT) \
	TOKEN(FIELD_NAME, PROPERTY, PROPERTY) \
	TOKEN(FIELD_NAME, USER_PROPERTY, USER_PROPERTY) \
	TOKEN(FIELD_COUNT_START, INTEGER, FIELD_COUNT_VALUE) \
	LITER(FIELD_COUNT_START, ']', FIELD_COUNT_END) \
	LITER(FIELD_COUNT_VALUE, ']', FIELD_COUNT_END) \
	LITER(FIELD_COUNT_END, '}', START) \
	LITER(FIELD_COUNT_END, '{', FIELD_START) \
	TOKEN(FIELD_COUNT_END, TYPE, FIELD) \
	TOKEN(FIELD_COUNT_END, CONSTANT, CONSTANT) \
	TOKEN(FIELD_COUNT_END, PROPERTY, PROPERTY) \
	TOKEN(FIELD_COUNT_END, USER_PROPERTY, USER_PROPERTY) \
	LITER(FIELD_START, '}', STRUCT_START) \
	TOKEN(FIELD_START, PROPERTY, PROPERTY) \
	TOKEN(FIELD_START, USER_PROPERTY, USER_PROPERTY) \
	TOKEN(PROPERTY, TYPE, FIELD) \
	TOKEN(PROPERTY, CONSTANT, CONSTANT) \
	TOKEN(PROPERTY, STRING, PROPERTY_VALUE) \
	TOKEN(PROPERTY, INTEGER, PROPERTY_VALUE) \
	TOKEN(PROPERTY, FLOAT, PROPERTY_VALUE) \
	TOKEN(PROPERTY, IDENTIFIER, PROPERTY_VALUE) \
	TOKEN(PROPERTY, PROPERTY, PROPERTY) \
	TOKEN(PROPERTY, USER_PROPERTY, USER_PROPERTY) \
	TOKEN(PROPERTY_VALUE, CONSTANT, CONSTANT) \
	LITER(PROPERTY_VALUE, ',', PROPERTY_NEXT) \
	TOKEN(PROPERTY_VALUE, TYPE, FIELD) \
	TOKEN(PROPERTY_VALUE, PROPERTY, PROPERTY) \
	TOKEN(PROPERTY_VALUE, USER_PROPERTY, USER_PROPERTY) \
	TOKEN(PROPERTY_NEXT, STRING, PROPERTY_VALUE) \
	TOKEN(PROPERTY_NEXT, INTEGER, PROPERTY_VALUE) \
	TOKEN(PROPERTY_NEXT, FLOAT, PROPERTY_VALUE) \
	TOKEN(PROPERTY_NEXT, IDENTIFIER, PROPERTY_VALUE) \
	TOKEN(USER_PROPERTY, INTEGER, USER_PROPERTY_ID) \
	TOKEN(USER_PROPERTY_ID, BINARY, USER_PROPERTY_VALUE) \
	TOKEN(USER_PROPERTY_VALUE, CONSTANT, CONSTANT) \
	TOKEN(USER_PROPERTY_VALUE, TYPE, FIELD) \
	TOKEN(USER_PROPERTY_VALUE, PROPERTY, PROPERTY) \
	TOKEN(USER_PROPERTY_VALUE, USER_PROPERTY, USER_PROPERTY) \
	TOKEN(CONSTANT, TYPE, CONSTANT_TYPE) \
	TOKEN(CONSTANT_TYPE, IDENTIFIER, CONSTANT_NAME) \
	TOKEN(CONSTANT_TYPE, PROPERTY, CONSTANT_NAME) \
	TOKEN(CONSTANT_TYPE, USER_PROPERTY, CONSTANT_NAME) \
	LITER(CONSTANT_NAME, '=', CONSTANT_START) \
	TOKEN(CONSTANT_START, INTEGER, CONSTANT_VALUE) \
	TOKEN(CONSTANT_START, FLOAT, CONSTANT_VALUE) \
	LITER(CONSTANT_VALUE, ',', CONSTANT_NEXT) \
	TOKEN(CONSTANT_VALUE, TYPE, FIELD) \
	TOKEN(CONSTANT_VALUE, PROPERTY, PROPERTY) \
	TOKEN(CONSTANT_VALUE, USER_PROPERTY, USER_PROPERTY) \
	TOKEN(CONSTANT_VALUE, CONSTANT, CONSTANT) \
	LITER(CONSTANT_VALUE, '}', STRUCT_START) \
	TOKEN(CONSTANT_NEXT, INTEGER, CONSTANT_VALUE) \
	TOKEN(CONSTANT_NEXT, FLOAT, CONSTANT_VALUE)

} // namespace fsm

static void MakeUpper(string *str);
static uint32_t GetCharUtf8(string::const_iterator &pos, const string::const_iterator &end);
static string GetTokenName(uint32_t token);
static string GetStateName(uint32_t state);
static string GetDoxygen(const Structure &structure);
static string GetDoxygen(const Field &field);
} // namespace text

static ControlId FindControlByName(const string &control_name);
static string GetControlSName(ControlId control_id);
static UserType FindUserTypeByName(const string &user_type_name);
static string GetUserTypeSName(UserType user_type);
static uint32_t GetBaseSize(BaseType base_type);
static string GetBaseSName(BaseType base_type);
static string GetBaseCTypeName(BaseType base_type);
static string GetPropertySName(PropertyId property_id);
static PropertyType GetPropertyType(PropertyId property_id);
static bool MatchPropertyFlags(PropertyId property_id, PropertyFlags flags);
static bool IsPropertyGlobal(const Field &field, PropertyId property_id);
static bool IsValidIdentifier(const string &identifier);

size_t Field::GetTypeSize() const
{
	return GetBaseSize(this->type);
}

string Field::GetName() const
{
	if (IsBase()) {
		if (IsValidIdentifier(this->name))
			return this->name;
		return "_field" + std::to_string(this->index);
	}
	return "";
}

string Field::GetPropertyString(PropertyId property_id, size_t index) const
{
	const auto iter = this->properties.find(property_id);
	if (iter != this->properties.end())
		return iter->second.GetValueString(index);
	return string();
}

void Field::SetType(BaseType type)
{
	if (this->type != type) {
		this->type = type;
		// Recalc size
		SetCountPow2(this->count_pow2);
	}
}

void Field::SetCountPow2(uint8_t pow2)
{
	if (pow2 == COUNT_POW2_FLEXIBLE) {
		this->count_pow2 = COUNT_POW2_FLEXIBLE;
		this->size = 0;
		return;
	}
	if (pow2 > COUNT_POW2_MAX)
		pow2 = 0;
	this->count_pow2 = pow2;
	this->size = GetTypeSize() << this->count_pow2;
//	std::cerr << "type_size " << GetTypeSize() << " pow2 " << this->count_pow2 << " size " << this->size << std::endl;
}

bool Field::ReadValue(const void *header, size_t size, BaseVector *value) const
{
	if (value)
		value->Reset();

	if (!IsBase() || IsFlexible())
		return false;
	if (!header || size < this->offset + GetSize() || !value)
		return false;

	const uint32_t count = (1ul << this->count_pow2);
	const char *src = static_cast<const char *>(header) + this->offset;

	value->SetType(this->type);
	if (this->type == BaseType::CHAR) {
		value->STRING()->push_back(string(src, count));
		return true;
	}
#define ZET_SCHEMA1_M(ID, ENUM, SNAME, CTYPE) \
	if (this->type == BaseType::ENUM) { \
		std::vector<CTYPE> *vec = value->ENUM(); \
		vec->resize(count); \
		for (uint32_t i = 0; i < count; i++) { \
			CTYPE data; \
			memcpy(&data, src + i * sizeof(CTYPE), sizeof(CTYPE)); \
			vec->at(i) = data; \
		} \
		return true; \
	}
	ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(ZET_SCHEMA1_M)
#undef ZET_SCHEMA1_M

	return false;
}

bool Field::SaveValue(void *header, size_t size, const BaseVector &value) const
{
	if (!IsBase() || IsFlexible())
		return false;
	if (!header || size < this->offset + GetSize())
		return false;
	if (value.GetType() != this->type)
		return false;

	bool has_default(false);
	const auto default_property = this->properties.find(PropertyId::DEFAULT);
	if (default_property != this->properties.end() && default_property->second.GetValueType() == this->type)
		has_default = true;

	const uint32_t count = (1ul << this->count_pow2);
	char *dst = static_cast<char *>(header) + this->offset;

	if (this->type == BaseType::CHAR) {
		const std::vector<string> *vec = value.STRING();
		const std::vector<string> *default_vec = (has_default ? default_property->second.GetValue().STRING() : nullptr);
		string data;
		if (vec && !vec->empty())
			data = vec->front();
		else if (default_vec && !default_vec->empty())
			data = default_vec->front();
		data.resize(count - 1, '\0');
		memcpy(dst, data.c_str(), count - 1);
		dst[count - 1] = '\0';
		return true;
	}
#define ZET_SCHEMA1_M(ID, ENUM, SNAME, CTYPE) \
	if (this->type == BaseType::ENUM) { \
		const std::vector<CTYPE> *vec = value.ENUM(); \
		const std::vector<CTYPE> *default_vec = (has_default ? default_property->second.GetValue().ENUM() : nullptr); \
		uint32_t vec_size = (vec ? vec->size() : 0); \
		if (vec_size > count) \
			vec_size = count; \
		for (uint32_t i = 0; i < vec_size; i++) { \
			CTYPE data = vec->at(i); \
			memcpy(dst, &data, sizeof(CTYPE)); \
			dst += sizeof(CTYPE); \
		} \
		if (vec_size < count) { \
			if (default_vec && !default_vec->empty()) { \
				CTYPE data = default_vec->at(0); \
				for (uint32_t i = vec_size; i < count; i++) { \
					memcpy(dst, &data, sizeof(CTYPE)); \
					dst += sizeof(CTYPE); \
				} \
			} else { \
				memset(dst, 0, (count - vec_size) * sizeof(CTYPE)); \
			} \
		} \
		return true; \
	}
	ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(ZET_SCHEMA1_M)
#undef ZET_SCHEMA1_M

	return false;
}

bool Field::SaveDefaultValue(void *header, size_t size) const
{
	return false;
}

string Structure::GetName() const
{
	if (IsValidIdentifier(this->name))
		return this->name;
	std::ostringstream out;
	out << "_structure" << std::hex << this->header.type;
	return out.str();
}

#if 0
void Structure::RealignLastField()
{
	auto last = std::find_if(this->fields.rbegin(), this->fields.rend(),
		[](const Field &f){ return (f.type != BaseType::METADATA); });
	if (last == this->fields.rend())
		return;

	// Multiple flexible fields must be realigned to max alignment
	if (this->flexible_fields.count > 1) {
		this->flexible_fields = FlexibleFields(); // reset

		// Find first flexible
		const auto first_flex = std::find_if(this->fields.begin(), this->fields.end(),
			[](const Field &f){ return f.IsFlexible(); });
		if (first_flex != this->fields.end()) {
			// Find max alignment among all flexible fields
			for (auto flex = first_flex; flex != this->fields.end(); ++flex) {
				if (flex->type != BaseType::METADATA) {
					this->flexible_fields.count++;

					uint32_t base_size = flex->GetTypeSize();
					if (this->flexible_fields.alignment < base_size)
						this->flexible_fields.alignment = base_size;
				}
			}

			// Realign first flexible and move by its base size (not by total size which is 0)
			first_flex->offset = GetAlignedOffset(first_flex->offset, this->flexible_fields.alignment);
			uint32_t first_offset = first_flex->offset;
			uint32_t next_offset = first_offset + first_flex->GetTypeSize();

			// Realign all other flexibles
			for (auto flex = first_flex + 1; flex != this->fields.end(); ++flex) {
				if (flex->type != BaseType::METADATA) {
					flex->offset = GetAlignedOffset(next_offset, this->flexible_fields.alignment);
					next_offset = flex->offset + flex->GetTypeSize(); // base size because total size is 0
				}
			}

			this->flexible_fields.size = GetAlignedOffset(next_offset - first_offset, this->flexible_fields.alignment);
			this->size = GetAlignedOffset(first_offset, sizeof(Footer)) + sizeof(Footer);
			return;
		}
	}

	auto prev = std::find_if(last + 1, this->fields.rend(),
		[](const Field &f){ return (f.type != BaseType::METADATA); });
	if (prev != this->fields.rend())
		last->offset = GetAlignedOffset(prev->offset + prev->GetSize(), last->GetTypeSize());
	else
		last->offset = sizeof(Header);

	if (last->IsFlexible()) {
		this->flexible_fields.count = 1;
		this->flexible_fields.size = last->GetSize();
		this->flexible_fields.alignment = last->GetTypeSize();
		// Do not count flexible field size
		this->size = GetAlignedOffset(last->offset, sizeof(Footer)) + sizeof(Footer);
	} else {
//		std::cerr << "field " << last->GetName() << " index " << last->index << " offset " << last->offset << " size " << last->size << std::endl;
		this->size = GetAlignedOffset(last->offset + last->GetSize(), sizeof(Footer)) + sizeof(Footer);
	}
}
#endif

void Structure::RealignFields()
{
	this->base_fields_count = 0;

	uint32_t last_offset = sizeof(Header);
	auto first_flex = this->fields.end();

	// Realign all base fields
	for (auto field = this->fields.begin(); field != this->fields.end(); ++field) {
		if (field->IsFlexible()) {
			first_flex = field;
			break;
		}
		if (field->IsBase()) {
			field->offset = GetAlignedOffset(last_offset, field->GetTypeSize());
			field->index = this->base_fields_count++;
			last_offset = field->offset + field->GetSize();
//			std::cerr << "field " << field->GetName() << " index " << field->index << " offset " << field->offset << " size " << field->size << std::endl;
		} else {
			field->offset = 0;
			field->index = 0;
		}
	}

	// Realign all flexible fields
	this->flexible_fields = FlexibleFields();
	if (first_flex != this->fields.end()) {
		// Find max alignment to realign first flexible
		this->flexible_fields.alignment = first_flex->GetTypeSize();
		for (auto field = first_flex + 1; field != this->fields.end(); ++field) {
			if (field->IsFlexible()) {
				uint32_t base_size = field->GetTypeSize();
				if (this->flexible_fields.alignment < base_size)
					this->flexible_fields.alignment = base_size;
			}
		}

		// Realign first flexible
		first_flex->offset = GetAlignedOffset(last_offset, this->flexible_fields.alignment);
		first_flex->index = this->base_fields_count++;
		last_offset = first_flex->offset;
		this->flexible_fields.count = 1;

//		std::cerr << "flex field " << field->GetName() << " index " << field->index << " offset " << field->offset << " size " << field->size << std::endl;

		// Realign other flexibles
		uint32_t first_flex_offset = first_flex->offset;
		uint32_t last_flex_offset = first_flex->offset + first_flex->GetTypeSize();
		for (auto field = first_flex + 1; field != this->fields.end(); ++field) {
			if (!field->IsBase()) {
				field->offset = 0;
				field->index = 0;
				continue;
			}
			if (field->IsFlexible()) {
				field->offset = GetAlignedOffset(last_flex_offset, field->GetTypeSize());
				field->index = this->base_fields_count++;
				this->flexible_fields.count++;
				last_flex_offset = field->offset + field->GetTypeSize();
//				std::cerr << "flex field " << field->GetName() << " index " << field->index << " offset " << field->offset << " size " << field->size << std::endl;
			}
		}
		this->flexible_fields.size = GetAlignedOffset(last_flex_offset - first_flex_offset, this->flexible_fields.alignment);
	}

	this->size = GetAlignedOffset(last_offset, sizeof(Footer)) + sizeof(Footer);
//	std::cerr << "total size " << this->size << std::endl << std::endl;
}

Reader::Reader()
{
}

Reader::~Reader()
{
}

bool Reader::ReadFromBinary(Structure *structure, const binary &input)
{
	/// Read header
	size_t pos = 0;

	auto Fail = [this, pos, &structure] (const char *message) -> bool {
		this->error.message = message;
		this->error.offset = pos;
		if (structure)
			*structure = Structure();
		return false;
	};

	if (input.size() < sizeof(Header) + sizeof(Footer))
		return Fail("size too small for header and footer");

	Structure s;
	pos = bin::Read(input, pos, &s.header);
	if (s.header.size > input.size())
		return Fail("structure size is too small");
	if (s.header.size % 8)
		return Fail("structure size has invalid alignment");
	if (!(s.header.type & 0x80000000UL))
		return Fail("structure is not schema");

//	std::cerr <<"structure: size " << s.header.size << " type 0x" << std::hex << s.header.type << std::dec << std::endl;

	uint32_t crc = CalcCrc32(0, input.data(), s.header.size);
	if (crc != crc32::RESIDUE)
		return Fail("checksum mismatch");

	bin::Block block;
	const size_t avail_size = input.size() - sizeof(Footer);
	while (pos + sizeof(block) <= avail_size) {
//		std::cerr << "[" << std::setw(8) << std::setfill('0') << std::hex << pos << std::dec << "]";
		pos = bin::Read(input, pos, &block);
		if (block.size == 0)
			break;
		if (block.size < sizeof(block))
			Fail("invalid block size");

		Field f;
		f.type = block.type;
		f.SetCountPow2(block.count_pow2);

//		std::cerr << " block: size " << block.size << " type " << GetBaseSName(f.type) << " count " << f.GetCount() << std::endl;

		BaseType const_type = BaseType::UNDEFINED;
		Property name_prop;
		binary list_binary;

		size_t block_left = block.size - sizeof(block);
		if (pos + block_left > avail_size)
			Fail("size too small for block");

		size_t next_pos = pos + block_left;

//		std::cerr << "\t\tproperties:";
		bin::Property prop;
		while (block_left >= sizeof(prop)) {
			pos = bin::Read(input, pos, &prop);
//			std::cerr << " " << GetPropertySName(prop.id) << "(" << prop.size << ")";
			if (prop.size == 0)
				break;
			if (prop.size > block_left || prop.size < sizeof(prop))
				Fail("invalid property size");

			block_left -= prop.size;

			Property p(prop.id, block.type);
			size_t value_size = prop.size - sizeof(prop);

			if (p.id == PropertyId::LIST && block.type == BaseType::METADATA) {
				bin::Read(input, pos, &list_binary, value_size);
			} else {
				switch (p.GetValueType()) {
				case BaseType::CHAR:
					bin::Read(input, pos, pos + value_size, p.value.STRING());
					break;
#define M(ID, ENUM, SNAME, CTYPE) \
				case BaseType::ENUM: \
					bin::Read(input, pos, p.value.ENUM(), value_size / sizeof(CTYPE)); \
					break;
				ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(M)
#undef M
				}

				if (p.id == PropertyId::NAME) {
					name_prop = p;
				} else if (p.id == PropertyId::CONSTANT) {
					const_type = static_cast<BaseType>(p.GetValueU32());
					if (const_type >= BaseType::CHAR && const_type <= BaseType::F64)
						f.properties[p.id] = p;
					else
						const_type = BaseType::UNDEFINED;
				} else if (IsPropertyGlobal(f, p.id)) {
					s.properties[p.id] = p;
				} else {
					f.properties[p.id] = p;
				}
			}

			// Go to next property
			pos += value_size;
		}
//		std::cerr << std::endl;

		// Apply name_prop
		if (name_prop.id == PropertyId::NAME) {
			if (IsPropertyGlobal(f, PropertyId::NAME)) {
				s.properties[PropertyId::NAME] = name_prop;
				s.name = name_prop.GetValueString(0);
			} else {
				f.properties[PropertyId::NAME] = name_prop;
				f.name = name_prop.GetValueString(0);
			}
			if (const_type != BaseType::UNDEFINED && f.type == BaseType::METADATA && !list_binary.empty()) {
				Property p(PropertyId::LIST, const_type);
				switch (const_type) {
				case BaseType::CHAR:
					bin::Read(list_binary, 0, list_binary.size(), p.value.STRING());
					break;
#define M(ID, ENUM, SNAME, CTYPE) \
				case BaseType::ENUM: \
					bin::Read(list_binary, 0, p.value.ENUM(), list_binary.size() / sizeof(CTYPE)); \
					break;
				ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(M)
#undef M
				}
				f.properties[PropertyId::LIST] = p;
			}
		}

		if (f.type != BaseType::METADATA) {
			// Add base field
			s.fields.push_back(f);
			s.RealignFields();
		} else if (f.properties.size() > 0) {
			// Add nonempty metadata field
			s.fields.push_back(f);
		}

		// Check and fix pos after block
		if (pos != next_pos) {
			pos = next_pos;
		}
	}

	if (structure)
		*structure = s;
	return true;
}

Writer::Writer()
{
}

Writer::~Writer()
{
}

bool Writer::WriteToBinary(const Structure &structure, binary *output) const
{
	if (output)
		output->clear();

	binary out;
	out.reserve(1024);

	// Encode global properties
	if (structure.GetProperties().size() > 0) {
//		std::cerr << "global" << structure.GetProperties().size() << std::endl;

		binary global_props;
		for (const auto &p : structure.GetProperties())
			WriteToBinary(p.second, &global_props);

		bin::Block bin_global;
		bin_global.size = sizeof(bin_global) + global_props.size();
		bin_global.type = BaseType::METADATA;
		bin_global.count_pow2 = 0;

		bin::Append(&out, bin_global);
		bin::Append(&out, global_props);
	}

	// Encode fields including metadata
	for (const auto &f : structure.GetFields())
		WriteToBinary(f, &out);

	// Align to 4 to satisfy crc32
	uint32_t aligned_size = GetAlignedOffset(out.size(), sizeof(Footer));
	if (aligned_size > out.size())
		bin::Append(&out, binary(aligned_size - out.size(), 0x00));

	Header header;
	header.size = sizeof(Header) + out.size() + sizeof(Footer);
	header.type = structure.GetHeader().type | Header::TYPE_MASK;

	auto header_ptr = reinterpret_cast<const uint8_t *>(std::addressof(header));
	out.insert(std::begin(out), header_ptr, header_ptr + sizeof(header));

	//Footer footer;
	bin::Append(&out, header.size);
	uint32_t crc = CalcCrc32(0, out.data(), out.size());
	bin::Append(&out, crc);

	if (output)
		*output = out;
	return true;
}

bool Writer::WriteToBinary(const Field &field, binary *output) const
{
	if (output) {
		// Encode properties first to know actual size
		binary field_props;
		for (const auto &p : field.GetProperties())
			WriteToBinary(p.second, &field_props);

		bin::Block bin_block;
		bin_block.size = sizeof(bin_block) + field_props.size();
		bin_block.type = field.GetType();
		bin_block.count_pow2 = field.GetCountPow2();

		bin::Append(output, bin_block);
		bin::Append(output, field_props);
	}
	return true;
}

bool Writer::WriteToBinary(const Property &prop, binary *output) const
{
	if (output) {
		// Encode value first to know actual size
		binary prop_value;
		bin::Append(&prop_value, prop.GetValue());

		bin::Property bin_prop;
		bin_prop.size = sizeof(bin_prop) + prop_value.size();
		bin_prop.id = prop.GetId();

//		std::cerr  << "prop " << static_cast<uint16_t>(prop.GetId()) << " " << bin_prop.size << std::endl;
		bin::Append(output, bin_prop);
		bin::Append(output, prop_value);
	}
	return true;
}

TextReader::TextReader()
{
	this->pos = 0;
	this->pos_next = 0;
	this->line_pos = 0;
	this->row = 0;
	this->column = 0;
	this->column_next = 0;

	this->token = text::NOT_UTF8;
	this->lexem.clear();

	this->error.row = 0;
	this->error.column = 0;
}

TextReader::~TextReader()
{
}

bool TextReader::ReadFromString(File *file, const string &input)
{
	this->input_end = input.end();
	this->input_pos = input.begin();
	this->input_next = this->input_pos;

	this->pos = 0;
	this->pos_next = 0;
	this->line_pos = 0;
	this->row = 0;
	this->column = 0;
	this->column_next = 0;

	this->error.message.clear();
	this->error.pos = 0;
	this->error.line_pos = 0;
	this->error.row = 0;
	this->error.column = 0;

	this->token = text::NOT_UTF8;
	this->lexem.clear();

	this->state = text::fsm::START;

	this->file = (file ? *file : File());
	this->structure = Structure();
	this->field = Field();
	this->property = Property();
	this->field_started = false;

	while (ParseToken()) {
//		std::cerr << "(" << (this->row + 1) << "," << (this->column + 1) << "): <" << this->lexem << "> " << text::GetTokenName(this->token) << "\n";
	}

//	if (this->token != text::END_OF_FILE) {
	//	std::cerr << "ERROR at (" << this->pos.row << "," << source.prev_pos << "): " << text::GetTokenName(source.token) << std::endl;
	//}
	if (this->state != text::fsm::END_OF_FILE)
		this->file = File();

	if (file)
		*file = this->file;
	if (this->state == text::fsm::END_OF_FILE)
		return true;

	if (this->error.message.empty()) {
		this->error.message = "unknown error";
	}
	return false;
}

bool TextReader::NextToken()
{
	this->column = this->column_next;
	this->lexem.clear();

	auto SetToken = [this] (uint32_t new_token) -> bool {
		this->token = new_token;
		return true;//(new_token != text::END_OF_FILE);
	};

	auto IsEol = [this] () -> bool {
		if (this->token == '\n') {
			this->line_pos = this->pos_next;
			this->row++;
			this->column = 0;
			this->column_next = 0;
			return true;
		}
		return false;
	};

	auto SkipTillEol = [this, IsEol] () {
		while (true) {
			NextChar();
			if (IsEol())
				break;
			if (this->token == 0) {
				PrevChar();
				break;
			}
			this->column++;
		}
	};

	while (true) {
		NextChar();
		if (this->token == '\0') {
			return SetToken(text::END_OF_FILE);
		}
		if (this->token == text::NOT_UTF8) {
			return SetError("unknown character");
		}
		if (this->token >= 0x80) {
			// We expect latin-1 encoding, all other characters are permitted only in strings and comments
			return SetError("invalid character");
		}
		if (IsEol())
			continue;

		if (std::isspace(this->token)) {
			// Skip whitespaces
			this->column++;
			continue;
		}
		if (std::strchr(text::OPERATORS, this->token)) {
			// Operator lexem is token itself
			this->lexem = this->token;
			return true;
		}
		if (std::isalpha(this->token) || this->token == '_') {
			// Identifier
			auto start = this->input_pos;
			do NextChar(); while (this->token < 0x80 && (std::isalnum(this->token) || this->token == '_'));
			this->lexem.assign(start, this->input_pos);
			PrevChar();

			if (this->lexem == "struct")
				return SetToken(text::STRUCT);
			if (this->lexem == "const")
				return SetToken(text::CONSTANT);
			if (this->lexem == "property")
				return SetToken(text::USER_PROPERTY);

#define M(ID, ENUM, SNAME, CTYPE) \
			if (this->lexem == SNAME) { \
				this->token_arg.base_type = BaseType::ENUM; \
				return SetToken(text::TYPE); \
			}
			ZET_SCHEMA1_GENERATE_BASE_TYPES(M)
#undef M

			// Backward compatibility for base types
			if (this->lexem == "float") {
				this->token_arg.base_type = BaseType::F32;
				return SetToken(text::TYPE);
			}
			if (this->lexem == "double") {
				this->token_arg.base_type = BaseType::F64;
				return SetToken(text::TYPE);
			}

#define M(ID, ENUM, SNAME, FLAGS, OPTION_TYPE) \
			if (this->lexem == SNAME) { \
				this->token_arg.property_id = PropertyId::ENUM; \
				return SetToken(text::PROPERTY); \
			}
			ZET_SCHEMA1_GENERATE_PROPERTIES(M)
#undef M

			// Backward compatibility for properties
			if (this->lexem == "end_group") {
				this->token_arg.property_id = PropertyId::GROUP;
				return SetToken(text::PROPERTY);
			}

			return SetToken(text::IDENTIFIER);
		}
		if (this->token == '\"') {
			// Double-quoted string
			while (true) {
				NextChar();
				if (this->token == text::NOT_UTF8 || this->token == '\0')
					return SetError("expected double quotes");
				if (this->token == '\"')
					break;
				if (this->token == '\\') {
					NextChar();
					switch (this->token) {
					case 'n': this->lexem += '\n'; break;
					case 't': this->lexem += '\t'; break;
					case 'r': this->lexem += '\r'; break;
					case '\"': this->lexem += '\"'; break;
					case '\\': this->lexem += '\\'; break;
					default:
						this->lexem.clear();
						return SetError("invalid escape sequence");
					}
				} else {
					// Add utf8 char (not always 1 byte)
					this->lexem.append(this->input_pos, this->input_next);
				}
			}
			return SetToken(text::STRING);
		}
		if (this->token == '<') {
			// Binary safe string <(xx)+>
			auto start = this->input_next;
			while (true) {
				NextChar();
				if (this->token == text::NOT_UTF8 || this->token == '\0')
					return SetError("expected '>'");
				if (this->token == '>')
					break;
				if (this->token >= 0x80 || !std::isxdigit(this->token))
					return SetError("invalid binary string");
			}
			this->lexem.assign(start, this->input_pos);
			if (this->lexem.empty() || (this->lexem.size() % 2))
				return SetError("invalid binary string");
			return SetToken(text::BINARY);
		}
		if (std::isdigit(this->token) || this->token == '-' || this->token == '+') {
			auto start = (this->token == '+') ? this->input_next : this->input_pos;

			size_t digits = 0;
			uint32_t first_token = this->token;

			NextChar();
			if (this->token < 0x80 && std::isdigit(first_token))
				digits++;
			else
				first_token = this->token;

			while (this->token < 0x80 && (std::isdigit(this->token) || this->token == '\'')) {
				digits++;
				NextChar();
			}

			if (digits == 1 && first_token == '0' && this->token == 'x') {
				// hex: /0x[\da-fA-F]+/
				NextChar();
				digits = 0;
				while (this->token < 0x80 && (std::isxdigit(this->token) || this->token == '\'')) {
					digits++;
					NextChar();
				}
				this->token = text::INTEGER;
			} else if (this->token == '.') {
				// Floating point: /[+-]?\d+
				NextChar();
				while (this->token < 0x80 && (std::isdigit(this->token) || this->token == '\''))
					NextChar();
				if (this->token == 'e' || this->token == 'E') {
					NextChar();
					if (this->token == '+' || this->token == '-')
						NextChar();
					digits = 0;
					while (this->token < 0x80 && (std::isdigit(this->token) || this->token == '\'')) {
						digits++;
						NextChar();
					}
				}
				this->token = text::FLOAT;
			} else if (this->token == 'e' || this->token == 'E') {
				NextChar();
				if (this->token == '+' || this->token == '-')
					NextChar();
				digits = 0;
				while (this->token < 0x80 && (std::isdigit(this->token) || this->token == '\'')) {
					digits++;
					NextChar();
				}
				this->token = text::FLOAT;
			} else {
				this->token = text::INTEGER;
			}
			if (digits == 0)
				return SetError("invalid number");
			this->lexem.assign(start, this->input_pos);
			PrevChar();
			return true;
		}
		if (this->token == '#') {
			// bash style comment
			SkipTillEol();
			continue;
		}
		if (this->token == '/') {
			NextChar();
			if (this->token == '/') {
				// C++ style comment
				SkipTillEol();
				continue;
			}
			if (this->token == '*') {
				// ANSI C style comment
				while (true) {
					NextChar();
					if (this->token == '\0') {
						PrevChar();
						break;
					}
					if (this->token == '\n') {
						this->line_pos = this->pos_next;
						this->row++;
						this->column = 0;
						this->column_next = 0;
						continue;
					}
					if (this->token == '*') {
						NextChar();
						if (this->token == '/')
							break;
					}
					this->column++;
				}
				// Skip comment
				continue;
			}
		}

		return SetError("unknown token");
	}
}

void TextReader::NextChar()
{
	this->input_pos = this->input_next;
	this->pos = this->pos_next;
	this->token = text::GetCharUtf8(this->input_next, this->input_end);
	size_t diff = this->input_next - this->input_pos;
	this->pos_next += diff;
	this->column_next++;
}

void TextReader::PrevChar()
{
	size_t diff = this->input_next - this->input_pos;
	if (this->pos_next >= diff)
		this->pos_next -= diff;
	if (this->column_next > 0)
		this->column_next--;
	this->input_next = this->input_pos;
}

bool TextReader::SetState(uint32_t new_state)
{
//	std::cerr << "[" << text::GetStateName(this->state) << "] " << text::GetTokenName(this->token) << " \"" << this->lexem << "\" -> [" << text::GetStateName(new_state) << "]" << std::endl;
	typedef text::fsm::State State;

	BaseVector value;

	this->state = new_state;
	switch (new_state) {
	case State::ERROR:
	case State::END_OF_FILE:
		return false;
	case State::START:
		return FinishStructure();
	case State::STRUCT_NAME:
		if (FinishStructure()) {
			this->structure.name = this->lexem;
			this->structure.header.size = sizeof(Header) + sizeof(Footer);
			this->property.SetId(PropertyId::NAME, BaseType::METADATA);
			this->property.value.Assign(this->structure.name);
			return FinishProperty();
		}
		return false;
	case State::STRUCT_ID:
		value.SetType(BaseType::U32);
		if (value.Assign(this->lexem)) {
			uint32_t type = value.U32()->at(0);
			if (!(type & Header::TYPE_MASK)) {
				this->structure.header.type = type | Header::TYPE_MASK;
//				std::cerr << "read struct " << this->structure.name << ": 0x" << std::hex << this->structure.header.type << std::dec << std::endl;
				return true;
			}
		}
		return SetError("invalid struct id");
	case State::STRUCT_START:
		return FinishField();
	case State::FIELD:
		if (!FinishField())
			return false;
		this->field.type = this->token_arg.base_type;
		this->field.SetCountPow2(0);
		return true;
	case State::FIELD_NAME:
		if (!IsValidIdentifier(this->lexem))
			return SetError("invalid field name");

		for (const auto &f : this->structure.fields) {
			if (f.name == this->lexem)
				return SetError("duplicate field");
		}
		this->field.name = this->lexem;
		this->property.SetId(PropertyId::NAME, BaseType::CHAR);
		this->property.value.Assign(this->field.name);
		return FinishProperty();
	case State::FIELD_COUNT_START:
		this->field.SetCountPow2(Field::COUNT_POW2_FLEXIBLE);
		return true;
	case State::FIELD_COUNT_VALUE:
		value.SetType(BaseType::U32);
		if (value.Assign(this->lexem)) {
			uint32_t required_count = value.U32()->at(0);
			uint8_t pow2 = 0;
			while (true) {
				uint64_t count = (1ull << pow2);
				if (count == required_count) {
					this->field.SetCountPow2(pow2);
					return true;
				}
				if (count > required_count)
					return SetError("field elements count is not a power of 2");
				pow2++;
			}
		}
		return SetError("invalid field array size");
	case State::FIELD_COUNT_END:
		return true;
	case State::FIELD_START:
		this->field_started = true;
		return true;
	case State::PROPERTY:
		if (!FinishProperty())
			return false;
		if (!this->field_started) {
			if (!FinishField())
				return false;
		}

		if (this->field.type == BaseType::UNDEFINED)
			this->field.type = BaseType::METADATA;

		if (this->token_arg.property_id == PropertyId::NAME) {
			this->lexem.clear();
			return SetError("property name cannot be defined explicitly");
		}

		if (IsPropertyGlobal(this->field, this->token_arg.property_id)) {
			if (this->structure.HasProperty(this->token_arg.property_id))
				return SetError("duplicate global property");
		} else {
			if (this->field.HasProperty(this->token_arg.property_id)) {
				if (this->token_arg.property_id != PropertyId::GROUP)
					return SetError("duplicate property");
			}
		}

		if (this->field.type == BaseType::METADATA) {
			if (!MatchPropertyFlags(this->token_arg.property_id, PropertyFlags::META)) {
				this->lexem.clear();
				return SetError("property ", GetPropertySName(this->token_arg.property_id), " cannot be placed inside struct");
			}
		} else {
			if (!MatchPropertyFlags(this->token_arg.property_id, PropertyFlags::BASE)) {
				this->lexem.clear();
				return SetError("property ", GetPropertySName(this->token_arg.property_id), " cannot be placed inside field");
			}
		}

		this->property.SetId(this->token_arg.property_id, this->field.type);
		return true;

	case State::PROPERTY_VALUE:
		return AddPropertyValue();

	case State::USER_PROPERTY:
		return FinishProperty();

	case State::USER_PROPERTY_ID:
		value.SetType(BaseType::U16);
		if (value.Assign(this->lexem)) {
			PropertyId property_id = static_cast<PropertyId>(value.U16()->at(0));
			this->property.id = property_id;
			this->property.value.SetType(BaseType::U8);
			return true;
		}
		return SetError("invalid property id");

	case State::USER_PROPERTY_VALUE:
		if ((this->lexem.size() % 2) == 0 && this->property.value.U8()) {
			binary *value = this->property.value.U8();
			auto iter = this->lexem.begin();
			while (iter != this->lexem.end()) {
				value->push_back(static_cast<uint8_t>(std::stoul(string(iter, iter + 2), 0, 16)));
				iter += 2;
			}
			return true;
		}
		return SetError("invalid binary string");

	case State::CONSTANT:
		return FinishField();

	case State::CONSTANT_TYPE:
		if (!FinishField())
			return false;

		if (this->token_arg.base_type == BaseType::CHAR)
			return SetError("invalid const type");

		this->field.type = BaseType::METADATA;
		this->property.SetId(PropertyId::CONSTANT, BaseType::U32);
		this->property.value.Assign(static_cast<uint32_t>(this->token_arg.base_type));
		return FinishProperty();
	case State::CONSTANT_NAME:
		this->property.SetId(PropertyId::NAME, BaseType::METADATA);
		this->property.value.Assign(this->lexem);
		return FinishProperty();
	case State::CONSTANT_START:
		this->property.SetId(PropertyId::LIST, static_cast<BaseType>(this->field.properties[PropertyId::CONSTANT].GetValueU32()));
		return true;
	case State::CONSTANT_VALUE:
		return AddPropertyValue();
	}
	return true;
}

template<typename Arg, typename... Args>
bool TextReader::SetError(const Arg &arg, const Args &... args)
{
	std::ostringstream out;
	out << arg;
	this->error.message.append(out.str());
	return SetError(args...);
}

bool TextReader::SetError()
{
	if (!this->error.message.empty() && !this->lexem.empty())
		this->error.message += " \"" + this->lexem + "\"";
	this->error.pos = this->pos;
	this->error.line_pos = this->line_pos;
	this->error.row = this->row;
	this->error.column = this->column;
	this->token = text::Token::ERROR;
	return SetState(text::fsm::State::ERROR);
}

bool TextReader::ParseToken()
{
	if (!NextToken())
		return false;

#define MAKE_STATE_EVENT(STATE, EVENT) ( ((STATE) << 24) | (EVENT & 0x00ffffff) )
#define STATE_LITER(STATE, EVENT) MAKE_STATE_EVENT(text::fsm::State::STATE, EVENT)
#define STATE_TOKEN(STATE, TOKEN) MAKE_STATE_EVENT(text::fsm::State::STATE, text::Token::TOKEN)

	switch (MAKE_STATE_EVENT(this->state, this->token)) {

#define LITER(FROM, EVENT, TO) \
	case STATE_LITER(FROM, EVENT): \
		return SetState(text::fsm::State::TO);
#define TOKEN(FROM, TOKEN, TO) LITER(FROM, text::Token::TOKEN, TO)
	ZET_SCHEMA1_GENERATE_SYNTAX_TRANSITIONS(TOKEN, LITER)
#undef TOKEN
#undef LITER

	case STATE_LITER(PROPERTY, '}'):
	case STATE_LITER(PROPERTY_VALUE, '}'):
	case STATE_LITER(USER_PROPERTY_VALUE, '}'):
		if (this->field.type == BaseType::UNDEFINED || this->field.type == BaseType::METADATA)
			return SetState(text::fsm::State::START);
		return SetState(text::fsm::State::STRUCT_START);
	case STATE_TOKEN(STRUCT_START, IDENTIFIER):
	case STATE_TOKEN(FIELD_START, IDENTIFIER):
	case STATE_TOKEN(FIELD_COUNT_END, IDENTIFIER):
	case STATE_TOKEN(PROPERTY_VALUE, IDENTIFIER):
		return SetError("unknown option");
	}
#undef STATE_TOKEN
#undef STATE_LITER
#undef MAKE_STATE_EVENT

	switch (this->state) {
#define EXPECTED(STATE, WHAT) \
	case text::fsm::State::STATE: \
		this->lexem.clear(); \
		return SetError("expected " WHAT);
	EXPECTED(START,          "struct")
	EXPECTED(STRUCT_START,   "field type or property name")
	EXPECTED(PROPERTY,       "property value")
	EXPECTED(PROPERTY_NEXT,  "next property value")
	EXPECTED(CONSTANT,       "const type")
	EXPECTED(CONSTANT_TYPE,  "const name")
	EXPECTED(CONSTANT_START, "const value")
	EXPECTED(CONSTANT_NEXT,  "next const value")
#undef EXPECTED
	}
	return SetError("unexpected token");
}

bool TextReader::FinishStructure()
{
	if (!FinishField())
		return false;
	if (this->structure.header.type == 0)
		return true;

#if 0
	DBG("}\n");
	if (!check_flexible_block())
		return false;

	current_table.size += ZET_STRUCTOR_FOOTER_SIZE;
	if (current_table.size % ZET_STRUCTOR_FOOTER_SIZE) {
		syntax_error("struct \"%s\" has size %lu"
				" that is not aligned to %lu",
				current_table.name.c_str(),
				current_table.size, ZET_STRUCTOR_FOOTER_SIZE);
		return false;
	}
	if (this->structure.fields.empty()) {
		this->lexem.clear();
		return SetError("struct ", this->structure.name, " is empty");
	}
#endif

	this->file.structures.push_back(this->structure);
	this->structure = Structure();
	return true;
}

bool TextReader::FinishField()
{
	if (!FinishProperty())
		return false;
	if (this->field.type == BaseType::UNDEFINED)
		return true;

//	std::cerr << "Add field " << GetBaseSName(this->field.type) << " size " << this->field.GetSize() << std::endl;
	if (this->field.type == BaseType::METADATA) {
		if (this->field.HasProperty(PropertyId::CONSTANT)) {
			if (!this->field.HasProperty(PropertyId::NAME)) {
				this->lexem.clear();
				return SetError("constant name is not defined");
			}
			if (!this->field.HasProperty(PropertyId::LIST)) {
				this->lexem.clear();
				return SetError("constant values are not defined");
			}
		}
		size_t prop_count = this->field.properties.size();
		if (this->field.HasProperty(PropertyId::NAME))
			prop_count--;
		if (prop_count > 0)
			this->structure.fields.push_back(this->field);
	} else {
		this->structure.fields.push_back(this->field);
		this->structure.RealignFields();
	}

	this->field = Field();
	this->field_started = false;
	return true;
#if 0

	DBG("\t}\n");
	if (current_block.type == kBaseTypeNone &&
			has_option(current_block.options, kOptionConst)) {
		// Check constant block
		if (!has_option(current_block.options, kOptionName)) {
			syntax_error("constant requires name");
			return false;
		}
		if (!has_option(current_block.options, kOptionList)) {
			syntax_error("constant requires list of values");
			return false;
		}
	}
	if (current_block.type != kBaseTypeNone &&
			!has_option(current_block.options, kOptionName)) {
		// Automatically insert property name into field block
		add_block_name();
	}

	current_table.blocks.push_back(current_block);
	current_block.type = -1;
	current_block.options.clear();
	return true;
#endif
}

bool TextReader::FinishProperty()
{
	if (this->property.id == PropertyId::UNDEFINED)
		return true;
	if (this->field.type == BaseType::UNDEFINED && this->property.id != PropertyId::NAME) {
		this->lexem.clear();
		return SetError("property ", GetPropertySName(this->property.id), " is outside of structure");
	}

	// Check values count
	std::string error;
	size_t count = this->property.GetValueCount();
	switch (this->property.GetType()) {
	case PropertyType::NONE:
		if (count != 0)
			error = "does not require a value";
		break;
	case PropertyType::IDENTIFIER:
	case PropertyType::STRING:
	case PropertyType::BASE:
	case PropertyType::BOOL:
	case PropertyType::BASE_TYPE:
	case PropertyType::USER_TYPE:
	case PropertyType::CONTROL:
		if (count != 1) {
			if (count != 0 || this->property.id != PropertyId::GROUP)
				error = "requires single value";
		}
		break;
	case PropertyType::STRING_LIST:
	case PropertyType::BASE_LIST:
		if (count == 0)
			error = "requires at least one value";
		break;
	}
	if (!error.empty()) {
		this->lexem.clear();
		return SetError("property ", GetPropertySName(this->property.id), " ", error);
	}

	if (IsPropertyGlobal(this->field, this->property.id)) {
		this->structure.properties.insert(std::make_pair(this->property.id, this->property));
	} else {
		this->field.properties.insert(std::make_pair(this->property.id, this->property));
	}

	this->property = Property();
	return true;
}

bool TextReader::AddPropertyValue()
{
	PropertyType prop_type = GetPropertyType(this->property.id);
	string error;

	// Check count
	size_t count = this->property.GetValueCount();
	switch (prop_type) {
	case PropertyType::NONE:
		error = "cannot have values";
		break;
	case PropertyType::STRING_LIST:
	case PropertyType::BASE_LIST:
		// Count is not restricted
		break;
	default:
		if (count != 0)
			error = "got too many values";
		break;
	}
	if (!error.empty()) {
		this->lexem.clear();
		return SetError("property ", GetPropertySName(this->property.id), " ", error);
	}

	// Check value
	switch (prop_type) {
	case PropertyType::NONE:
		break;
	case PropertyType::IDENTIFIER:
		if (this->token == text::Token::IDENTIFIER)
			return this->property.value.Append(this->lexem);
		break;
	case PropertyType::STRING:
	case PropertyType::STRING_LIST:
		if (this->token == text::Token::STRING)
			return this->property.value.Append(this->lexem);
		break;
	case PropertyType::BASE:
	case PropertyType::BASE_LIST:
		if (this->property.value.Append(this->lexem))
			return true;
		break;
	case PropertyType::BOOL:
		if (this->lexem == "true")
			return this->property.value.Assign(true);
		if (this->lexem == "false")
			return this->property.value.Assign(false);
		break;
	case PropertyType::USER_TYPE:
		if (this->token == text::Token::INTEGER) {
			if (this->property.value.Assign(this->lexem))
				return true;
		} else {
			UserType user_type = FindUserTypeByName(this->lexem);
			if (user_type != UserType::UNDEFINED)
				return this->property.value.Assign(static_cast<uint32_t>(user_type));
		}
		break;
	case PropertyType::CONTROL:
		if (this->token == text::Token::INTEGER) {
			if (this->property.value.Assign(this->lexem))
				return true;
		} else {
			ControlId control = FindControlByName(this->lexem);
			if (control != ControlId::UNDEFINED)
				return this->property.value.Assign(static_cast<uint32_t>(control));
		}
		break;
	}
	return SetError("property ", GetPropertySName(this->property.id), " got invalid value");
}

TextWriter::TextWriter() : indent(text::DEFAULT_INDENT)
{
}

TextWriter::~TextWriter()
{
}

bool TextWriter::Write(const File &file, std::ostream &output) const
{
	for (const auto &s : file.GetStructures())
	{
		string temp;
		if (!Write(s, output))
			return false;
	}
	return true;
}

bool TextWriter::Write(const Structure &structure, std::ostream &output) const
{
	uint32_t type = structure.GetHeader().type & ~(Header::TYPE_MASK);
	output <<
		/**/"struct " << structure.GetName() << ": 0x" << std::hex << type << std::dec << " {\n";

	bool has_props(false);
	for (const auto &p : structure.GetProperties()) {
		if (p.second.GetId() != PropertyId::NAME)
			has_props = Write(p.second, output, this->indent) || has_props;
	}
	if (has_props) {
		output <<
			/**/"\n";
	}

	for (const auto &f : structure.GetFields()) {
		if (f.GetType() != BaseType::UNDEFINED)
			Write(f, output);
	} // for f

	output <<
		/**/"}\n" <<
		/**/"\n";
	return true;
}

bool TextWriter::Write(const Field &field, std::ostream &output) const
{
	const auto &properties = field.GetProperties();
	bool base_field = (field.GetType() != BaseType::METADATA);
	bool curly_brace = false;
	std::string prop_indent(this->indent);

	std::set<PropertyId> skip_list;
	skip_list.insert(PropertyId::NAME);

//		std::cerr << "field type " << GetBaseSName(f.GetType()) << std::endl;

	if (base_field) {
		output <<
			/**/this->indent << GetBaseSName(field.GetType()) << " " << field.GetName();
		if (field.GetCountPow2() > 0) {
			uint32_t count = field.GetCount();
			if (count > 0)
				output << "[" << count << "]";
			else
				output << "[]";
		}
		prop_indent += this->indent;
	} else {
//			if (f.GetProperties().empty())
//				continue;

		if (field.HasProperty(PropertyId::CONSTANT)) {
			// Write constant info
			skip_list.insert(PropertyId::CONSTANT);
//				skip_list.insert(PropertyId::NAME);
			skip_list.insert(PropertyId::LIST);
			if (field.HasProperty(PropertyId::NAME) && field.HasProperty(PropertyId::LIST)) {
				const auto &type_prop = properties.at(PropertyId::CONSTANT);
				const auto &name_prop = properties.at(PropertyId::NAME);
				const auto &list_prop = properties.at(PropertyId::LIST);
				output <<
					/**/this->indent << "const " << GetBaseSName(static_cast<BaseType>(type_prop.GetValueU32()));
				output << " " << name_prop.GetValueString(0) << " =";
				for (uint32_t i = 0; i < list_prop.GetValueCount(); ++i) {
					if (i > 0)
						output << ",";
					output << " " << list_prop.GetValueString(i);
				}
				output << "\n";
			}
		}
	}

	for (const auto &p : properties) {
		if (skip_list.find(p.second.GetId()) != skip_list.end())
			continue;

		if (base_field && !curly_brace) {
			curly_brace = true;
			output << " {\n";
		}

		Write(p.second, output, prop_indent);
	} // for properties

	if (curly_brace) {
		// Close opened curly brace
		output <<
			/**/this->indent << "}";
	}
	if (base_field) {
		// Newline after base field
		output << "\n";
	}
	return true;
}

bool TextWriter::Write(const Property &prop, std::ostream &output, const std::string &indent) const
{
	output <<
		/**/indent;

	if (GetPropertyType(prop.GetId()) != PropertyType::UNDEFINED) {
		output << GetPropertySName(prop.GetId());
	} else {
		// Unknown property
		output << "property " << static_cast<uint16_t>(prop.GetId()) << " <";
		const binary *value = prop.GetValue().U8();
		if (value) {
			for (uint32_t i = 0; i < value->size(); ++i) {
				output << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(value->at(i)) << std::dec;
			}
		}
		output << ">";
	}

	switch (prop.GetType()) {
	case PropertyType::NONE:
		break;
	case PropertyType::IDENTIFIER:
		output << " " << prop.GetValueString(0);
		break;
	case PropertyType::STRING:
		if (prop.GetValueCount() > 0)
			output << " \"" << prop.GetValueString(0) << "\"";
		break;
	case PropertyType::STRING_LIST:
		for (uint32_t i = 0; i < prop.GetValueCount(); ++i) {
			if (i > 0)
				output << ",";
			output << " \"" << prop.GetValueString(i) << "\"";
		}
		break;
	case PropertyType::BASE:
		if (prop.GetValueCount() > 0)
			output << " " << prop.GetValueString(0);
		break;
	case PropertyType::BASE_LIST:
		for (uint32_t i = 0; i < prop.GetValueCount(); ++i) {
			if (i > 0)
				output << ",";
			output << " " << prop.GetValueString(i);
		}
		break;
	case PropertyType::BOOL:
		output << " " << std::boolalpha << !!(prop.GetValueU32());
		break;
	case PropertyType::USER_TYPE:
		output << " " << GetUserTypeSName(static_cast<UserType>(prop.GetValueU32()));
		break;
	case PropertyType::CONTROL:
		output << " " << GetControlSName(static_cast<ControlId>(prop.GetValueU32()));
		break;
	}
	output << "\n";
	return true;
}

bool TextWriter::WriteToString(const File &file, string *output) const
{
	if (output)
		output->clear();
	std::stringstream out;
	if (!Write(file, out))
		return false;
	if (output)
		*output = out.str();
	return true;
}

bool TextWriter::WriteToString(const Structure &structure, string *output) const
{
	if (output)
		output->clear();
	std::stringstream out;
	if (!Write(structure, out))
		return false;
	if (output)
		*output = out.str();
	return true;
}

void TextWriter::SetIndent(uint32_t spaces)
{
	this->indent.clear();
	if (spaces > 0)
		this->indent.resize(spaces, ' ');
	else
		this->indent = text::DEFAULT_INDENT;
}

HeaderWriter::HeaderWriter() : indent(text::DEFAULT_INDENT)
{
}

HeaderWriter::~HeaderWriter()
{
}

bool HeaderWriter::Write(const File &file, std::ostream &output) const
{
	if (file.GetStructures().empty())
		return true;

	string schema_name(file.GetStructures().front().GetName());
	text::MakeUpper(&schema_name);

	output <<
		/**/"#ifndef " << text::MACRO_PREFIX << schema_name << "\n" <<
		/**/"#define " << text::MACRO_PREFIX << schema_name << "\n" <<
		/**/"\n" <<
		/**/"#include <zet_struct1.h>\n" <<
		/**/"\n";

	for (const auto &s : file.GetStructures()) 
	{
		string temp;
		if (!Write(s, output))
			return false;
	}

	output <<
		/**/"#endif\n";
	return true;
}

bool HeaderWriter::Write(const Structure &structure, std::ostream &output) const
{
	string upper_name(structure.GetName());
	text::MakeUpper(&upper_name);

	uint32_t type(structure.GetHeader().type & (~Header::TYPE_MASK));
	output <<
		/**/"#ifndef " << text::MACRO_PREFIX << upper_name << "_TYPE\n" <<
		/**/"#define " << text::MACRO_PREFIX << upper_name << "_TYPE (0x" << std::hex << type << std::dec << "ul)\n";

	if (structure.GetFlexibleFields().count > 0) {
		output <<
			/**/"#define " << text::MACRO_PREFIX << upper_name << "_MIN_SIZE (" << structure.GetSize() << ")\n";
	} else {
		output <<
			/**/"#define " << text::MACRO_PREFIX << upper_name << "_SIZE (" << structure.GetSize() << ")\n";
	}

	output <<
		/**/text::GetDoxygen(structure) <<
		/**/"struct " << structure.GetName() << " {\n" <<
		/**/this->indent << "struct ZetHeader zet_header;\n";

	bool flex_started(false);
	bool group_separator(true); // force line break for the first field
	string group_name;
	size_t field_count(0);
	uint32_t next_offset(sizeof(Header));
	uint32_t first_flex_offset(0);

	for (const auto &f : structure.GetFields()) {
		if (f.GetType() == BaseType::METADATA) {
			if (f.HasProperty(PropertyId::GROUP)) {
				group_name = f.GetPropertyString(PropertyId::GROUP);
				group_separator = true;
			}
			continue;
		}

		field_count++;

		if (group_separator) {
			group_separator = false;
			output <<
				/**/"\n";
			if (!group_name.empty()) {
				output <<
					/**/this->indent << "/* " << group_name << " */\n";
				group_name.clear();
			}
		}

		uint32_t padding = f.GetOffset() - next_offset;
		if (padding > 0) {
			if (flex_started)
				WritePadding(padding, f.GetOffset() - first_flex_offset, true, output);
			else
				WritePadding(padding, f.GetOffset(), false, output);
			next_offset += padding;
		}

		if (f.IsFlexible())
			next_offset += f.GetTypeSize();
		else
			next_offset += f.GetSize();

		if (f.IsFlexible() && !flex_started) {
			flex_started = true;
			first_flex_offset = f.GetOffset();
			// Begin nested struct for flexible fields
			output <<
				/**/this->indent << "struct {\n";
		}

		string ctype(GetBaseCTypeName(f.GetType()));

		if (f.IsFlexible()) {
			output <<
				/**/this->indent << this->indent << ctype << " " << f.GetName() << ";" << text::GetDoxygen(f) << "\n";
		} else {
			output <<
				/**/this->indent << ctype << " " << f.GetName();

			if (f.GetCountPow2() > 0) {
				output << "[";
				if (f.GetCountPow2() < 32)
					output << (1UL << f.GetCountPow2());
				else
					output << "ZETLAB_FLEXIBLE_ARRAY_SIZE";
				output << "]";
			}

			output << ";" << text::GetDoxygen(f) << "\n";
		}
	} // for f

	if (structure.GetFlexibleFields().count > 0) {
		uint32_t aligned_offset = GetAlignedOffset(next_offset, structure.GetFlexibleFields().alignment);
		if (aligned_offset > next_offset) {
			WritePadding(aligned_offset - next_offset, aligned_offset - first_flex_offset, true, output);
			next_offset = aligned_offset;
		}
		output <<
			/**/this->indent << "} _[ZETLAB_FLEXIBLE_ARRAY_SIZE]; \n";
	}

	if (field_count) {
		output <<
			/**/"\n";
	}

	// Add footer with automatic padding
	if (structure.GetFlexibleFields().count > 0) {
		output <<
			/**/this->indent << "/* struct ZetFooter zet_footer; */\n";
	} else {
		uint32_t footer_offset = structure.GetSize() - sizeof(Footer);
		if (footer_offset > next_offset)
			WritePadding(footer_offset - next_offset, footer_offset, false, output);
		output <<
			/**/this->indent << "struct ZetFooter zet_footer;\n";
	}

	output <<
		/**/"};\n" <<
		/**/"#endif /* " << text::MACRO_PREFIX << upper_name << "_TYPE */\n" <<
		/**/"\n";
	return true;
}

bool HeaderWriter::WriteToString(const File &file, string *output, bool append) const
{
	if (output && !append)
		output->clear();
	std::stringstream out;
	if (!Write(file, out))
		return false;
	if (output) {
		if (append)
			output->append(out.str());
		else
			*output = out.str();
	}
	return true;
}

bool HeaderWriter::WriteToString(const Structure &structure, string *output, bool append) const
{
	if (output && !append)
		output->clear();
	std::stringstream out;
	if (!Write(structure, out))
		return false;
	if (output) {
		if (append)
			output->append(out.str());
		else
			*output = out.str();
	}
	return true;
}

void HeaderWriter::SetIndent(uint32_t spaces)
{
	this->indent.clear();
	if (spaces > 0)
		this->indent.resize(spaces, ' ');
	else
		this->indent = text::DEFAULT_INDENT;
}

void HeaderWriter::WritePadding(uint32_t size, uint32_t target, bool flexible, std::ostream &output) const
{
	if (size < 1)
		return;

	uint32_t count(0);
	string type_name;
	string additional_indent;
	string padding_prefix;

#define DETECT_PADDING_TYPE(T) \
	if (type_name.empty() && size >= sizeof(T) && (size % sizeof(T)) == 0) { \
		type_name = # T; \
		count = size / sizeof(T); \
	}

	DETECT_PADDING_TYPE(uint64_t);
	DETECT_PADDING_TYPE(uint32_t);
	DETECT_PADDING_TYPE(uint16_t);
	DETECT_PADDING_TYPE(uint8_t);

	if (flexible) {
		output <<
			/**/this->indent << this->indent << type_name << " _flexible_padding_to_" << target;
	} else {
		output <<
			/**/this->indent << type_name << " _padding_to_" << target;
	}
	if (count > 1)
		output << "[" << count << "]";
	output << ";\n";
}

static ControlId FindControlByName(const string &control_name)
{
#define M(ID, ENUM, SNAME) \
		if (control_name == SNAME) \
			return ControlId::ENUM;
	ZET_SCHEMA1_GENERATE_CONTROLS(M)
#undef M
	return ControlId::UNDEFINED;
}

static string GetControlSName(ControlId control_id)
{
	switch (control_id) {
#define M(ID, ENUM, SNAME) \
		case ControlId::ENUM: return SNAME;
	ZET_SCHEMA1_GENERATE_CONTROLS(M)
#undef M
	}
	return "undefined_control";
}

static UserType FindUserTypeByName(const string &user_type_name)
{
#define M(ID, ENUM, SNAME, MIN_SIZE, MAX_SIZE) \
		if (user_type_name == SNAME) \
			return UserType::ENUM;
	ZET_SCHEMA1_GENERATE_USER_TYPES(M)
#undef M
	return UserType::UNDEFINED;
}

static string GetUserTypeSName(UserType user_type)
{
	switch (user_type) {
#define M(ID, ENUM, SNAME, MIN_SIZE, MAX_SIZE) \
	case UserType::ENUM: \
		return SNAME;
	ZET_SCHEMA1_GENERATE_USER_TYPES(M)
#undef M
	}
	return "undefined_user_type";
}

static uint32_t GetBaseSize(BaseType base_type)
{
	switch (base_type) {
	case BaseType::METADATA: return 0;
#define M(ID, ENUM, SNAME, CTYPE) \
	case BaseType::ENUM: return sizeof(CTYPE);
	ZET_SCHEMA1_GENERATE_BASE_TYPES(M)
#undef M
	}
	return 0;
}

static string GetBaseSName(BaseType base_type)
{
	switch (base_type) {
	case BaseType::METADATA:
		return "metadata";
#define M(ID, ENUM, SNAME, CTYPE) \
	case BaseType::ENUM: return SNAME;
	ZET_SCHEMA1_GENERATE_BASE_TYPES(M)
#undef M
	}
	return "undefined_type";
}

static string GetBaseCTypeName(BaseType base_type)
{
	switch (base_type) {
#define M(ID, ENUM, SNAME, CTYPE) \
		case BaseType::ENUM: return # CTYPE;
	ZET_SCHEMA1_GENERATE_BASE_TYPES(M)
#undef M
	}
	return "undefined_type";
}

static PropertyType GetPropertyType(PropertyId property_id)
{
	switch (property_id) {
#define M(ID, ENUM, SNAME, FLAGS, OPTION_TYPE) \
	case PropertyId::ENUM: return OPTION_TYPE;
	ZET_SCHEMA1_GENERATE_PROPERTIES(M)
#undef M
	}
	return PropertyType::UNDEFINED;
}

static bool MatchPropertyFlags(PropertyId property_id, PropertyFlags flags)
{
	switch (property_id) {
#define M(ID, ENUM, SNAME, FLAGS, OPTION_TYPE) \
	case PropertyId::ENUM: return !!(static_cast<uint8_t>(PropertyFlags::FLAGS) & static_cast<uint8_t>(flags));
	ZET_SCHEMA1_GENERATE_PROPERTIES(M)
#undef M
	}
	return false;
}

static string GetPropertyTypeName(PropertyType property_type)
{
	switch (property_type) {
#define M(ID, ENUM, SNAME) \
		case PropertyType::ENUM: return SNAME;
	ZET_SCHEMA1_GENERATE_PROPERTY_TYPES(M)
#undef M
	}
	return "undefined_property_type";
}

static bool IsPropertyGlobal(const Field &field, PropertyId property_id)
{
	switch (field.GetType()) {
	case BaseType::UNDEFINED:
		return true;
	case BaseType::METADATA:
		switch (property_id) {
		case PropertyId::NAME:
			return !field.HasProperty(PropertyId::CONSTANT);
		case PropertyId::TITLE:
		case PropertyId::DESCRIPTION:
		case PropertyId::READONLY:
		case PropertyId::HIDDEN:
			return true;
		}
		return false;
	}
	return false;
}

static string GetPropertySName(PropertyId property_id)
{
	switch (property_id) {
#define M(ID, ENUM, SNAME, FLAGS, OPTION_TYPE) \
	case PropertyId::ENUM: return SNAME;
	ZET_SCHEMA1_GENERATE_PROPERTIES(M)
#undef M
	}
	std::ostringstream oss;
	oss << "undefined_property(" << static_cast<uint16_t>(property_id) << ")";
	return oss.str();
}

/* Check for valid identifier

zs::Property p;
p.id = zs::PropertyId::Name;
p.type = zs::PropertyType::Identifier;

uint32_t fail_count = 0;

p.strings.push_back(""); // invalid: empty string
if (p.IsValid()) fail_count++;

p.strings[0] = "Abc23"; // valid
if (!p.IsValid()) fail_count++;

p.strings[0] = "3"; // invalid: starts with digit
if (p.IsValid()) fail_count++;

p.strings[0] = "_23"; // valid
if (!p.IsValid()) fail_count++;

p.strings[0] = "abc3 2"; // invalid: multiple words
if (p.IsValid()) fail_count++;

*/
static bool IsValidIdentifier(const string &identifier)
{
	string::const_iterator pos(identifier.begin());
	string::const_iterator end(identifier.end());

	// First char
	uint32_t token = text::GetCharUtf8(pos, end);
	if (token == text::NOT_UTF8)
		return false;
	if (token >= 0x80 || (!std::isalpha(token) && token != '_'))
		return false;

	while (true) {
		token = text::GetCharUtf8(pos, end);
		if (token == 0)
			return (pos == end);
		if (token >= 0x80 || (!std::isalnum(token) && token != '_'))
			return false;
	}
	return true;
}

namespace text {

static void MakeUpper(string *str)
{
	if (str)
		std::transform(std::begin(*str), std::end(*str), std::begin(*str), toupper);
}

/// Simple UTF8 decoder
static uint32_t GetCharUtf8(string::const_iterator &pos, const string::const_iterator &end)
{
	uint32_t utf8 = 0;
	uint32_t sequence = 0;

	while (pos != end) {
		uint8_t b = static_cast<uint32_t>(*pos);
		++pos;

		if (sequence > 0) { // get next byte
			if ((b & 0xc0) != 0x80) { // != 10xx xxxx
				utf8 = NOT_UTF8;
				break;
			}

			utf8 = (utf8 << 6) + (b & 0x3f);
			if (--sequence)
				continue;
			// sequence completed
			break;
		}

		if ((b & 0x80) == 0x00) { // 0xxx xxxx
			utf8 = b;
			// single byte
			break;
		}

		if ((b & 0xc0) == 0x80) { // 10xx xxxx
			// skip unfinished sequence
			continue;
		}

		// start sequence
		if ((b & 0xe0) == 0xc0) { // 110x xxxx
			sequence = 1;
			utf8 = b & 0x1f;
		} else if ((b & 0xf0) == 0xe0) { // 1110 xxxx
			sequence = 2;
			utf8 = b & 0x0f;
		} else if ((b & 0xf8) == 0xf0) { // 1111 0xxx
			sequence = 3;
			utf8 = b & 0x07;
		} else {
			break;
		}
	}

	if (sequence > 0) // unfinished sequence
		utf8 = NOT_UTF8;

	return utf8;
}

static string GetTokenName(uint32_t token)
{
	switch (token) {
	case text::NOT_UTF8: return "invalid_char";
#define M(ID, ENUM, COMMENT) \
	case text::ENUM: return COMMENT;
	ZET_SCHEMA1_GENERATE_TOKENS(M)
#undef M
	}

	std::ostringstream out;
	if (token < 0x80 && std::isalnum(token)) {
		out << static_cast<char>(token);
	} else {
		out << "0x" << std::hex << token;
	}
	return out.str();
}

static string GetStateName(uint32_t state)
{
	switch (state) {
#define M(ID, ENUM, COMMENT) \
	case text::fsm::ENUM: return COMMENT;
	ZET_SCHEMA1_GENERATE_SYNTAX_STATES(M)
#undef M
	}
	return "unknown_state";
}

static string GetDoxygen(const Structure &structure)
{
	const auto &properties = structure.GetProperties();
	const auto iter(properties.find(PropertyId::DESCRIPTION));
	if (iter != properties.end()) {
		string desc(iter->second.GetValueString(0));
		if (!desc.empty())
			return "/** " + desc + " */\n";
	}

	return string();
}

static string GetDoxygen(const Field &field)
{
	const auto &properties = field.GetProperties();
	const auto iter(properties.find(PropertyId::DESCRIPTION));
	if (iter != properties.end()) {
		string desc(iter->second.GetValueString(0));
		if (!desc.empty())
			return " /**< " + desc + " */";
	}

	return string();
}

} // namespace text

Editor::Editor()
{
}

Editor::~Editor()
{
}

Structure *Editor::AppendStructure(File *file, const Structure &structure) const
{
	for (const auto &s : file->structures) {
		if (s.GetType() == structure.GetType())
			return nullptr;
	}

	auto s = file->structures.insert(file->structures.end(), structure);
	return &(*s);
}

bool Editor::RemoveLastStructure(File *file) const
{
	if (file->structures.empty())
		return false;

	file->structures.pop_back();
	return true;
}

#if 0
void Editor::AppendPadding(Structure *structure, size_t padding_size) const
{
	const BaseType padding_list[] = {BaseType::U8, BaseType::U16, BaseType::U32, BaseType::UNDEFINED};

	Field f;
	f.padding = true;

	for (size_t i = 0; padding_list[i] != BaseType::UNDEFINED; i++) {
		f.type = padding_list[i];
		f.size = f.GetSize();

		uint32_t required_alignment = f.size * 2;
		while ((padding_size % required_alignment) != 0) {
			structure->padding_counter++;
			std::stringstream padding_name;
			padding_name << "_padding" << structure->padding_counter;

			SetName(&f, padding_name.str());
			SetHidden(&f, true);

			f.index = structure->next_field_index;
			f.offset = structure->next_field_offset;

			structure->fields.push_back(f);
			structure->next_field_index++;
			structure->next_field_offset += f.size;
			padding_size -= f.size;
		}
	}
}
#endif

bool Editor::SetId(Structure *structure, const StructId &id) const
{
	if (!structure)
		return false;

	structure->header.id = id;
	return true;
}

bool Editor::SetType(Structure *structure, uint32_t type) const
{
	if (!structure)
		return false;

	structure->header.type = type | Header::TYPE_MASK;
	return true;
}

bool Editor::SetName(Structure *structure, const string &name) const
{
	if (!structure)
		return false;

	structure->name = name;
	if (name.empty())
		return RemoveProperty(structure, PropertyId::NAME);
	return SetProperty(structure, PropertyId::NAME, name);
}

bool Editor::RemoveProperty(Structure *structure, PropertyId property_id) const
{
	if (!structure)
		return false;

	auto result = structure->properties.find(property_id);
	if (result == structure->properties.end())
		return false;

	structure->properties.erase(result);
	return true;
}

Field *Editor::FindFieldByIndex(Structure *structure, size_t index) const
{
	if (!structure)
		return nullptr;

	if (index >= structure->fields.size())
		return nullptr;

	return &structure->fields[index];
}

Field *Editor::AppendField(Structure *structure, const Field &field) const
{
	if (!structure)
		return nullptr;

	if (field.type == BaseType::UNDEFINED)
		return nullptr;

	if (field.type == BaseType::METADATA) {
		auto f = structure->fields.insert(structure->fields.end(), field);
		f->index = 0;
		f->offset = 0;
		return &(*f);
	}

	// Check that fixed field is not coming after flexible
	if (structure->flexible_fields.count > 0 && !field.IsFlexible())
		return nullptr;

	auto f = structure->fields.insert(structure->fields.end(), field);
	structure->RealignFields();
	return &(*f);
}

Field *Editor::AppendField(Structure *structure, BaseType base_type, const string &name, uint32_t count_pow2) const
{
	Field f;
	f.type = base_type;
	f.name = name;
	f.SetCountPow2(count_pow2);

	SetName(&f, name);
	return AppendField(structure, f);
}

bool Editor::RemoveLastField(Structure *structure) const
{
	if (!structure || structure->fields.empty())
		return false;

	const auto &last = structure->fields.back();
	bool is_base = last.IsBase();
	structure->fields.pop_back();
	if (is_base)
		structure->RealignFields();
	return true;
}

bool Editor::RemoveProperty(Field *field, PropertyId property_id) const
{
	if (!field)
		return false;

	auto result = field->properties.find(property_id);
	if (result == field->properties.end())
		return false;

	field->properties.erase(result);
	return true;
}

bool Editor::SetName(Field *field, const string &name) const
{
	if (!field)
		return false;

	field->name = name;
	if (name.empty())
		return RemoveProperty(field, PropertyId::NAME);
	return SetProperty(field, PropertyId::NAME, name);
}

bool Editor::SetType(Field *field, BaseType type) const
{
	if (field) {
		field->SetType(type);
		return true;
	}
	return false;
}

bool Editor::SetCountPow2(Field *field, uint8_t pow2) const
{
	if (field) {
		field->SetCountPow2(pow2);
		return true;
	}
	return false;
}

void BaseVector::SetType(BaseType base_type)
{
	Reset();

	this->type = base_type;
	switch (this->type) {
	case BaseType::CHAR:
		this->pointer.STRING = new (std::addressof(this->storage)) std::vector<string>;
		break;
#define M(ID, ENUM, SNAME, CTYPE) \
	case BaseType::ENUM: \
		this->pointer.ENUM = new (std::addressof(this->storage)) std::vector<CTYPE>; \
		break;
	ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(M)
#undef M
	default:
		this->pointer.STRING = nullptr;
	}
}

void BaseVector::Reset()
{
	switch (this->type) {
	case BaseType::CHAR:
		this->pointer.STRING->~vector();
		break;
#define M(ID, ENUM, SNAME, CTYPE) \
	case BaseType::ENUM: \
		this->pointer.ENUM->~vector(); \
		break;
	ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(M)
#undef M
	}

	this->pointer.STRING = nullptr;
	this->type = BaseType::UNDEFINED;
}

BaseVector &BaseVector::operator = (const BaseVector &init)
{
	Reset();

	this->type = init.type;
	switch (this->type) {
	case BaseType::CHAR:
		this->pointer.STRING = new (std::addressof(this->storage)) std::vector<string>(*init.pointer.STRING);
		break;
#define M(ID, ENUM, SNAME, CTYPE) \
	case BaseType::ENUM: \
		this->pointer.ENUM = new (std::addressof(this->storage)) std::vector<CTYPE>(*init.pointer.ENUM); \
		break;
	ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(M)
#undef M
	default:
		this->pointer.STRING = nullptr;
	}

	return *this;
}

size_t BaseVector::GetCount() const
{
	switch (this->type) {
	case BaseType::CHAR:
		return this->pointer.STRING->size();
#define M(ID, ENUM, SNAME, CTYPE) \
	case BaseType::ENUM: \
		return this->pointer.ENUM->size(); \
		break;
	ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(M)
#undef M
	}
	return 0;
}

string BaseVector::GetString(size_t index) const
{
	switch (this->type) {
	case BaseType::CHAR:
		if (index < this->pointer.STRING->size())
			return this->pointer.STRING->at(index);
		break;
#define M(ID, ENUM, SNAME, CTYPE) \
	case BaseType::ENUM: \
		if (index < this->pointer.ENUM->size()) { \
			std::ostringstream out; \
			out << this->pointer.ENUM->at(index); \
			return out.str(); \
		} \
		break;
	ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(M)
#undef M
	}
	return string();
}

void Property::SetId(PropertyId property_id, BaseType field_type)
{
	Reset();

	this->id = property_id;
	switch (GetType()) {
	case PropertyType::IDENTIFIER:
	case PropertyType::STRING:
	case PropertyType::STRING_LIST:
		this->value.SetType(BaseType::CHAR);
		break;
	case PropertyType::BASE:
	case PropertyType::BASE_LIST:
		this->value.SetType(field_type);
		break;
	case PropertyType::BOOL:
	case PropertyType::BASE_TYPE:
	case PropertyType::USER_TYPE:
	case PropertyType::CONTROL:
		this->value.SetType(BaseType::U32);
		break;
	}
}

void Property::Reset()
{
	this->id = PropertyId::UNDEFINED;
	this->value.Reset();
}

PropertyType Property::GetType() const
{
	return GetPropertyType(this->id);
}

uint32_t Property::GetValueU32() const
{
	switch (GetValueType()) {
	case BaseType::UNDEFINED:
	case BaseType::METADATA:
	case BaseType::CHAR:
		return 0;
#define M(ID, ENUM, SNAME, CTYPE) \
	case BaseType::ENUM: \
		{ \
			const std::vector<CTYPE> *v = this->value.ENUM(); \
			if (v && !v->empty()) \
				return static_cast<uint32_t>(v->at(0)); \
		} \
		return 0;
	ZET_SCHEMA1_GENERATE_ARITHMETIC_TYPES(M)
#undef M
	}
	return 0;
}

/// Checks whether property value conforms to its type
bool Property::IsValid() const
{
	switch (GetType()) {
	case PropertyType::NONE:
		return (GetValueCount() == 0);
	case PropertyType::IDENTIFIER: // first string is a valid identifier
		return IsValidIdentifier(GetValueString(0));
	case PropertyType::STRING: // exactly one string
	case PropertyType::BASE: // exactly one number of base type (or one string for BaseType::CHAR)
		return (GetValueCount() == 1);
	case PropertyType::STRING_LIST: // one or more strings
	case PropertyType::BASE_LIST: // one or more numbers of base type
		return (GetValueCount() >= 1);
	case PropertyType::BOOL: // one or zero u32
		return (GetValueCount() <= 1);
	case PropertyType::BASE_TYPE: // exactly one u32 (BaseType)
	case PropertyType::USER_TYPE: // exactly one u32 (UserType)
	case PropertyType::CONTROL: // exactly one u32 (ControlId)
		return (GetValueCount() == 1);
	}
	return false;
}

namespace crc32 {
/*
*  COPYRIGHT (C) 1986 Gary S. Brown.  You may use this program, or
*  code or tables extracted from it, as desired without restriction.
*
*  First, the polynomial itself and its table of feedback terms.  The
*  polynomial is
*  X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0
*
*  Note that we take it "backwards" and put the highest-order term in
*  the lowest-order bit.  The X^32 term is "implied"; the LSB is the
*  X^31 term, etc.  The X^0 term (usually shown as "+1") results in
*  the MSB being 1.
*
*  Note that the usual hardware shift register implementation, which
*  is what we're using (we're merely optimizing it by doing eight-bit
*  chunks at a time) shifts bits into the lowest-order term.  In our
*  implementation, that means shifting towards the right.  Why do we
*  do it this way?  Because the calculated CRC must be transmitted in
*  order from highest-order term to lowest-order term.  UARTs transmit
*  characters in order from LSB to MSB.  By storing the CRC this way,
*  we hand it to the UART in the order low-byte to high-byte; the UART
*  sends each low-bit to hight-bit; and the result is transmission bit
*  by bit from highest- to lowest-order term without requiring any bit
*  shuffling on our part.  Reception works similarly.
*
*  The feedback terms table consists of 256, 32-bit entries.  Notes
*
*      The table can be generated at runtime if desired; code to do so
*      is shown later.  It might not be obvious, but the feedback
*      terms simply represent the results of eight shift/xor opera-
*      tions for all combinations of data and CRC register values.
*
*      The values must be right-shifted by eight bits by the "updcrc"
*      logic; the shift must be unsigned (bring in zeroes).  On some
*      hardware you could probably optimize the shift in assembler by
*      using byte-swap instructions.
*      polynomial $edb88320
*
*/

static const uint32_t TABLE[] = {
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
	0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
	0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
	0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
	0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
	0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
	0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
	0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
	0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
	0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
	0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
	0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
	0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
	0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
	0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
	0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
	0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
	0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
	0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
	0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
	0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
	0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
	0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
	0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
	0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
	0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
	0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
	0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
	0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
	0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
	0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
	0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
	0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
	0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
	0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
	0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
	0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
	0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
	0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
	0x2d02ef8dL
};

/*
* This a generic crc32() function, it takes seed as an argument,
* and does __not__ xor at the end. Then individual users can do
* whatever they need.
*/
static uint32_t Calculate(uint32_t crc, const void *buf, size_t len)
{
	const uint8_t *p = (const uint8_t *)buf;

	while (len-- > 0)
		crc = TABLE[(crc ^ *p++) & 0xff] ^ (crc >> 8);

	return crc;
}

} // namespace crc32

uint32_t CalcCrc32(uint32_t crc32, const void *buf, size_t len)
{
	return crc32::Calculate(crc32 ^ crc32::XORMASK, buf, len) ^ crc32::XORMASK;
}

uint32_t GetAlignedOffset(uint32_t offset, size_t base_size)
{
	return ((offset + base_size - 1) / base_size) * base_size;
}

} // namespace schema1
} // namespace zet
