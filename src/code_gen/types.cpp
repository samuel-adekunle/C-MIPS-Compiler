#include "../lang/AST.hpp"
#include "utilities.hpp"
#include "constants.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

Function_Declaration::Function_Declaration(const std::string &name, const Variable_Type * const return_type, std::vector<std::pair<const Variable_Type*, std::string>> parameter_list) : return_type(return_type), function_name(name), parameter_list(parameter_list){
	
}

const Variable_Type * Function_Declaration::getReturnType() const{
	return this->return_type;
}
std::string Function_Declaration::getName() const{
	return function_name;
}
const std::vector<std::pair<const Variable_Type*, std::string>> Function_Declaration::getParameterList() const{
	return parameter_list;
}

const Variable_Type * Variable_Type_Alias::binaryOperator(std::ostream & output, Token operator_token, const unsigned int& offset_var_1, const Variable_Type * const var2, const unsigned int& offset_var_2) const {
	return alias_for->binaryOperator(output, operator_token, offset_var_1, var2, offset_var_2);
}
void Variable_Type_Alias::unaryOperator(std::ostream& output, Token operator_token, const unsigned int& offset_var_1) const {
	alias_for->unaryOperator(output, operator_token, offset_var_1);
}
void Variable_Type_Alias::stackMove(std::ostream& output, const unsigned int& offset) const {
	alias_for->stackMove(output, offset);
}

Variable_Type_Alias::Variable_Type_Alias(const std::string& identifier, const Variable_Type* const alias_for) : identifier(identifier), alias_for(alias_for) {

};

bool Variable_Type::compare(const Variable_Type* const var) const {
	//NOTE check if both built in
	// FIXME - wrong behaviour for non built in types
	std::cerr<<this->getTypeName();
	std::cerr<<var->getTypeName()<<std::endl;
	const Variable_Type_Built_In* const b2 = dynamic_cast<const Variable_Type_Built_In*>(var);
	const Variable_Type_Built_In* const b1 = dynamic_cast<const Variable_Type_Built_In*>(this);

	if (b1 && b2) {
		return b2->getType() == b1->getType();
	}
	return false;
}

bool Variable_Type::compare(const Variable_Type& var) const {
	return this->compare(&var);
}
unsigned int Variable_Type_Alias::getSizeOf(const unsigned int &offset) const {
	return alias_for->getSizeOf(offset);
}
void Variable_Type_Alias::moveFromRegister1(std::ostream& output, const unsigned int& offset) const {
	alias_for->moveFromRegister1(output, offset);
}
void Variable_Type_Alias::moveFromRegister2(std::ostream& output, const unsigned int& offset) const {
	alias_for->moveFromRegister2(output, offset);
}
void Variable_Type_Alias::moveFromRegister3(std::ostream& output, const unsigned int& offset) const {
	alias_for->moveFromRegister3(output, offset);
}
void Variable_Type_Alias::moveToRegister1(std::ostream& output, const unsigned int& offset, bool up) const {
	alias_for->moveToRegister1(output, offset, up);
}
void Variable_Type_Alias::moveToRegister2(std::ostream& output, const unsigned int& offset) const {
	alias_for->moveToRegister2(output, offset);
}
void Variable_Type_Alias::moveToRegister3(std::ostream& output, const unsigned int& offset) const {
	alias_for->moveToRegister3(output, offset);
}
void Variable_Type_Alias::moveIntoReturnRegister(std::ostream& output, const unsigned int& offset) const {
	alias_for->moveIntoReturnRegister(output, offset);
}
void Variable_Type_Alias::moveFromReturnRegister( std::ostream & output, const unsigned int &offset ) const{
	alias_for->moveFromReturnRegister(output, offset);
}
void Variable_Type_Alias::moveFromStackPointer(std::ostream & output, const unsigned int & offset) const{
	alias_for->moveFromStackPointer(output, offset);
}
void Variable_Type_Alias::moveToStackPointer(std::ostream &output, const unsigned int & offset) const{
	alias_for->moveToStackPointer(output, offset);
}
Variable_Type_Alias::~Variable_Type_Alias() {
}

Variable_Type_Built_In::Variable_Type_Built_In(const Built_In type) : type(type) {
	assert(type != Built_In::POINTER && "Incorrect constructor called");
}
unsigned int Variable_Type_Built_In::getSizeOf(const unsigned int &offset) const {
	switch (type)
	{
	case Variable_Type_Built_In::Built_In::CHAR:
		return CHAR_SIZE + (CHAR_ALLIGNMENT - offset % CHAR_ALLIGNMENT ) % CHAR_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::SIGNED_CHAR:
		return CHAR_SIZE + (CHAR_ALLIGNMENT - offset % CHAR_ALLIGNMENT ) % CHAR_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::SHORT:
		return SHORT_SIZE + (SHORT_ALLIGNMENT - offset % SHORT_ALLIGNMENT ) % SHORT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_SHORT:
		return SHORT_SIZE + (SHORT_ALLIGNMENT - offset % SHORT_ALLIGNMENT ) % SHORT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::INT:
		return INT_SIZE + (INT_ALLIGNMENT - offset % INT_ALLIGNMENT ) % INT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::LONG:
		return LONG_SIZE + (LONG_ALLIGNMENT - offset % LONG_ALLIGNMENT ) % LONG_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::ENUM:
		return ENUM_SIZE + (INT_ALLIGNMENT - offset % INT_ALLIGNMENT ) % INT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		return INT_SIZE + (INT_ALLIGNMENT - offset % INT_ALLIGNMENT ) % INT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_LONG:
		return LONG_SIZE +  (LONG_ALLIGNMENT - offset % LONG_ALLIGNMENT ) % LONG_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::POINTER:
		return POINTER_SIZE +  (POINTER_ALLIGNMENT - offset % POINTER_ALLIGNMENT ) % POINTER_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::FLOAT:
		return FLOAT_SIZE +  (FLOAT_ALLIGNMENT - offset % FLOAT_ALLIGNMENT ) % FLOAT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::DOUBLE:
		return DOUBLE_SIZE +  (DOUBLE_ALLIGNMENT - offset % DOUBLE_ALLIGNMENT ) % DOUBLE_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::LONG_DOUBLE:
		return LONG_SIZE +  (LONG_DOUBLE_ALLIGNMENT - offset % LONG_DOUBLE_ALLIGNMENT ) % LONG_DOUBLE_ALLIGNMENT;;
		break;
	default:
		assert(0 && "Type Size not Supported yet");
		break;
	}

};

