#include <iostream>
#include "utilities.hpp"
#include "constants.hpp"
#include <bitset>
#include <climits>


void loadImmediateRegister1(std::ostream& output, unsigned int imm) {
	unsigned upper_val = imm >> 16;
	unsigned lower_val = (imm << 16) >> 16;
	std::string upper = std::to_string(upper_val);
	std::string lower = std::to_string(lower_val);
	output << tab << "move 	$t0, $0" << std::endl;
	output << tab << "lui		$t0, " << upper << std::endl;
	output << tab << "ori		$t0, " << lower << std::endl;
}
void loadImmediateRegister2(std::ostream& output, unsigned int imm) {
	unsigned upper_val = imm >> 16;
	unsigned lower_val = (imm << 16) >> 16;
	std::string upper = std::to_string(upper_val);
	std::string lower = std::to_string(lower_val);
	output << tab << "move 	$t2, $0" << std::endl;
	output << tab << "lui		$t2, " << upper << std::endl;
	output << tab << "ori		$t2, " << lower << std::endl;
}

void loadImmediateStackPointer(std::ostream & output, unsigned int imm){
	unsigned upper_val = imm>>16;
	unsigned lower_val = (imm<<16)>>16;
	std::string upper = std::to_string(upper_val);
	std::string lower = std::to_string(lower_val);
	output << tab << "move 	$sp, $0" << std::endl;
	output << tab << "lui		$sp, " << upper << std::endl;
	output << tab << "ori		$sp, " << lower << std::endl;
}

unsigned int getFloatBinaryRepresentation(std::string float_str){
	union
    {
        float input; // assumes sizeof(float) == sizeof(int)
        int   output;
    } data;

    data.input = std::stof(float_str);

    std::bitset<sizeof(float) * CHAR_BIT> bits(data.output);
	return bits.to_ulong();
}

unsigned long long getDoubleBinaryRepresentation(double in){
	union myUnion {
		double input;
		uint64_t output;
	} data;

	data.input=in;
	std::bitset<sizeof(double) * CHAR_BIT> bits(data.output);
	return bits.to_ullong();
}
