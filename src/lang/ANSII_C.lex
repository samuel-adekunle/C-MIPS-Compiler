D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*
start 		\/\*
end  		\*\/

%option yylineno
%{
#include <stdio.h>
#include "../code_gen/types.hpp"
#include "y.tab.h"

void count();
int check_type(std::string*);
%}

%%

"/*"			{}

"auto"			{ yylval.ast_node = new AST_node(Token::T_AUTO, {}); count(); return(AUTO); }
"break"			{ yylval.ast_node = new AST_node(Token::T_BREAK, {}); count(); return(BREAK); }
"case"			{ yylval.ast_node = new AST_node(Token::T_CASE, {}); count(); return(CASE); }
"char"			{ yylval.ast_node = new AST_node(Token::T_CHAR, {}); count(); return(CHAR); }
"const"			{ yylval.ast_node = new AST_node(Token::T_CONST, {}); count(); return(CONST); }
"continue"		{ yylval.ast_node = new AST_node(Token::T_CONTINUE, {}); count(); return(CONTINUE); }
"default"		{ yylval.ast_node = new AST_node(Token::T_DEFAULT, {}); count(); return(DEFAULT); }
"do"			{ yylval.ast_node = new AST_node(Token::T_DO, {}); count(); return(DO); }
"double"		{ yylval.ast_node = new AST_node(Token::T_DOUBLE, {}); count(); return(DOUBLE); }
"else"			{ yylval.ast_node = new AST_node(Token::T_ELSE, {}); count(); return(ELSE); }
"enum"			{ yylval.ast_node = new AST_node(Token::T_ENUM, {}); count(); return(ENUM); }
"extern"		{ yylval.ast_node = new AST_node(Token::T_EXTERN, {}); count(); return(EXTERN); }
"float"			{ yylval.ast_node = new AST_node(Token::T_FLOAT, {}); count(); return(FLOAT); }
"for"			{ yylval.ast_node = new AST_node(Token::T_FOR, {}); count(); return(FOR); }
"goto"			{ yylval.ast_node = new AST_node(Token::T_GOTO, {}); count(); return(GOTO); }
"if"			{ yylval.ast_node = new AST_node(Token::T_IF, {}); count(); return(IF); }
"int"			{ yylval.ast_node = new AST_node(Token::T_INT, {}); count(); return(INT); }
"long"			{ yylval.ast_node = new AST_node(Token::T_LONG, {}); count(); return(LONG); }
"register"		{ yylval.ast_node = new AST_node(Token::T_REGISTER, {}); count(); return(REGISTER); }
"return"		{ yylval.ast_node = new AST_node(Token::T_RETURN, {}); count(); return(RETURN); }
"short"			{ yylval.ast_node = new AST_node(Token::T_SHORT, {}); count(); return(SHORT); }
"signed"		{ yylval.ast_node = new AST_node(Token::T_SIGNED, {}); count(); return(SIGNED); }
"sizeof"		{ yylval.ast_node = new AST_node(Token::T_SIZEOF, {}); count(); return(SIZEOF); }
"static"		{ yylval.ast_node = new AST_node(Token::T_STATIC, {}); count(); return(STATIC); }
"struct"		{ yylval.ast_node = new AST_node(Token::T_STRUCT, {}); count(); return(STRUCT); }
"switch"		{ yylval.ast_node = new AST_node(Token::T_SWITCH, {}); count(); return(SWITCH); }
"typedef"		{ yylval.ast_node = new AST_node(Token::T_TYPEDEF, {}); count(); return(TYPEDEF); }
"union"			{ yylval.ast_node = new AST_node(Token::T_UNION, {}); count(); return(UNION); }
"unsigned"		{ yylval.ast_node = new AST_node(Token::T_UNSIGNED, {}); count(); return(UNSIGNED); }
"void"			{ yylval.ast_node = new AST_node(Token::T_VOID, {}); count(); return(VOID); }
"volatile"		{ yylval.ast_node = new AST_node(Token::T_VOLATILE, {}); count(); return(VOLATILE); }
"while"			{ yylval.ast_node = new AST_node(Token::T_WHILE, {}); count(); return(WHILE); }

{L}({L}|{D})*			{
							std::string* text = new std::string(yytext);
							int type = check_type(text);
							if(type == IDENTIFIER) 
								yylval.ast_node = new AST_literal(Token::T_IDENTIFIER, text);
							else
								yylval.ast_node = new AST_literal(Token::T_TYPE_NAME, text);
							count(); return(type);
						}

0[xX]{H}+{IS}?			{ yylval.ast_node = new AST_literal(Token::T_CONSTANT, new std::string(yytext)); count(); return(CONSTANT); }
0{D}+{IS}?				{ yylval.ast_node = new AST_literal(Token::T_CONSTANT, new std::string(yytext)); count(); return(CONSTANT); }
{D}+{IS}?				{ yylval.ast_node = new AST_literal(Token::T_CONSTANT, new std::string(yytext)); count(); return(CONSTANT); }
L?'(\\.|[^\\'])+'		{ yylval.ast_node = new AST_literal(Token::T_CONSTANT, new std::string(yytext)); count(); return(CONSTANT); }
{D}+{E}{FS}?			{ yylval.ast_node = new AST_literal(Token::T_CONSTANT, new std::string(yytext)); count(); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ yylval.ast_node = new AST_literal(Token::T_CONSTANT, new std::string(yytext)); count(); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ yylval.ast_node = new AST_literal(Token::T_CONSTANT, new std::string(yytext)); count(); return(CONSTANT); }

