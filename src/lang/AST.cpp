#include "AST.hpp"
#include "../code_gen/utilities.hpp"
#include <cassert>
#include <iostream>
#include <regex>

std::vector<std::string> enum_names{
	"T_IDENTIFIER", "T_CONSTANT", "T_STRING_LITERAL", "T_SIZEOF", "T_PTR_OP", "T_INC_OP", "T_DEC_OP", "T_LEFT_OP", "T_RIGHT_OP", "T_LE_OP", "T_GE_OP",
	"T_EQ_OP", "T_NE_OP", "T_AND_OP", "T_OR_OP", "T_MUL_ASSIGN", "T_DIV_ASSIGN", "T_MOD_ASSIGN", "T_ADD_ASSIGN", "T_SUB_ASSIGN", "T_LEFT_ASSIGN",
	"T_RIGHT_ASSIGN", "T_AND_ASSIGN", "T_XOR_ASSIGN", "T_OR_ASSIGN", "T_TYPE_NAME", "T_TYPEDEF", "T_EXTERN", "T_STATIC", "T_AUTO", "T_REGISTER",
	"T_INLINE", "T_RESTRICT", "T_CHAR", "T_SHORT", "T_INT", "T_LONG", "T_SIGNED", "T_UNSIGNED", "T_FLOAT", "T_DOUBLE", "T_CONST", "T_VOLATILE",
	"T_VOID", "T_BOOL", "T_COMPLEX", "T_IMAGINARY", "T_STRUCT", "T_UNION", "T_ENUM", "T_ELLIPSIS", "T_CASE", "T_DEFAULT", "T_IF", "T_ELSE",
	"T_SWITCH", "T_WHILE", "T_DO", "T_FOR", "T_GOTO", "T_CONTINUE", "T_BREAK", "T_RETURN", "T_LEFT_BRACKET", "T_RIGHT_BRACKET",
	"T_LEFT_SQUARE_BRACKET", "T_RIGHT_SQUARE_BRACKET", "T_FULL_STOP", "T_LEFT_CURLY_BRACKET", "T_RIGHT_CURLY_BRACKET", "T_COMMA", "T_AMPERSAND",
	"T_STAR", "T_PLUS", "T_MINUS", "T_TILDE", "T_EXCLAMATION_MARK", "T_FORWARD_SLASH", "T_PERCENT", "T_LEFT_ANGLE_BRACKET", "T_RIGHT_ANGLE_BRACKET",
	"T_CARET", "T_PIPE", "T_QUESTION_MARK", "T_COLON", "T_EQUAL_SIGN", "T_SEMICOLON",
	// Nonterminals
	"primary_expression", "postfix_expression", "argument_expression_list", "unary_expression", "unary_operator", "cast_expression",
	"multiplicative_expression", "additive_expression", "shift_expression", "relational_expression", "equality_expression", "and_expression",
	"exclusive_or_expression", "inclusive_or_expression", "logical_and_expression", "logical_or_expression", "conditional_expression",
	"assignment_expression", "assignment_operator", "expression", "constant_expression", "declaration", "declaration_specifiers",
	"init_declarator_list", "init_declarator", "storage_class_specifier", "type_specifier", "struct_or_union_specifier", "struct_or_union",
	"struct_declaration_list", "struct_declaration", "specifier_qualifier_list", "struct_declarator_list", "struct_declarator", "enum_specifier",
	"enumerator_list", "enumerator", "type_qualifier", "function_specifier", "declarator", "direct_declarator", "pointer", "type_qualifier_list",
	"parameter_type_list", "parameter_list", "parameter_declaration", "identifier_list", "type_name", "abstract_declarator",
	"direct_abstract_declarator", "initializer", "initializer_list", "designation", "designator_list", "designator", "statement", "labeled_statement",
	"compound_statement", "block_item_list", "block_item", "expression_statement", "selection_statement", "iteration_statement", "jump_statement",
	"translation_unit", "external_declaration", "function_definition", "declaration_list" };

