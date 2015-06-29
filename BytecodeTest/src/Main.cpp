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

	auto node_left = TIS::ComputeNode();
	auto node_right = TIS::ComputeNode();

	try
	{
		node_left.load(
			"MOV 1 RIGHT"
		);
		node_left.setPort(TIS::Node::Destination::RIGHT, node_right.getPort(TIS::Node::Destination::LEFT));
		node_right.load(
			"ADD LEFT"
		);

		std::cout << "\nComputeNode Test:" << std::endl;

		auto start = std::chrono::high_resolution_clock::now();
		auto steps = 0;

		//for (size_t i = 0; i < node_left.getInstructionCount(); i++)
		//{
		//	auto instruction = node_left.getCurrentInstruction();

		//	std::cout << "----------------------" << std::endl;
		//	std::cout << TIS::Disassembler::disassemble(&instruction, 1);
		//	node_left.step();
		//	std::cout	<< std::right
		//				<< "ACC: " << std::setw(4) << node_left.getACC() << " | "
		//				<< "BAK: " << std::setw(4) << node_left.getBAK()
		//				<< std::endl;
		//	std::cout << "----------------------" << std::endl;
		//}

		while (node_right.getACC() < 2)
		{
			node_left.step();
			node_right.step();
		
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