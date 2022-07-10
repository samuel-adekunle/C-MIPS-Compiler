%code requires{
	#include "AST.hpp"
	#include <iostream>
	#include <cassert>

	#define SEMICOLON new AST_node(Token::T_SEMICOLON, {})
	#define LEFT_CURLY_BRACKET new AST_node(Token::T_LEFT_CURLY_BRACKET, {})
	#define RIGHT_CURLY_BRACKET new AST_node(Token::T_RIGHT_CURLY_BRACKET, {})
	#define COMMA new AST_node(Token::T_COMMA, {})
	#define COLON new AST_node(Token::T_COLON, {})
	#define EQUAL_SIGN new AST_node(Token::T_EQUAL_SIGN, {})
	#define LEFT_BRACKET new AST_node(Token::T_LEFT_BRACKET, {})
	#define RIGHT_BRACKET new AST_node(Token::T_RIGHT_BRACKET, {})
	#define LEFT_SQUARE_BRACKET new AST_node(Token::T_LEFT_SQUARE_BRACKET,{})
	#define RIGHT_SQUARE_BRACKET new AST_node(Token::T_RIGHT_SQUARE_BRACKET,{})
	#define FULL_STOP new AST_node(Token::T_FULL_STOP, {})
	#define AMPERSAND new AST_node(Token::T_AMPERSAND, {})
	#define EXCLAMATION_MARK new AST_node(Token::T_EXCLAMATION_MARK, {})
	#define TILDE new AST_node(Token::T_TILDE, {})
	#define MINUS new AST_node(Token::T_MINUS, {})
	#define PLUS new AST_node(Token::T_PLUS, {})
	#define STAR new AST_node(Token::T_STAR, {})
	#define FORWARD_SLASH new AST_node(Token::T_FORWARD_SLASH, {})
	#define PERCENT new AST_node(Token::T_PERCENT, {})
	#define LEFT_ANGLE_BRACKET new AST_node(Token::T_LEFT_ANGLE_BRACKET, {})
	#define RIGHT_ANGLE_BRACKET new AST_node(Token::T_RIGHT_ANGLE_BRACKET, {})
	#define CARET new AST_node(Token::T_CARET, {})
	#define PIPE new AST_node(Token::T_PIPE, {})
	#define QUESTION_MARK new AST_node(Token::T_QUESTION_MARK, {})
	
	extern const AST_node * root;
	int yylex(void);
	void yyerror(const char *);
}

%union { AST_node *ast_node; }

%token <ast_node> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN AUTO
%token <ast_node> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token <ast_node> TYPEDEF EXTERN STATIC REGISTER INLINE RESTRICT SIZEOF
%token <ast_node> BOOL COMPLEX IMAGINARY
%token <ast_node> STRUCT UNION ENUM

%token <ast_node> IDENTIFIER TYPE_NAME CONSTANT STRING_LITERAL

%token <ast_node> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token <ast_node> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <ast_node> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token <ast_node> XOR_ASSIGN OR_ASSIGN
%token <ast_node> ELLIPSIS

%type <ast_node> primary_expression postfix_expression argument_expression_list
%type <ast_node> unary_expression unary_operator cast_expression
%type <ast_node> multiplicative_expression additive_expression shift_expression
%type <ast_node> relational_expression equality_expression and_expression
%type <ast_node> exclusive_or_expression inclusive_or_expression
%type <ast_node> logical_and_expression logical_or_expression
%type <ast_node> conditional_expression assignment_expression assignment_operator
%type <ast_node> expression constant_expression declaration
%type <ast_node> declaration_specifiers init_declarator_list init_declarator
%type <ast_node> storage_class_specifier type_specifier struct_or_union_specifier
%type <ast_node> struct_or_union struct_declaration_list struct_declaration
%type <ast_node> specifier_qualifier_list struct_declarator_list
%type <ast_node> struct_declarator enum_specifier enumerator_list enumerator
%type <ast_node> type_qualifier function_specifier declarator
%type <ast_node> direct_declarator pointer type_qualifier_list
%type <ast_node> parameter_type_list parameter_list parameter_declaration
%type <ast_node> identifier_list type_name abstract_declarator
%type <ast_node> direct_abstract_declarator initializer initializer_list
%type <ast_node> designation designator_list designator statement
%type <ast_node> labeled_statement compound_statement block_item_list
%type <ast_node> block_item expression_statement selection_statement
%type <ast_node> iteration_statement jump_statement translation_unit
%type <ast_node> external_declaration function_definition declaration_list

