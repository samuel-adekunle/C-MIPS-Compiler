#ifndef GUARD_AST_HPP
#define GUARD_AST_HPP

#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>
#include "../code_gen/types.hpp"

extern std::vector<std::string> enum_names;
// non-terminals

class AST_node
{
protected:
	AST_node(std::initializer_list<AST_node*> child_nodes);

public:
	Token token_type;
	std::vector<AST_node*> child_nodes;

	AST_node(const Token& token_type, std::initializer_list<AST_node*> child_nodes);

	virtual ~AST_node();
	// virtual
};
class AST_declaration_specifiers : public AST_node {
private:
	using TL = std::vector<Token>;
	TL function_TL;
	const Variable_Type* variable_type;
public:
	const Variable_Type* getVariableType() const;
	AST_declaration_specifiers(std::initializer_list<AST_node*> child_nodes);

	virtual ~AST_declaration_specifiers();
};



class AST_function : public AST_node
{
	const Function_Declaration * function_declaration;
	const Variable_Type* return_type;

public:
	AST_function(std::initializer_list<AST_node*> child_nodes);

	virtual ~AST_function();

	const Function_Declaration * getFunctionDeclaration() const;
	const std::string* getFunctionName() const;
	const Variable_Type* getReturnType() const;
};

class AST_literal : public AST_node
{
	std::string clean_literal;
	const Variable_Type_Built_In* literal_type;
public:
	const std::string* const literal;
	AST_literal(const Token& token_type, const std::string* const literal);
	std::string getCleanLiteral() const;
	virtual ~AST_literal();

	// NOTE 	Here we are parsing the literal to determine its type.
	const Variable_Type_Built_In* getLiteralType() const;
	void loadIntoRegister(std::ostream& output) const;
};

class AST_declarator : public AST_node {
public:
	enum Declarator_Type {
		FUNCTION, POINTER, VARIABLE, ARRAY
	};
	std::vector<Declarator_Type> getTypeVector() const;
	AST_declarator(std::initializer_list<AST_node *> child_nodes);
	const AST_literal * getLiteral() const;
	std::vector<std::pair<const Variable_Type *, std::string>> getParameterList() const;
	virtual ~AST_declarator() =default;
	std::vector<int> array_sizes;

private:
	std::vector<Declarator_Type> type; 
	std::vector<std::pair<const Variable_Type *, std::string>> parameter_list;
	const AST_literal* identifier_literal;
	
};
class AST_parameter_list : public AST_node {
	std::vector<std::pair<const Variable_Type *, std::string>> parameter_list;
public:
	AST_parameter_list(std::initializer_list<AST_node *> child_nodes);
	std::vector<std::pair<const Variable_Type *, std::string>> getParameterList() const;
};

class Function_Scope;
class AST_expression : public AST_node {
public:
	AST_expression(Token expression_type, std::initializer_list<AST_node*> child_nodes);
	void pushLValueAbsoluteOffsetToStack(Function_Scope* const current_function_scope, std::ostream& output) const;
	const Variable_Type* getType(const Function_Scope* const current_function_scope) const;
};

extern std::unordered_map<std::string, const Variable_Type*> user_defined_types;
using Type_Table_Pair = std::pair<std::string, const Variable_Type*>;

#endif