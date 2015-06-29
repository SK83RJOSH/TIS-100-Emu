#include "Assembler.hpp"
#include "Disassembler.hpp"
#include "ComputeNode.hpp"

#include <chrono>
#include <iostream>
#include <iomanip>

int main()
{
	auto testString = 
		"label_0:\n"
		"MOV LEFT ACC\n"
		"label_1:\n"
		"ADD ACC\n"
		"label_2:\n"
		"SUB 8\n"
		"label_3:\n"
		"NEG\n"
		"MOV ACC, RIGHT\n"
		"SAV\n"
		"SWP\n"
		"JMP label_4\n"
		"label_4:\n"
		"NOP\n"
		"JRO 1\n"
		"JMP label_0\n"
		"JEZ label_1\n"
		"JNZ label_2\n"
		"JGZ label_3\n"
		"JLZ label_4\n"
		;

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
		std::cout << TIS::Disassembler::disassemble(bytecode.data(), bytecode.size());
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

		auto start = std::chrono::high_resolution_clock::now();

		for (size_t i = 0; i < node.getInstructionCount(); i++)
		{
			auto instruction = node.getCurrentInstruction();

			std::cout << "----------------------" << std::endl;
			std::cout << TIS::Disassembler::disassemble(&instruction, 1);
			node.step();
			std::cout	<< std::right
						<< "ACC: " << std::setw(4) << node.getACC() << " | "
						<< "BAK: " << std::setw(4) << node.getBAK() 
						<< std::endl;
			std::cout << "----------------------" << std::endl;
		}

		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - start
		).count() << "ms" << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cerr << "Exception while creating node: " << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}