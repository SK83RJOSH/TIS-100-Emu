#include "Assembler.hpp"
#include "Disassembler.hpp"
#include "ComputeNode.hpp"

#include <chrono>
#include <iostream>
#include <iomanip>

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
		std::cout << TIS::Disassembler::disassemble(bytecode.data(), bytecode.size());
	}
	catch (std::exception const& e)
	{
		std::cerr << "Exception while disassembling: " << e.what();
		return EXIT_FAILURE;
	}

	TIS::ComputeNode nodeLeft;
	TIS::ComputeNode nodeRight;

	nodeLeft.right = &nodeRight;
	nodeRight.left = &nodeLeft;

	try
	{
		nodeLeft.load("MOV 1, RIGHT");
		nodeRight.load("ADD LEFT");

		std::cout << "\nComputeNode Test:" << std::endl;

		auto start = std::chrono::high_resolution_clock::now();
		auto steps = 0;

		while (nodeRight.getACC() < 2)
		{
			auto instruction = nodeLeft.getCurrentInstruction();

			std::cout << "Step " << steps << std::endl;

			std::cout << "nodeLeft: " << TIS::Disassembler::disassemble(&instruction, 1);
			nodeLeft.step();
			std::cout	<< std::right
						<< "ACC: " << std::setw(4) << nodeLeft.getACC() << " | "
						<< "BAK: " << std::setw(4) << nodeLeft.getBAK() << " | "
						<< "STATE: " << nodeLeft.getStateString()
						<< std::endl;
			std::cout << "------------------------------------" << std::endl;
			
			instruction = nodeRight.getCurrentInstruction();

			std::cout << "nodeRight: " << TIS::Disassembler::disassemble(&instruction, 1);
			nodeRight.step();
			std::cout	<< std::right
						<< "ACC: " << std::setw(4) << nodeRight.getACC() << " | "
						<< "BAK: " << std::setw(4) << nodeRight.getBAK() << " | "
						<< "STATE: " << nodeRight.getStateString()
						<< std::endl;
			std::cout << "------------------------------------" << std::endl;
			std::cout << std::endl;

			++steps;
		}

		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - start
		).count() << "ms - " << steps << " steps" << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cerr << "Exception while creating node: " << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}