AST_node::AST_node(std::initializer_list<AST_node*> child_nodes) : child_nodes(child_nodes)
{
}

AST_node::AST_node(const Token& token_type, std::initializer_list<AST_node*> child_nodes) : token_type(token_type), child_nodes(child_nodes)
{
	// handle declarations
	if (token_type == Token::declaration)
	{	
		// empty declaration
		if (child_nodes.size() > 1) {
			
		// typdef spec
			if (this->child_nodes[0]->child_nodes[0]->token_type == Token::T_TYPEDEF)
			{
				AST_declaration_specifiers * specifiers = dynamic_cast<AST_declaration_specifiers *>(this->child_nodes[0]->child_nodes[1]);

				AST_declarator * decl = dynamic_cast<AST_declarator *>(this->child_nodes[1]->child_nodes[0]);
				std::vector<AST_declarator::Declarator_Type> dec_vec = decl->getTypeVector();
				const Variable_Type * alias_for = specifiers->getVariableType();
				auto cursor_array_sizes = decl->array_sizes.rbegin();
				for(auto mod : dec_vec ){
					if(mod==AST_declarator::Declarator_Type::POINTER){
						alias_for = Type_Manager::getPointerType(alias_for);
					}
					else if(mod==AST_declarator::Declarator_Type::ARRAY){
						alias_for = Type_Manager::getArrayType(alias_for,*cursor_array_sizes);
						cursor_array_sizes++;
					}
				}
				Type_Manager::inventAlias(decl->getLiteral()->getCleanLiteral(), alias_for);

			}
		}
	}
}

AST_node::~AST_node()
{
	for (auto child : child_nodes)
	{
		delete child;
	}
} // virtual

AST_literal::AST_literal(const Token& token_type, const std::string* const literal) : AST_node(token_type, {}), literal(literal)
{
	// FIXME Remove any prefixs or suffixes
	clean_literal = *literal;
}
std::string AST_literal::getCleanLiteral() const{
	return clean_literal;
}

AST_literal::~AST_literal()
{
	delete literal;
} // virtual

void AST_literal::loadIntoRegister(std::ostream& output) const
{
	const std::string tab("\t");
	switch (this->getLiteralType()->getType())
	{
	case Variable_Type_Built_In::Built_In::CHAR: {
		int c;
		if(clean_literal.length() == 4){
			switch(clean_literal[2]){
				case 'n':
					c = (int)'\n';
					break;
				case 't':
					c = (int)'\t';
					break;
				case 'v':
					c = (int)'\v';
					break;
				case 'b':
					c = (int)'\b';
					break;
				case 'r':
					c = (int)'\r';
					break;
				case 'a':
					c = (int)'\a';
					break;
				default:
					c = (int)clean_literal[2];
					break;
			}
		}
		else{
			c = clean_literal[1];
		}
		output << tab << "li	$t0, " << c << std::endl;
		break;
	}
	case Variable_Type_Built_In::Built_In::UNSIGNED_INT :
	case Variable_Type_Built_In::Built_In::INT: {
		unsigned upper_val = unsigned(std::stoul(clean_literal)) >> 16;
		unsigned lower_val = unsigned((std::stoul(clean_literal)) << 16) >> 16;
		std::string upper = std::to_string(upper_val);
		std::string lower = std::to_string(lower_val);
		output << tab << "move		$t0, $0" << std::endl;
		output << tab << "lui		$t0, " << upper << std::endl;
		output << tab << "ori		$t0, " << lower << std::endl;
		break;
	}
	case Variable_Type_Built_In::Built_In::FLOAT : {
		float to_load = std::stof(clean_literal);
		output << tab << "li.s $f4, " << to_load << std::endl;
		break;
	}
	case Variable_Type_Built_In::Built_In::DOUBLE : {
		double to_load = std::stod(clean_literal);
		output << tab << "li.d $f4, " << to_load << std::endl;
		break;
	}
	default: {
		assert(0 && "Not yet supported loading that type into register");
		break;
	}
	}
}

