#pragma once

#include <vector>

#include "Instruction.hpp"

namespace TIS { namespace Disassembler {

	std::string disassemble(Instruction const* instructions, size_t count);

} }