%start ROOT

%%

primary_expression
	: IDENTIFIER 			{ $$ = $1; }
	| CONSTANT 				{ $$ = $1; }
	| STRING_LITERAL 		{ $$ = $1; }
	| '(' expression ')'	{ $$ = new AST_node(Token::primary_expression, { LEFT_BRACKET, $2, RIGHT_BRACKET }); }
	;

postfix_expression
	: primary_expression				    				{ $$ = new AST_expression( Token::postfix_expression, {$1}); }
	| postfix_expression '[' expression ']' 			  	{ $$ = new AST_expression( Token::postfix_expression, { $1, LEFT_SQUARE_BRACKET, $3, RIGHT_SQUARE_BRACKET }); }
	| postfix_expression '(' ')' 						  	{ $$ = new AST_expression( Token::postfix_expression, { $1 , LEFT_BRACKET, RIGHT_BRACKET }); }
	| postfix_expression '(' argument_expression_list ')' 	{ $$ = new AST_expression( Token::postfix_expression, { $1, LEFT_BRACKET, $3, RIGHT_BRACKET }); }
	| postfix_expression '.' IDENTIFIER 					{ $$ = new AST_expression( Token::postfix_expression, { $1, FULL_STOP, $3 }); }
	| postfix_expression PTR_OP IDENTIFIER 					{ $$ = new AST_expression( Token::postfix_expression, { $1, $2, $3 }); }
	| postfix_expression INC_OP 							{ $$ = new AST_expression( Token::postfix_expression, { $1, $2 }); }
	| postfix_expression DEC_OP 							{ $$ = new AST_expression( Token::postfix_expression, { $1, $2 }); }
	| '(' type_name ')' '{' initializer_list '}' 			{ $$ = new AST_expression( Token::postfix_expression, { LEFT_BRACKET, $2, RIGHT_BRACKET, LEFT_CURLY_BRACKET, $5, RIGHT_CURLY_BRACKET }); }
	| '(' type_name ')' '{' initializer_list ',' '}' 		{ $$ = new AST_expression( Token::postfix_expression, { LEFT_BRACKET, $2, RIGHT_BRACKET, LEFT_CURLY_BRACKET, $5, COMMA, RIGHT_CURLY_BRACKET }); }
	;

argument_expression_list
	: assignment_expression 							 { $$ = new AST_node(Token::argument_expression_list, { $1 }); }
	| argument_expression_list ',' assignment_expression { $$ = new AST_node(Token::argument_expression_list, { $1, COMMA, $3 }); }
	;

unary_expression
	: postfix_expression 			 { $$ = $1; }
	| INC_OP unary_expression 		 { $$ = new AST_expression(Token::unary_expression, { $1, $2 }); }
	| DEC_OP unary_expression 		 { $$ = new AST_expression(Token::unary_expression, { $1, $2 }); }
	| unary_operator cast_expression { $$ = new AST_expression(Token::unary_expression, { $1, $2 }); }
	| SIZEOF unary_expression 		 { $$ = new AST_expression(Token::unary_expression, { $1, $2 }); }
	| SIZEOF '(' type_name ')' 		 { $$ = new AST_expression(Token::unary_expression, { $1, LEFT_BRACKET, $3, RIGHT_BRACKET }); }
	;

unary_operator
	: '&' { $$ = AMPERSAND; }
	| '*' { $$ = STAR; }
	| '+' { $$ = PLUS; }
	| '-' { $$ = MINUS; }
	| '~' { $$ = TILDE; }
	| '!' { $$ = EXCLAMATION_MARK; }
	;

cast_expression
	: unary_expression 					{ $$ = $1; }
	| '(' type_name ')' cast_expression { $$ = new AST_expression(Token::cast_expression, { LEFT_BRACKET, $2, RIGHT_BRACKET, $4 }); }
	;

