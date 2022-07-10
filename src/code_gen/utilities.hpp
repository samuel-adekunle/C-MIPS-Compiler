#ifndef GUARD_UTILITIES_HPP
#define GUARD_UTILITIES_HPP

void loadImmediateRegister1(std::ostream & output, unsigned int imm);
void loadImmediateRegister2(std::ostream & output, unsigned int imm);
void loadImmediateStackPointer(std::ostream &output, unsigned int imm);

unsigned int getFloatBinaryRepresentation(std::string float_str);
unsigned long long getDoubleBinaryRepresentation(double in);

#endif