L?\"(\\.|[^\\"])*\"		{ yylval.ast_node = new AST_literal(Token::T_STRING_LITERAL, new std::string(yytext)); count(); return(STRING_LITERAL); }

\/\/(.*)		{} 

{start}.*{end}	{} 


"..."			{ yylval.ast_node = new AST_node(Token::T_ELLIPSIS, {}); count(); return(ELLIPSIS); }
">>="			{ yylval.ast_node = new AST_node(Token::T_RIGHT_ASSIGN, {}); count(); return(RIGHT_ASSIGN); }
"<<="			{ yylval.ast_node = new AST_node(Token::T_LEFT_ASSIGN, {}); count(); return(LEFT_ASSIGN); }
"+="			{ yylval.ast_node = new AST_node(Token::T_ADD_ASSIGN, {}); count(); return(ADD_ASSIGN); }
"-="			{ yylval.ast_node = new AST_node(Token::T_SUB_ASSIGN, {}); count(); return(SUB_ASSIGN); }
"*="			{ yylval.ast_node = new AST_node(Token::T_MUL_ASSIGN, {}); count(); return(MUL_ASSIGN); }
"/="			{ yylval.ast_node = new AST_node(Token::T_DIV_ASSIGN, {}); count(); return(DIV_ASSIGN); }
"%="			{ yylval.ast_node = new AST_node(Token::T_MOD_ASSIGN, {}); count(); return(MOD_ASSIGN); }
"&="			{ yylval.ast_node = new AST_node(Token::T_AND_ASSIGN, {}); count(); return(AND_ASSIGN); }
"^="			{ yylval.ast_node = new AST_node(Token::T_XOR_ASSIGN, {}); count(); return(XOR_ASSIGN); }
"|="			{ yylval.ast_node = new AST_node(Token::T_OR_ASSIGN, {}); count(); return(OR_ASSIGN); }
">>"			{ yylval.ast_node = new AST_node(Token::T_RIGHT_OP, {}); count(); return(RIGHT_OP); }
"<<"			{ yylval.ast_node = new AST_node(Token::T_LEFT_OP, {}); count(); return(LEFT_OP); }
"++"			{ yylval.ast_node = new AST_node(Token::T_INC_OP, {}); count(); return(INC_OP); }
"--"			{ yylval.ast_node = new AST_node(Token::T_DEC_OP, {}); count(); return(DEC_OP); }
"->"			{ yylval.ast_node = new AST_node(Token::T_PTR_OP, {}); count(); return(PTR_OP); }
"&&"			{ yylval.ast_node = new AST_node(Token::T_AND_OP, {}); count(); return(AND_OP); }
"||"			{ yylval.ast_node = new AST_node(Token::T_OR_OP, {}); count(); return(OR_OP); }
"<="			{ yylval.ast_node = new AST_node(Token::T_LE_OP, {}); count(); return(LE_OP); }
">="			{ yylval.ast_node = new AST_node(Token::T_GE_OP, {}); count(); return(GE_OP); }
"=="			{ yylval.ast_node = new AST_node(Token::T_EQ_OP, {}); count(); return(EQ_OP); }
"!="			{ yylval.ast_node = new AST_node(Token::T_NE_OP, {}); count(); return(NE_OP); }
";"				{ count(); return(';'); }
("{"|"<%")		{ count(); return('{'); }
("}"|"%>")		{ count(); return('}'); }
","				{ count(); return(','); }
":"				{ count(); return(':'); }
"="				{ count(); return('='); }
"("				{ count(); return('('); }
")"				{ count(); return(')'); }
("["|"<:")		{ count(); return('['); }
("]"|":>")		{ count(); return(']'); }
"."				{ count(); return('.'); }
"&"				{ count(); return('&'); }
"!"				{ count(); return('!'); }
"~"				{ count(); return('~'); }
"-"				{ count(); return('-'); }
"+"				{ count(); return('+'); }
"*"				{ count(); return('*'); }
"/"				{ count(); return('/'); }
"%"				{ count(); return('%'); }
"<"				{ count(); return('<'); }
">"				{ count(); return('>'); }
"^"				{ count(); return('^'); }
"|"				{ count(); return('|'); }
"?"				{ count(); return('?'); }

[ \t\v\n\f]		{ count(); }
.				{ /* ignore bad characters */ }

%%

int yywrap()
{
	return(1);
}

int column = 0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++){
		if (yytext[i] == '\n'){
			column = 0;
		}
		else if (yytext[i] == '\t'){
			column += 8 - (column % 8);
		}
		else{
			column++;
		}
	}

}

int check_type(std::string* text)
{
	const Variable_Type_Alias * const alias = Type_Manager::getAlias(*text);
	if (!alias) return(IDENTIFIER);
	return(TYPE_NAME);
}