multiplicative_expression
	: cast_expression 								{ $$ = $1; }
	| multiplicative_expression '*' cast_expression { $$ = new AST_expression(Token::multiplicative_expression, { $1, STAR, $3 }); }
	| multiplicative_expression '/' cast_expression { $$ = new AST_expression(Token::multiplicative_expression, { $1, FORWARD_SLASH, $3 }); }
	| multiplicative_expression '%' cast_expression { $$ = new AST_expression(Token::multiplicative_expression, { $1, PERCENT, $3 }); }
	;

additive_expression
	: multiplicative_expression { $$ = $1; }
	| additive_expression '+' multiplicative_expression { $$ = new AST_expression(Token::additive_expression, { $1, PLUS, $3 }); }
	| additive_expression '-' multiplicative_expression { $$ = new AST_expression(Token::additive_expression, { $1, MINUS, $3 }); }
	;

shift_expression
	: additive_expression { $$ = $1; }
	| shift_expression LEFT_OP additive_expression { $$ = new AST_expression(Token::shift_expression, { $1, $2, $3 }); }
	| shift_expression RIGHT_OP additive_expression { $$ = new AST_expression(Token::shift_expression, { $1, $2, $3 }); }
	;

relational_expression
	: shift_expression { $$ = $1; }
	| relational_expression '>' shift_expression { $$ = new AST_expression(Token::relational_expression, { $1, RIGHT_ANGLE_BRACKET, $3 }); }
	| relational_expression '<' shift_expression { $$ = new AST_expression(Token::relational_expression, { $1, LEFT_ANGLE_BRACKET, $3 }); }
	| relational_expression LE_OP shift_expression { $$ = new AST_expression(Token::relational_expression, { $1, $2, $3 }); }
	| relational_expression GE_OP shift_expression { $$ = new AST_expression(Token::relational_expression, { $1, $2, $3 }); }
	;

equality_expression
	: relational_expression 							{ 	$$ = $1; }
	| equality_expression EQ_OP relational_expression 	{	$$ = new AST_expression(Token::equality_expression, { $1, $2, $3 }); }
	| equality_expression NE_OP relational_expression 	{ 	$$ = new AST_expression(Token::equality_expression, { $1, $2, $3 }); }
	;

and_expression
	: equality_expression 						{ $$ = $1; }
	| and_expression '&' equality_expression 	{ $$ = new AST_expression(Token::and_expression, { $1, AMPERSAND, $3 }); }
	;

exclusive_or_expression
	: and_expression 								{ $$ = $1; }
	| exclusive_or_expression '^' and_expression 	{ $$ = new AST_expression(Token::and_expression, { $1, CARET, $3 }); }
	;

