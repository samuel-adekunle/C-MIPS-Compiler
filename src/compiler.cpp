#include "lang/AST.hpp"
#include "code_gen/code_gen.hpp"
#include <iostream>

extern int yyparse();
const AST_node* root;

int main(int argc, char* argv[])
{
	root = nullptr;
	Type_Manager::enterScope();
	yyparse();
	compile(root, std::cout);
	Type_Manager::exitScope();
	Type_Manager::cleanUp();

}
