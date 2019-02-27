
#include "gtest/gtest.h"

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include "zet_schema1.h"

namespace zs = zet::schema1;

// Проверка констант
TEST(Schema, Constants)
{
	EXPECT_EQ(sizeof(zs::Header), 40);
	EXPECT_EQ(static_cast<zs::BaseType>(11), zs::BaseType::F64);
}

// Проверка генераторов констант
TEST(Schema, Generators)
{
#define TEST_M(ID, ENUM, SNAME, CTYPE) \
	EXPECT_EQ(static_cast<zs::BaseType>(ID), zs::BaseType::ENUM);
ZET_SCHEMA1_GENERATE_BASE_TYPES(TEST_M)
#undef TEST_M
}

// Проверка конвертеров
TEST(Schema, Writer)
{
	// Создаем структур с нуля с помощью Editor
	zs::Editor editor;

	zs::Structure s;
	editor.SetType(&s, 0xbeef);
	editor.SetName(&s, "ZetTest1");

	// Первое поле - модифицируем прямо в схеме структуры по указателю
	zs::Field *f1 = editor.AppendField(&s, zs::BaseType::U32, "first");
	editor.SetControl(f1, zs::ControlId::COMBOBOX);
	editor.SetDefault(f1, 13);
	editor.SetDescription(f1, "First field");
	editor.SetStringList(f1, "AB", "BCDEF");
	editor.SetDefault(f1, "31");
	editor.SetMinimum(f1, "0x30");

	// Второе поле - сначала создаем, потом копируем в схему структуры
	zs::Field f2;
	editor.SetType(&f2, zs::BaseType::U64);
	editor.SetCountPow2(&f2, zs::Field::COUNT_POW2_FLEXIBLE);
	editor.SetName(&f2, "second");
	editor.SetControl(&f2, zs::ControlId::CHECKBOX);
	editor.SetDescription(&f2, "Field 2");
	editor.SetDefault(&f2, 1.0/3);
	editor.SetReadonly(&f2, true);
	editor.AppendField(&s, f2);

	zs::File file;
	editor.AppendStructure(&file, s);

	zs::string file_text;
	zs::TextWriter text_writer;
	text_writer.SetIndent(4);
	text_writer.WriteToString(file, &file_text);

	zs::File file2;
	zs::TextReader text_reader;
	text_reader.ReadFromString(&file2, file_text);

	EXPECT_EQ(file.GetStructures().size(), file2.GetStructures().size());
	EXPECT_EQ(file.GetStructures().front().GetType(), file2.GetStructures().front().GetType());

#if 0

//		zs::Schema schema;

#if 1
		{
			zs::string text;
			zs::TextWriter text_writer;
			text_writer.SetIndent(3);
			text_writer.WriteToString(s, &text);
			std::cout << "\nTextWriter:\n" << text << "\n";
		}
#endif

#if 1
		{
			zs::binary bin;

			{
				zs::Writer writer;
				writer.WriteToBinary(s, &bin);
				std::cout << "Writer: " << bin.size() << "\n";

#if 0
				int count = 0;
				for (auto x : bin) {
					uint32_t hex = x & 0xFF;
					printf(" %02x", hex);
					count++;
					if (count >= 16) {
						std::cout << "\n";
						count = 0;
					}
				}
#endif
			}

			{
				zs::Reader reader;
				if (!reader.ReadFromBinary(&s, bin)) {
					std::cerr << "ERROR: " << reader.GetError().message << std::endl;
				}

				zs::string text;
				zs::TextWriter text_writer;
				text_writer.SetIndent(1);
				text_writer.WriteToString(s, &text);
				std::cout << "Reader check:\n" << text << "\n";

				zs::Schema schema;
				zs::TextReader text_reader;
				text_reader.ReadFromString(&schema, text);
				zs::TextReader::Error error = text_reader.GetError();
				if (!error.message.empty()) {
					zs::string error_line(text.begin() + error.line_pos, std::find(text.begin() + error.line_pos, text.end(), '\n'));
					zs::string error_mark;
					error_mark.resize(error.column, ' ');
					std::cerr << "ERROR at (" << (error.row + 1) << "," << (error.column + 1) << "): " << error.message << "\n";
					std::cerr << error_line << "\n" << error_mark << "^" << std::endl;
				}

				text.clear();
				text_writer.WriteToString(schema, &text);
				std::cout << "TextReader check:\n" << text << "\n";
			}
		}
#endif

#if 0
		std::string haha = "struct haha {}";
		zs::TextReader text_reader;
		zs::Schema s2;
		text_reader.ReadFromString(&s2, haha);
#endif
		return 1;
	}