inclusive_or_expression
	: exclusive_or_expression 								{ $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression 	{ $$ = new AST_expression(Token::inclusive_or_expression, { $1, PIPE, $3 }); }
	;

logical_and_expression
	: inclusive_or_expression 									{ $$ = $1; }
	| logical_and_expression AND_OP inclusive_or_expression 	{ $$ = new AST_expression(Token::logical_and_expression, { $1, $2, $3 }); }
	;

logical_or_expression
	: logical_and_expression 								{ $$ = $1; }
	| logical_or_expression OR_OP logical_and_expression 	{ $$ = new AST_expression(Token::logical_or_expression, { $1, $2, $3 }); }
	;

conditional_expression
	: logical_or_expression 											{ $$ = $1; }
	| logical_or_expression '?' expression ':' conditional_expression 	{ $$ = new AST_expression(Token::conditional_expression, { $1, QUESTION_MARK, $3, COLON, $5 }); }
	;

assignment_expression
	: conditional_expression 										{ $$ = $1; }
	| unary_expression assignment_operator assignment_expression 	{ $$ = new AST_node(Token::assignment_expression, { $1, $2, $3 }); }
	;

assignment_operator
	: '=' 			{ $$ =  EQUAL_SIGN; }
	| MUL_ASSIGN 	{ $$ = $1; }
	| DIV_ASSIGN 	{ $$ = $1; }
	| MOD_ASSIGN 	{ $$ = $1; }
	| ADD_ASSIGN 	{ $$ = $1; }
	| SUB_ASSIGN 	{ $$ = $1; }
	| LEFT_ASSIGN	{ $$ = $1; }
	| RIGHT_ASSIGN 	{ $$ = $1; }
	| AND_ASSIGN 	{ $$ = $1; }
	| XOR_ASSIGN 	{ $$ = $1; }
	| OR_ASSIGN 	{ $$ = $1; }
	;

expression
	: assignment_expression 				{ $$ = $1; }
	| expression ',' assignment_expression 	{ $$ = new AST_expression(Token::expression, { $1, COMMA, $3 }); }
	;

constant_expression
	: conditional_expression { $$ = $1; }
	;

declaration
	: declaration_specifiers ';' 						{ $$ = new AST_node(Token::declaration, { $1, SEMICOLON }); }
	| declaration_specifiers init_declarator_list ';' 	{ $$ = new AST_node(Token::declaration, { $1, $2, SEMICOLON }); }
	;

declaration_specifiers
	: storage_class_specifier 						 { $$ = new AST_declaration_specifiers( { $1 }); }
	| storage_class_specifier declaration_specifiers { $$ = new AST_declaration_specifiers( { $1, $2 }); }
	| type_specifier 								 { $$ = new AST_declaration_specifiers( { $1 }); }
	| type_specifier declaration_specifiers 		 { $$ = new AST_declaration_specifiers( { $1, $2 }); }
	| type_qualifier 								 { $$ = new AST_declaration_specifiers( { $1 }); }
	| type_qualifier declaration_specifiers 		 { $$ = new AST_declaration_specifiers( { $1, $2 }); }
	| function_specifier				 			 { $$ = new AST_declaration_specifiers( { $1 }); }
	| function_specifier declaration_specifiers 	 { $$ = new AST_declaration_specifiers( { $1, $2 }); }
	;

init_declarator_list
	: init_declarator 								{ $$ = $1; }
	| init_declarator_list ',' init_declarator 		{ $$ = new AST_node(Token::init_declarator_list, { $1, COMMA, $3 }); }
	;

init_declarator
	: declarator 									{ $$ = new AST_node(Token::init_declarator, {$1}); }
	| declarator '=' initializer 					{ $$ = new AST_node(Token::init_declarator, {$1, EQUAL_SIGN, $3}); }
	;

storage_class_specifier
	: TYPEDEF 	{ $$ = $1; }
	| EXTERN 	{ $$ = $1; }
	| STATIC 	{ $$ = $1; }
	| AUTO 		{ $$ = $1; }
	| REGISTER 	{ $$ = $1; }
	;

type_specifier
	: VOID 						{ $$ = $1; }
	| CHAR 						{ $$ = $1; }
	| SHORT 					{ $$ = $1; }
	| INT 						{ $$ = $1; }
	| LONG	 					{ $$ = $1; }
	| FLOAT 					{ $$ = $1; }
	| DOUBLE 					{ $$ = $1; }
	| SIGNED 					{ $$ = $1; }
	| UNSIGNED 					{ $$ = $1; }
	| BOOL 						{ $$ = $1; }
	| COMPLEX 					{ $$ = $1; }
	| IMAGINARY 				{ $$ = $1; }
	| struct_or_union_specifier { $$ = $1; }
	| enum_specifier 			{ $$ = $1; }
	| TYPE_NAME 				{ $$ = $1; }
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}' 	{  $$ = new AST_node(Token::struct_or_union_specifier, {$1, $2, LEFT_CURLY_BRACKET, $4, RIGHT_CURLY_BRACKET}); }
	| struct_or_union '{' struct_declaration_list '}' 				{  $$ = new AST_node(Token::struct_or_union_specifier, {$1, LEFT_CURLY_BRACKET, $3, RIGHT_CURLY_BRACKET}); }
	| struct_or_union IDENTIFIER						 			{  $$ = new AST_node(Token::struct_or_union_specifier, {$1, $2}); }
	;

