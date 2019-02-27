#ifndef ZET_STRUCTOR_REVISION
#define ZET_STRUCTOR_REVISION 13
#endif

#define GENERATE_SCHEMAS 1

//#include <cstdlib>
#include <cstdarg>
//#include <string>
#include <iomanip>
//#include <iostream>
#include <fstream>
#include <streambuf>
//#include <algorithm>

#include <list>
#include <set>

//#include <stdlib.h>
//#include <stdint.h>
//#include <string.h>
#include <fcntl.h>

#ifdef _WIN32
// MSVS specific stdlib routines
#define strtoull _strtoui64
#define strtoll _strtoi64
#define snprintf sprintf_s
#endif

#include "zet_schema1.h"

namespace zs = zet::schema1;

const zs::string EXT_ZSCHEMA1 = ".zschema1";
const zs::string EXT_ZBINARY1 = ".zbinary1";

#define OUT(_fmt, ...) fprintf(stdout, _fmt, ## __VA_ARGS__)
#define ERR(_fmt, ...) fprintf(stderr, _fmt, ## __VA_ARGS__)

#if 0
#define DBG(_fmt, ...) fprintf(stderr, _fmt, ## __VA_ARGS__)
#else
#define DBG(_fmt, ...) do {} while (0)
#endif

static void print_version()
{
	std::cerr <<
		/**/"zet_structor R" << ZET_STRUCTOR_REVISION << " (" << __DATE__ << " " << __TIME__ << ")\n";
}

static void print_help()
{
	print_version();
	std::cerr <<
		/**/"\n"
		/**/"Usage: zet_structor <schema...> | [options]\n"
		/**/"\n"
		/**/"  <schema>       : file with struct schema (*.zschema1)\n"
		/**/"  --h            : generate all structs as C header file\n"
#ifdef GENERATE_SCHEMAS
		/**/"  --s            : generate all schemas as text files (*.zschema1)\n"
#endif
		/**/"  --bin <id>     : generate schema <id> as binary file\n"
		/**/"  --bin all      : generate all schemas as binary files\n"
		/**/"  --hex <id>     : generate schema <id> as C hex file\n"
		/**/"  --hex all      : generate all schemas as C hex files\n"
		/**/"  --types        : print field base types\n"
		/**/"  --properties   : print supported properties\n"
		/**/"  --controls     : print supported controls\n"
		/**/"  --usertypes    : print supported user types\n"
		/**/"  --version      : print version\n"
		/**/"  --help         : print this help\n"
		/**/"\n";
}

static void print_base_types()
{
	std::cout <<
		/**/"Base types:\n" <<
#define M(ID, ENUM, SNAME, CTYPE) \
		/**/std::setw(2) << std::setfill(' ') << std::right << ID << ": " << \
			std::setw(6) << std::setfill(' ') << std::left << SNAME << " (size " << sizeof(CTYPE) << ")\n" <<
		ZET_SCHEMA1_GENERATE_BASE_TYPES(M)
#undef M
		/**/"\n";
}

static void print_properties()
{
	std::cout <<
		/**/"Supported properties:\n" <<
#define M(ID, ENUM, SNAME, FLAGS, OPTION_TYPE) \
		/**/std::setw(2) << std::setfill(' ') << std::right << ID << ": " << \
			std::setw(12) << std::setfill(' ') << std::left << SNAME << "\n" <<
		ZET_SCHEMA1_GENERATE_PROPERTIES(M)
#undef M
		/**/"\n";
}

static void print_controls()
{
	std::cout <<
		/**/"Supported controls:\n" <<
#define M(ID, ENUM, SNAME) \
		/**/std::setw(2) << std::setfill(' ') << std::right << ID << ": " << \
		/**/std::setw(16) << std::setfill(' ') << std::left << SNAME << "\n" <<
		ZET_SCHEMA1_GENERATE_CONTROLS(M)
#undef M
		/**/"\n";
}

