#include <cstdarg>
#include <iostream>
#include <vector>
#include "lang/AST.hpp"

extern int yyparse();
const AST_node* root;


int unique_name = 0;

void recursiveDescent(const AST_node* node, int number) {
	std::vector<int> ids;
	for (auto n : node->child_nodes) {
		ids.push_back(++unique_name);
		if (n->child_nodes.size() != 0) {
			recursiveDescent(n, unique_name);
		}
	}

	std::cout << enum_names[static_cast<int>(node->token_type)] << number << " -> ";

	int i = 0;
	for (auto n : node->child_nodes) {
		std::cout << enum_names[static_cast<int>(n->token_type)] << ids[i++] << " ";
	}

	std::cout << std::endl;

}

int main(int argc, char* argv[])
{
	root = nullptr;
	Type_Manager::enterScope();


	yyparse();
	Type_Manager::exitScope();

	recursiveDescent(root, unique_name);

}
