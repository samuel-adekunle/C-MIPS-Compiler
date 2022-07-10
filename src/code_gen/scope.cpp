#include "scope.hpp"
#include "utilities.hpp"
#include <cassert>

//FIXME find way to remove this
Variable::Variable() : type(nullptr), offset(-1), current_function_scope(nullptr), function_scope_offset_before_init(0)
{
}

Variable::Variable(const std::string& name, Function_Scope* const current_function_scope, const Variable_Type* const type)
	: name(name), type(type), current_function_scope(current_function_scope), function_scope_offset_before_init(current_function_scope->offset)
{
	this->offset = current_function_scope->offset + current_function_scope->offsetToNextAvaliableSlot(type->getAllignment(), type->getSizeOf());
}

void Variable::changeTypeAndAllignment(const Variable_Type * const type ){
	assert(this == current_function_scope->getLastVariable() && "Not last variable");
	this->type = type;
	current_function_scope->offset = function_scope_offset_before_init;
	this->offset = current_function_scope->offset + current_function_scope->offsetToNextAvaliableSlot(type->getAllignment(), type->getSizeOf());
	current_function_scope->offset = this->offset + 1;
}
// Variable::Variable( const std::string &name, const Function_Scope *const current_function_scope, Variable_Type_Aggregate_Enum aggregate,
// 					std::initializer_list<Variable> sub_types )
// 	: name( name ), sub_types( sub_types ), current_function_scope( current_function_scope )
// {
// 	switch ( aggregate )
// 	{
// 	case Variable_Type_Aggregate_Enum::STRUCT:
// 		type = Variable_Type::STRUCT;
// 		break;
// 	case Variable_Type_Aggregate_Enum::UNION:
// 		type = Variable_Type::UNION;
// 		break;
// 	case Variable_Type_Aggregate_Enum::ARRAY:
// 		type = Variable_Type::ARRAY;
// 		break;
// 	}
// }

unsigned int Variable::getOffset() const
{
	return offset;
}
std::string Variable::getName() const
{
	return name;
}

unsigned int Variable::getSize() const
{
	return this->getType()->getSizeOf();
}
const Variable_Type* Variable::getType() const
{
	if(auto alias = dynamic_cast<const Variable_Type_Alias *>(type); alias){
		return alias->resolveAlias();
	}
	return type;
}

void Variable::moveIntoReturnRegister(std::ostream& output) const
{
	this->getType()->moveIntoReturnRegister(output, this->getOffset());
}

void Variable::moveFromRegister1(std::ostream& output) const
{
	this->getType()->moveFromRegister1(output, this->getOffset());
}
void Variable::moveFromRegister2(std::ostream& output) const
{
	this->getType()->moveFromRegister2(output, this->getOffset());
}
void Variable::moveFromRegister3(std::ostream& output) const
{
	this->getType()->moveFromRegister3(output, this->getOffset());
}
void Variable::moveToRegister1(std::ostream& output) const
{
	this->getType()->moveToRegister1(output, this->getOffset());
}
void Variable::moveToRegister2(std::ostream& output) const
{
	this->getType()->moveToRegister2(output, this->getOffset());
}
void Variable::moveToRegister3(std::ostream& output) const
{
	this->getType()->moveToRegister3(output, this->getOffset());
}
void Variable::moveFromReturnRegister( std::ostream & output ) const{
	this->getType()->moveFromReturnRegister(output, offset);
}
void Variable::moveFromStackPointer(std::ostream & output) const{
	this->getType()->moveFromStackPointer(output, offset);
}
void Variable::moveToStackPointer(std::ostream &output) const{
	this->getType()->moveToStackPointer(output, offset);
}

const Variable_Type * Variable::binaryOperator(std::ostream& output, Token operator_token, const Variable* const var_2, Function_Scope * const function_scope) const {
	return type->binaryOperator(output, operator_token, this->getOffset(), var_2->getType(), var_2->getOffset());
}
void Variable::unaryOperator(std::ostream& output, Token operator_token) const {
	type->unaryOperator(output, operator_token, this->getOffset());
}

