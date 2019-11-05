#pragma once
#include <vector>
#include <string>

#include "valve.hpp"

// Pasta'd from CS:GO Simple

class netvar_table final {
public:
	explicit netvar_table(RecvTable* table, std::int32_t offset = 0, RecvProp* prop = nullptr);

	void dump(std::ostream& stream, std::uint32_t indentation = 1, std::int32_t offset = 0) const;

	std::string					m_name;
	RecvProp*					m_prop;
	std::uint32_t				m_offset;
	std::vector<RecvProp*>		m_child_props;
	std::vector<netvar_table>	m_child_tables;
};

class netvar_system final {
public:
	explicit netvar_system(ClientClass* client_classes);

	std::int32_t get_offset(const std::string& table, const std::string& prop);
	std::int32_t get_offset(const netvar_table& table, const std::string& prop) const;
	void dump();

private:
	std::vector<netvar_table> m_tables;
};