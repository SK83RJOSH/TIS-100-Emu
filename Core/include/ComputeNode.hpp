#pragma once

#include "Node.hpp"
#include "Instruction.hpp"

#include <string>
#include <vector>

namespace TIS
{
	class ComputeNode : public Node
	{
	public:
		ComputeNode();
		virtual ~ComputeNode() {}

		void load(std::string const& code);
		short getACC();
		short getBAK();
		virtual void step();
	private:
		short acc = 0;
		short bak = 0;
		size_t instructionPointer = 0;
		std::vector<Instruction> instructions;
		bool read(Destination destination, short& value);
		bool getArgumentValue(Instruction::Argument argument, short& value);
	};
}