Symbol_Table::Symbol_Table(Function_Scope* const function_scope) : function_scope(function_scope) {

}
bool Symbol_Table::containsIdentifier(const std::string& name) const {
	return table.find(name) != table.end();
}
const Variable* Symbol_Table::getIdentifier(const std::string& name) const {
	if (auto found = table.find(name); found != table.end()) {
		return found->second;
	}
	return nullptr;
}
Variable* Symbol_Table::bottom() const {
	assert(stack.size() != 0);
	return stack.back();
}
const Variable* Symbol_Table::renameBottom(const std::string& name) {
	assert(stack.size() != 0);
	auto node = this->table.extract(stack.back()->name);
	node.key() = name;
	table.insert(std::move(node));
	stack.back()->name = name;
	return stack.back();
}

void Symbol_Table::popStack() {
	assert(stack.size() != 0 && "Tried to remove element from empty identifier list");
	Variable* var = stack.back();
	stack.pop_back();
	table.erase(table.find(var->getName()));
	this->function_scope->offset = var->function_scope_offset_before_init;

	delete var;
}
const Variable* Symbol_Table::popNoDelete() {
	assert(stack.size() != 0 && "Tried to remove element from empty identifier list");
	Variable* var = stack.back();
	stack.pop_back();
	table.erase(table.find(var->getName()));
	this->function_scope->offset = 	var->function_scope_offset_before_init;
	return var;
}
const Variable* Symbol_Table::addVariableToStack(const std::string& name, const Variable_Type* const type) {
	Variable* var = new Variable(name, function_scope, type);
	assert(table.find(name) == table.end() && "Variable in the same scope with the same name");
	table.insert(std::pair<std::string, Variable*>(name, var));
	stack.push_back(var);
	function_scope->offset = var->getOffset() + 1;
	return var;
}

void Variable::copyToVariable(std::ostream& output, const Variable* var_dest) const {
	const static bool USING_BIG_ENDIAN = true;
	//REVIEW could perform some optimisation here to load words
	this->loadAbsoluteOffsetRegister1(output);
	var_dest->moveToRegister2(output);
	if (USING_BIG_ENDIAN) {
		//assert(this->getSize() <= 4 && "Not yet tested on variables of size larger than a word proceed with caution!");
		int counter = 0;
		for (unsigned int i = 0; i < this->getSize(); i++) {
			std::cout << tab << "lb		$t1, " << counter << "($t0)" << std::endl;
			std::cout << tab << "sb		$t1, " << counter << "($t2)" << std::endl;
			counter++;
		}
	}
	else {
		for (unsigned int i = 0; i < this->getSize(); i++) {
			std::cout << tab << "lb		$t1, " << i << "($t0)" << std::endl;
			std::cout << tab << "sb		$t1, " << i << "($t2)" << std::endl;
		}
	}

}

void Symbol_Table::copyToVariable(std::ostream& output, const Variable* const var_dest, const Variable* const var_origin) {
	var_origin->copyToVariable(output, var_dest);
	popStack();
}
void Function_Scope::copyToVariable(std::ostream& output, const Variable* const var_dest, const Variable* const var_origin) {
	inner_scope_stack.back().copyToVariable(output, var_dest, var_origin);
}
void Inner_Scope::copyToVariable(std::ostream& output, const Variable* const var_dest, const Variable* const var_origin) {
	this->symbol_table.copyToVariable(output, var_dest, var_origin);
}
void Variable::loadAbsoluteOffsetRegister1(std::ostream& output) const {
	loadImmediateRegister1(output, this->getOffset());
	output << tab << "sub $t0, $fp, $t0" << std::endl;
}
void Variable::loadAbsoluteOffsetRegister2(std::ostream& output) const {
	loadImmediateRegister2(output, this->getOffset());
	output << "add $t2, $t2, $fp" << std::endl;
}
void Symbol_Table::binaryOperator(std::ostream& output, Token operator_token) {
	const Variable* const bottom = this->popNoDelete();
	const Variable* const penBottom = this->popNoDelete();
	const Variable_Type * const new_type = bottom->binaryOperator(output, operator_token, penBottom, function_scope);
	function_scope->addVariableToStack(function_scope->generateUniqueLiteralName("binary_operator"), new_type)->moveFromRegister1(output);
	delete bottom;
	delete penBottom;
}

