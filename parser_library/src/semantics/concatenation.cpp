#include "concatenation.h"

using namespace hlasm_plugin::parser_library::semantics;

char_str * concatenation_point::access_str() { return dynamic_cast<char_str*>(this); }

var_sym * concatenation_point::access_var() { return dynamic_cast<var_sym*>(this); }

dot * concatenation_point::access_dot() { return dynamic_cast<dot*>(this); }

equals * concatenation_point::access_equ()
{
	return dynamic_cast<equals*>(this);
}

sublist * concatenation_point::access_sub() { return dynamic_cast<sublist*>(this); }

char_str::char_str(std::string value) :value(std::move(value)) {}

concat_type char_str::get_type() const { return concat_type::STR; }

concat_point_ptr char_str::clone() const
{
	return std::make_unique<char_str>(*this);
}

var_sym::var_sym(std::string name, std::vector<antlr4::ParserRuleContext*> subscript, symbol_range range)
	: created(false), name(std::move(name)), range(range), subscript(std::move(subscript)) {}

var_sym::var_sym(concat_chain created_name, std::vector<antlr4::ParserRuleContext*> subscript, symbol_range range)
	: created(true), created_name(std::move(created_name)), range(range), subscript(std::move(subscript))  {}

var_sym::var_sym(const var_sym & variable_symbol)
	: created(variable_symbol.created), name(variable_symbol.name), range(variable_symbol.range), subscript(variable_symbol.subscript)
{
	created_name.insert(created_name.end(), make_clone_iterator(variable_symbol.created_name.begin()), make_clone_iterator(variable_symbol.created_name.end()));
}

var_sym& var_sym::operator=(const var_sym & variable_symbol)
{
	created = variable_symbol.created;
	name = variable_symbol.name;
	range = variable_symbol.range;

	created_name.insert(created_name.end(), make_clone_iterator(variable_symbol.created_name.begin()), make_clone_iterator(variable_symbol.created_name.end()));


	for (const auto& expr : variable_symbol.subscript)
		subscript.push_back(expr);

	return *this;
}

var_sym::var_sym() : created(false) {}

concat_type var_sym::get_type() const { return concat_type::VAR; }

concat_point_ptr var_sym::clone() const
{
	return std::make_unique<var_sym>(*this);
}

concat_type dot::get_type() const { return concat_type::DOT; }

concat_point_ptr dot::clone() const
{
	return std::make_unique<dot>();
}

concat_type equals::get_type() const { return concat_type::EQUALS; }

concat_point_ptr equals::clone() const
{
	return std::make_unique<equals>();
}

sublist::sublist(concat_chain list) : list(std::move(list)) {}

concat_type sublist::get_type() const { return concat_type::SUB; }

concat_point_ptr sublist::clone() const
{
	concat_chain new_chain;

	new_chain.insert(new_chain.end(), make_clone_iterator(list.begin()), make_clone_iterator(list.end()));

	return std::make_unique<sublist>(std::move(new_chain));
}

void hlasm_plugin::parser_library::semantics::clear_concat_chain(concat_chain & chain)
{
	size_t offset = 0;
	for (size_t i = 0; i < chain.size(); ++i)
	{
		if (chain[i] && !(chain[i]->get_type() == concat_type::STR && chain[i]->access_str()->value.empty())) //if not empty ptr and not empty string
			chain[offset++] = std::move(chain[i]);
	}

	chain.resize(offset);
}