const Variable_Type_Built_In* AST_literal::getLiteralType() const
{
	/*
		Integer Literals:
		INT
		UNSIGNED_INT
		LONG
		UNSIGNED_LONG

		Float Literals
		FLOAT
		DOUBLE

		Character Literals
		CHAR
	*/
	std::string test = *literal;
	std::string hexLiteral = "0[xX][a-fA-F0-9]+";
	std::string octLiteral = "0[0-9]+";
	std::string decimalLiteral = "[0-9]+";
	std::string integerLiteral = "(" + hexLiteral + ")|(" + octLiteral + ")|(" + decimalLiteral + ")";

	std::regex RE_INT(integerLiteral);
	std::regex RE_LONG(integerLiteral + "[lL]");
	std::regex RE_UINT(integerLiteral + "[uU]");
	std::regex RE_ULONG(integerLiteral + "([uU][lL])|([lL][uU])");

	std::string exponentialLiteral = "[0-9]+[Ee][+-]?[0-9]+";
	std::string floatLiteral1 = "[0-9]*\.[0-9]+([Ee][+-]?[0-9]+)?";
	std::string floatLiteral2 = "[0-9]+\.[0-9]*([Ee][+-]?[0-9]+)?";
	std::string floatLiteral = "(" + exponentialLiteral + ")|(" + floatLiteral1 + ")|(" + floatLiteral2 + ")";

	std::regex RE_DOUBLE(floatLiteral + "[lL]?");
	std::regex RE_FLOAT(floatLiteral + "[fF]");

	std::regex RE_CHAR("(\\.|[^\\'])+");

	if (literal == nullptr) {
		// FIXME - not sure when this happens and what to do
		return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
	}

	if (std::regex_match(*literal, RE_INT)) {
		return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
	}
	else if (std::regex_match(*literal, RE_LONG)) {
		return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::LONG);
	}
	else if (std::regex_match(*literal, RE_UINT)) {
		return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
	}
	else if (std::regex_match(*literal, RE_ULONG)) {
		return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_LONG);
	}
	else if (std::regex_match(*literal, RE_DOUBLE)) {
		return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE);
	}
	else if (std::regex_match(*literal, RE_FLOAT)) {
		return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT);
	}
	else if (std::regex_match(*literal, RE_CHAR)) {
		return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR);
	}
	else {
		// REVIEW - unsure about type of literal
		return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR);
	}
}

AST_function::AST_function(std::initializer_list<AST_node*> child_nodes) : AST_node(Token::function_definition, child_nodes)
{
	//NOTE determine return type
	const Variable_Type * return_type = nullptr;
	if(AST_declaration_specifiers * spec = dynamic_cast<AST_declaration_specifiers*>(this->child_nodes[0]); spec){
		return_type = spec->getVariableType();
	}
	assert(return_type && "Could not find return type of function, declaration specifiers not found");
	const AST_declarator * const dec = dynamic_cast<const AST_declarator *>(this->child_nodes[1]);
	if(!dec){
		assert(0 && "Could not find function declarator");
	}
	std::vector<AST_declarator::Declarator_Type> type_vec = dec->getTypeVector();
	for(int i = 0; i < type_vec.size() && (type_vec[i] == AST_declarator::Declarator_Type::POINTER); i++){
		return_type = Type_Manager::getPointerType(return_type);
	}
	auto lit = dec->getLiteral();
	std::string function_name = *lit->literal;
	this->function_declaration = Type_Manager::inventFunction(function_name, return_type, dec->getParameterList());
}

const Function_Declaration * AST_function::getFunctionDeclaration() const{
	return this->function_declaration;
}

const std::string* AST_function::getFunctionName() const
{
	if (this->token_type == Token::function_definition)
	{
		const AST_literal* const identifier = dynamic_cast<AST_literal*>(this->child_nodes[1]->child_nodes[0]->child_nodes[0]->child_nodes[0]);
		if (identifier != nullptr)
		{
			return identifier->literal;
		}
	}
	assert(false && "Could not resolve function name");
	return nullptr;
}

