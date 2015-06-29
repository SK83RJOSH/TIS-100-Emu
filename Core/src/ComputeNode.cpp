#include "ComputeNode.hpp"
#include "Assembler.hpp"

#include <algorithm>

namespace TIS
{
	void ComputeNode::load(std::string const& code)
	{
		this->instructions = TIS::Assembler::assemble(code);
	}

	short ComputeNode::getACC()
	{
		return this->acc;
	}

	short ComputeNode::getBAK()
	{
		return this->bak;
	}

	void ComputeNode::step()
	{
		if (this->getState() == State::DEADLOCK || this->instructions.size() == 0) return;

		Instruction instruction = this->instructions[this->currentInstruction];

		switch (instruction.opcode)
		{
		case Opcode::SWP:
			this->acc ^= this->bak;
			this->bak ^= this->acc;
			this->acc ^= this->bak;
			break;
		case Opcode::SAV:
			this->bak = this->acc;
			break;
		case Opcode::NEG:
			this->acc *= -1;
			break;
		case Opcode::JRO:
			{
			short value;

			if (!this->getArgumentValue(instruction.arguments[0], value)) return;
			
			this->currentInstruction = std::min(std::max(this->currentInstruction + value, static_cast<size_t>(0)), this->instructions.size());
			return;
			}
		case Opcode::ADD:
			{
			short value;

			if (!this->getArgumentValue(instruction.arguments[0], value)) return;
			
			this->acc = std::min(std::max(static_cast<short>(this->acc + value), static_cast<short>(-999)), static_cast<short>(999));
			break;
			}
		case Opcode::SUB:
			{
			short value;

			if (!this->getArgumentValue(instruction.arguments[0], value)) return;

			this->acc = std::min(std::max(static_cast<short>(this->acc - value), static_cast<short>(-999)), static_cast<short>(999));
			break;
			}
		case Opcode::MOV:
			{
			short value;

			if (!this->getArgumentValue(instruction.arguments[0], value)) return;
			
			this->ports[instruction.arguments[1].destination].write(this, value); // TODO: write(Argument, value)
			break;
			}
		}

		this->currentInstruction = (this->currentInstruction + 1) % this->instructions.size();
	}

	bool ComputeNode::read(Destination destination, short& value)
	{
		switch (destination)
		{
		case Destination::NIL:
			value = 0;
			return true;
		case Destination::ACC:
			value = this->acc;
			return true;
		case Destination::ANY:
		{
			Destination ports[4] = { Destination::LEFT, Destination::UP, Destination::RIGHT, Destination::DOWN };

			for (Destination port : ports)
			{
				if (this->ports[port].read(this, value)) return true;
			}
		}
		default:
			return this->ports[destination].read(this, value);
		}

		return false;
	}

	bool ComputeNode::getArgumentValue(Instruction::Argument argument, short& value)
	{
		if (argument.isOffset)
		{
			value = argument.offset;
			return true;
		}

		return this->read(argument.destination, value);
	}
}