void Variable_Type_Built_In::moveFromRegister(std::ostream& output, const unsigned int& offset, const std::string& register_start, const std::string& register_overflow) const {
	switch (this->type)
	{
	case Variable_Type_Built_In::Built_In::CHAR:
		output << tab << "sb	" << register_start << ", -" << offset << "($fp)" << std::endl;
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
	case Variable_Type_Built_In::Built_In::POINTER:
	case Variable_Type_Built_In::Built_In::INT:
		output << tab << "sw		" << register_start << ", -" << offset << "($fp)" << std::endl;

		break;
	case Variable_Type_Built_In::Built_In::FLOAT:{
		std::string copy_register_start = register_start;
		copy_register_start = (copy_register_start=="$t0") ? "$f4" : copy_register_start;
		copy_register_start = (copy_register_start=="$t1") ? "$f5" : copy_register_start;
		copy_register_start = (copy_register_start=="$t2") ? "$f6" : copy_register_start;
		copy_register_start = (copy_register_start=="$t3") ? "$f7" : copy_register_start;
		copy_register_start = (copy_register_start=="$t4") ? "$f8" : copy_register_start;
		copy_register_start = (copy_register_start=="$t5") ? "$f9" : copy_register_start;
		output << tab << "swc1 " << copy_register_start << ", -" << offset << "($fp)" << std::endl;
		break;
	}
	case Variable_Type_Built_In::Built_In::DOUBLE:{
		std::string copy_register_start = register_start;
		copy_register_start = (copy_register_start=="$t0") ? "$f4" : copy_register_start;
		copy_register_start = (copy_register_start=="$t1") ? "$f5" : copy_register_start;
		copy_register_start = (copy_register_start=="$t2") ? "$f6" : copy_register_start;
		copy_register_start = (copy_register_start=="$t3") ? "$f7" : copy_register_start;
		copy_register_start = (copy_register_start=="$t4") ? "$f8" : copy_register_start;
		copy_register_start = (copy_register_start=="$t5") ? "$f9" : copy_register_start;
		output << tab << "sdc1 " << copy_register_start << ", -" << offset << "($fp)" << std::endl;
		break;
	}
	default:
		assert(0 && "moveFromTempRegisterToStack\nType not supported yet");
		break;
	}
}

void Variable_Type_Built_In::moveToRegister(std::ostream& output, const unsigned int& offset, const std::string& register_start, const std::string& register_overflow, bool up) const {
	std::string neg = up ? "" : "-";
	switch (this->type)
	{
	case Variable_Type_Built_In::Built_In::CHAR:
		if(up){
			output << tab << "move	" << register_start <<", $zero" <<std::endl;
			output << tab << "lw	" << register_start << ", "<< neg << offset << "($fp)" << std::endl;
		}
		else{
			output << tab << "move	" << register_start <<", $zero" <<std::endl;
			output << tab << "lb	" << register_start << ", "<< neg << offset << "($fp)" << std::endl;
		}
		
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
	case Variable_Type_Built_In::Built_In::POINTER:
	case Variable_Type_Built_In::Built_In::LONG:
	case Variable_Type_Built_In::Built_In::INT:
		output << tab << "lw		" << register_start << ","<< neg << offset << "($fp)" << std::endl;

		break;
	case Variable_Type_Built_In::Built_In::FLOAT:{
		std::string copy_register_start = register_start;
		copy_register_start = (copy_register_start=="$t0") ? "$f4" : copy_register_start;
		copy_register_start = (copy_register_start=="$t1") ? "$f5" : copy_register_start;
		copy_register_start = (copy_register_start=="$t2") ? "$f6" : copy_register_start;
		copy_register_start = (copy_register_start=="$t3") ? "$f7" : copy_register_start;
		copy_register_start = (copy_register_start=="$t4") ? "$f8" : copy_register_start;
		copy_register_start = (copy_register_start=="$t5") ? "$f9" : copy_register_start;
		output << tab << "lwc1 " << copy_register_start << ","<< neg << offset << "($fp)" << std::endl;
		break;
	}
	case Variable_Type_Built_In::Built_In::DOUBLE:{
		std::string copy_register_start = register_start;
		copy_register_start = (copy_register_start=="$t0") ? "$f4" : copy_register_start;
		copy_register_start = (copy_register_start=="$t1") ? "$f5" : copy_register_start;
		copy_register_start = (copy_register_start=="$t2") ? "$f6" : copy_register_start;
		copy_register_start = (copy_register_start=="$t3") ? "$f7" : copy_register_start;
		copy_register_start = (copy_register_start=="$t4") ? "$f8" : copy_register_start;
		copy_register_start = (copy_register_start=="$t5") ? "$f9" : copy_register_start;
		output << tab << "ldc1 " << copy_register_start << "," << neg << offset << "($fp)" << std::endl;
		break;
	}
	default:
		assert(0 && "moveFromTempRegisterToStack\nType not supported yet");
		break;
	}
}
void Variable_Type_Built_In::moveIntoReturnRegister(std::ostream& output, const unsigned int& offset) const {
	//FIXME
	switch (this->getType())
	{
	case Variable_Type_Built_In::Built_In::CHAR:
		output << tab << "move $2, $0" << std::endl;
		output << tab << "lb	$2" << ", -" << offset << "($fp)" << std::endl;
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
	case Variable_Type_Built_In::Built_In::POINTER:
	case Variable_Type_Built_In::Built_In::INT:
		output << tab << "lw		$2, -" << offset << "($fp)" << std::endl;
		break;
	case Variable_Type_Built_In::Built_In::FLOAT:
		output << tab << "lwc1		$f0, -" << offset << "($fp)" << std::endl;
		break;
	case Variable_Type_Built_In::Built_In::DOUBLE:
		output << tab << "ldc1		$f0, -" << offset << "($fp)" << std::endl;
		break;
	default:
		assert(false && "Currently Unsupported Return Type");
	}
}
void Variable_Type_Built_In::moveFromRegister1(std::ostream& output, const unsigned int& offset) const {
	moveFromRegister(output, offset, "$t0", "$t1");
}
void Variable_Type_Built_In::moveFromRegister2(std::ostream& output, const unsigned int& offset) const {
	moveFromRegister(output, offset, "$t2", "$t3");
}
void Variable_Type_Built_In::moveFromRegister3(std::ostream& output, const unsigned int& offset) const {
	moveFromRegister(output, offset, "$t4", "$t5");
}
void Variable_Type_Built_In::moveToRegister1(std::ostream& output, const unsigned int& offset, bool up) const {
	moveToRegister(output, offset, "$t0", "$t1", up);
}
void Variable_Type_Built_In::moveToRegister2(std::ostream& output, const unsigned int& offset) const {
	moveToRegister(output, offset, "$t2", "$t3");
}
void Variable_Type_Built_In::moveToRegister3(std::ostream& output, const unsigned int& offset) const {
	moveToRegister(output, offset, "$t4", "$t5");
}
void Variable_Type_Built_In::moveFromReturnRegister( std::ostream & output, const unsigned int &offset ) const{
	moveFromRegister(output, offset, "$v0", "$v1");
}
void Variable_Type_Built_In::moveFromStackPointer(std::ostream & output, const unsigned int & offset) const{
	moveFromRegister(output, offset, "$sp", "");
}
void Variable_Type_Built_In::moveToStackPointer(std::ostream &output, const unsigned int & offset) const{
	moveToRegister(output, offset, "$sp", "");
}
Variable_Type_Built_In::Built_In Variable_Type_Built_In::getType() const{
	return type;
}
Variable_Type_Built_In::~Variable_Type_Built_In() {

}