const Variable_Type* AST_function::getReturnType() const
{
	return return_type;
}
AST_function::~AST_function() {

}
const Variable_Type* AST_declaration_specifiers::getVariableType() const {
	return variable_type;
}
AST_declaration_specifiers::AST_declaration_specifiers(std::initializer_list<AST_node*> child_nodes) : AST_node(Token::declaration_specifiers, child_nodes) {
	function_TL.push_back(this->child_nodes[0]->token_type);
	if(this->child_nodes.size() >= 2){
		TL function_TL_lower = dynamic_cast<const AST_declaration_specifiers *>(this->child_nodes[1])->function_TL;
		function_TL.insert(function_TL.end(), function_TL_lower.begin(), function_TL_lower.end());
	}
	
	if (function_TL == TL{ Token::T_CHAR }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR);
	}
	else if (function_TL == TL{ Token::T_SIGNED, Token::T_CHAR }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::SIGNED_CHAR);
	}
	else if (function_TL == TL{ Token::T_SHORT }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::SHORT);
	}
	else if (function_TL == TL{ Token::T_UNSIGNED, Token::T_SHORT }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_SHORT);
	}
	else if (function_TL == TL{ Token::T_INT }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
	}
	else if (function_TL == TL{ Token::T_LONG }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::LONG);
	}
	else if (function_TL == TL{ Token::T_UNSIGNED}) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
	}
	else if (function_TL == TL{ Token::T_UNSIGNED, Token::T_INT }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
	}
	else if (function_TL == TL{ Token::T_UNSIGNED, Token::T_LONG }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_LONG);
	}
	else if (function_TL == TL{ Token::T_FLOAT }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT);
	}
	else if (function_TL == TL{ Token::T_DOUBLE }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE);
	}
	else if (function_TL == TL{ Token::T_LONG, Token::T_DOUBLE }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::LONG_DOUBLE);
	}
	else if (function_TL == TL{ Token::T_LONG, Token::T_DOUBLE }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::LONG_DOUBLE);
	}
	else if (function_TL == TL{ Token::T_VOID }) {
		this->variable_type = Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::VOID);
	}
	else {
		if(this->child_nodes[0]->token_type==Token::T_TYPE_NAME){
			const AST_literal * const type_name = dynamic_cast<const AST_literal *>(this->child_nodes[0]);
			assert(type_name && "Type_name node not a literal");
			this->variable_type = Type_Manager::getAlias(type_name->getCleanLiteral());
		}
	}
}

AST_declaration_specifiers::~AST_declaration_specifiers() {
}


std::vector<AST_declarator::Declarator_Type> AST_declarator::getTypeVector() const {
	return type;
}
int descendPointer(const AST_node * node ){
	assert(node->token_type == Token::pointer && "Passed in non-pointer");
	if(node->child_nodes.size()==2){
		return 1 + descendPointer(node->child_nodes[1]);
	}
	return 1;
}
void descendDirectDeclarator(const AST_node * const direct_dec, const AST_literal ** literal, std::vector<AST_declarator::Declarator_Type> *type_vector, std::vector<int> *array_sizes, std::vector<std::pair<const Variable_Type *, std::string>> * const param_list){
	assert(direct_dec->token_type==Token::direct_declarator && "Passed non-direct declarator");
	if(direct_dec->child_nodes[0]->token_type == Token::T_IDENTIFIER){
		*literal = dynamic_cast<const AST_literal *>(direct_dec->child_nodes[0]);
	}
	else if(direct_dec->child_nodes.size()==3 && direct_dec->child_nodes[0]->token_type==Token::T_LEFT_BRACKET){
		const AST_declarator * const dec = dynamic_cast<AST_declarator *>(direct_dec->child_nodes[1]);
		if(dec){
			std::vector<AST_declarator::Declarator_Type> sub_type_vector = dec->getTypeVector();
			type_vector->insert(type_vector->end(), sub_type_vector.begin(),sub_type_vector.end());
		}
		else{
			assert(0 && "Malformed direct declarator");
		}
	}
	else if(direct_dec->child_nodes.size()==4 || (direct_dec->child_nodes.size()==3 && direct_dec->child_nodes[1]->token_type==Token::T_LEFT_BRACKET)){
		assert(direct_dec->child_nodes[0]->token_type==Token::direct_declarator && "Malformed direct declarator");
		descendDirectDeclarator(direct_dec->child_nodes[0], literal, type_vector, array_sizes, param_list);
		if(direct_dec->child_nodes[1]->token_type == Token::T_LEFT_SQUARE_BRACKET){
			//REVIEW should change this to allow compile time constants for arrays
			type_vector->push_back(AST_declarator::Declarator_Type::ARRAY);
			array_sizes->push_back(std::stoi(*dynamic_cast<const AST_literal *>(direct_dec->child_nodes[2]->child_nodes[0])->literal));
		}

		else if(direct_dec->child_nodes[1]->token_type == Token::T_LEFT_BRACKET){
			type_vector->push_back(AST_declarator::Declarator_Type::FUNCTION);
			if(auto param_list_node = dynamic_cast<const AST_parameter_list *>(direct_dec->child_nodes[2])){
				*param_list = param_list_node->getParameterList();
			}
		}

	}
	
}

