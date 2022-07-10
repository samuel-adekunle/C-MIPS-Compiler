#include "code_gen.hpp"
#include "constants.hpp"
#include "scope.hpp"
#include "types.hpp"
#include "utilities.hpp"
#include <cassert>
#include <list>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <numeric>

std::string uniqueLabel(std::string base) {
	static int num = 1;
	return "$" + base + std::to_string(num++);
}
void descendInitializer(std::ostream& output, const AST_node * node, Function_Scope * function_scope);
void descendInitializerList(std::ostream& output, const AST_node * node, Function_Scope * function_scope){
	if(node->child_nodes[2]->token_type==Token::initializer){
		descendInitializer(output, node->child_nodes[2], function_scope);
	}
	else if(node->child_nodes[2]->token_type==Token::initializer_list){
		descendInitializerList(output, node->child_nodes[2], function_scope);
	}
	else{
		compile(node->child_nodes[2], output);
	}
	if(node->child_nodes[0]->token_type==Token::initializer){
		descendInitializer(output, node->child_nodes[0], function_scope);
	}
	if(node->child_nodes[0]->token_type==Token::initializer_list){
		descendInitializerList(output, node->child_nodes[0], function_scope);
	}
	else{
		compile( node->child_nodes[0], output);
	}
}
void descendInitializer(std::ostream& output, const AST_node * node, Function_Scope * function_scope){
	assert(node->token_type==Token::initializer);
	if(node->child_nodes.size() >= 2 && node->child_nodes[1]->token_type==Token::initializer_list){
		descendInitializerList(output,node->child_nodes[1], function_scope);
	}
	else {
		compile( node->child_nodes[1], output);
	}
}
void compile(const AST_node* node, std::ostream& output)
{
	// static bool in_function = false;
	static Function_Scope* current_function_scope = nullptr;

	switch (node->token_type)
	{
	case Token::initializer : {
		const Variable * const start_init = current_function_scope->getLastVariable();
		descendInitializer(output, node, current_function_scope);
		while(start_init != current_function_scope->getLastVariable()){
			current_function_scope->popStack();
		}
		break;
	}
	case Token::T_TYPE_NAME:{
		const AST_literal * const lit = dynamic_cast<const AST_literal *>(node);
		loadImmediateRegister1(output, Type_Manager::getAlias(lit->getCleanLiteral())->getSizeOf());
		current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName("size_of"), Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))->moveFromRegister1(output);
		break;
	}
	case Token::T_INT:{
		loadImmediateRegister1(output, INT_SIZE);
		current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName("size_of"), Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))->moveFromRegister1(output);
		break;
	}
	case Token::T_UNSIGNED:{
		loadImmediateRegister1(output, INT_SIZE);
		current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName("size_of"), Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))->moveFromRegister1(output);
		break;
	}
	case Token::T_CHAR:{
		loadImmediateRegister1(output, CHAR_SIZE);
		current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName("size_of"), Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))->moveFromRegister1(output);
		break;
	}
	case Token::T_SHORT:{
		loadImmediateRegister1(output, SHORT_SIZE);
		current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName("size_of"), Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))->moveFromRegister1(output);
		break;
	}
	case Token::T_FLOAT:{
		loadImmediateRegister1(output, FLOAT_SIZE);
		current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName("size_of"), Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))->moveFromRegister1(output);
		break;
	}
	case Token::T_DOUBLE:{
		loadImmediateRegister1(output, DOUBLE_SIZE);
		current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName("size_of"), Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))->moveFromRegister1(output);
		break;
	}
	case Token::T_LONG:{
		loadImmediateRegister1(output, LONG_SIZE);
		current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName("size_of"), Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))->moveFromRegister1(output);
		break;
	}
	case Token::T_STRING_LITERAL: {
		const AST_literal * const lit = dynamic_cast<const AST_literal *>(node);
		assert(lit && "Malformed string literal");
		const Variable * character;
		std::vector<char> list;
		for(int i = 1; i < lit->literal->length()-1; i++){
			char c = lit->getCleanLiteral()[i];
			if('\\'==c){
				switch(lit->getCleanLiteral()[i+1]){
				case 'n':
					c = '\n';
					break;
				case 't':
					c = '\t';
					break;
				case 'v':
					c = '\v';
					break;
				case 'b':
					c = '\b';
					break;
				case 'r':
					c = '\r';
					break;
				case 'a':
					c = '\a';
					break;
				default:
					break;
				}
				i++;
			}
			list.push_back(c);
		}
		list.push_back('\0');
		for(auto c = list.rbegin(); c!=list.rend(); c++){
			character = current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName(), Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR));
			output << tab << "li	$t0, " << (int) *c << std::endl;
			character->moveFromRegister1(output);
		}
		
		character->loadAbsoluteOffsetRegister1(output);
		const Variable * const character_ptr = current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName("character_ptr"), Type_Manager::getPointerType(character->getType()));
		character_ptr->moveFromRegister1(output);
		break;
	}
	case Token::function_definition: {

		const AST_function* function = dynamic_cast<const AST_function*>(node);
		output << ".global " << (function->getFunctionDeclaration()->getName()) << std::endl;
		output << (function->getFunctionDeclaration()->getName()) << ":" << std::endl;
		int temp_offset = 1;
		output << tab << "addiu	$sp,$sp, -" << SP_BYTE_OFFSET << std::endl;
		output << tab << "sw		$31," << (SP_BYTE_OFFSET - BYTES_PER_WORD * temp_offset++) << "($sp)" << std::endl;
		output << tab << "sw		$fp," << (SP_BYTE_OFFSET - BYTES_PER_WORD * temp_offset++) << "($sp)" << std::endl;

		output << tab << "move	$fp,$sp" << std::endl;
		output << std::endl;
		// Compile compound statement
		if (current_function_scope)
		{
			assert(0 && "Current Function Scope Not Deleted Correctly from previous function");
		}
		current_function_scope = new Function_Scope(function->getFunctionDeclaration()->getReturnType());
		current_function_scope->copyArgumentsToNewFrame(function->getFunctionDeclaration(), output);

		compile(node->child_nodes[2], output);

		//NOTE if the function has return type void or is the main function there may not be a return statement.
		if (function->getFunctionDeclaration()->getReturnType()->compare(Variable_Type_Built_In(Variable_Type_Built_In::Built_In::VOID)) || function->getFunctionDeclaration()->getName() == "main") {
			output << tab << tab << "#Possibly superfluous return for void functions and main function " << std::endl;
			int temp_offset = 1;
			output << tab << "move	$sp,$fp" << std::endl;
			output << tab << "lw		$31," << (SP_BYTE_OFFSET - BYTES_PER_WORD * temp_offset++) << "($sp)" << std::endl;
			output << tab << "lw		$fp," << (SP_BYTE_OFFSET - BYTES_PER_WORD * temp_offset++) << "($sp)" << std::endl;

			output << tab << "addiu	$sp,$sp, " << SP_BYTE_OFFSET << std::endl;

			output << tab << "jr		$ra" << std::endl;
			output << tab << "nop" << std::endl;

		}
		delete current_function_scope;
		current_function_scope = nullptr;
		break;
	}
	case Token::shift_expression: {}
	case Token::additive_expression: {}
	case Token::equality_expression: {}
	case Token::relational_expression: {}
	case Token::and_expression: {}
	case Token::exclusive_or_expression: {}
	case Token::inclusive_or_expression: {}
	case Token::logical_or_expression: {}
	case Token::logical_and_expression: {}
	case Token::multiplicative_expression: {
		compile(node->child_nodes[0], output);
		compile(node->child_nodes[2], output);
		current_function_scope->binaryOperator(output, node->child_nodes[1]->token_type);
		break;
	}
	case Token::conditional_expression: {
		std::string else_tenary = uniqueLabel("else_tenary");
		std::string end_tenary = uniqueLabel("end_tenary");
		compile(node->child_nodes[0], output);
		const Variable* var = current_function_scope->getLastVariable();
		var->moveToRegister1(output);
		output << tab << "beq		$t0, $zero, " << else_tenary << std::endl;
		output << tab << "nop" << std::endl;
		compile(node->child_nodes[2], output);
		output << tab << "j		" << end_tenary << std::endl;
		output << tab << "nop" << std::endl;
		output << else_tenary << ":" << std::endl;
		compile(node->child_nodes[4], output);
		output << end_tenary << ":" << std::endl;
		break;
	}
	case Token::labeled_statement: {
		switch (node->child_nodes[0]->token_type)
		{
		case Token::T_CASE: {
			const AST_literal* literal = dynamic_cast<const AST_literal*>(node->child_nodes[1]->child_nodes[0]);
			assert(literal != nullptr && "Only constant literals allowed in the case branches");
			const std::string label = uniqueLabel("switchcaselabel");
			current_function_scope->addSwitchLabel(label, *(literal->literal));
			output << label << ":" << std::endl;
			compile(node->child_nodes[3], output);
			break;
		}
		case Token::T_DEFAULT: {
			std::string label = uniqueLabel("switchdefaultlabel");
			current_function_scope->addSwitchLabel(label, "default");
			output << label << ":" << std::endl;
			compile(node->child_nodes[2], output);
			break;
		}
		default: {
			assert(0 && "Unsupported labelled statement");
			break;
		}
		}
		break;
	}
	case Token::jump_statement: {
		// REVIEW need to check dec_type
		switch (node->child_nodes[0]->token_type)
		{
		case Token::T_RETURN: {
			if (node->child_nodes.size() == 3)
			{
				compile(node->child_nodes[1], output);
			}
			// NOTE pseudo code for what should be here
			// if(function->getReturnType() != current_function_scope->getLastType()->getType()){
			// 	addToStack(cast())
			// }
			if (const Variable_Type* return_type = dynamic_cast<const Variable_Type*>(current_function_scope->return_type); return_type) {
				if (!return_type->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::VOID)))
				{
					// NOTE assume that most recent item on stack is return value
					if(!current_function_scope->return_type->compare(current_function_scope->getLastVariable()->getType())){
						std::cerr<<current_function_scope->return_type->getTypeName()<<std::endl;
						std::cerr<<current_function_scope->getLastVariable()->getType()->getTypeName()<<std::endl;
					}

					//assert(current_function_scope->return_type->compare(current_function_scope->getLastVariable()->getType()) && "Not yet supporting casting of return dec_type expression");

					current_function_scope->getLastVariable()->moveIntoReturnRegister(output);
				}
				int temp_offset = 1;
				output << tab << "move	$sp,$fp" << std::endl;
				output << tab << "lw		$31," << (SP_BYTE_OFFSET - BYTES_PER_WORD * temp_offset++) << "($sp)" << std::endl;
				output << tab << "lw		$fp," << (SP_BYTE_OFFSET - BYTES_PER_WORD * temp_offset++) << "($sp)" << std::endl;

				output << tab << "addiu	$sp,$sp, " << SP_BYTE_OFFSET << std::endl;
				output << tab << "jr		$31" << std::endl;
				output << tab << "nop" << std::endl;
				break;
			}
			break;
		}
		case Token::T_CONTINUE: {
			const std::string* label = current_function_scope->getContinueLabel();
			assert(label && "Invalid continue statement");
			output << tab << "j		" << *label << std::endl;
			output << tab << "nop" << std::endl;
			break;
		}
		case Token::T_BREAK: {
			const std::string* label = current_function_scope->getBreakLabel();
			assert(label && "Invalid break statement");
			output << tab << "j		" << *label << std::endl;
			output << tab << "nop" << std::endl;
			break;
		}
		default: {
			assert(false && "Not Supported Jump Statement Used");
			break;
		}
		}
		break;
	}
	case Token::block_item_list: {
		for (auto n : node->child_nodes)
		{
			compile(n, output);
		}
		break;
	}
	case Token::translation_unit: {
		for (auto n : node->child_nodes)
		{
			compile(n, output);
		}
		break;
	}
	case Token::compound_statement: {
		current_function_scope->enterInnerScope();
		if (node->child_nodes.size() == 3)
		{
			compile(node->child_nodes[1], output);
		}
		current_function_scope->exitInnerScope();
		break;
	}
	case Token::T_CONSTANT: {
		//NOTE Puts constant on stack
		const AST_literal* const literal = dynamic_cast<const AST_literal*>(node);
		literal->loadIntoRegister(output);
		const Variable* const var =
			current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName(), literal->getLiteralType());
		var->moveFromRegister1(output);
		break;
	}
	case Token::unary_expression: {
		if(node->child_nodes[0]->token_type == Token::T_SIZEOF){
			if(node->child_nodes.size() == 4){
				if(node->child_nodes[2]->token_type==Token::type_name){
					loadImmediateRegister1(output,4);
					current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName("size_of"),Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))->moveFromRegister1(output);
				}
				else{
					compile(node->child_nodes[2] ,output);
				}
			}
			if(node->child_nodes.size() == 2){
				compile(node->child_nodes[1], output);
				const Variable * var = current_function_scope->getLastVariable();
				unsigned int size = var->getSize();
				current_function_scope->popStack();
				loadImmediateRegister1(output,size);
				current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName("size_of"),Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))->moveFromRegister1(output);
			}
		}else{
			if (node->child_nodes.size() == 1) {
				compile(node->child_nodes[1], output);
			}
			else if (node->child_nodes.size() == 2) {
				if (node->child_nodes[0]->token_type != Token::T_AMPERSAND && node->child_nodes[0]->token_type != Token::T_INC_OP && node->child_nodes[0]->token_type!=Token::T_DEC_OP) {
					compile(node->child_nodes[1], output);
					current_function_scope->unaryOperator(output, node->child_nodes[0]->token_type);
				}
				else if (node->child_nodes[0]->token_type == Token::T_AMPERSAND) {
					const AST_expression* const postfix = dynamic_cast<const AST_expression*>(node->child_nodes[1]);
					assert(postfix && "Malformed unary expression");
					postfix->pushLValueAbsoluteOffsetToStack(current_function_scope, output);
					current_function_scope->unaryOperator(output, node->child_nodes[0]->token_type);
				}
				else if( node->child_nodes[0]->token_type == Token::T_INC_OP || node->child_nodes[0]->token_type == Token::T_DEC_OP){
					const AST_expression * const expr = dynamic_cast<const AST_expression*>(node->child_nodes[1]);
					assert(expr && "malformed postfix expression inc/dec operator");
					expr->pushLValueAbsoluteOffsetToStack(current_function_scope,output);
					const Variable * const ptr = current_function_scope->getLastVariable();
					current_function_scope->unaryOperator(output, node->child_nodes[0]->token_type);
					current_function_scope->popStack();
					compile(expr, output);
				}
			}
			else {
				assert(0 && "Unsupported Unary Expression");
			}
		}
		break;
	}
	case Token::T_IDENTIFIER: {
		//NOTE Puts identifier on stack
		const AST_literal* const literal = dynamic_cast<const AST_literal*>(node);
		const Variable* var = current_function_scope->getIdentifier(*literal->literal);

		if (var == nullptr) {
			output << tab << "lui		$t0, %hi(" << *literal->literal << ")" << std::endl;
			output << tab << "lw		$t0, %lo(" << *literal->literal << ")($t0)" << std::endl;
			// FIXME: create a global scope class to keep track of types
			// create a temp variable and push to stack.
			// const Variable_Type_Built_In
			current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName(), nullptr);
		}
		else {
			var->moveToRegister1(output);
			const Variable* stack_temp = current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName(), var->getType());
			stack_temp->moveFromRegister1(output);
		}
		break;
	}
	case Token::expression: {
		for (auto n : node->child_nodes) {
			compile(n, output);
		}
		break;
	}
	case Token::postfix_expression: {
		if (node->child_nodes.size() > 2 && node->child_nodes[1]->token_type == Token::T_LEFT_BRACKET && node->child_nodes.back()->token_type == Token::T_RIGHT_BRACKET) {
			//NOTE function call
			const Variable* const old_sp = current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName(), Type_Manager::getPointerType(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT)));
			old_sp->moveFromStackPointer(output);
			const AST_literal* lit = dynamic_cast<AST_literal*>(node->child_nodes[0]->child_nodes[0]);
			assert(lit && "Could not find function name");
			std::string function_name = *lit->literal;

			const Function_Declaration* const func_dec = Type_Manager::getFunctionDeclaration(function_name);
			int padding = 0;
			while(Function_Scope::offsetToStackPointer(current_function_scope->offsetAfterAddingArguments(padding, func_dec)) % DOUBLE_ALLIGNMENT != 0){
				padding++;
			}
			current_function_scope->offset += padding;
			compile(node->child_nodes[2], output);

			loadImmediateRegister1(output, Function_Scope::offsetToStackPointer(current_function_scope->offset));
			output << tab << "sub $sp, $sp, $t0" << std::endl;
			func_dec->moveIntoParameterRegisters(output);

			output << tab << "jal	" << func_dec->getName() << std::endl;
			output << tab << "nop" << std::endl;
			old_sp->moveToStackPointer(output);
			current_function_scope->popStack();
			//move return value to top of stack
			for (decltype(func_dec->getParameterList())::size_type i = 0; i < func_dec->getParameterList().size(); i++) {
				current_function_scope->popStack();
			}
			if(!func_dec->getReturnType()->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::VOID))){
				const Variable* const var = current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName(), func_dec->getReturnType());
				var->moveFromReturnRegister(output);
			}
		}
		else if(node->child_nodes.size() >= 4 && node->child_nodes[1]->token_type==Token::T_LEFT_SQUARE_BRACKET && node->child_nodes[3]->token_type==Token::T_RIGHT_SQUARE_BRACKET){
			output << "# pushing r value offset to stack" << std::endl;
			compile(node->child_nodes[0], output);

			const Variable * const var_ptr = current_function_scope->getLastVariable();
			std::string a = var_ptr->getType()->getTypeName();
			const Variable_Type_Pointer * const type_ptr = dynamic_cast<const Variable_Type_Pointer *>(var_ptr->getType());
			assert(type_ptr && "Can't build R value expression");

			compile(node->child_nodes[2],output);
			const unsigned int size = type_ptr->getPointsTo()->getSizeOf();
			loadImmediateRegister1(output, size);
			current_function_scope->getLastVariable()->moveToRegister2(output);
			current_function_scope->popStack();
			output << tab << "mult	$t0, $t2" << std::endl;
			output << tab << "MFLO	$t2" << std::endl;
			
			var_ptr->moveToRegister1(output);
			output << tab << "add $t0, $t0, $t2" << std::endl;
			var_ptr->moveFromRegister1(output);
			current_function_scope->unaryOperator(output, Token::T_STAR);
			
		}
		else if(node->child_nodes.size() ==2 && (node->child_nodes[1]->token_type == Token::T_INC_OP || node->child_nodes[1]->token_type == Token::T_DEC_OP)){
			const AST_expression * const expr = dynamic_cast<const AST_expression*>(node->child_nodes[0]);
			compile(expr, output);
			assert(expr && "malformed postfix expression inc/dec operator");
			expr->pushLValueAbsoluteOffsetToStack(current_function_scope,output);
			const Variable * const ptr = current_function_scope->getLastVariable();
			current_function_scope->unaryOperator(output, node->child_nodes[1]->token_type);
			current_function_scope->popStack();
		}
		else {
			for (auto n : node->child_nodes) {
				compile(n, output);
			}
		}
		break;
	}
	case Token::argument_expression_list: {
		for (auto arg = node->child_nodes.rbegin(); arg != node->child_nodes.rend(); arg++) {
			compile(*arg, output);
		}
		break;
	}
	case Token::declaration: {
		//REVIEW should probably assert this dynamic cast can take place
		AST_declarator* const dec = dynamic_cast<AST_declarator*>(node->child_nodes[1]->child_nodes[0]);
		assert(dec);
		const Variable_Type* var_type = dynamic_cast<const AST_declaration_specifiers*>(node->child_nodes[0])->getVariableType();
		std::vector<AST_declarator::Declarator_Type> dec_type_vec = dec->getTypeVector();
		//NOTE determine what dec_type of declaration we have

		//NOTE function declaration
		//NOTE function declaration (pointer return dec_type)
		if(node->child_nodes.size() >= 2 && node->child_nodes[1]->child_nodes.size() >= 3 && node->child_nodes[1]->child_nodes[2]->token_type == Token::initializer){
			compile(node->child_nodes[1]->child_nodes[2], output);
		}
		auto cursor_array_sizes = dec->array_sizes.rbegin();
		if (dec_type_vec.size() >= 1 && dec_type_vec[0] == AST_declarator::Declarator_Type::FUNCTION) {
			const Variable_Type * return_type = nullptr;
			return_type = var_type;
			assert(return_type && "Could not find return type of function, declaration specifiers not found");
			for(int i =0; i < dec_type_vec.size() && (dec_type_vec[i] == AST_declarator::Declarator_Type::POINTER); i++){
				return_type = Type_Manager::getPointerType(return_type);
			}
			Type_Manager::inventFunction(*dec->getLiteral()->literal, return_type, dec->getParameterList());
		}
		else{
			if (node->child_nodes.size() == 3) {
				if (current_function_scope) {
					if (node->child_nodes[1]->child_nodes.size() == 3 && node->child_nodes[1]->child_nodes[2]->token_type != Token::initializer) {
						if (node->child_nodes[1]->child_nodes[1]->token_type == Token::T_EQUAL_SIGN) {
							compile(node->child_nodes[1]->child_nodes[2], output);
							current_function_scope->renameBottom(*dec->getLiteral()->literal);
						}
						else {
							assert(0 && "Malformed declaration");
						}
					}
					else {
						std::vector<unsigned int> array_start_offsets;
						for (int i = 0; i < dec_type_vec.size() && (dec_type_vec[i] == AST_declarator::Declarator_Type::POINTER || AST_declarator::Declarator_Type::ARRAY);i++) {
							switch (dec_type_vec[i])
							{
							case AST_declarator::Declarator_Type::POINTER: {
								var_type = Type_Manager::getPointerType(var_type);
								break;
							}
							case AST_declarator::Declarator_Type::ARRAY: {
								const Variable_Type_Array * arr_type = Type_Manager::getArrayType(var_type, *cursor_array_sizes);
								if(dec_type_vec.front()==AST_declarator::Declarator_Type::ARRAY){
									if(cursor_array_sizes == dec->array_sizes.rbegin()){
										for(int i = 0; i < std::accumulate(cursor_array_sizes, dec->array_sizes.rend(), 1, std::multiplies<int>());i++){
											if(node->child_nodes[1]->child_nodes.size() < 2){
												loadImmediateRegister1(output, 0);
												current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName(*dec->getLiteral()->literal + std::to_string(i)+"_"),arr_type->getPointsTo())->moveFromRegister1(output);
											}
											else{
												current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName(*dec->getLiteral()->literal + std::to_string(i)+"_"),arr_type->getPointsTo());
											}
											if((i+1) % *cursor_array_sizes == 0){
												array_start_offsets.push_back(current_function_scope->getLastVariable()->getOffset());
											}
										}
									}
									var_type = Type_Manager::getArrayType(var_type, *cursor_array_sizes);
									std::vector<unsigned int> next_array_start_offsets;
									int counter = 0;
									for(auto off : array_start_offsets){
										output << tab << "addi $t0, $fp, -" << off << std::endl;
										std::string name = (i == dec_type_vec.size() - 1) ? *dec->getLiteral()->literal :current_function_scope->generateUniqueLiteralName("array_i_ptr_start" + *dec->getLiteral()->literal);

										const Variable * const last_pointer = current_function_scope->addVariableToStack(name, var_type);
										last_pointer->moveFromRegister1(output);
										if(cursor_array_sizes != --dec->array_sizes.rend() && (counter+1) % *(cursor_array_sizes+1) == 0){
											next_array_start_offsets.push_back(current_function_scope->getLastVariable()->getOffset());
										}
										counter++;
									}
									array_start_offsets = next_array_start_offsets;
								}
								var_type = arr_type;
								cursor_array_sizes++;
								break;
							}
							default: {
								break;
							}
							}
						}

						if(!dynamic_cast<const Variable_Type_Array*>(var_type)){
							current_function_scope->addVariableToStack(*dec->getLiteral()->literal, var_type);
						}
					}
				}
				else {
					if (node->child_nodes[1]->child_nodes.size() == 3) {
						if (node->child_nodes[1]->child_nodes[1]->token_type == Token::T_EQUAL_SIGN) {
							const AST_literal* literal = dynamic_cast<const AST_literal*>(node->child_nodes[1]->child_nodes[2]->child_nodes[0]);
							assert(literal && "Only constant expressions allowed");
							output << ".global " << *(dec->getLiteral()->literal) << std::endl;
							output << *(dec->getLiteral()->literal) << ":" << std::endl;
							output << tab << ".word " << *(literal->literal) << std::endl;
						}
						else {
							assert(0 && "Malformed declaration");
						}
					}
				}
			}
		}

		//NOTE variable delcaration (non pointer)
		//NOTE pointer declaration
		//const Variable * const var = current_function_scope->addVariableToStack();
		break;
	}
	case Token::expression_statement: {
		if (node->child_nodes.size() == 1) {
			compile(node->child_nodes[0], output);
		}
		break;
	}
	case Token::assignment_expression: {
		// TODO
		compile(node->child_nodes[2], output);
		const AST_expression* const dest = dynamic_cast<const AST_expression*>(node->child_nodes[0]);
		switch (node->child_nodes[1]->token_type)
		{
		case Token::T_EQUAL_SIGN:
			break;
		case Token::T_MUL_ASSIGN: {
			unsigned int numItems = current_function_scope->getStackSize();
			compile(node->child_nodes[0], output);
			assert( current_function_scope->getStackSize() - numItems == 1 && "Stack not cleared properly");
			current_function_scope->binaryOperator(output, Token::T_STAR);
			break;
		}
		case Token::T_DIV_ASSIGN: {
			unsigned int numItems = current_function_scope->getStackSize();
			compile(node->child_nodes[0], output);
			assert( current_function_scope->getStackSize() - numItems == 1 && "Stack not cleared properly");
			current_function_scope->binaryOperator(output, Token::T_FORWARD_SLASH );
			break;
		}
		case Token::T_MOD_ASSIGN: {
			unsigned int numItems = current_function_scope->getStackSize();
			compile(node->child_nodes[0], output);
			assert( current_function_scope->getStackSize() - numItems == 1 && "Stack not cleared properly");
			current_function_scope->binaryOperator(output, Token::T_PERCENT );
			break;
		}
		case Token::T_ADD_ASSIGN: {
			unsigned int numItems = current_function_scope->getStackSize();
			compile(node->child_nodes[0], output);
			assert( current_function_scope->getStackSize() - numItems == 1 && "Stack not cleared properly");
			current_function_scope->binaryOperator(output, Token::T_PLUS );
			break;
		}
		case Token::T_SUB_ASSIGN: {
			unsigned int numItems = current_function_scope->getStackSize();
			compile(node->child_nodes[0], output);
			assert( current_function_scope->getStackSize() - numItems == 1 && "Stack not cleared properly");
			current_function_scope->binaryOperator(output, Token::T_MINUS );
			break;
		}
		case Token::T_LEFT_ASSIGN: {
			unsigned int numItems = current_function_scope->getStackSize();
			compile(node->child_nodes[0], output);
			assert( current_function_scope->getStackSize() - numItems == 1 && "Stack not cleared properly");
			current_function_scope->binaryOperator(output, Token::T_LEFT_OP );
			break;
		}
		case Token::T_RIGHT_ASSIGN: {
			unsigned int numItems = current_function_scope->getStackSize();
			compile(node->child_nodes[0], output);
			assert( current_function_scope->getStackSize() - numItems == 1 && "Stack not cleared properly");
			current_function_scope->binaryOperator(output, Token::T_RIGHT_OP );
			break;
		}
		case Token::T_AND_ASSIGN: {
			unsigned int numItems = current_function_scope->getStackSize();
			compile(node->child_nodes[0], output);
			assert( current_function_scope->getStackSize() - numItems == 1 && "Stack not cleared properly");
			current_function_scope->binaryOperator(output, Token::T_AMPERSAND );
			break;
		}
		case Token::T_XOR_ASSIGN: {
			unsigned int numItems = current_function_scope->getStackSize();
			compile(node->child_nodes[0], output);
			assert( current_function_scope->getStackSize() - numItems == 1 && "Stack not cleared properly");
			current_function_scope->binaryOperator(output, Token::T_CARET );
			break;
		}
		case Token::T_OR_ASSIGN: {
			unsigned int numItems = current_function_scope->getStackSize();
			compile(node->child_nodes[0], output);
			assert( current_function_scope->getStackSize() - numItems == 1 && "Stack not cleared properly");
			current_function_scope->binaryOperator(output, Token::T_PIPE);
			break;
		}
		default:
			break;
		}

		assert(dest && "Assignment Expression assigns to non L value");
		const Variable* value = current_function_scope->getLastVariable();
		dest->pushLValueAbsoluteOffsetToStack(current_function_scope, output);
		const Variable* destination_offset = current_function_scope->getLastVariable();
		current_function_scope->copyToVariable(output, destination_offset, value);
		break;
	}
	case Token::selection_statement: {
		switch (node->child_nodes[0]->token_type) {
		case Token::T_IF: {
			std::string end_if = uniqueLabel("endIf");
			std::string else_if = uniqueLabel("elseIf");
			compile(node->child_nodes[2], output);
			const Variable* const var = current_function_scope->getLastVariable();
			var->moveToRegister1(output);
			output << tab << "beq		$0, $t0, " << else_if << std::endl;
			output << tab << "nop" << std::endl;
			compile(node->child_nodes[4], output);
			output << tab << "j		" << end_if << std::endl;
			output << tab << "nop" << std::endl;
			output << else_if << ":" << std::endl;
			if (node->child_nodes.size() > 5) {
				compile(node->child_nodes[6], output);
			}
			output << end_if << ":" << std::endl;

			break;
		}
		case Token::T_SWITCH: {
			std::string end_switch = uniqueLabel("endswitch");
			std::string start_switch = uniqueLabel("startswitch");
			current_function_scope->setBreakLabel(&end_switch);

			output << tab << "b		" << start_switch << std::endl;
			output << tab << "nop" << std::endl;

			std::vector<std::pair<std::string, std::string>> labels;
			current_function_scope->setSwitchLabels(&labels);

			compile(node->child_nodes[4], output);

			// jump to the end of the switch statement
			output << tab << "b		" << end_switch << std::endl;
			output << tab << "nop" << std::endl;

			output << start_switch << ":" << std::endl;

			compile(node->child_nodes[2], output);
			const Variable* const var = current_function_scope->getLastVariable();
			var->moveToRegister1(output); // $t0

			// pair of {labels,literals}, load literals into $t2
			const std::pair<std::string, std::string>* defaultBranch = nullptr;
			for (const auto& kv : labels) {
				const std::string& label = kv.first;
				const std::string& literal = kv.second;
				if (literal == "default") { defaultBranch = &kv; continue; }

				output << tab << "li		$t2, " << literal << std::endl;
				output << tab << "beq		$t0, $t2, " << label << std::endl;
				output << tab << "nop" << std::endl;
			}

			if (defaultBranch) {
				output << tab << "b		" << defaultBranch->first << std::endl;
				output << tab << "nop" << std::endl;
			}

			output << end_switch << ":" << std::endl;

			current_function_scope->clearSwitchLabels();
			current_function_scope->setBreakLabel(nullptr);
			break;
		}
		default: {
			assert(0 && "Malformed If statement");
		}
		}
		break;
	}
	case Token::iteration_statement: {
		switch (node->child_nodes[0]->token_type) {
		case Token::T_WHILE: {
			std::string start_while = uniqueLabel("startwhile");
			std::string end_while = uniqueLabel("endwhile");
			current_function_scope->setBreakLabel(&end_while);
			current_function_scope->setContinueLabel(&start_while);

			output << start_while << ":" << std::endl;
			compile(node->child_nodes[2], output);
			const Variable* const var = current_function_scope->getLastVariable();
			var->moveToRegister1(output);
			output << tab << "beq		$0, $t0, " << end_while << std::endl;
			output << tab << "nop" << std::endl;
			compile(node->child_nodes[4], output);
			output << tab << "j		" << start_while << std::endl;
			output << tab << "nop" << std::endl;
			output << end_while << ":" << std::endl;

			current_function_scope->setBreakLabel(nullptr);
			current_function_scope->setContinueLabel(nullptr);
			break;
		}
		case Token::T_DO: {
			std::string begin_dowhile = uniqueLabel("begindowhile");
			std::string start_dowhile = uniqueLabel("startdowhile");
			std::string end_dowhile = uniqueLabel("enddowhile");
			current_function_scope->setBreakLabel(&end_dowhile);
			current_function_scope->setContinueLabel(&start_dowhile);

			output << begin_dowhile << ":" << std::endl;
			compile(node->child_nodes[1], output);
			output << start_dowhile << ":" << std::endl;
			compile(node->child_nodes[4], output);

			const Variable* const var = current_function_scope->getLastVariable();
			var->moveToRegister1(output);

			output << tab << "beq		$0, $t0, " << end_dowhile << std::endl;
			output << tab << "nop" << std::endl;
			output << tab << "j		" << begin_dowhile << std::endl;
			output << tab << "nop" << std::endl;
			output << end_dowhile << ":" << std::endl;

			current_function_scope->setBreakLabel(nullptr);
			current_function_scope->setContinueLabel(nullptr);
			break;
		}
		case Token::T_FOR: {
			std::string start_for = uniqueLabel("startFor");
			std::string end_for = uniqueLabel("endFor");
			std::string continue_label = uniqueLabel("continueLabel");
			current_function_scope->setBreakLabel(&end_for);
			current_function_scope->setContinueLabel(&continue_label);
			current_function_scope->enterInnerScope();

			int i = 2;
			compile(node->child_nodes[i++], output);
			output << start_for << ":" << std::endl;
			compile(node->child_nodes[i++], output);

			const Variable* const var = current_function_scope->getLastVariable();
			var->moveToRegister1(output);
			output << tab << "beq		$0, $t0, " << end_for << std::endl;
			output << tab << "nop" << std::endl;
			compile(node->child_nodes.back(), output);

			output << continue_label << ":" << std::endl;
			if (node->child_nodes[i]->token_type != Token::T_RIGHT_BRACKET) {
				compile(node->child_nodes[i], output);
			}
			output << tab << "j		" << start_for << std::endl;
			output << tab << "nop" << std::endl;

			current_function_scope->exitInnerScope();
			current_function_scope->setBreakLabel(nullptr);
			current_function_scope->setContinueLabel(nullptr);
			output << end_for << ":" << std::endl;
			break;
		}
		default: {
			assert(0 && "Malformed iteration statement");
			break;
		}
		}
		break;
	}
	case Token::primary_expression : {
		for( auto n : node->child_nodes){
			compile(n, output);
		}
	}
	default: {
		break;
	}
	}
	return;
}