void Variable_Type_Built_In::stackMove(std::ostream& output, const unsigned int& offset) const {
	switch (this->type)
	{
	case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
	case Variable_Type_Built_In::Built_In::POINTER:
	case Variable_Type_Built_In::Built_In::INT:
		std::cout << tab << "lw 	$t2, 0($t4)" << std::endl;
		this->moveFromRegister2(output, offset);
		break;
	case Variable_Type_Built_In::Built_In::CHAR:
		std::cout << tab << "lb 	$t2, 0($t4)" << std::endl;
		this->moveFromRegister2(output, offset);
		break;
	case Variable_Type_Built_In::Built_In::FLOAT:
		std::cout << tab << "l.s 	$f4, 0($t4)" << std::endl;
		this->moveFromRegister2(output, offset);
		break;
	case Variable_Type_Built_In::Built_In::DOUBLE:
		std::cout << tab << "l.d 	$f4, 0($t4)" << std::endl;
		this->moveFromRegister2(output, offset);
		break;
	default:
		assert(0 && "Stack move of selected type is not yet implemented");
		break;
	}
}
const Variable_Type* Variable_Type_Built_In::binaryOperator(std::ostream& output, Token operator_token, const unsigned int& offset_var_1, const Variable_Type* const var2, const unsigned int& offset_var_2) const {
	switch (operator_token) {
	case(Token::T_PLUS): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::CHAR:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "add 	$t0, $t0, $t2" << std::endl;
			return this;
		break;
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "addu 	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
			break;	
		case Variable_Type_Built_In::Built_In::INT:
			if(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))){
				this->moveToRegister2(output, offset_var_1);
				var2->moveToRegister1(output, offset_var_2);
				output << tab << "add 	$t0, $t0, $t2" << std::endl;
				return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			}
			else if(auto ptr_type = dynamic_cast<const Variable_Type_Pointer *>(var2); ptr_type){
				this->moveToRegister1(output, offset_var_1);
				loadImmediateRegister2(output, ptr_type->getPointsTo()->getSizeOf());
				output << tab << "mult $t0, $t2" << std::endl;
				output << tab << "MFLO $t0" << std::endl;
				var2->moveToRegister2(output, offset_var_2);
				output << tab << "add 	$t0, $t0, $t2" << std::endl;
				return var2;
			}
			else{
				std::cerr<<var2->getTypeName()<<std::endl;
				assert(0&&"Tried to add integer with unsupported type, see stdcerr");
			}
			break;
		case Variable_Type_Built_In::Built_In::POINTER:
			if(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))){
				var2->moveToRegister1(output, offset_var_2);
				auto ptr_type = dynamic_cast<const Variable_Type_Pointer *>(this);
				loadImmediateRegister2(output, ptr_type->getPointsTo()->getSizeOf());
				output << tab << "mult $t0, $t2" << std::endl;
				output << tab << "MFLO $t0" << std::endl;
				this->moveToRegister2(output, offset_var_1);
				output << tab << "add 	$t0, $t0, $t2" << std::endl;
				return this;
			}
			break;
		case Variable_Type_Built_In::Built_In::FLOAT:
			if(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT))){
				this->moveToRegister1(output, offset_var_1);
				var2->moveToRegister2(output, offset_var_2);
				
				output << tab << "add.s		$f4, $f4, $f6" << std::endl;

				return this;
			}
			else{
				std::cerr << var2->getTypeName() << std::endl;
				assert(0 && "Casting not supported");
			}
		case Variable_Type_Built_In::Built_In::DOUBLE:
			if(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE))){
				this->moveToRegister1(output, offset_var_1);
				var2->moveToRegister2(output, offset_var_2);
				
				output << tab << "add.d		$f4, $f4, $f6" << std::endl;

				return this;
			}
			else{
				std::cerr << var2->getTypeName() << std::endl;
				assert(0 && "Casting not supported");
			}
		default:
			assert(0 && "plusOperator not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_MINUS): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::CHAR: {
			assert(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR))&&"Casting not supported");
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "sub 	$t0, $t0, $t2" << std::endl;

			return this;
			break;
		}
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT: {
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "subu 	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
			break;
		}
		case Variable_Type_Built_In::Built_In::INT: {
			if(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))){
				this->moveToRegister2(output, offset_var_1);
				var2->moveToRegister1(output, offset_var_2);
				output << tab << "sub 	$t0, $t0, $t2" << std::endl;

				return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
				break;
			}
			else if(auto ptr_type = dynamic_cast<const Variable_Type_Pointer *>(var2); ptr_type){
				this->moveToRegister1(output, offset_var_1);
				loadImmediateRegister2(output, ptr_type->getPointsTo()->getSizeOf());
				output << tab << "mult $t0, $t2" << std::endl;
				output << tab << "MFLO $t0" << std::endl;
				var2->moveToRegister2(output, offset_var_2);
				output << tab << "sub 	$t0, $t2, $t0" << std::endl;
				return var2;
			}
			else{
				std::cerr<<var2->getTypeName()<<std::endl;
				assert(0&&"Tried to add integer with unsupported type, see stdcerr");
			}
			break;
		}
		case Variable_Type_Built_In::Built_In::POINTER: {
			if(auto ptr_type = dynamic_cast<const Variable_Type_Pointer *>(var2); ptr_type){
				this->moveToRegister2(output, offset_var_1);
				var2->moveToRegister1(output, offset_var_2);
				output << tab << "sub 	$t0, $t0, $t2" << std::endl;
				output << tab << "sra	$t0, $t0, 2" << std::endl;
				return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
				break;
			}
			break;
		}
		case Variable_Type_Built_In::Built_In::FLOAT:
			if(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT))){
				this->moveToRegister1(output, offset_var_1);
				var2->moveToRegister2(output, offset_var_2);
				
				output << tab << "sub.s		$f4, $f6, $f4" << std::endl;
				return this;
			}
			else{
				std::cerr << var2->getTypeName() << std::endl;
				assert(0 && "Casting not supported");
			}
		case Variable_Type_Built_In::Built_In::DOUBLE:
			if(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE))){
				this->moveToRegister1(output, offset_var_1);
				var2->moveToRegister2(output, offset_var_2);
				
				output << tab << "sub.d		$f4, $f6, $f4" << std::endl;
				return this;
			}
			else{
				std::cerr << var2->getTypeName() << std::endl;
				assert(0 && "Casting not supported");
			}
		default:
			assert(0 && "minus not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_STAR): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::CHAR:
			assert(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR))&&"Casting not supported");
			this->moveToRegister1(output, offset_var_1);
			var2->moveToRegister2(output, offset_var_2);
			output << tab << "mult	$t0, $t2" << std::endl;
			output << tab << "MFLO	$t0" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR);
			break;
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
			assert(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT))&&"Casting not supported");
			this->moveToRegister1(output, offset_var_1);
			var2->moveToRegister2(output, offset_var_2);
			output << tab << "multu	$t0, $t2" << std::endl;
			output << tab << "MFLO	$t0" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
			break;
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister1(output, offset_var_1);
			var2->moveToRegister2(output, offset_var_2);
			output << tab << "mult	$t0, $t2" << std::endl;
			output << tab << "MFLO	$t0" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		case Variable_Type_Built_In::Built_In::FLOAT:
			if(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT))){
				this->moveToRegister1(output, offset_var_1);
				var2->moveToRegister2(output, offset_var_2);
				
				output << tab << "mul.s		$f4, $f4, $f6" << std::endl;
				return this;
			}
			else{
				std::cerr << var2->getTypeName() << std::endl;
				assert(0 && "Casting not supported");
			}
		case Variable_Type_Built_In::Built_In::DOUBLE:
			if(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE))){
				this->moveToRegister1(output, offset_var_1);
				var2->moveToRegister2(output, offset_var_2);
				
				output << tab << "mul.d		$f4, $f4, $f6" << std::endl;
				return this;
			}
			else{
				std::cerr << var2->getTypeName() << std::endl;
				assert(0 && "Casting not supported");
			}
		default:
			assert(0 && "star operator not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_FORWARD_SLASH): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::CHAR:
			assert(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR))&&"Casting not supported");
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "div	$t0, $t2" << std::endl;
			output << tab << "MFLO	$t0" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR);
			break;
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
			assert(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT))&&"Casting not supported");
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "divu	$t0, $t2" << std::endl;
			output << tab << "MFLO	$t0" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
			break;
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "div	$t0, $t2" << std::endl;
			output << tab << "MFLO	$t0" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		case Variable_Type_Built_In::Built_In::FLOAT:
			if(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT))){
				this->moveToRegister1(output, offset_var_1);
				var2->moveToRegister2(output, offset_var_2);
				
				output << tab << "div.s		$f4, $f6, $f4" << std::endl;

				return this;
			}
			else{
				std::cerr << var2->getTypeName() << std::endl;
				assert(0 && "Casting not supported");
			}
		case Variable_Type_Built_In::Built_In::DOUBLE:
			if(var2->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE))){
				this->moveToRegister1(output, offset_var_1);
				var2->moveToRegister2(output, offset_var_2);
				
				output << tab << "div.d		$f4, $f6, $f4" << std::endl;
				return this;
			}
			else{
				std::cerr << var2->getTypeName() << std::endl;
				assert(0 && "Casting not supported");
			}
		default:
			assert(0 && "forward slash not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_AMPERSAND): {
		switch (this->type) {
		case Variable_Type_Built_In::Built_In::CHAR:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "and	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR);
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "and	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "and	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		default:
			assert(0 && "ampersand not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_PIPE): {
		switch (this->type) {
		case Variable_Type_Built_In::Built_In::CHAR:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "or	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR);
			break;
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "or	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
			break;
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "or	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		default:
			assert(0 && "pipe not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_CARET): {
		switch (this->type) {
		case Variable_Type_Built_In::Built_In::CHAR:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "xor	$t0, $t0, $t2" << std::endl;

			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR);
			break;
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "xor	$t0, $t0, $t2" << std::endl;

			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
			break;
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "xor	$t0, $t0, $t2" << std::endl;

			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		default:
			assert(0 && "caret not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_OR_OP): {
		switch (this->type) {
		case Variable_Type_Built_In::Built_In::CHAR:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "sne 	$t0, $zero, $t0" << std::endl;
			output << tab << "sne 	$t2, $zero, $t2" << std::endl;
			output << tab << "or 	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR);
			break;
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "sne 	$t0, $zero, $t0" << std::endl;
			output << tab << "sne 	$t2, $zero, $t2" << std::endl;
			output << tab << "or 	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
			break;
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "sne 	$t0, $zero, $t0" << std::endl;
			output << tab << "sne 	$t2, $zero, $t2" << std::endl;
			output << tab << "or 	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		default:
			assert(0 && "forward slash not yet supported for this type");
			break;
		}
		break;
	}

	case(Token::T_LEFT_OP): {
		switch (this->type) {
		case Variable_Type_Built_In::Built_In::CHAR:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "sllv 	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR);
			break;
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "sllv 	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
			break;
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "sllv 	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		default:
			assert(0 && "forward slash not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_RIGHT_OP): {
		const Variable_Type_Built_In * const num = dynamic_cast<const Variable_Type_Built_In*>(var2);
		switch (num->type) {
		case Variable_Type_Built_In::Built_In::CHAR:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "srlv 	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR);
			break;
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "srlv 	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT);
			break;
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "srav 	$t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		default:
			assert(0 && "forward slash not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_AND_OP): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::CHAR:
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);
			output << tab << "sne 	$t0, $zero, $t0" << std::endl;
			output << tab << "sne 	$t2, $zero, $t2" << std::endl;
			output << tab << "and 	$t0, $t0, $t2" << std::endl;

			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		default:
			assert(0 && "and operation not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_EQ_OP): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		case Variable_Type_Built_In::Built_In::CHAR:
		case Variable_Type_Built_In::Built_In::INT: {
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);

			output << tab << "seq $t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		}
		case Variable_Type_Built_In::Built_In::FLOAT:
			this->moveToRegister1(output, offset_var_1);
			var2->moveToRegister2(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.eq.s	$f4, $f6" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;
			output << tab << "xori		$t0, $t0, 1" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		case Variable_Type_Built_In::Built_In::DOUBLE:
			this->moveToRegister1(output, offset_var_1);
			var2->moveToRegister2(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.eq.d	$f4, $f6" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;
			output << tab << "xori		$t0, $t0, 1" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		default:
			assert(0 && "equal to operator not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_LE_OP): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::CHAR:
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);

			output << tab << "sle $t0, $t0, $t2" << std::endl;

			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		case Variable_Type_Built_In::Built_In::FLOAT:
			this->moveToRegister1(output, offset_var_1);
			var2->moveToRegister2(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.le.s	$f4, $f6" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;
			output << tab << "xori		$t0, $t0, 1" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		case Variable_Type_Built_In::Built_In::DOUBLE:
			this->moveToRegister1(output, offset_var_1);
			var2->moveToRegister2(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.le.d	$f4, $f6" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;
			output << tab << "xori		$t0, $t0, 1" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		default:
			assert(0 && "less than or equal to operator not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_GE_OP): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::CHAR:
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);

			output << tab << "sge $t0, $t0, $t2" << std::endl;

			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		case Variable_Type_Built_In::Built_In::FLOAT:
			this->moveToRegister1(output, offset_var_1);
			var2->moveToRegister2(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.lt.s	$f4, $f6" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;

			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		case Variable_Type_Built_In::Built_In::DOUBLE:
			this->moveToRegister1(output, offset_var_1);
			var2->moveToRegister2(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.lt.d	$f4, $f6" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		default:
			assert(0 && "greater than or equal to operator not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_LEFT_ANGLE_BRACKET): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::CHAR:
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);

			output << tab << "slt $t0, $t0, $t2" << std::endl;

			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		case Variable_Type_Built_In::Built_In::FLOAT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.lt.s	$f4, $f6" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;
			output << tab << "xori		$t0, $t0, 1" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);

			break;
		case Variable_Type_Built_In::Built_In::DOUBLE:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.lt.d	$f4, $f6" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;
			output << tab << "xori		$t0, $t0, 1" << std::endl;

			output << tab << " " << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);

			break;
		default:
			assert(0 && "left angle bracket not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_RIGHT_ANGLE_BRACKET): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::CHAR:
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);

			output << tab << "sgt $t0, $t0, $t2" << std::endl;

			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		case Variable_Type_Built_In::Built_In::FLOAT:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.le.s	$f6, $f4" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);

			break;
		case Variable_Type_Built_In::Built_In::DOUBLE:
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.le.d	$f6, $f4" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;

			output << tab << " " << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);

			break;
		default:
			assert(0 && "right angle bracket not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_NE_OP): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		case Variable_Type_Built_In::Built_In::CHAR:
		case Variable_Type_Built_In::Built_In::INT: {
			this->moveToRegister2(output, offset_var_1);
			var2->moveToRegister1(output, offset_var_2);

			output << tab << "sne $t0, $t0, $t2" << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		}
		case Variable_Type_Built_In::Built_In::FLOAT:
			this->moveToRegister1(output, offset_var_1);
			var2->moveToRegister2(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.eq.s	$f4, $f6" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
			break;
		case Variable_Type_Built_In::Built_In::DOUBLE:
			this->moveToRegister1(output, offset_var_1);
			var2->moveToRegister2(output, offset_var_2);

			output << tab << "li		$t0, 1 " << std::endl;
			output << tab << "c.eq.d	$f4, $f6" << std::endl;
			output << tab << "movt		$t0, $0, $fcc0  " << std::endl;
			return Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT);
		default:
			assert(0 && "not equal to operator not yet supported for this type");
			break;
		}
		break;
	}
	default: {
		assert(0 && "Unexpected or not supported token type, incorrectly formed binary expression");
	}
	}
	return nullptr;
}

void Variable_Type_Built_In::unaryOperator(std::ostream& output, Token operator_token, const unsigned int& offset_var_1) const {
	switch (operator_token) {
	case(Token::T_AMPERSAND): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::POINTER:
			break;
		default:
			assert(0 && "ampersand unary operator not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_STAR): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::POINTER: {
			const Variable_Type_Pointer* const ptr = dynamic_cast<const Variable_Type_Pointer*>(this);
			assert(ptr);
			ptr->moveToStackMoveRegister(output, offset_var_1);
			ptr->getPointsTo()->stackMove(output, offset_var_1);
			break;
		}
		default: {
			assert(0 && "star unary operator not yet supported for this type");
			break;
		}
		}
		break;
	}
	case(Token::T_PLUS): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		case Variable_Type_Built_In::Built_In::INT:
			break;
		default:
			assert(0 && "plus unary operator not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_MINUS): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		case Variable_Type_Built_In::Built_In::INT:
			this->moveToRegister1(output, offset_var_1);
			output << tab << "sub 	$t0, $0, $t0" << std::endl;
			this->moveFromRegister1(output, offset_var_1);
			break;
		case Variable_Type_Built_In::Built_In::FLOAT:
			this->moveToRegister1(output, offset_var_1);
			output << tab << "neg.s 	$f4, $f4" << std::endl;
			this->moveFromRegister1(output, offset_var_1);
			break;
		case Variable_Type_Built_In::Built_In::DOUBLE:
			this->moveToRegister1(output, offset_var_1);
			output << tab << "neg.d 	$f4, $f4" << std::endl;
			this->moveFromRegister1(output, offset_var_1);
			break;
		default:
			assert(0 && "minus unary operator not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_TILDE): {
		switch (this->type)
		{
		default:
			assert(0 && "tilde unary operator not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_EXCLAMATION_MARK): {
		switch (this->type)
		{
		default:
			assert(0 && "exclamation mark operator not yet supported for this type");
			break;
		}
		break;
	}
	case(Token::T_INC_OP): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::POINTER:{
			const Variable_Type_Pointer* const ptr = dynamic_cast<const Variable_Type_Pointer*>(this);
			assert(ptr);
			if(ptr->getPointerChainDepth() == 1 && ptr->getPointerChainRoot()->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))){
				ptr->moveToRegister1(output, offset_var_1);
				output << tab << "lw	$t2, 0($t0)" << std::endl;
				output << tab << "addi	$t2, $t2, 1" << std::endl;
				output << tab << "sw	$t2, 0($t0)" << std::endl;
			}
			else if(ptr->getPointerChainDepth() == 1 && ptr->getPointerChainRoot()->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR))){
				ptr->moveToRegister1(output, offset_var_1);
				output << tab << "lb	$t2, 0($t0)" << std::endl;
				output << tab << "addiu	$t2, $t2, 1" << std::endl;
				output << tab << "sb	$t2, 0($t0)" << std::endl;
			}
			else if(ptr->getPointerChainDepth() == 1 && ptr->getPointerChainRoot()->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT))){
				ptr->moveToRegister1(output, offset_var_1);
				output << tab << "lw	$t2, 0($t0)" << std::endl;
				output << tab << "addiu	$t2, $t2, 1" << std::endl;
				output << tab << "sw	$t2, 0($t0)" << std::endl;
			}
			else if(ptr->getPointerChainDepth() == 1 && ptr->getPointerChainRoot()->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT))){
				ptr->moveToRegister1(output, offset_var_1);
				output << tab << "lwc1	$f4, 0($t0)" << std::endl;
				output << tab <<"li.s	$f6, 1" << std::endl;
				output << tab << "add.s	$f4, $f4, $f6" << std::endl;
				output << tab << "s.s	$f4, 0($t0)" << std::endl;
			}
			else if(ptr->getPointerChainDepth() == 1 && ptr->getPointerChainRoot()->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE))){
				ptr->moveToRegister1(output, offset_var_1);
				output << tab << "ldc1	$f4, 0($t0)" << std::endl;
				output << tab <<"li.d	$f6, 1" << std::endl;
				output << tab << "add.d	$f4, $f4, $f6" << std::endl;
				output << tab << "s.d	$f4, 0($t0)" << std::endl;
			}
			else if(ptr->getPointerChainDepth() == 2 ){
				this->moveToRegister1(output, offset_var_1);
				output << tab << "lw	$t2, 0($t0)" << std::endl;
				auto sub_ptr = dynamic_cast<const Variable_Type *>(ptr->getPointsTo());
				assert(sub_ptr);
				output << tab << "addi $t2, $t2, " << sub_ptr->getSizeOf() << std::endl;
				output << tab << "sw	$t2, 0($t0)" << std::endl;
			}
			else{
				std::cerr<<ptr->getPointerChainDepth()<<std::endl;
				std::cerr<<ptr->getPointerChainRoot()->getTypeName()<<std::endl;
				assert(0 && "Type not implemented for ++ operator");
			}
			break;
		}
		default:{
			std::cerr<<"Type "<< this->getTypeName()<<std::endl;
			assert(0 && "T_INC_OP  operator not yet supported for this type");
			break;
		}
		}
		break;
	}
	case(Token::T_DEC_OP): {
		switch (this->type)
		{
		case Variable_Type_Built_In::Built_In::POINTER:{
			const Variable_Type_Pointer* const ptr = dynamic_cast<const Variable_Type_Pointer*>(this);
			assert(ptr);
			if(ptr->getPointerChainDepth() == 1 && ptr->getPointerChainRoot()->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::INT))){
				ptr->moveToRegister1(output, offset_var_1);
				output << tab << "lw	$t2, 0($t0)" << std::endl;
				output << tab << "addi	$t2, $t2, -1" << std::endl;
				output << tab << "sw	$t2, 0($t0)" << std::endl;
			}
			else if(ptr->getPointerChainDepth() == 1 && ptr->getPointerChainRoot()->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::CHAR))){
				ptr->moveToRegister1(output, offset_var_1);
				output << tab << "lb	$t2, 0($t0)" << std::endl;
				output << tab << "addiu	$t2, $t2, -1" << std::endl;
				output << tab << "sb	$t2, 0($t0)" << std::endl;
			}
			else if(ptr->getPointerChainDepth() == 1 && ptr->getPointerChainRoot()->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::UNSIGNED_INT))){
				ptr->moveToRegister1(output, offset_var_1);
				output << tab << "lw	$t2, 0($t0)" << std::endl;
				output << tab << "addiu	$t2, $t2, -1" << std::endl;
				output << tab << "sw	$t2, 0($t0)" << std::endl;
			}
			else if(ptr->getPointerChainDepth() == 1 && ptr->getPointerChainRoot()->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT))){
				ptr->moveToRegister1(output, offset_var_1);
				output << tab << "lwc1	$f4, 0($t0)" << std::endl;
				output << tab <<"li.s	$f6, 1" << std::endl;
				output << tab << "sub.s	$f4, $f4, $f6" << std::endl;
				output << tab << "s.s	$f4, 0($t0)" << std::endl;
			}
			else if(ptr->getPointerChainDepth() == 1 && ptr->getPointerChainRoot()->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE))){
				ptr->moveToRegister1(output, offset_var_1);
				output << tab << "ldc1	$f4, 0($t0)" << std::endl;
				output << tab <<"li.d	$f6, 1" << std::endl;
				output << tab << "sub.d	$f4, $f4, $f6" << std::endl;
				output << tab << "s.d	$f4, 0($t0)" << std::endl;
			}
			else if(ptr->getPointerChainDepth() == 2 ){
				this->moveToRegister1(output, offset_var_1);
				output << tab << "lw	$t2, 0($t0)" << std::endl;
				auto sub_ptr = dynamic_cast<const Variable_Type *>(ptr->getPointsTo());
				assert(sub_ptr);
				output << tab << "addi $t2, $t2, -" << sub_ptr->getSizeOf() << std::endl;
				output << tab << "sw	$t2, 0($t0)" << std::endl;
			}
			else{
				std::cerr<<ptr->getPointerChainRoot()->getTypeName()<<std::endl;
				assert(0 && "Type not implemented for -- operator");
			}
			break;
		}
		default:{
			std::cerr<<"Type "<< this->getTypeName()<<std::endl;
			assert(0 && "T_DEC_OP  operator not yet supported for this type");
			break;
		}
		}
		break;
	}
	default: {
		assert(0 && "Unexpected or not supported token type, incorrectly formed unary expression");
	}
	}
};

