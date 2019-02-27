#include "stdafx.h"
#include <md5\Cmd5.h>

//#include <boost/foreach.hpp>

Cmd5::Cmd5() : bInit(false)
{
	// Zero fill md5 container
	memset( md5_bytes, 0, sizeof( md5_bytes ) );
}

Cmd5::Cmd5( std::tstring tstrHash ) : bInit(true)
{
	// Zero fill md5 container
	memset( md5_bytes, 0, sizeof( md5_bytes ) );
	// Fill container from hex string
	hex_to_bytes( tstrHash, md5_bytes );
}

Cmd5::~Cmd5()
{
}

const bool Cmd5::operator==( const Cmd5& rhs ) const
{
	if (this->bInit && rhs.bInit)
	{
		for (int i = 0; i < 16;++i)
		{
			if (this->md5_bytes[i] != rhs.md5_bytes[i])
				// Section does not match
				return false;
		}

		// All md5 section are matched
		return true;
	}

	// At least one object wasn't initialized
	return false;
}

const bool Cmd5::operator!=( const Cmd5& rhs ) const
{
	return !operator==( rhs );
}

std::tstring Cmd5::md5OfString( const std::tstring& tstrStr  )
{
	if (bInit)
	{
		// Return early calculated value
		return md5_str;
	}

	// Calculate and return
	return (md5_str = md5s( reinterpret_cast< const md5_byte_t* >( tstrStr.c_str() ), tstrStr.size() * sizeof( _TCHAR ) ));
}

std::tstring Cmd5::md5OfFile( const std::tstring& tstrFileName )
{
	if (bInit)
	{
		// Return early calculated value
		return md5_str;
	}

	FILE* f(nullptr);

	// Open file for reading in binary mode
	if (!_tfopen_s(&f, tstrFileName.c_str(), _T("rb")) && (f != nullptr))
	{
		// Get file size (in bytes)
		fseek(f, 0, SEEK_END);
		long size = ftell(f);
		fseek(f, 0, SEEK_SET);

		// Read entire file
		md5_byte_t* buf = new md5_byte_t[size];
		fread_s(buf, size, sizeof(md5_byte_t), size, f);

		// Calculate and remember
		md5_str = md5s(buf, size);
		delete[]buf;
		buf = nullptr;
		// Close file
		fclose( f );
		f = nullptr;

	}

	return md5_str;
}

std::tstring Cmd5::md5OfFileWithSalt(const std::tstring& tstrFileName, const std::string& salt)
{
	if (bInit)
	{
		// Return early calculated value
		return md5_str;
	}

	FILE* f(nullptr);

	// Open file for reading in binary mode
	if (!_tfopen_s(&f, tstrFileName.c_str(), _T("rb")) && (f != nullptr))
	{
		// Get file size (in bytes)
		fseek(f, 0, SEEK_END);
		long size = ftell(f);
		fseek(f, 0, SEEK_SET);

		// Read entire file
		md5_byte_t* buf = new md5_byte_t[size + salt.size()];
		fread_s(buf, size + salt.size(), sizeof(md5_byte_t), size, f);

		memcpy_s(buf + size, salt.size(), salt.c_str(), salt.size());

		// Calculate and remember
		md5_str = md5s(buf, size + salt.size());
		delete[]buf;
		buf = nullptr;
		// Close file
		fclose(f);
		f = nullptr;
	}

	return md5_str;
}


std::tstring Cmd5::md5OfFiles( const std::vector< std::tstring >& v_tstrFileNames )
{
	if (bInit)
	{
		// Return early calculated value
		return md5_str;
	}

	FILE* f(nullptr);
	md5_state_t state;

	// Initialize md5 hash
	md5_init( &state );

	for each( std::tstring tstrFileName in v_tstrFileNames )
	{
		// Open file for reading in binary mode
		if (!_tfopen_s( &f, tstrFileName.c_str(), _T("rb") ) && (f != nullptr))
		{
			// Get file size (in bytes)
			fseek( f, 0, SEEK_END );
			long size = ftell( f );
			fseek( f, 0, SEEK_SET );

			// Read entire file
			md5_byte_t* buf = new md5_byte_t[size];
			fread_s( buf, size, sizeof( md5_byte_t ), size, f );

			// Calculate and remember
			md5_append( &state, buf, size );

			delete[]buf;
			buf = nullptr;
			// Close file
			fclose( f );
			f = nullptr;
		}
	}

	// Finish
	md5_finish( &state, md5_bytes );

	// Convert to hex string
	bytes_to_hex( md5_bytes, md5_str );

	return md5_str;
}

md5_byte_t* Cmd5::md5b( const md5_byte_t* bytes, int size_in_bytes )
{
	md5_state_t state;

	// Initialize md5 hash
	md5_init( &state );

	// Calculate hash
	md5_append( &state, bytes, size_in_bytes );

	// Finish
	md5_finish( &state, md5_bytes );

	// Initialized
	bInit = true;

	// Return raw bytes
	return md5_bytes;
}

std::tstring Cmd5::md5s( const md5_byte_t* bytes, int size_in_bytes )
{
	// Convert to hex string
	bytes_to_hex( md5b( bytes, size_in_bytes ), md5_str );

	// Return hex string
	return md5_str;
}

void Cmd5::bytes_to_hex( const md5_byte_t* bytes, std::tstring& tstr )
{
	// Clear string
	tstr.clear();

	// Output is twice the length of input length
	tstr.resize( 16 * 2, ' ' );

	static const std::tstring hex_char = _T("0123456789abcdef");

	for (int i = 0; i < 16;++i)
	{
		// High
		tstr[i << 1] = hex_char[(bytes[i] >> 4) & 0x0F];
		// Low
		tstr[(i << 1) + 1] = hex_char[bytes[i] & 0x0F];
	}
}

void Cmd5::hex_to_bytes( const std::tstring& tstr, md5_byte_t* bytes )
{
	// Buffer for conversion
	static _TCHAR buf[3];
	buf[2] = 0;
	// End of input
	_TCHAR* pEnd = &buf[2];

	for (int i = 0, c = 0; i < 16;++i)
	{
		buf[0] = tstr[c++];

		if (!isxdigit(buf[0]))
		{
			throw std::invalid_argument("Invalid character in hex string");
		}

		buf[1] = tstr[c++];

		md5_byte_t x = static_cast< md5_byte_t >( _tcstoul( buf, &pEnd, 16 ) );

		// Parsing should stop at terminating zero
		if (pEnd != &buf[2])
		{
			throw std::invalid_argument("Invalid character in hex string");
		}

		bytes[i] = x;
	}

	md5_str = tstr;
}