void AST_expression::pushLValueAbsoluteOffsetToStack(Function_Scope* const current_function_scope, std::ostream& output) const {
	output << "# pushing l value offset to stack" << std::endl;
	switch (this->token_type) {
	case Token::postfix_expression: {
		if (const AST_expression* const expression = dynamic_cast<const AST_expression*>(child_nodes[0]); expression) {
			expression->pushLValueAbsoluteOffsetToStack(current_function_scope, output);
		}
		else {
			if (child_nodes[0]->token_type == Token::T_IDENTIFIER) {

				const AST_literal* const literal = dynamic_cast<const AST_literal*>(child_nodes[0]);
				const Variable* const var = current_function_scope->getIdentifier(*literal->literal);
				loadImmediateRegister1(output, var->getOffset());
				const Variable_Type_Pointer* ptr = Type_Manager::getPointerType(var->getType());
				current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName(), ptr);
				output << tab << "sub		$t0, $fp, $t0 " << std::endl;
				current_function_scope->getLastVariable()->moveFromRegister1(output);
			}
		}



		if (child_nodes.size() != 1) {
			//NOTE  supporting [] and -> and struct.member_variable
			if(child_nodes[1]->token_type == Token::T_FULL_STOP){
				assert(0 && "Not yet supporting structs");
			}
			else if(child_nodes[1]->token_type==Token::T_PTR_OP){
				assert(0 && "Not yet supporting structs");
			}
			else if(child_nodes.size() >= 4 && child_nodes[1]->token_type==Token::T_LEFT_SQUARE_BRACKET && child_nodes[3]->token_type==Token::T_RIGHT_SQUARE_BRACKET){
				output << tab << "#square bracket operator" << std::endl;				
				const Variable * const var_ptr = current_function_scope->getLastVariable();
				const Variable_Type_Pointer * const type_ptr = dynamic_cast<const Variable_Type_Pointer *>(var_ptr->getType());
				assert(type_ptr && "Can't build L value expression");


				compile(child_nodes[2],output);
				const unsigned int size = type_ptr->getPointsTo()->getSizeOf();
				loadImmediateRegister1(output, size);
				current_function_scope->getLastVariable()->moveToRegister2(output);
				current_function_scope->popStack();
				output << tab << "mult	$t0, $t2" << std::endl;
				output << tab << "MFLO	$t2" << std::endl;

				var_ptr->moveToRegister1(output);
				output << tab << "lw	$t0, 0($t0)" <<std::endl;
				current_function_scope->popStack();
				const Variable * const new_ptr = current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName(),Type_Manager::getPointerType(type_ptr->getPointsTo()));
				const Variable * const new_ptr_absolute_offset = current_function_scope->addVariableToStack(current_function_scope->generateUniqueLiteralName(),type_ptr->getPointsTo());

				output << tab << "add $t0, $t0, $t2" << std::endl;
				new_ptr->moveFromRegister1(output);
				loadImmediateRegister1(output, new_ptr->getOffset());
				output << tab << "sub		$t0, $fp, $t0 " << std::endl;
				new_ptr_absolute_offset->moveFromRegister1(output);
				
				new_ptr_absolute_offset->unaryOperator(output, Token::T_STAR);
			}
			else{
				assert(0 && "Malformed postfix expression");
			}
		}


		break;
	}
	case Token::unary_expression: {
		if (this->child_nodes[0]->token_type == Token::T_STAR) {
			if (const AST_expression* const expression = dynamic_cast<const AST_expression*>(child_nodes[1]); expression) {
				expression->pushLValueAbsoluteOffsetToStack(current_function_scope, output);
			}
			const Variable* const var = current_function_scope->getLastVariable();
			output << tab << "#unary operator" << std::endl;
			var->unaryOperator(output, Token::T_STAR);
		}
		break;
	}
	default: {
		assert(0 && "Malformed L value postfix expression");
	}
	}
}