Variable_Type_Pointer::Variable_Type_Pointer(const Variable_Type* const points_to) : points_to(points_to) {
	type = Variable_Type_Built_In::Built_In::POINTER;
}
void Variable_Type_Pointer::moveFromStackToAddress(std::ostream& output, const unsigned int& value_to_save, const unsigned int& pointer_offset, const unsigned int& relative_offset) {
	moveToRegister1(output, relative_offset);
	loadImmediateRegister2(output, points_to->getSizeOf());
	output << tab << "mult	$t0, $t2" << std::endl;
	output << tab << "MFLO	$t0" << std::endl;
	moveToRegister2(output, pointer_offset);
	output << tab << "add 	$t0, $t0, $t2" << std::endl;
	//points_to->moveFromOffsetToRegisterDefinedLocation(output, "$t0", value_to_save);
}

void Variable_Type_Pointer::moveFromAddressToStack(std::ostream& output, const unsigned& bottom_of_stack, const unsigned int& pointer_offset, const unsigned int& relative_offset) {
	moveToRegister1(output, relative_offset);
	loadImmediateRegister2(output, points_to->getSizeOf());
	output << tab << "mult	$t0, $t2" << std::endl;
	output << tab << "MFLO	$t0" << std::endl;
	moveToRegister2(output, pointer_offset);
	output << tab << "add 	$t0, $t0, $t2" << std::endl;
	//points_to->moveToOffsetFromRegisterDefinedLocation(output, "$t0", bottom_of_stack);
}

