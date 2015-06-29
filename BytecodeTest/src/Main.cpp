#include "Assembler.hpp"
#include "Disassembler.hpp"
#include "ComputeNode.hpp"

#include <iostream>

int main()
{
	auto testString = 
		"MOV LEFT ACC\n"
		"ADD ACC\n"
		"SUB 8\n"
		"NEG\n"
		"MOV ACC, RIGHT\n"
		"SAV\n"
		"SWP\n"
		"NOP\n"
		"JRO 0\n";

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

	auto node = TIS::ComputeNode();

	try
	{
		node.load(
			"ADD 2\n"
			"ADD ACC\n"
			"SUB 3\n"
			"NEG\n"
			"NEG\n"
			"JRO ACC\n"
			"SWP\n"
			"SWP\n"
			"SAV\n"
		);

		std::cout << "\nComputeNode Test:" << std::endl;

		node.step();
		std::cout << "ACC = " << node.getACC() << "; Expected 2." << std::endl;

		node.step();
		std::cout << "ACC = " << node.getACC() << "; Expected 4." << std::endl;

		node.step();
		std::cout << "ACC = " << node.getACC() << "; Expected 1." << std::endl;

		node.step();
		std::cout << "ACC = " << node.getACC() << "; Expected -1." << std::endl;

		node.step();
		node.step();

		node.step();
		std::cout << "ACC = " << node.getACC() << "; Expected 0." << std::endl;

		node.step();
		std::cout << "ACC = " << node.getACC() << "; Expected 1." << std::endl;

		node.step();
		std::cout << "BAK = " << node.getBAK() << "; Expected 1." << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cerr << "Exception while creating node: " << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}