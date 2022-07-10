#ifndef GUARD_TYPES_HPP
#define GUARD_TYPES_HPP
#include <iostream>
#include <map>
#include <vector>
#include <list>
#include "../lang/tokens.hpp"


class Type {
public:
	virtual ~Type() = default;
};

class Variable_Type;

class Function_Declaration : public Type{
	const Variable_Type * return_type = nullptr;
	std::string function_name;
	std::vector<std::pair<const Variable_Type*, std::string>> parameter_list; 
public:
	Function_Declaration(const std::string &name, const Variable_Type * const return_type, std::vector<std::pair<const Variable_Type*, std::string>> parameter_list);
	
	const Variable_Type * getReturnType() const;
	std::string getName() const;
	const std::vector<std::pair<const Variable_Type*, std::string>> getParameterList() const;
	void saveArgumentRegisterToStack(std::ostream &output) const;
	void moveIntoParameterRegisters(std::ostream &output) const;
	int parameterListSize(unsigned int size=0) const;
};
class Variable_Type : public Type {
	friend class Variable;
	friend class Variable_Type_Alias;
protected:
	virtual const Variable_Type * binaryOperator(std::ostream& output, Token operator_token, const unsigned int& offset_var_1, const Variable_Type* const var2, const unsigned int& offset_var_2) const = 0;
	virtual void unaryOperator(std::ostream& output, Token operator_token, const unsigned int& offset_var_1) const = 0;

public:
	virtual unsigned int getSizeOf(const unsigned int&offset = 0) const = 0;
	virtual unsigned int getParameterSize() const = 0;
	virtual void moveIntoReturnRegister( std::ostream &output, const unsigned int &offset ) const = 0;
	virtual void moveFromReturnRegister( std::ostream & output, const unsigned int &offset ) const = 0;
	virtual void moveFromRegister1(std::ostream & output, const unsigned int& offset) const = 0;
	virtual void moveFromRegister2(std::ostream & output, const unsigned int& offset) const = 0;
	virtual void moveFromRegister3(std::ostream & output, const unsigned int& offset) const = 0;
	virtual void moveToRegister1(std::ostream & output, const unsigned int& offset, bool up=false) const = 0;
	virtual void moveToRegister2(std::ostream & output, const unsigned int& offset) const = 0;
	virtual void moveToRegister3(std::ostream & output, const unsigned int& offset) const = 0;

	virtual void stackMove(std::ostream & output, const unsigned int& offset) const = 0;
	
	virtual void moveFromStackPointer(std::ostream & output, const unsigned int & offset) const = 0;
	virtual void moveToStackPointer(std::ostream &output, const unsigned int & offset) const = 0;
	virtual ~Variable_Type() = default;
	virtual std::string getTypeName() const = 0;
	virtual int getAllignment() const = 0;
	bool compare(const Variable_Type* const var) const;
	bool compare(const Variable_Type& var) const;
};

class Variable_Type_Alias : public Variable_Type {
	const std::string& identifier;
	const Variable_Type* const alias_for;
protected:
	const Variable_Type * binaryOperator(std::ostream& output, Token operator_token, const unsigned int& offset_var_1, const Variable_Type* const var2, const unsigned int& offset_var_2) const;

	virtual void unaryOperator(std::ostream& output, Token operator_token, const unsigned int& offset_var_1) const;

public:
	Variable_Type_Alias(const std::string& identifier, const Variable_Type* const alias_for);
	const Variable_Type * resolveAlias() const;
	virtual unsigned int getSizeOf(const unsigned int&offset = 0) const;
	virtual void moveIntoReturnRegister( std::ostream &output, const unsigned int &offset ) const;
	virtual void moveFromReturnRegister( std::ostream & output, const unsigned int &offset ) const;
	virtual void moveFromRegister1(std::ostream & output, const unsigned int& offset) const;
	virtual void moveFromRegister2(std::ostream & output, const unsigned int& offset) const;
	virtual void moveFromRegister3(std::ostream & output, const unsigned int& offset) const;
	virtual void moveToRegister1(std::ostream & output, const unsigned int& offset, bool up=false) const;
	virtual void moveToRegister2(std::ostream & output, const unsigned int& offset) const;
	virtual void moveToRegister3(std::ostream & output, const unsigned int& offset) const;
	virtual void stackMove(std::ostream & output, const unsigned int& offset) const;
	virtual void moveFromStackPointer(std::ostream & output, const unsigned int & offset) const;
	virtual void moveToStackPointer(std::ostream &output, const unsigned int & offset) const;
	virtual int getAllignment() const;
	virtual std::string getTypeName() const;
	virtual unsigned int getParameterSize() const;