struct_or_union
	: STRUCT 	{  $$ = $1; } 
	| UNION 	{  $$ = $1; }
	;

struct_declaration_list
	: struct_declaration							{  $$ = $1; }
	| struct_declaration_list struct_declaration	{  $$ = new AST_node(Token::struct_declaration_list, {$1, $2}); }
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'	{ $$ = new AST_node(Token::struct_declarator, {$1, $2, SEMICOLON}); }
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list				{  $$ = new AST_node(Token::specifier_qualifier_list, {$1, $2}); }
	| type_specifier										{  $$ = $1; }
	| type_qualifier specifier_qualifier_list				{  $$ = new AST_node(Token::specifier_qualifier_list, {$1, $2}); }
	| type_qualifier										{  $$ = $1; }
	;

struct_declarator_list
	: struct_declarator								{  $$ = $1; }
	| struct_declarator_list ',' struct_declarator	{  $$ = new AST_node(Token::struct_declarator_list, {$1, COMMA, $3}); }
	;

struct_declarator
	: declarator							{ $$ = $1; }
	| ':' constant_expression				{ $$ = new AST_node(Token::struct_declarator, {COLON, $2}); }
	| declarator ':' constant_expression	{ $$ = new AST_node(Token::struct_declarator, {$1, COLON, $3}); }
	;

enum_specifier
	: ENUM '{' enumerator_list '}'					{ $$ = new AST_node(Token::enum_specifier, {$1, LEFT_CURLY_BRACKET, $3, RIGHT_CURLY_BRACKET}); }
	| ENUM IDENTIFIER '{' enumerator_list '}'		{ $$ = new AST_node(Token::enum_specifier, {$1, $2, LEFT_CURLY_BRACKET, $4, RIGHT_CURLY_BRACKET}); }
	| ENUM '{' enumerator_list ',' '}'				{ $$ = new AST_node(Token::enum_specifier, {$1, LEFT_CURLY_BRACKET, $3, COMMA, RIGHT_CURLY_BRACKET}); }
	| ENUM IDENTIFIER '{' enumerator_list ',' '}'	{ $$ = new AST_node(Token::enum_specifier, {$1, $2, LEFT_CURLY_BRACKET, $4, COMMA, RIGHT_CURLY_BRACKET}); }
	| ENUM IDENTIFIER								{ $$ = new AST_node(Token::enum_specifier, {$1, $2}); }
	;

enumerator_list
	: enumerator									{ $$ = $1; }
	| enumerator_list ',' enumerator				{ $$ = new AST_node(Token::enumerator_list, {$1, COMMA, $3}); }
	;

enumerator
	: IDENTIFIER									{ $$ = $1; }
	| IDENTIFIER '=' constant_expression			{ $$ = new AST_node(Token::enumerator, {$1, EQUAL_SIGN, $3}); }
	;

type_qualifier
	: CONST											{ $$ = $1; }
	| RESTRICT										{ $$ = $1; }
	| VOLATILE										{ $$ = $1; }
	;

function_specifier
	: INLINE										{ $$ = $1; }
	;

declarator
	: pointer direct_declarator						{ $$ = new AST_declarator( {$1, $2}); }
	| direct_declarator								{ $$ = new AST_declarator( {$1}); }
	;