void Variable_Type_Pointer::moveToStackMoveRegister(std::ostream& output, const unsigned int& offset) const {
	moveToRegister(output, offset, "$t4", "$t5");
}
const Variable_Type* Variable_Type_Pointer::getPointsTo() const {
	return points_to;
}
Variable_Type_Pointer::~Variable_Type_Pointer() {
}

Variable_Type_Array::Variable_Type_Array(const Variable_Type* const array_of, const unsigned int & number_of_elements) : Variable_Type_Pointer(array_of), number_of_elements(number_of_elements){

}

std::list<std::map<std::string, const Variable_Type_Alias*>> Type_Manager::alias_types = {};


std::map<Variable_Type_Built_In::Built_In, const Variable_Type_Built_In*> Type_Manager::built_in_types = {
	{	{Variable_Type_Built_In::Built_In::INT,new Variable_Type_Built_In(Variable_Type_Built_In::Built_In::INT) },
		{Variable_Type_Built_In::Built_In::LONG,new Variable_Type_Built_In(Variable_Type_Built_In::Built_In::LONG) },
		{Variable_Type_Built_In::Built_In::UNSIGNED_INT,new Variable_Type_Built_In(Variable_Type_Built_In::Built_In::UNSIGNED_INT)},
		{Variable_Type_Built_In::Built_In::DOUBLE,new Variable_Type_Built_In(Variable_Type_Built_In::Built_In::DOUBLE) },
		{Variable_Type_Built_In::Built_In::FLOAT,new Variable_Type_Built_In(Variable_Type_Built_In::Built_In::FLOAT)},
		{Variable_Type_Built_In::Built_In::CHAR,new Variable_Type_Built_In(Variable_Type_Built_In::Built_In::CHAR)},
		{Variable_Type_Built_In::Built_In::VOID,new Variable_Type_Built_In(Variable_Type_Built_In::Built_In::VOID)}		
	}
};
std::map<const Variable_Type*, const Variable_Type_Pointer*> Type_Manager::pointer_types = {};

