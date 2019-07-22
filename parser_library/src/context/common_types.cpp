#include "common_types.h"
#include <cctype>

namespace hlasm_plugin::parser_library::context
{

std::string & to_upper(std::string & s)
{
	for (auto & c : s) c = static_cast<char>(std::toupper(c));
	return s;
}

}