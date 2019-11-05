#include <chrono>
#include "hpp.h"
#include "utils.h"
#include "config.hpp"

void hpp::dump()
{
	stream = std::ofstream( "dump\\csgo.hpp" );
	this->header( );
	stream << "namespace blazedumper {" << std::endl;
	this->timestamp( );
	this->netvars( );
	this->signatures( );
	stream << "} // namespace blazedumper" << std::endl;
	stream.close( );
}

void hpp::header()
{
	stream << "#pragma once" << std::endl;
	stream << "#include <cstdint>" << std::endl << std::endl;
	stream << "// " << utils::get_current_time() << std::endl << std::endl;
}

void hpp::netvars()
{
	stream << "namespace netvars {" << std::endl;
	for ( auto it = data[ "netvars" ].begin( ); it != data[ "netvars" ].end( ); ++it ) {
		stream << "constexpr ::std::ptrdiff_t " << it.key() << " = 0x" << std::hex << std::uppercase << int(it.value()) << ";" << std::endl;
	}
	stream << "} // namespace netvars" << std::endl;
}

void hpp::signatures()
{
	stream << "namespace signatures {" << std::endl;
	for ( auto it = data[ "signatures" ].begin( ); it != data[ "signatures" ].end( ); ++it ) {
		stream << "constexpr ::std::ptrdiff_t " << it.key( ) << " = 0x" << std::hex << std::uppercase << int( it.value( ) ) << ";" << std::endl;
	}
	stream << "} // namespace signatures" << std::endl;
}

void hpp::timestamp()
{
	stream << "constexpr ::std::int64_t timestamp = " << 
		std::chrono::system_clock::now( ).time_since_epoch( ).count( ) << std::endl; // hours since epoch
}