AST_declarator::AST_declarator(std::initializer_list<AST_node *> child_nodes) : AST_node(Token::declarator,child_nodes){
	descendDirectDeclarator(this->child_nodes.back(),&this->identifier_literal,&this->type, &this->array_sizes, &this->parameter_list);
	if(this->child_nodes[0]->token_type==Token::pointer){
		int pointer_depth = descendPointer(this->child_nodes[0]);
		for(int d = 0; d < pointer_depth; d++){
			this->type.push_back(AST_declarator::Declarator_Type::POINTER);
		}
	}
	assert(this->identifier_literal);
}
std::vector<std::pair<const Variable_Type*, std::string>> AST_declarator::getParameterList() const{
	if(type[0] != AST_declarator::Declarator_Type::FUNCTION){
		assert(0 && "Attempted to get parameter list from non function type");
	}
	for(auto n : this->child_nodes.back()->child_nodes){
		if(n->token_type == Token::parameter_list){
			const AST_parameter_list * param_list =  dynamic_cast<const AST_parameter_list *>(n);
			return param_list->getParameterList();
		}
	}
	//NOTE no parameter list
	return std::vector<std::pair<const Variable_Type*, std::string>>();
}

const AST_literal* AST_declarator::getLiteral() const {
	return this->identifier_literal;
}

AST_expression::AST_expression(Token expression_type, std::initializer_list<AST_node*> child_nodes) : AST_node(expression_type, child_nodes) {

}


AST_parameter_list::AST_parameter_list(std::initializer_list<AST_node *> child_nodes) : AST_node(Token::parameter_list, child_nodes) {
	int i = 0;
	if(this->child_nodes[i]->token_type == Token::parameter_list){
		parameter_list = dynamic_cast<const AST_parameter_list *>(this->child_nodes[i])->getParameterList();
		i++;
	}
	assert(this->child_nodes[i]->token_type == Token::parameter_declaration && "Malformed parameter list");
	const Variable_Type * var_type = dynamic_cast<const AST_declaration_specifiers *>(this->child_nodes[i]->child_nodes[0])->getVariableType();
	const AST_declarator * const declarator = dynamic_cast<const AST_declarator *>(this->child_nodes[i]->child_nodes[1]);
	std::string name = *declarator->getLiteral()->literal;
	std::vector<AST_declarator::Declarator_Type> type_vec = declarator->getTypeVector();
	for(int i = 0; i < type_vec.size() && (type_vec[i] == AST_declarator::Declarator_Type::POINTER || type_vec[i] == AST_declarator::Declarator_Type::ARRAY);i++){
		var_type = Type_Manager::getPointerType(var_type);
	}
	parameter_list.push_back(std::pair<const Variable_Type *, std::string>(var_type, name));
}

std::vector<std::pair<const Variable_Type *, std::string>> AST_parameter_list::getParameterList() const{
	return this->parameter_list;
}