static void print_user_types()
{
	std::cout <<
		/**/"Supported user types:\n" <<
#define M(ID, ENUM, SNAME, MIN_SIZE, MAX_SIZE) \
		/**/std::setw(2) << std::setfill(' ') << std::right << ID << ": " << \
		/**/std::setw(16) << std::setfill(' ') << std::left << SNAME << "\n" <<
		ZET_SCHEMA1_GENERATE_USER_TYPES(M)
#undef M
		/**/"\n";
}

int main(int argc, char *argv[])
{
	if (argc <= 1) {
#if 1
		print_help();
#else
		std::ifstream file("test.zbinary1", std::ifstream::in | std::ifstream::binary);
		if (file.is_open()) {
			zs::binary input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();

			zs::Structure s;
			zs::Reader reader;
			reader.ReadFromBinary(&s, input);

			zs::string text;
			zs::TextWriter text_writer;
			text_writer.WriteToString(s, &text);

			std::cerr << text << std::endl;
		}
#endif
		return -1;
	}

	struct Entry
	{
		std::string source;
		zs::File file;

		Entry(const std::string &init) : source(init) {}
	};
	std::list<Entry> db;

	struct GenerateOptions
	{
		bool headers;
		bool schemas;
		bool bin_all;
		bool hex_all;
		std::set<uint32_t> bin_ids;
		std::set<uint32_t> hex_ids;

		GenerateOptions() : headers(false), schemas(false), bin_all(false), hex_all(false) {}
	};
	GenerateOptions gen;

	for (int i = 1; i < argc; i++) {
		std::string arg(argv[i]);

		if (arg == "--h") {
			gen.headers = true;
			continue;
		}
#ifdef GENERATE_SCHEMAS
		if (arg == "--s") {
			gen.schemas = true;
			continue;
		}
#endif
		if (arg == "--bin") {
			i++;
			if (i >= argc) {
				std::cerr << "Invalid --bin option" << std::endl;
				return -1;
			}
			arg.assign(argv[i]);
			if (arg == "all") {
				gen.bin_all = true;
				continue;
			}
			try {
				gen.bin_ids.insert(zs::Header::TYPE_MASK | std::stoul(arg, 0, 0));
			} catch (const std::exception &) {
				std::cerr << "Invalid --bin " << arg << std::endl;
				return -1;
			}
			continue;
		}
		if (arg == "--hex") {
			i++;
			if (i >= argc) {
				std::cerr << "Invalid --hex option" << std::endl;
				return -1;
			}
			arg.assign(argv[i]);
			if (arg == "all") {
				gen.hex_all = true;
				continue;
			}
			try {
				gen.hex_ids.insert(zs::Header::TYPE_MASK | std::stoul(arg, 0, 0));
			} catch (const std::exception &) {
				std::cerr << "Invalid --hex " << arg << std::endl;
				return -1;
			}
			continue;
		}

		if (arg == "--types") {
			print_base_types();
			return 0;
		}
		if (arg == "--properties") {
			print_properties();
			return 0;
		}
		if (arg == "--controls") {
			print_controls();
			return 0;
		}
		if (arg == "--usertypes") {
			print_user_types();
			return 0;
		}
		if (arg == "--version") {
			print_version();
			return 0;
		}
		if (arg == "--help") {
			print_help();
			return 0;
		}
		if (arg[0] == '-') {
			std::cerr << "Unknown option " << arg << std::endl;
			return -1;
		}
		db.push_back(Entry(arg));
	}

	if (db.empty()) {
		std::cerr << "No sources" << std::endl;
		return -1;
	}

	zs::TextReader text_reader;
	for (auto &entry : db) {
		zs::string input;

		if (input.empty()) {
			std::ifstream file(entry.source, std::ifstream::in);
			if (file.is_open())
				input = zs::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		}
		if (input.empty()) {
			std::ifstream file(entry.source + EXT_ZSCHEMA1, std::ifstream::in);
			if (file.is_open())
				input = zs::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		}
		if (input.empty()) {
			std::cerr << entry.source << ": cannot open\n";
			return -1;
		}
		if (!text_reader.ReadFromString(&entry.file, input)) {
			const auto &error = text_reader.GetError();
			if (!error.message.empty()) {
				zs::string error_line(input.begin() + error.line_pos, std::find(input.begin() + error.line_pos, input.end(), '\n'));
				size_t error_tabs(std::count(error_line.begin(), error_line.begin() + error.column, '\t'));
				std::replace(error_line.begin(), error_line.begin() + error.column, '\t', ' ');
				zs::string error_mark;
				error_mark.resize(error.column + error_tabs * 0, ' ');
				std::cerr << entry.source << "(" << (error.row + 1) << "): error: " << error.message << "\n";
				std::cerr << error_line << "\n" << error_mark << "^" << std::endl;
			}
			return -1;
		}
	}

	if (gen.headers) {
		zs::HeaderWriter writer;

		for (const auto &entry : db) {
			std::string path(entry.source + ".h");
			std::ofstream file(path);
			if (!file.is_open() || file.bad()) {
				std::cerr << path << ": cannot create header file" << std::endl;
				return -1;
			}

			writer.Write(entry.file, file);
		}
	}

#ifdef GENERATE_SCHEMAS
	if (gen.schemas) {
		zs::TextWriter writer;

		for (const auto &entry : db) {
			std::string path(entry.source + EXT_ZSCHEMA1);
			std::ofstream file(path);
			if (!file.is_open() || file.bad()) {
				std::cerr << path << ": cannot create schema file" << std::endl;
				return -1;
			}

			writer.Write(entry.file, file);
		}
	}
#endif

	if (gen.bin_all || gen.hex_all || !gen.bin_ids.empty() || !gen.hex_ids.empty()) {
		zs::Writer bin_writer;

		for (const auto &entry : db) {
			for (const auto &s : entry.file.GetStructures()) {
				uint32_t type = s.GetHeader().type;
				bool bin = (gen.bin_all || gen.bin_ids.find(type) != gen.bin_ids.end());
				bool hex = (gen.hex_all || gen.hex_ids.find(type) != gen.hex_ids.end());
				if (!bin && !hex)
					continue;

				zs::binary blob;
				bin_writer.WriteToBinary(s, &blob);

				if (bin) {
					std::ostringstream path_out;
					path_out << entry.source << "." << std::setw(8) << std::setfill('0') << std::hex << type << EXT_ZBINARY1;
					std::string path(path_out.str());
					std::ofstream file(path, std::ofstream::binary);
					if (!file.is_open() || file.bad()) {
						std::cerr << path << ": cannot create bin file" << std::endl;
						return -1;
					}
					file.write(reinterpret_cast<const char *>(blob.data()), blob.size());
					file.close();
				}

				if (hex) {
					std::ostringstream path_out;
					path_out << entry.source << "." << std::setw(8) << std::setfill('0') << std::hex << type << ".hex";
					std::string path(path_out.str());
					std::ofstream file(path);
					if (!file.is_open() || file.bad()) {
						std::cerr << path << ": cannot create hex file" << std::endl;
						return -1;
					}
					file <<
						/**/"\t/* struct " << s.GetName() << " schema (" << blob.size() << " bytes) */\n";

					file << std::setfill('0') << std::hex;
					size_t column = 0;
					for (uint8_t b : blob) {
						if (column == 0) {
							file <<
								/**/"\t";
						} else {
							file << ' ';
						}
						file << "0x" << std::setw(2) << static_cast<uint32_t>(b) << ",";
						column++;
						if (column >= 8) {
							column = 0;
							file << "\n";
						}
					}
					if (column > 0) {
						file << "\n";
					}
					file.close();
				}
			}
		}
	}
//	print_help();
	return 0;
}

