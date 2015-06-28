#pragma once

#include "Node.hpp"

namespace TIS
{
	class Port;

	enum class Opcode
	{
		/* NOP is a pseudoinstruction */
		MOV,
		SWP,
		SAV,
		ADD,
		SUB,
		NEG,
		JMP,
		JEZ,
		JNZ,
		JGZ,
		JLZ,
		JRO
	};

	struct Instruction
	{
		Opcode opcode;
		
		struct Argument
		{
			bool isOffset;

			union
			{
				Node::Port port;
				short offset;
			};
		};

		Argument arguments[2];
	};
}