#endif
}

// Проверка редактора схемы
TEST(Schema, Editor)
{
	zs::Editor editor;

//	EXPECT_EQ(nullptr, editor.SetDefault(nullptr, 1));

	zs::Structure s;

	zs::Field *f = editor.AppendField(&s, zs::BaseType::U32, "some_field", 0);

//	EXPECT_FALSE(editor.RemoveProperty(&f, zs::PropertyId::STRING_LIST));

	EXPECT_TRUE(editor.SetDefault(f, 13));
	EXPECT_TRUE(editor.SetDescription(f, "Field 1"));
	EXPECT_TRUE(editor.SetStringList(f, "AB", "BCDEF"));
	EXPECT_TRUE(editor.SetDefault(f, "31"));
	EXPECT_TRUE(editor.SetMinimum(f, "0x30"));
	editor.RemoveProperty(f, zs::PropertyId::STRING_LIST);
	EXPECT_FALSE(f->HasProperty(zs::PropertyId::STRING_LIST));

	std::vector<uint32_t> uints = {1, 2, 3};
	std::vector<zs::string> strings = {"4", "5", "6"};
	EXPECT_TRUE(editor.SetList(f, 3.5, "0x45", uints, strings));
}

TEST(Schema, Value)
{
	zs::Editor e;

	zs::Structure s;

	e.SetTitle(&s, "Test struct");

	zs::Field *f = e.AppendField(&s, zs::BaseType::U32, "field1");
	e.SetDefault(f, "3");

	struct Test
	{
		zs::Header zet_header;
		uint32_t field1;
		zs::Footer zet_footer;
	};

	Test test;
	test.field1 = 5;

	zs::BaseVector value;
	EXPECT_TRUE(f->ReadValue(&test, sizeof(test), &value));
	EXPECT_EQ(value.U32()->at(0), test.field1);

	value.U32()->at(0) = 1234;
	EXPECT_TRUE(f->SaveValue(&test, sizeof(test), value));
	EXPECT_EQ(test.field1, 1234);
}

// Вычисление crc
TEST(Schema, Crc)
{
	uint32_t crc;

	crc = zs::CalcCrc32(0, NULL, 0);
	EXPECT_EQ(0, crc);

	uint8_t buffer[32];

	const char *STR = "Hello!";
	const uint32_t LEN = strlen(STR);
	memcpy(buffer, STR, LEN);
	crc = zs::CalcCrc32(crc, buffer, LEN);
	EXPECT_EQ(0x09d2acc56, crc);

	memcpy(buffer + LEN, &crc, sizeof(crc));
	crc = zs::CalcCrc32(0, buffer, LEN + sizeof(crc));
	EXPECT_EQ(0x2144df1cUL, crc);
}

TEST(Schema, Alignment)
{
	struct ZetTest {
		struct ZetHeader zet_header;
		uint32_t field;
		uint32_t _padding;
		struct ZetFooter zet_footer;
	};

	ZetTest test;
	uint64_t addr_test = reinterpret_cast<uint64_t>(&test);
	uint64_t addr_footer = reinterpret_cast<uint64_t>(&test.zet_footer);
	EXPECT_EQ(addr_footer - addr_test, ZETLAB_ZETHEADER_SIZE + sizeof(uint32_t) + sizeof(uint32_t));
	EXPECT_EQ(sizeof(ZetTest), ZETLAB_ZETHEADER_SIZE + sizeof(uint32_t) + sizeof(uint32_t) + ZETLAB_ZETFOOTER_SIZE);
}
