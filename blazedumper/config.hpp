#pragma once
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;

namespace settings {
	struct netvar {
		std::string name;
		std::string prop;
		std::string table;
		std::int32_t offset;
	};

	struct signature {
		std::string name;
		std::string pattern;
		int extra;
		bool relative;
		std::string module;
		std::vector<int> offsets;

		// added to support alternative sigscan 
		std::string mask;
		std::vector<char> alt_pattern;
	};

	void to_json(json& j, const netvar& n);
	void from_json(const json& j, netvar& n);

	void to_json(json& j, const signature& s);
	void from_json(const json& j, signature& s);
}
