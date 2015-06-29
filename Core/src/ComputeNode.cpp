#include "ComputeNode.hpp"
#include "Assembler.hpp"
#include "Util.hpp"

#include <iostream>

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

		auto instruction = this->getCurrentInstruction();
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
			this->instructionPointer = Util::clamp(newInstructionPointer, 0u, this->instructions.size() - 1);
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
			auto destination = args[1].destination;

			if (this->getState() == State::IDLE)
			{
				short value;

				if (!this->getArgumentValue(args[0], value))
					return;

				this->write(destination, value);
				return;
			}
			else
			{
				if (destination == Destination::ANY)
				{
					short value;

					this->read(destination, value);
					this->write(destination, value);
					return;
				}
				else {
					if (this->ports[destination]->getState() != Port::State::EMPTY)
						return;

					this->setState(State::IDLE);
				}
			}
			break;
		}
		}

		this->instructionPointer = ++this->instructionPointer % this->instructions.size();
	}

	Instruction ComputeNode::getCurrentInstruction()
	{
		return this->instructions[this->instructionPointer];
	}

	size_t ComputeNode::getInstructionCount()
	{
		return this->instructions.size();
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
				if (this->ports[port]->read(this, value)) 
					return true;
		}
		default:
			return this->ports[destination]->read(this, value);
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

	void ComputeNode::write(Destination destination, short value)
	{
		switch (destination)
		{
		// Don't write in the case of NIL
		case Destination::NIL:
			break;
		case Destination::ACC:
			this->acc = value;
			break;
		default:
			if (this->ports[destination]->getState() != Port::State::WAITING_FOR_DATA)
				this->setState(State::WAIT);

			if (destination == Destination::ANY)
			{
				Destination ports[4] = {
					Destination::LEFT, Destination::UP,
					Destination::RIGHT, Destination::DOWN };

				for (Destination port : ports)
				{
					if (this->ports[port]->getState() == Port::State::WAITING_FOR_DATA)
					{
						this->ports[port]->write(this, value);
						this->setState(State::IDLE);
						return;
					}
				}
			}
			
			this->ports[destination]->write(this, value);
		}
	}
}
