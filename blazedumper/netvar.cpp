#include "netvar.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <tchar.h>

// Also pasta'd from CSGOSimple but unAIDS'd™

netvar_table::netvar_table(RecvTable* table, std::int32_t offset, RecvProp* prop): m_prop(prop), m_offset(offset) {
	m_name = table->m_pNetTableName;

	for (auto i = 0; i < table->m_nProps; ++i) {
		const auto property = &table->m_pProps[i];

		if (property == nullptr || isdigit(property->m_pVarName[0])) {
			continue;
		}
		if (strcmp("baseclass", property->m_pVarName) == 0) {
			continue;
		}

		if (property->m_RecvType == DPT_DataTable && property->m_pDataTable) {
			m_child_tables.emplace_back(property->m_pDataTable, property->m_Offset, property);
		}
		else {
			m_child_props.emplace_back(property);
		}
	}
}

void netvar_table::dump(std::ostream& stream, std::uint32_t indentation, std::int32_t offset) const {
	char line_buffer[1024];

	for (const auto& prop : m_child_props) {
		sprintf_s(line_buffer, "%*c%*s: 0x%08X", indentation * 4, ' ', -(50 - (int)indentation * 4), prop->m_pVarName,
		          offset + m_offset + prop->m_Offset);
		stream << line_buffer << '\n';
	}
	for (const auto& child : m_child_tables) {
		sprintf_s(line_buffer, "%*c%*s: 0x%08X", indentation * 4, ' ', -(50 - (int)indentation * 4),
		          child.m_prop->m_pVarName, offset + m_offset + child.m_offset);
		stream << line_buffer << '\n';
		child.dump(stream, indentation + 1, offset + m_offset);
	}
}

netvar_system::netvar_system(ClientClass* client_classes) {
	for (auto current = client_classes; current != nullptr; current = current->m_pNext) {
		m_tables.emplace_back(current->m_pRecvTable);
	}
}

std::int32_t netvar_system::get_offset(const std::string& table, const std::string& prop) {
	auto result = std::find_if(m_tables.begin(), m_tables.end(), [&table](const auto& netvar_table) {
		return netvar_table.m_name == table;
	});

	if(result == m_tables.end()) {
		return 0;
	}

	const auto offset = get_offset( *result, prop );
	if (!offset)
		_tprintf( _T( "Unable to find %s in %s unless it's supposed to be %i\n" ), prop.c_str(), table.c_str(), offset );
	return offset;
}

std::int32_t netvar_system::get_offset(const netvar_table& table, const std::string& prop_name) const {
	for (const auto& property : table.m_child_props) {
		if (strcmp(property->m_pVarName, prop_name.data()) == 0) {
			return table.m_offset + property->m_Offset;
		}
	}
	
	for (const auto& child : table.m_child_tables) {
		if (const auto offset = get_offset(child, prop_name); offset != 0) {
			return table.m_offset + offset;
		}
	}

	for (const auto& child : table.m_child_tables) {
		if (strcmp(child.m_prop->m_pVarName, prop_name.data()) == 0) {
			return table.m_offset + child.m_offset;
		}
	}

	return 0;
}

void netvar_system::dump() {
	auto outfile = std::ofstream("netvar_dump.txt");
	for (auto&& table : m_tables) {
		if (table.m_child_props.empty() && table.m_child_tables.empty()) {
			continue;
		}
		outfile << table.m_name << '\n';
		table.dump(outfile);
		outfile << '\n';
	}
	outfile << std::endl;
}
