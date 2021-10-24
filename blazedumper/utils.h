#pragma once
#include <string>
#include "config.hpp"

class utils
{
public:
	static std::string get_current_time( );
	static void fix_signature( settings::signature & sig );
};
