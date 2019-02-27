#pragma once
#ifndef __CMD5_INCLUDED__
#define __CMD5_INCLUDED__

// std::string, std::wstring
#include <string>
// vector
#include <vector>
// memset
#include <memory.h>
// _TCHAR
#include <tchar.h>
// md5 implementation
#include "md5.h"

#ifdef UNICODE
	namespace std{ typedef wstring tstring; }
#else
	namespace std{ typedef string tstring; }
#endif

class Cmd5
{
public:
	Cmd5();
	Cmd5( std::tstring tstrHash );
	~Cmd5();

	//
	const bool operator==( const Cmd5& rhs ) const;
	//
	const bool operator!=( const Cmd5& rhs ) const;

	// String must be NULL TERMINATED
	std::tstring md5OfString( const std::tstring& tstrStr );

	// Calculates md5 hash of file contents
	std::tstring md5OfFile( const std::tstring& tstrFileName );

	// Calculates salted md5 hash of file contents
	std::tstring md5OfFileWithSalt(const std::tstring& tstrFileName, const std::string& salt);

	// Calculates md5 hash of files contents
	std::tstring md5OfFiles( const std::vector< std::tstring >& v_tstrFileNames );

private:
	// Calculates md5 hash of raw bytes
	md5_byte_t* md5b( const md5_byte_t* bytes, int size_in_bytes );

	// Calculates md5 hash of raw bytes
	std::tstring md5s( const md5_byte_t* bytes, int size_in_bytes );

	// Converts md5 bytes to hex string
	void bytes_to_hex( const md5_byte_t* bytes, std::tstring& tstr );

	// Converts hex string to md5 bytes
	void hex_to_bytes( const std::tstring& tstr, md5_byte_t* bytes );

private:
	md5_byte_t		md5_bytes[16];
	std::tstring	md5_str;
	bool			bInit;
};

#endif /*__CMD5_INCLUDED__*/
