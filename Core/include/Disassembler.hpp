#pragma once

#include <vector>

#include "Instruction.hpp"

namespace TIS { namespace Disassembler {

	std::string disassemble(std::vector<Instruction> const& bytecode);

} }