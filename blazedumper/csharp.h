#pragma once
#include <fstream>
#include "output.h"

class csharp : public output
{
	void header( );
	void timestamp( );
	void netvars( );
	void signatures( );
	std::ofstream stream;
public:
	void dump( ) override;
	csharp( nlohmann::json j ) { data = j; }
};