const Variable_Type_Pointer* Type_Manager::getPointerType(const Variable_Type* const points_to) {
	if (auto it = pointer_types.find(points_to); it != pointer_types.end()) {
		return it->second;
	}
	const Variable_Type_Pointer* const ptr_type = new Variable_Type_Pointer(points_to);
	pointer_types.insert(std::pair<const Variable_Type*, const Variable_Type_Pointer*>(points_to, ptr_type));
	return ptr_type;
}

std::map<std::pair<const Variable_Type *, unsigned int>, const Variable_Type_Array *> Type_Manager::array_types = {};
const Variable_Type_Array * Type_Manager::getArrayType(const Variable_Type * array_of, const unsigned int & size){
	if( auto it = array_types.find(std::pair<const Variable_Type *, unsigned int>(array_of, size)); it != array_types.end()){
		return it->second;
	}
	const Variable_Type_Array * const arr_type = new Variable_Type_Array(array_of, size);
	using array_pair = std::pair<const Variable_Type *, unsigned int>;
	array_types.insert(std::pair<array_pair, const Variable_Type_Array *>(array_pair(array_of, size),(arr_type)));
	return arr_type;
}

const Variable_Type_Built_In* Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In type) {
	assert(type != Variable_Type_Built_In::Built_In::POINTER && "Wrong type get function called");
	return built_in_types.find(type)->second;
}

