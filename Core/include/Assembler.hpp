#pragma once

#include <vector>

#include "Instruction.hpp"

namespace TIS { namespace Assembler {

	std::vector<Instruction> assemble(std::string const& code);

} }