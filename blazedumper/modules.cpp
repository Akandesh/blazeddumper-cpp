#define NOMINMAX
#include <Windows.h>
#include <psapi.h>
#include <tchar.h>
#include <string>

#include "modules.hpp"
#include "config.hpp"

#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))

using tstring = std::basic_string<TCHAR>;

tstring format_error( const DWORD error ) {
	LPTSTR buffer = nullptr;
	const auto size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		error,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		reinterpret_cast< LPTSTR >( &buffer ),
		0,
		nullptr
	);
	// Even if FormatMessage fails and returns 0 this will still work. 
	const auto message = tstring{ buffer, size };
	// Free the buffer allocated by FormatMessage
	LocalFree( buffer );
	return message;
}


foreign_module::foreign_module( const TCHAR* mod ) {
	h_module = LoadLibraryEx( mod, nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS );

	if ( h_module == nullptr ) {
		// Missing dependencies? 
		if ( const auto error = GetLastError( ); error != ERROR_SUCCESS ) {
			throw std::runtime_error{
				format_error( error )
			};
		}
	}

	const auto result = GetModuleInformation(
		GetCurrentProcess( ),
		static_cast< HMODULE >( h_module ),
		&m_module_info,
		sizeof( m_module_info )
	);

	if ( result == NULL ) {
		if ( const auto error = GetLastError( ); error != ERROR_SUCCESS ) {
			throw std::runtime_error{
				format_error( error )
			};
		}
	}

	_tprintf( _T( "%s is loaded at 0x%p (size: 0x%lX)\n" ), mod, h_module, m_module_info.SizeOfImage );
}

const std::uint8_t* foreign_module::find_pattern( const settings::signature& sig ) const {
	auto result = reinterpret_cast< std::uint8_t* >( find_pattern( sig.pattern.c_str( ) ) );
	if ( !result ) {
		_tprintf( _T( "Did not find %s with sig %s\n" ), sig.name.c_str( ), sig.pattern.c_str( ) );
		_tprintf( _T( "loaded at 0x%p (size: 0x%lX)\n" ), h_module, m_module_info.SizeOfImage );
		Sleep( 10000 );
		return result;
	}

	for ( auto&& offset : sig.offsets ) {
		result = reinterpret_cast< std::uint8_t* >(
			*reinterpret_cast< std::uintptr_t* >( result + offset )
			);
	}

	result += sig.extra;

	if ( sig.relative ) {
		result -= reinterpret_cast< std::uintptr_t >( m_module_info.lpBaseOfDll );
	}

	_tprintf( _T( "%s: 0x%p\n" ), sig.name.c_str( ), result );

	return result;
}

std::uintptr_t foreign_module::find_pattern( const char* pattern ) const {
	auto pat = pattern;
	std::uintptr_t first_match = 0;
	const auto range_start = reinterpret_cast< std::uintptr_t >( m_module_info.lpBaseOfDll );
	const std::uintptr_t range_end = range_start + m_module_info.SizeOfImage;

	for ( DWORD cur = range_start; cur < range_end; cur++ ) {
		if ( *pat == '\0' ) {
			return first_match;
		}

		if ( *( PBYTE ) pat == '\?' || *( BYTE* ) cur == GET_BYTE( pat ) ) {
			if ( !first_match )
				first_match = cur;

			if ( !pat[ 2 ] )
				return first_match;

			if ( *( PWORD ) pat == 16191 || *( PBYTE ) pat != '\?' ) {
				pat += 3;
			} else {
				pat += 2;
			}
		} else {
			pat = pattern;
			first_match = 0;
		}
	}
	return 0u;
}

std::uintptr_t foreign_module::module_base( ) const {
	return reinterpret_cast< std::uintptr_t >( m_module_info.lpBaseOfDll );
}