const Variable_Type_Alias* Type_Manager::inventAlias(std::string identifier, const Variable_Type* alias_for) {
	const Variable_Type_Alias* const alias = new Variable_Type_Alias(identifier, alias_for);
	assert(alias_types.size() != 0);
	alias_types.back().insert(std::pair<std::string, const Variable_Type_Alias*>(identifier, alias));
	return alias;
}
void Type_Manager::enterScope(){
	alias_types.push_back(std::map<std::string, const Variable_Type_Alias * >());
}
void Type_Manager::exitScope(){
	for (auto u : alias_types.back()) {
		delete u.second;
	}
	alias_types.pop_back();
}
const Variable_Type_Alias* Type_Manager::getAlias(std::string identifier) {

	for(auto it = alias_types.rbegin(); it != alias_types.rend(); it++){
		auto type_entry = it->find(identifier);
		if(type_entry != it->end()){
			return type_entry->second;
		}

	}
	return nullptr;
	
}
const Function_Declaration * Type_Manager::getFunctionDeclaration(std::string name){
	assert(functions.find(name) != functions.end() && "Function name does not exist");
	return functions.find(name)->second;
}

void Type_Manager::cleanUp() {
	for (auto b : built_in_types) {
		delete b.second;
	}
	for (auto p : pointer_types) {
		delete p.second;
	}
	for(auto f : functions){
		delete f.second;
	}
}

std::map<std::string, const Function_Declaration * > Type_Manager::functions = {};
const Function_Declaration * Type_Manager::inventFunction(const std::string &name, const Variable_Type * const return_type, std::vector<std::pair<const Variable_Type *, std::string>> parameter_list){
	Function_Declaration * func = new Function_Declaration(name, return_type, parameter_list);
	functions.insert(std::pair<std::string,const Function_Declaration *>(func->getName(), func));
	return func;
}

//NOTE in mips some arguments are passed to functions in registers, for simplicity we'll just move them straight to the stack
void Function_Declaration::saveArgumentRegisterToStack(std::ostream &output) const{
	std::cout<<std::endl;
	output << tab << "sw $a0, "<<0 + SP_BYTE_OFFSET<<"($fp)" << std::endl;
	output << tab << "sw $a1, "<<4 + SP_BYTE_OFFSET<<"($fp)" << std::endl;
	output << tab << "sw $a2, "<<8 + SP_BYTE_OFFSET<<"($fp)" << std::endl;
	output << tab << "sw $a3, "<<12 + SP_BYTE_OFFSET << "($fp)" << std::endl;
	int offset = 8;
	bool fp = false;
	if(parameter_list.size() >= 1) {
		if(parameter_list[0].first->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE))){
			output << tab << "s.d $f12, "<<offset<<"($fp)" <<std::endl;
			offset += DOUBLE_SIZE;
			fp = true;
		}
		else if(parameter_list[0].first->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT))){
			output << tab << "swc1 $f12, "<<offset<<"($fp)" <<std::endl;
			offset += FLOAT_SIZE;
			fp = true;
		}
	}
	if(fp && parameter_list.size() >= 2){
		if(parameter_list[1].first->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE))){
			output << tab << "s.d $f14, "<<offset<<"($fp)" <<std::endl;
			offset += DOUBLE_SIZE;
			fp = true;
		}
		else if(parameter_list[1].first->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT))){
			output << tab << "swc1 $f14, "<<offset<<"($fp)" <<std::endl;
			offset += FLOAT_SIZE;
			fp = true;
		}
	}
}