void Symbol_Table::unaryOperator(std::ostream& output, Token operator_token) {
	const Variable* const bottom = this->bottom();
	bottom->unaryOperator(output, operator_token);
	if (operator_token == Token::T_STAR) {
		std::string name = bottom->getName();
		this->popNoDelete();
		const Variable_Type_Pointer* const ptr = dynamic_cast<const Variable_Type_Pointer*>(bottom->getType());
		assert(ptr);
		const Variable * const new_var = this->addVariableToStack(name, ptr->getPointsTo());
		new_var->loadContentsFrom(output, bottom->getOffset());
		delete bottom;
	}
}

void Variable::loadContentsFrom(std::ostream &output, const unsigned int & offset) const{
	for (unsigned int i = 0; i < this->getSize(); i++) {
		std::cout << tab << "lb		$t1, -" << offset + i << "($fp)" << std::endl;
		std::cout << tab << "sb		$t1, -" << this->getOffset() + i << "($fp)" << std::endl;
	}
}

Symbol_Table::~Symbol_Table() {
	while (stack.size() != 0) {
		this->popStack();
	}
}

const Variable* Function_Scope::renameBottom(const std::string& name) {
	return inner_scope_stack.back().renameBottom(name);
}
Function_Scope::Function_Scope(const Variable_Type* const return_type) : return_type(return_type)
{
	enterInnerScope();
}
bool Function_Scope::getOffset(const std::string& name, std::string& offset)
{
	for (auto it = inner_scope_stack.rbegin(); it != inner_scope_stack.rend(); it++)
	{
		if (it->getOffset(name, offset))
		{
			return true;
		}
	}
	return false;
}
void Function_Scope::enterInnerScope()
{
	Type_Manager::enterScope();
	inner_scope_stack.push_back(Inner_Scope(this));
}
void Function_Scope::exitInnerScope()
{
	Type_Manager::enterScope();
	inner_scope_stack.pop_back();
}
std::string Function_Scope::generateUniqueLiteralName(std::string base)
{
	return inner_scope_stack.back().generateUniqueLiteralName(base);
}
const Variable* Function_Scope::addVariableToStack(const std::string& name, const Variable_Type* const type)
{
	return inner_scope_stack.back().addVariableToStack(name, type);
}
const Variable* Function_Scope::getLastVariable() const
{
	for(auto s = inner_scope_stack.rbegin(); s != inner_scope_stack.rend(); s++){
		if(const Variable * const var = s->getLastVariable(); var){
			return var;
		}
	}
	return nullptr;
}
const Variable* Function_Scope::getIdentifier(const std::string& name) const {
	for (auto it = inner_scope_stack.rbegin(); it != inner_scope_stack.rend(); it++)
	{
		const Variable* const var = it->getIdentifier(name);
		if (var) {
			return var;
		}
	}
	return nullptr;
}
void Function_Scope::binaryOperator(std::ostream& output, Token operator_token) {
	return inner_scope_stack.back().binaryOperator(output, operator_token);
}
void Function_Scope::unaryOperator(std::ostream& output, Token operator_token) {
	return inner_scope_stack.back().unaryOperator(output, operator_token);
}

void Function_Scope::setBreakLabel(const std::string* label) {
	inner_scope_stack.back().break_label = label;
}

void Function_Scope::setContinueLabel(const std::string* label) {
	inner_scope_stack.back().continue_label = label;
}

const std::string* Function_Scope::getBreakLabel() const {
	for (auto it = inner_scope_stack.rbegin(); it != inner_scope_stack.rend(); it++)
	{
		if (it->break_label)
		{
			return it->break_label;
		}
	}
	return nullptr;
}

const std::string* Function_Scope::getContinueLabel() const {
	for (auto it = inner_scope_stack.rbegin(); it != inner_scope_stack.rend(); it++)
	{
		if (it->continue_label)
		{
			return it->continue_label;
		}
	}
	return nullptr;
}

