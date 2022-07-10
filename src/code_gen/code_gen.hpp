#ifndef GUARD_IR_COMPILER_HPP
#define GUARD_IR_COMPILER_HPP

#include <ostream>
#include "../lang/AST.hpp"

void compile(const AST_node* node, std::ostream& output);

#endif