direct_declarator
	: IDENTIFIER																	{ $$ = new AST_node(Token::direct_declarator, {$1}); }
	| '(' declarator ')'															{ $$ = new AST_node(Token::direct_declarator,{LEFT_BRACKET, $2, RIGHT_BRACKET}); }
	| direct_declarator '[' type_qualifier_list assignment_expression ']'			{ $$ = new AST_node(Token::direct_declarator,{$1, LEFT_SQUARE_BRACKET, $3, $4, RIGHT_SQUARE_BRACKET}); }
	| direct_declarator '[' type_qualifier_list ']'									{ $$ = new AST_node(Token::direct_declarator,{$1, LEFT_SQUARE_BRACKET, $3, RIGHT_SQUARE_BRACKET}); }
	| direct_declarator '[' assignment_expression ']'								{ $$ = new AST_node(Token::direct_declarator,{$1, LEFT_SQUARE_BRACKET, $3, RIGHT_SQUARE_BRACKET}); }
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'	{ $$ = new AST_node(Token::direct_declarator,{$1, LEFT_SQUARE_BRACKET, $3, $4, $5, RIGHT_SQUARE_BRACKET}); }
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'	{ $$ = new AST_node(Token::direct_declarator,{$1, LEFT_SQUARE_BRACKET, $3, $4, $5, RIGHT_SQUARE_BRACKET}); }
	| direct_declarator '[' type_qualifier_list '*' ']'								{ $$ = new AST_node(Token::direct_declarator,{$1, LEFT_SQUARE_BRACKET, $3, STAR, RIGHT_SQUARE_BRACKET}); }
	| direct_declarator '[' '*' ']'													{ $$ = new AST_node(Token::direct_declarator,{$1, LEFT_SQUARE_BRACKET, STAR, RIGHT_SQUARE_BRACKET}); }
	| direct_declarator '[' ']'														{ $$ = new AST_node(Token::direct_declarator,{$1, LEFT_SQUARE_BRACKET, RIGHT_SQUARE_BRACKET}); }
	| direct_declarator '(' parameter_type_list ')'									{ $$ = new AST_node(Token::direct_declarator,{$1, LEFT_BRACKET, $3, RIGHT_BRACKET}); }
	| direct_declarator '(' identifier_list ')'										{ $$ = new AST_node(Token::direct_declarator,{$1, LEFT_BRACKET, $3, RIGHT_BRACKET}); }
	| direct_declarator '(' ')'														{ $$ = new AST_node(Token::direct_declarator,{$1, LEFT_BRACKET, RIGHT_BRACKET}); }
	;

pointer
	: '*'												{ $$ = new AST_node(Token::pointer, { STAR }); }
	| '*' type_qualifier_list							{ $$ = new AST_node(Token::pointer, {STAR, $2 }); }
	| '*' pointer										{ $$ = new AST_node(Token::pointer, {STAR, $2}); }
	| '*' type_qualifier_list pointer					{ $$ = new AST_node(Token::pointer, {STAR, $2, $3}); }
	;

type_qualifier_list
	: type_qualifier									{ $$ = $1; }
	| type_qualifier_list type_qualifier				{ $$ = new AST_node(Token::type_qualifier_list, {$1, $2}); }		
	;


parameter_type_list
	: parameter_list									{ $$ = $1; }
	| parameter_list ',' ELLIPSIS						{ $$ = new AST_node(Token::parameter_type_list, {$1, COMMA, $3}); }
	;

parameter_list
	: parameter_declaration								{ $$ = new AST_parameter_list({$1}); }
	| parameter_list ',' parameter_declaration			{ $$ = new AST_parameter_list({$1, $3}); }
	;

parameter_declaration
	: declaration_specifiers declarator					{ $$ = new AST_node(Token::parameter_declaration, {$1, $2}); }
	| declaration_specifiers abstract_declarator		{ $$ = new AST_node(Token::parameter_declaration, {$1, $2}); }
	| declaration_specifiers							{ $$ = $1; }
	;

identifier_list
	: IDENTIFIER						{ $$ = $1; }
	| identifier_list ',' IDENTIFIER	{ $$ = new AST_node(Token::identifier_list, {$1, COMMA, $3}); }
	;

type_name
	: specifier_qualifier_list						{ $$ = $1; }
	| specifier_qualifier_list abstract_declarator	{ $$ = new AST_node(Token::type_name, {$1, $2}); }
	;

