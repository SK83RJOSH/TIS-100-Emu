#include "Assembler.hpp"
#include "Disassembler.hpp"

#include <iostream>

int main()
{
	auto testString = 
		"MOV LEFT ACC\n"
		"ADD ACC\n"
		"MOV ACC, RIGHT\n"
		"NOP\n";

	std::cout << "Assembling test case: " << std::endl;
	std::cout << testString << std::endl;
	
	std::vector<TIS::Instruction> bytecode;
	try
	{
		bytecode = TIS::Assembler::assemble(testString);
	}
	catch (std::exception const& e)
	{
		std::cerr << "Exception while assembling: " << e.what();
		return EXIT_FAILURE;
	}

	std::cout << "Disassembled assembly: " << std::endl;
	try
	{
		std::cout << TIS::Disassembler::disassemble(bytecode);
	}
	catch (std::exception const& e)
	{
		std::cerr << "Exception while disassembling: " << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}