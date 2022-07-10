#ifndef GUARD_SCOPE_HPP
#define GUARD_SCOPE_HPP
#include "../lang/AST.hpp"
#include "../lang/tokens.hpp"
#include "constants.hpp"
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>

class Variable
{
	friend class Symbol_Table;
	std::string name;

	const Variable_Type* type;

	std::vector<Variable> sub_types;

	unsigned int offset;
	Function_Scope* const current_function_scope;
	void copyToVariable(std::ostream& output, const Variable* const var_dest) const;
	Variable(const std::string& name, Function_Scope* const current_function_scope, const Variable_Type* const type);	

public:
	const unsigned int function_scope_offset_before_init;
	// REVIEW should probably delete this but needed for unordered_map
	Variable();

	std::string getName() const;

	unsigned int getSize() const;
	unsigned int getOffset() const;
	const Variable_Type* getType() const;

	void moveIntoReturnRegister( std::ostream &output ) const;
	void moveFromReturnRegister(std::ostream & output) const;
	void moveFromRegister1(std::ostream& output) const;
	void moveFromRegister2(std::ostream& output) const;
	void moveFromRegister3(std::ostream& output) const;
	void moveToRegister1(std::ostream& output) const;
	void moveToRegister2(std::ostream& output) const;
	void moveToRegister3(std::ostream& output) const;
	void moveFromStackPointer(std::ostream &output) const;
	void moveToStackPointer(std::ostream &output) const;

	void loadAbsoluteOffsetRegister1(std::ostream & output) const;
	void loadAbsoluteOffsetRegister2(std::ostream & output) const;

	virtual const Variable_Type * binaryOperator(std::ostream & output, Token operator_token, const Variable * var_2, Function_Scope * const function_scope) const;
	//FIXME the unary operator should also returna a pointer type
	virtual void unaryOperator(std::ostream & output, Token operator_token) const;

	void changeTypeAndAllignment(const Variable_Type * const new_type);
	void loadContentsFrom(std::ostream& output, const unsigned int & offset) const;

	virtual ~Variable() = default;

};

class Symbol_Table
{
private:
	Function_Scope* const function_scope;
	std::unordered_map<std::string, Variable*> table;
	std::list<Variable*> stack;
	const Variable* popNoDelete();

public:
	Symbol_Table(Function_Scope* const function_scope);
	bool containsIdentifier(const std::string& name) const;
	const Variable* getIdentifier(const std::string& name) const;
	Variable* bottom() const;
	void popStack();
	const Variable *addVariableToStack(const std::string &name, const Variable_Type * const type);
	void copyArgumentsToNewFrame(const Function_Declaration * declaration, std::ostream & output);

	const Variable *renameBottom(const std::string & name);
	virtual void binaryOperator(std::ostream & output, Token operator_token);
	virtual void unaryOperator(std::ostream & output, Token operator_token);
	void copyToVariable( std::ostream & output, const Variable * const var_dest, const Variable * var_origin ) ;
	std::list<Variable *>::size_type getStackSize() const;
	~Symbol_Table();

};

class Inner_Scope;

class Function_Scope
{
	friend class Symbol_Table;
private:
	std::list<Inner_Scope> inner_scope_stack;

public:
	// FIXME improve encapsulation of this class
	const Variable_Type* const return_type;
	unsigned int offset = 1;
	Function_Scope(const Variable_Type* const return_type);
	bool getOffset(const std::string& name, std::string& offset);
	void enterInnerScope();
	void exitInnerScope();
	std::string generateUniqueLiteralName(std::string base="literal_");
	
	const Variable *addVariableToStack( const std::string &name, const Variable_Type * const type );
	const Variable *getLastVariable() const;
	const Variable *renameBottom(const std::string & name);
	const Variable *getIdentifier(const std::string &name) const;
	void copyArgumentsToNewFrame(const Function_Declaration * declaration, std::ostream & output);
	void copyToVariable( std::ostream & output, const Variable * const var_dest, const Variable * var_origin );
	void popStack();
	std::list<Variable *>::size_type getStackSize() const;
	virtual void binaryOperator(std::ostream& output, Token operator_token);
	virtual void unaryOperator(std::ostream& output, Token operator_token);

	void setContinueLabel(const std::string* label);
	const std::string* getContinueLabel() const;

	void setBreakLabel(const std::string* label);
	const std::string* getBreakLabel() const;

	void addSwitchLabel(const std::string& label, const std::string& literal);
	void setSwitchLabels(std::vector<std::pair<std::string, std::string>>*);
	void clearSwitchLabels();

	int offsetAfterAddingArguments(int padding, const Function_Declaration * declaration) const;
	static int offsetToStackPointer(int offset);
	virtual ~Function_Scope() = default;
	int offsetToNextAvaliableSlot(const unsigned int& allignment, const unsigned int&size) const;
};
class Inner_Scope
{
	friend class Function_Scope;


	std::vector<std::pair<std::string, std::string>>* switch_labels = nullptr;

	const std::string* continue_label = nullptr;
	const std::string* break_label = nullptr;

	// REVIEW Might have to make this vector due to allignment requirements of certain types.
	Symbol_Table symbol_table;
	Function_Scope* const function_scope;
	unsigned int unique = 0;

	// NOTE Has to return string because could be global variable which needs to print non integer characters in assembly
	bool getOffset(const std::string& name, std::string& offset);

	Inner_Scope(Function_Scope* const function_scope);
	std::string generateUniqueLiteralName(std::string base);

	const Variable *addVariableToStack( const std::string &name, const Variable_Type * const type );
	void copyArgumentsToNewFrame(const Function_Declaration * declaration, std::ostream & output);

	const Variable *getLastVariable() const;
	const Variable *renameBottom(const std::string &name);
	const Variable *getIdentifier(const std::string &name) const;
	void copyToVariable( std::ostream & output, const Variable * const var_dest, const Variable * var_origin ) ;
	void popStack();
	virtual void binaryOperator(std::ostream & output, Token operator_token);
	virtual void unaryOperator(std::ostream & output, Token operator_token);
	std::list<Variable *>::size_type getStackSize() const;

  public:
  	const unsigned int initial_offset;
	~Inner_Scope();
};

// class Variable_Global {
// };

// class Global_Scope {
// 	std::unordered_map<std::string, Variable_Global *> table;
// };

#endif