abstract_declarator
	: pointer										{ $$ = $1; }
	| direct_abstract_declarator					{ $$ = $1; }
	| pointer direct_abstract_declarator			{ $$ = new AST_node(Token::abstract_declarator, {$1, $2}); }
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'								{ $$ = new AST_node(Token::direct_abstract_declarator, {LEFT_BRACKET, $2, RIGHT_BRACKET }); }
	| '[' ']'													{ $$ = new AST_node(Token::direct_abstract_declarator, {LEFT_SQUARE_BRACKET, RIGHT_SQUARE_BRACKET}); }
	| '[' assignment_expression ']'								{ $$ = new AST_node(Token::direct_abstract_declarator, {LEFT_SQUARE_BRACKET, $2, RIGHT_SQUARE_BRACKET}); }
	| direct_abstract_declarator '[' ']'						{ $$ = new AST_node(Token::direct_abstract_declarator, {$1, LEFT_SQUARE_BRACKET, RIGHT_SQUARE_BRACKET}); }
	| direct_abstract_declarator '[' assignment_expression ']'	{ $$ = new AST_node(Token::direct_abstract_declarator, {$1, LEFT_SQUARE_BRACKET, $3, RIGHT_SQUARE_BRACKET}); }
	| '[' '*' ']'												{ $$ = new AST_node(Token::direct_abstract_declarator, {LEFT_SQUARE_BRACKET, STAR, RIGHT_SQUARE_BRACKET}); }
	| direct_abstract_declarator '[' '*' ']'					{ $$ = new AST_node(Token::direct_abstract_declarator, {$1, LEFT_SQUARE_BRACKET, STAR, RIGHT_SQUARE_BRACKET}); }
	| '(' ')'													{ $$ = new AST_node(Token::direct_abstract_declarator, {LEFT_BRACKET, RIGHT_BRACKET}); }
	| '(' parameter_type_list ')'								{ $$ = new AST_node(Token::direct_abstract_declarator, {LEFT_BRACKET, $2, RIGHT_BRACKET}); }
	| direct_abstract_declarator '(' ')'						{ $$ = new AST_node(Token::direct_abstract_declarator, {$1, LEFT_BRACKET, RIGHT_BRACKET}); }
	| direct_abstract_declarator '(' parameter_type_list ')'	{ $$ = new AST_node(Token::direct_abstract_declarator, {$1, LEFT_BRACKET, $3, RIGHT_BRACKET}); }
	;

initializer
	: assignment_expression					{ $$ = $1; }
	| '{' initializer_list '}'				{ $$ = new AST_node(Token::initializer, {LEFT_CURLY_BRACKET, $2, RIGHT_CURLY_BRACKET}); }
	| '{' initializer_list ',' '}'			{ $$ = new AST_node(Token::initializer, {LEFT_CURLY_BRACKET, $2, COMMA, RIGHT_CURLY_BRACKET}); }
	;

initializer_list
	: initializer										{ $$ = $1; }
	| designation initializer							{ $$ = $1; }
	| initializer_list ',' initializer					{ $$ = new AST_node(Token::initializer_list, {$1, COMMA, $3}); }
	| initializer_list ',' designation initializer		{ $$ = new AST_node(Token::initializer_list, {$1, COMMA, $3}); }
	;

designation
	: designator_list '='			{ $$ = new AST_node(Token::designation, {$1, EQUAL_SIGN}); }
	;

designator_list
	: designator					{ $$ = $1; }
	| designator_list designator	{ $$ = new AST_node(Token::designator_list, {$1, $2}); }
	;

designator
	: '[' constant_expression ']'	{ $$ = new AST_node(Token::designator, {LEFT_SQUARE_BRACKET, $2, RIGHT_SQUARE_BRACKET}); }
	| '.' IDENTIFIER				{ $$ = new AST_node(Token::designator, {FULL_STOP, $2}); }
	;

statement
	: labeled_statement							{ $$ = $1; }
	| compound_statement						{ $$ = $1; }
	| expression_statement						{ $$ = $1; }
	| selection_statement						{ $$ = $1; }
	| iteration_statement						{ $$ = $1; }
	| jump_statement							{ $$ = $1; }
	;

labeled_statement
	: IDENTIFIER ':' statement					{ $$ = new AST_node(Token::labeled_statement, {$1, COLON, $3}); }
	| CASE constant_expression ':' statement	{ $$ = new AST_node(Token::labeled_statement, {$1, $2, COLON, $4}); }
	| DEFAULT ':' statement						{ $$ = new AST_node(Token::labeled_statement, {$1, COLON, $3}); }
	;

