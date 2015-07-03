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
		this->setState(State::INITIAL);
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
		if (this->getState() == State::INITIAL)
		{
			this->setState(State::IDLE);
			return;
		}

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
			break;
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

			short sourceValue = 0;
			if (!this->getArgumentValue(args[0], sourceValue))
			{
				// no value
				// TODO: Handle
				break;
			}

			this->write(destination, sourceValue);
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
		default:
			this->setState(State::READ);
			return this->getPortValue(value);
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
		case Destination::UP:
			if (this->up)
				this->up->setPortValue(value);
			break;
		case Destination::DOWN:
			if (this->down)
				this->down->setPortValue(value);
			break;
		case Destination::LEFT:
			if (this->left)
				this->left->setPortValue(value);
			break;
		case Destination::RIGHT:
			if (this->right)
				this->right->setPortValue(value);
			break;
		}

		this->setState(State::WRITE);
	}
}