	virtual ~Variable_Type_Alias();
};
class Variable_Type_Built_In : public Variable_Type {
protected:
	void moveFromRegister(std::ostream& output, const unsigned int& offset, const std::string& register_start, const std::string& register_overflow) const;
	void moveToRegister(std::ostream& output, const unsigned int& offset, const std::string& register_start, const std::string& register_overflow,  bool up=false) const;
	virtual const Variable_Type * binaryOperator(std::ostream& output, Token operator_token, const unsigned int& offset_var_1, const Variable_Type* const var2, const unsigned int& offset_var_2) const;
	virtual void unaryOperator(std::ostream& output, Token operator_token, const unsigned int& offset_var_1) const;
	Variable_Type_Built_In() = default;
public:
	enum class Built_In
	{
		CHAR,
		SIGNED_CHAR,
		SHORT,
		UNSIGNED_SHORT,
		INT,
		LONG,
		ENUM,
		UNSIGNED_INT,
		UNSIGNED_LONG,
		POINTER,
		FLOAT,
		DOUBLE,
		LONG_DOUBLE,
		VOID //REVIEW Probably should remove
	};
	Variable_Type_Built_In(const Built_In type);
	virtual unsigned int getSizeOf(const unsigned int &offset=0) const;
	virtual void moveIntoReturnRegister( std::ostream &output, const unsigned int &offset ) const;
	virtual void moveFromReturnRegister( std::ostream & output, const unsigned int &offset ) const;
	virtual void moveFromRegister1(std::ostream & output, const unsigned int& offset) const;
	virtual void moveFromRegister2(std::ostream & output, const unsigned int& offset) const;
	virtual void moveFromRegister3(std::ostream & output, const unsigned int& offset) const;
	virtual void moveToRegister1(std::ostream & output, const unsigned int& offset, bool up=false) const;
	virtual void moveToRegister2(std::ostream & output, const unsigned int& offset) const;
	virtual void moveToRegister3(std::ostream & output, const unsigned int& offset) const;
	virtual void stackMove(std::ostream & output, const unsigned int& offset) const;
	virtual void moveFromStackPointer(std::ostream & output, const unsigned int & offset) const;
	virtual void moveToStackPointer(std::ostream &output, const unsigned int & offset) const;
	virtual std::string getTypeName() const;
	virtual unsigned int getParameterSize() const;

	virtual Variable_Type_Built_In::Built_In getType() const;
	virtual int getAllignment() const;
	virtual ~Variable_Type_Built_In();
protected:
	Variable_Type_Built_In::Built_In type;
};

class Variable_Type_Pointer : public Variable_Type_Built_In {
protected:
	const Variable_Type* const points_to;
public:
	Variable_Type_Pointer(const Variable_Type* const points_to);

	virtual void moveFromStackToAddress(std::ostream& output, const unsigned int& value_to_save, const unsigned int& pointer_offset, const unsigned int& relative_offset = 0);
	virtual void moveFromAddressToStack(std::ostream& output, const unsigned& bottom_of_stack, const unsigned int& pointer_offset, const unsigned int& relative_offset = 0);
	virtual void moveToStackMoveRegister(std::ostream& output, const unsigned int& offset) const;
	virtual ~Variable_Type_Pointer();
	virtual std::string getTypeName() const;
	virtual const Variable_Type* getPointsTo() const;
	virtual const Variable_Type * getPointerChainRoot() const;
	int getPointerChainDepth() const;
};

class Variable_Type_Array : public Variable_Type_Pointer {
public:
	const unsigned int number_of_elements;
	Variable_Type_Array(const Variable_Type* const array_of, const unsigned int & number_of_elements);
	virtual std::string getTypeName() const;
	virtual ~Variable_Type_Array() = default;
};


class Type_Manager{
	static std::map<std::string, const Function_Declaration * > functions;
	static std::map<Variable_Type_Built_In::Built_In, const Variable_Type_Built_In *> built_in_types;
	static std::map<const Variable_Type *, const Variable_Type_Pointer *> pointer_types;
	static std::map<std::pair<const Variable_Type *, unsigned int>, const Variable_Type_Array *> array_types;
	static std::list<std::map<std::string, const Variable_Type_Alias * >> alias_types;
public:
	static const Variable_Type_Array * getArrayType(const Variable_Type * const array_of, const unsigned int& size);
	static const Variable_Type_Pointer * getPointerType(const Variable_Type * const points_to);
	static const Variable_Type_Alias * inventAlias(std::string identifier, const Variable_Type * alias_for);
	static const Variable_Type_Alias * getAlias(std::string identifier);
	static void enterScope();
	static void exitScope();
	static const Variable_Type * getUserDefinedType(std::string identifier);
	static const Variable_Type_Built_In * getBuiltInType(Variable_Type_Built_In::Built_In type);
	static const Function_Declaration * inventFunction(const std::string &name, const Variable_Type * const return_type, std::vector<std::pair<const Variable_Type *, std::string>> parameter_list);
	static const Function_Declaration * getFunctionDeclaration(std::string name);
	static void cleanUp();
};

#endif