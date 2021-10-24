#include "utils.h"
#include <iomanip>
#include <sstream>
#include <regex>

std::string utils::get_current_time()
{
	auto t = std::time( nullptr );
	auto tm = *std::localtime( &t );
	std::ostringstream oss;
	oss << std::put_time( &tm, "%d-%m-%Y %H:%M:%S" );
	return oss.str( );
}

std::vector<char> hex_to_bytes( const std::string & hex ) {
	std::vector<char> bytes;

	for ( unsigned int i = 0; i < hex.length( ); i += 2 ) {
		std::string byteString = hex.substr( i, 2 );
		char byte = ( char )strtol( byteString.c_str( ), NULL, 16 );
		bytes.push_back( byte );
	}

	return bytes;
}

void utils::fix_signature( settings::signature & sig ) {
	static std::regex regexp( R"(([\d\w]{2}))" );
	static std::regex question( R"(\?)" );
	static std::regex mask_x( R"(\\x(?!00)[\w\d]{2})" ); // ignore cases where its 00
	static std::regex spacelol( R"( )" );

	auto sc = sig.pattern;
	sc = std::regex_replace( sc, question, "00" );
	sc = std::regex_replace( sc, spacelol, "" );

	sig.alt_pattern = hex_to_bytes( sc );

	sc = sig.pattern;
	sc = std::regex_replace( sc, regexp, "x" );
	sc = std::regex_replace( sc, spacelol, "" );
	
	sig.mask = sc;
}
