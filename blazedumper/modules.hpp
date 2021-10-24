#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <cstdint>
#include <algorithm>
#include <map>
#include <vector>
#include <deque>

using span = std::basic_string_view<std::uint8_t>;

namespace settings
{
    struct signature;
}

class foreign_module final {
public:
    explicit foreign_module( const TCHAR * mod );

    const std::uint8_t * find_pattern( const settings::signature & sig );


    std::uintptr_t find_pattern( const char * pattern ) const;


    std::uint8_t * find_pattern2( const std::vector<char>& pattern, const std::string & mask );

    template <std::size_t N>
    const std::uint8_t * find_pattern( const char( &pattern )[ N ], const char( &mask )[ N ] ) {
        // src by birdd: https://www.unknowncheats.me/forum/c-and-c-/271496-pattern-match-super-supoer-fast.html#post2072470
        const auto module = span{
            static_cast< std::uint8_t * >( m_module_info.lpBaseOfDll ),
            m_module_info.SizeOfImage
        };

        std::array<std::pair<char, char>, N - 1> pattern_data;

        // Compile pattern
        for ( auto i = 0u; i < N - 1; i++ ) {
            pattern_data[ i ] = std::make_pair( pattern[ i ], mask[ i ] );
        }

        const auto result = std::search( module.begin( ), module.end( ), pattern_data.begin( ), pattern_data.end( ),
            []( const char left, const std::pair<char, char> right ) -> bool {
                return ( right.second == '?' || left == right.first );
            } );

        return ( result == module.end( ) ) ? nullptr : result.operator->( );
    }

    std::uintptr_t module_base( ) const;

private:
    HANDLE h_module = nullptr;
    MODULEINFO m_module_info{};
};
