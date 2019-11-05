#pragma once
#include "nlohmann/json.hpp"

class output
{
public:
	virtual ~output() = default;
	virtual void dump( ) = 0;
	nlohmann::json data;
};