compound_statement
	: '{' '}'							{ $$ = new AST_node(Token::compound_statement, { LEFT_CURLY_BRACKET, RIGHT_CURLY_BRACKET}); }
	| '{' block_item_list '}'			{ $$ = new AST_node(Token::compound_statement, { LEFT_CURLY_BRACKET,$2, RIGHT_CURLY_BRACKET}); }
	;

block_item_list
	: block_item						{ $$ = $1; }
	| block_item_list block_item		{ $$ = new AST_node(Token::block_item_list, {$1, $2}); }
	;

block_item
	: declaration				{ $$ = $1; }	
	| statement					{ $$ = $1; }	
	;

expression_statement
	: ';'						{ ; }	
	| expression ';'			{ $$ = $1; }	
	;							

selection_statement
	: IF '(' expression ')' statement 												{ $$ = new AST_node(Token::selection_statement, {$1,LEFT_BRACKET, $3, RIGHT_BRACKET, $5}); }
	| IF '(' expression ')' statement ELSE statement 								{ $$ = new AST_node(Token::selection_statement, {$1,LEFT_BRACKET, $3, RIGHT_BRACKET, $5, $6, $7}); }	
	| SWITCH '(' expression ')' statement											{ $$ = new AST_node(Token::selection_statement, {$1, LEFT_BRACKET, $3, RIGHT_BRACKET, $5}); }	
	;

iteration_statement
	: WHILE '(' expression ')' statement											{ $$ = new AST_node(Token::iteration_statement, {$1, LEFT_BRACKET, $3, RIGHT_BRACKET, $5}); }	
	| DO statement WHILE '(' expression ')' ';'										{ $$ = new AST_node(Token::iteration_statement, {$1, $2, $3, LEFT_BRACKET, $5, RIGHT_BRACKET}); }	
	| FOR '(' expression_statement expression_statement ')' statement				{ $$ = new AST_node(Token::iteration_statement, {$1, LEFT_BRACKET, $3, $4, RIGHT_BRACKET, $6}); }	
	| FOR '(' expression_statement expression_statement expression ')' statement	{ $$ = new AST_node(Token::iteration_statement, {$1, LEFT_BRACKET, $3, $4, $5, RIGHT_BRACKET, $7}); }		
	| FOR '(' declaration expression_statement ')' statement						{ $$ = new AST_node(Token::iteration_statement, {$1, LEFT_BRACKET, $3, $4, RIGHT_BRACKET, $6}); }
	| FOR '(' declaration expression_statement expression ')' statement				{ $$ = new AST_node(Token::iteration_statement, {$1, LEFT_BRACKET, $3, $4, $5, RIGHT_BRACKET, $7}); }	
	;

jump_statement
	: GOTO IDENTIFIER ';'										{ $$ = new AST_node(Token::jump_statement, {$1, SEMICOLON}); }
	| CONTINUE ';'												{ $$ = new AST_node(Token::jump_statement, {$1, SEMICOLON}); }
	| BREAK ';'													{ $$ = new AST_node(Token::jump_statement, {$1, SEMICOLON}); }
	| RETURN ';'												{ $$ = new AST_node(Token::jump_statement, {$1, SEMICOLON}); }
	| RETURN expression ';'										{ $$ = new AST_node(Token::jump_statement, {$1, $2, SEMICOLON}); }
	;

ROOT
	: translation_unit											{ root = $1; }


translation_unit
	: external_declaration										{ $$ = $1; }
	| translation_unit external_declaration						{ $$ = new AST_node(Token::translation_unit, {$1, $2}); }
	;

external_declaration
	: function_definition								{ $$ = $1; }
	| declaration										{ $$ = $1; }
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement 				{ $$ = new AST_function({$1, $2, $3, $4}); }
	| declaration_specifiers declarator compound_statement 									{ $$ = new AST_function({$1, $2, $3}); }
	;

declaration_list
	: declaration															{ $$ = $1; }
	| declaration_list declaration											{ $$ = new AST_node(Token::declaration_list, {$1, $2}); }
	;


%%
#include <stdio.h>

extern char yytext[];
extern int column;
extern int yylineno;

void yyerror(char const *s)
{
	fprintf(stderr, "Error (%d:%d): %s\n", yylineno, column, s);
	fflush(stderr);
}