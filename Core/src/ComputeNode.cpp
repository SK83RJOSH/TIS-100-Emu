#include "ComputeNode.hpp"
#include "Assembler.hpp"
#include "Util.hpp"

#include <algorithm>

namespace TIS
{
	ComputeNode::ComputeNode()
	{
	}

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
		if (this->getState() == State::DEADLOCK || this->instructions.size() == 0) 
			return;

		auto instruction = this->instructions[this->instructionPointer];
		auto& args = instruction.arguments;

		switch (instruction.opcode)
		{
		case Opcode::SWP:
		{
			auto oldBak = this->bak;
			this->bak = this->acc;
			this->acc = oldBak;
			break;
		}
		case Opcode::SAV:
		{
			this->bak = this->acc;
			break;
		}
		case Opcode::NEG:
		{
			this->acc = -this->acc;
			break;
		}
		case Opcode::JRO:
		{
			short value;

			if (!this->getArgumentValue(args[0], value)) 
				return;

			auto newInstructionPointer = this->instructionPointer + value;			
			this->instructionPointer = Util::clamp(newInstructionPointer, 0u, this->instructions.size());
			return;
		}
		case Opcode::ADD:
		{
			short value;

			if (!this->getArgumentValue(args[0], value)) 
				return;

			this->acc = Util::clamp(this->acc + value, -999, 999);
			break;
		}
		case Opcode::SUB:
		{
			short value;

			if (!this->getArgumentValue(args[0], value)) 
				return;

			this->acc = Util::clamp(this->acc - value, -999, 999);
			break;
		}
		case Opcode::MOV:
		{
			short value;

			if (!this->getArgumentValue(args[0], value)) 
				return;
			
			// TODO: write(Argument, value)
			this->ports[args[1].destination].write(this, value);
			break;
		}
		}

		this->instructionPointer = ++this->instructionPointer % this->instructions.size();
	}

	bool ComputeNode::read(Destination destination, short& value)
	{
		switch (destination)
		{
		case Destination::NIL:
		{
			value = 0;
			return true;
		}
		case Destination::ACC:
		{
			value = this->acc;
			return true;
		}
		case Destination::ANY:
		{
			Destination ports[4] = { 
				Destination::LEFT, Destination::UP, 
				Destination::RIGHT, Destination::DOWN };

			for (Destination port : ports)
				if (this->ports[port].read(this, value)) 
					return true;
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
