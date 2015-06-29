#include "Disassembler.hpp"

#include <sstream>

namespace TIS { namespace Disassembler {

	void disassembleDestination(Node::Destination destination, std::stringstream& stream)
	{
		switch (destination)
		{
		case Node::Destination::UP:
			stream << "UP";
			break;
		case Node::Destination::DOWN:
			stream << "DOWN";
			break;
		case Node::Destination::LEFT:
			stream << "LEFT";
			break;
		case Node::Destination::RIGHT:
			stream << "RIGHT";
			break;
		case Node::Destination::ANY:
			stream << "ANY";
			break;
		case Node::Destination::LAST:
			stream << "LAST";
			break;
		case Node::Destination::ACC:
			stream << "ACC";
			break;
		case Node::Destination::NIL:
			stream << "NIL";
			break;
		default:
			throw std::logic_error("Tried to disassemble invalid destination");
		}
	}

	void disassembleArgument(Instruction::Argument argument, std::stringstream& stream)
	{
		if (argument.isOffset)
			stream << argument.offset;
		else
			disassembleDestination(argument.destination, stream);
	}

	void disassembleMov(Instruction instruction, std::stringstream& stream)
	{
		stream << "MOV ";
		disassembleArgument(instruction.arguments[0], stream);
		stream << ", ";
		disassembleArgument(instruction.arguments[1], stream);
		stream << std::endl;
	}

	void disassembleSwp(Instruction instruction, std::stringstream& stream)
	{
		stream << "SWP" << std::endl;
	}

	void disassembleSav(Instruction instruction, std::stringstream& stream)
	{
		stream << "SAV" << std::endl;
	}

	void disassembleAdd(Instruction instruction, std::stringstream& stream)
	{
		stream << "ADD ";
		disassembleArgument(instruction.arguments[0], stream);
		stream << std::endl;
	}

	void disassembleSub(Instruction instruction, std::stringstream& stream)
	{
		stream << "SUB ";
		disassembleArgument(instruction.arguments[0], stream);
		stream << std::endl;
	}

	void disassembleNeg(Instruction instruction, std::stringstream& stream)
	{
		stream << "NEG" << std::endl;
	}

	void disassembleJro(Instruction instruction, std::stringstream& stream)
	{
		stream << "JRO ";
		disassembleArgument(instruction.arguments[0], stream);
		stream << std::endl;
	}

	std::string disassemble(Instruction const* instructions, size_t count)
	{
		std::stringstream stream;

		for (size_t i = 0u; i < count; ++i)
		{
			auto& instruction = instructions[i];

			switch (instruction.opcode)
			{
			case TIS::Opcode::MOV:
				disassembleMov(instruction, stream);
				break;
			case TIS::Opcode::SWP:
				disassembleSwp(instruction, stream);
				break;
			case TIS::Opcode::SAV:
				disassembleSav(instruction, stream);
				break;
			case TIS::Opcode::ADD:
				disassembleAdd(instruction, stream);
				break;
			case TIS::Opcode::SUB:
				disassembleSub(instruction, stream);
				break;
			case TIS::Opcode::NEG:
				disassembleNeg(instruction, stream);
				break;
			case TIS::Opcode::JRO:
				disassembleJro(instruction, stream);
				break;
			}
		}

		return stream.str();
	}

} }