#pragma once
#include "output.h"
#include <fstream>

class hpp : public output
{
	void header( );
	void timestamp( );
	void netvars( );
	void signatures( );
	std::ofstream stream;
public:
	void dump() override;
	hpp( nlohmann::json j ) { data = j; }
};
