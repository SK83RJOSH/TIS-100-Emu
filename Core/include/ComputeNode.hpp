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
		ComputeNode() : Node(), acc(0), bak(0), currentInstruction(0) {};
		void load(std::string const& code);
		short getACC();
		short getBAK();
		virtual void step();
	private:
		short acc, bak;
		size_t currentInstruction;
		std::vector<Instruction> instructions;
		bool read(Destination destination, short& value);
		bool getArgumentValue(Instruction::Argument argument, short& value);
	};
}