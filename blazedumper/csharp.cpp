#include <chrono>
#include "csharp.h"
#include "utils.h"


void csharp::dump()
{
	stream = std::ofstream( "dump\\csgo.cs" );
	this->header( );
	stream << "namespace blazedumper {" << std::endl;
	this->timestamp( );
	this->netvars( );
	this->signatures( );
	stream << "} // namespace blazedumper" << std::endl;
	stream.close( );
}

void csharp::header()
{
	stream << "using System;" << std::endl;
	stream << "// " << utils::get_current_time( ) << std::endl << std::endl;
}

void csharp::timestamp()
{
	stream << "    public const Int32 timestamp = " <<
		std::chrono::system_clock::now( ).time_since_epoch( ).count( ) << std::endl; // hours since epoch
}

void csharp::netvars()
{
	stream << "    public static class netvars" << std::endl << "    {" << std::endl;
	for ( auto it = data[ "netvars" ].begin( ); it != data[ "netvars" ].end( ); ++it ) {
		stream << "        public const Int32 " << it.key( ) << " = 0x" << std::hex << std::uppercase << int( it.value( ) ) << ";" << std::endl;
	}
	stream << "    }" << std::endl;
}

void csharp::signatures()
{
	stream << "    public static class signatures" << std::endl << "    {" << std::endl;
	for ( auto it = data[ "signatures" ].begin( ); it != data[ "signatures" ].end( ); ++it ) {
		stream << "        public const Int32 " << it.key( ) << " = 0x" << std::hex << std::uppercase << int( it.value( ) ) << ";" << std::endl;
	}
	stream << "    }" << std::endl;
}
