#pragma once
#include "utils.h"
#include <iomanip>
#include <sstream>


std::string utils::get_current_time()
{
	auto t = std::time( nullptr );
	auto tm = *std::localtime( &t );
	std::ostringstream oss;
	oss << std::put_time( &tm, "%d-%m-%Y %H:%M:%S" );
	return oss.str( );
}
