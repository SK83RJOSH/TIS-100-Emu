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

	void disassembleAdd(Instruction instruction, std::stringstream& stream)
	{
		stream << "ADD ";
		disassembleArgument(instruction.arguments[0], stream);
		stream << std::endl;
	}

	std::string disassemble(std::vector<Instruction> const& bytecode)
	{
		std::stringstream stream;

		for (auto& instruction : bytecode)
		{
			switch (instruction.opcode)
			{
			case TIS::Opcode::MOV:
				disassembleMov(instruction, stream);
				break;
			case TIS::Opcode::ADD:
				disassembleAdd(instruction, stream);
				break;
			}
		}

		return stream.str();
	}

} }