int Function_Declaration::parameterListSize(unsigned int offset) const{
	int size = 0;
	for(auto param : this->parameter_list){
		int delta_size =  param.first->getParameterSize();
		offset += delta_size;
		size += delta_size;
	}
	return size;
}

void Function_Declaration::moveIntoParameterRegisters(std::ostream &output) const{
	output << tab << "lw $a0, "<<0<<"($sp)" << std::endl;
	output << tab << "lw $a1, "<<4<<"($sp)" << std::endl;
	output << tab << "lw $a2, "<<8<<"($sp)" << std::endl;
	output << tab << "lw $a3, "<<12 << "($sp)" << std::endl;
	int offset = 0;
	bool fp = false;
	if(parameter_list.size() >= 1) {
		if(parameter_list[0].first->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE))){
			output << tab << "ldc1 $f13, "<<offset<<"($sp)" <<std::endl;
			offset += DOUBLE_SIZE;
			fp = true;
		}
		else if(parameter_list[0].first->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT))){
			output << tab << "lwc1 $f12, "<<offset<<"($sp)" <<std::endl;
			offset += FLOAT_SIZE;
			fp = true;
		}
		if(fp){
			if(parameter_list[0].first->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::DOUBLE))){
				output << tab << "ldc1 $f14, "<<offset<<"($sp)" <<std::endl;
				fp = true;
			}
			else if(parameter_list[0].first->compare(Type_Manager::getBuiltInType(Variable_Type_Built_In::Built_In::FLOAT))){
				output << tab << "lwc1 $f14, "<<offset<<"($sp)" <<std::endl;
				fp = true;
			}
		}
	}
}
int Variable_Type_Built_In::getAllignment() const{
	switch (type)
	{
	case Variable_Type_Built_In::Built_In::CHAR:
		return CHAR_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::SIGNED_CHAR:
		return CHAR_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::SHORT:
		return SHORT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_SHORT:
		return SHORT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::INT:
		return INT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::LONG:
		return LONG_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::ENUM:
		return INT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		return INT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_LONG:
		return LONG_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::POINTER:
		return POINTER_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::FLOAT:
		return FLOAT_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::DOUBLE:
		return DOUBLE_ALLIGNMENT;
		break;
	case Variable_Type_Built_In::Built_In::LONG_DOUBLE:
		return LONG_DOUBLE_ALLIGNMENT;;
		break;
	default:
		assert(0 && "Type Size not Supported yet");
		break;
	}
}
int Variable_Type_Alias::getAllignment() const{
	return alias_for->getAllignment();
}

std::string Variable_Type_Built_In::getTypeName() const {
	switch (type)
	{
	case Variable_Type_Built_In::Built_In::CHAR:
		return "CHAR";
		break;
	case Variable_Type_Built_In::Built_In::SIGNED_CHAR:
		return "SIGNED CHAR";
		break;
	case Variable_Type_Built_In::Built_In::SHORT:
		return "SHORT";
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_SHORT:
		return "UNSIGNED SHORT";
		break;
	case Variable_Type_Built_In::Built_In::INT:
		return "INT";
		break;
	case Variable_Type_Built_In::Built_In::LONG:
		return "LONG";
		break;
	case Variable_Type_Built_In::Built_In::ENUM:
		return "ENUM";
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
		return "UNSIGNED INT";
		break;
	case Variable_Type_Built_In::Built_In::UNSIGNED_LONG:
		return "UNSIGNED LONG";
		break;
	case Variable_Type_Built_In::Built_In::POINTER:
		return "Broken, token to type pointer but is base class";
		break;
	case Variable_Type_Built_In::Built_In::FLOAT:
		return "FLOAT";
		break;
	case Variable_Type_Built_In::Built_In::DOUBLE:
		return "DOUBLE";
		break;
	case Variable_Type_Built_In::Built_In::LONG_DOUBLE:
		return "LONG DOUBLE";;
		break;
	case Variable_Type_Built_In::Built_In::VOID:
		return "VOID TYPE";
		break;
	default:
		assert(0 && "Type Size not Supported yet");
		break;
	}
}
std::string Variable_Type_Alias::getTypeName() const {
	return "alias for " + alias_for->getTypeName();
}
std::string Variable_Type_Pointer::getTypeName() const {
	return "pointer to " + points_to->getTypeName();
}

std::string Variable_Type_Array::getTypeName() const {
	return std::string("array of size ")+std::to_string(this->number_of_elements)+std::string(" containing ")+ points_to->getTypeName();

}

const Variable_Type * Variable_Type_Pointer::getPointerChainRoot() const {
	if(auto pointer_to_cast =  dynamic_cast<const Variable_Type_Pointer*>(this->getPointsTo()); pointer_to_cast){
		return pointer_to_cast->getPointerChainRoot();
	}
	return this->getPointsTo();
}

int Variable_Type_Pointer::getPointerChainDepth() const{
	if(auto pointer_to_cast =  dynamic_cast<const Variable_Type_Pointer*>(this->getPointsTo()); pointer_to_cast){
		return 1 + pointer_to_cast->getPointerChainDepth();
	}
	return 1;
}

unsigned int Variable_Type_Built_In::getParameterSize() const{
	switch (type)
	{
	case Variable_Type_Built_In::Built_In::CHAR:
	case Variable_Type_Built_In::Built_In::SIGNED_CHAR:
	case Variable_Type_Built_In::Built_In::SHORT:
	case Variable_Type_Built_In::Built_In::UNSIGNED_SHORT:
	case Variable_Type_Built_In::Built_In::INT:
	case Variable_Type_Built_In::Built_In::LONG:
	case Variable_Type_Built_In::Built_In::ENUM:
	case Variable_Type_Built_In::Built_In::UNSIGNED_INT:
	case Variable_Type_Built_In::Built_In::UNSIGNED_LONG:
	case Variable_Type_Built_In::Built_In::POINTER:
		return INT_SIZE;
	case Variable_Type_Built_In::Built_In::FLOAT:
		return FLOAT_SIZE;
		break;
	case Variable_Type_Built_In::Built_In::DOUBLE:
		return DOUBLE_SIZE;
		break;
	case Variable_Type_Built_In::Built_In::LONG_DOUBLE:
		return LONG_DOUBLE_SIZE;
		break;
	default:
		assert(0 && "Type Size not Supported yet");
		break;
	}
}
unsigned int Variable_Type_Alias::getParameterSize() const{
	return alias_for->getParameterSize();
}

const Variable_Type * Variable_Type_Alias::resolveAlias() const {
	if(auto alias = dynamic_cast<const Variable_Type_Alias *>(alias_for); alias){
		return alias->resolveAlias();
	}
	return alias_for;
}