void Function_Scope::copyArgumentsToNewFrame(const Function_Declaration * declaration, std::ostream & output){
	assert(inner_scope_stack.size() != 0 && "First scope not yet created");
	inner_scope_stack.front().copyArgumentsToNewFrame(declaration, output);
}
void Inner_Scope::copyArgumentsToNewFrame(const Function_Declaration * declaration, std::ostream & output){
	symbol_table.copyArgumentsToNewFrame(declaration, output);
}
//NOTE copies arguments from last frame into current frame
void Symbol_Table::copyArgumentsToNewFrame(const Function_Declaration * declaration, std::ostream & output){
	for(auto p : declaration->getParameterList()){
		std::cerr<<p.first->getTypeName()<<std::endl;
	}
	declaration->saveArgumentRegisterToStack(output);
	

	unsigned int param_list_offset = 0;
	for(int i = 0; i < declaration->getParameterList().size(); i++){
		declaration->getParameterList()[i].first->moveToRegister1(output, SP_BYTE_OFFSET + param_list_offset, true);
		if(declaration->getParameterList()[i].first->getSizeOf())
		this->addVariableToStack(declaration->getParameterList()[i].second, declaration->getParameterList()[i].first)->moveFromRegister1(output);
		param_list_offset += declaration->getParameterList()[i].first->getParameterSize();
	}


}

void Function_Scope::addSwitchLabel(const std::string& label, const std::string& literal) {

	for (auto it = inner_scope_stack.rbegin(); it != inner_scope_stack.rend(); it++)
	{
		if (it->switch_labels)
		{
			return it->switch_labels->push_back({ label, literal });
		}
	}
	assert(0 && "Incorrect use of case statement");
}

void Function_Scope::clearSwitchLabels() {
	inner_scope_stack.back().switch_labels = nullptr;
}

void Function_Scope::setSwitchLabels(std::vector<std::pair<std::string, std::string>>* labels) {
	inner_scope_stack.back().switch_labels = labels;
}

bool Inner_Scope::getOffset(const std::string& name, std::string& offset)
{
	if (symbol_table.containsIdentifier(name)) {
		offset = std::to_string(symbol_table.getIdentifier(name)->getOffset());
		return true;
	}
	return false;
}

Inner_Scope::Inner_Scope(Function_Scope* const function_scope) : symbol_table(function_scope), function_scope(function_scope), initial_offset(function_scope->offset)
{
}

Inner_Scope::~Inner_Scope()
{

}
std::string Inner_Scope::generateUniqueLiteralName(std::string base)
{
	return base + std::to_string(unique++);
}

const Variable* Inner_Scope::addVariableToStack(const std::string& name, const Variable_Type* const type)
{
	return symbol_table.addVariableToStack(name, type);
}
const Variable* Inner_Scope::renameBottom(const std::string& name) {
	return symbol_table.renameBottom(name);
}

std::list<Variable *>::size_type Symbol_Table::getStackSize() const{
	return this->stack.size();
}

const Variable* Inner_Scope::getLastVariable() const {
	if(symbol_table.getStackSize() != 0){
		return symbol_table.bottom();
	}
	return nullptr;
}

const Variable* Inner_Scope::getIdentifier(const std::string& name) const {
	return symbol_table.getIdentifier(name);
}

void Inner_Scope::binaryOperator(std::ostream& output, Token operator_token) {
	symbol_table.binaryOperator(output, operator_token);
}
void Inner_Scope::unaryOperator(std::ostream& output, Token operator_token) {
	symbol_table.unaryOperator(output, operator_token);
}

void Function_Scope::popStack(){
	inner_scope_stack.back().popStack();
}
void Inner_Scope::popStack(){
	symbol_table.popStack();
}

int Function_Scope::offsetAfterAddingArguments(int padding, const Function_Declaration * declaration) const{
	int offset_after = this->offset + padding;
	for(auto param : declaration->getParameterList()){
		offset_after += param.first->getSizeOf(offset_after);
	}
	return offset_after;
}
int Function_Scope::offsetToStackPointer(int offset){
	if((offset% 4) == 0){
		return offset - 4;
	}
	return offset - offset %4;
}

int Function_Scope::offsetToNextAvaliableSlot(const unsigned int& allignment, const unsigned int& size) const{
	unsigned int off =  (allignment - this->offset % allignment ) % allignment;
	off += (off < size - 1) ? size : 0;
	return off;
}

std::list<Variable *>::size_type Function_Scope::getStackSize() const{
	return inner_scope_stack.back().getStackSize();
}
std::list<Variable *>::size_type Inner_Scope::getStackSize() const {
	return this->symbol_table.getStackSize();
}