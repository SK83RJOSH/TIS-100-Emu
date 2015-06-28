#pragma once

#include <vector>
#include <memory>

#include "Node.hpp"

namespace TIS
{
	class VirtualMachine
	{
	public:
		VirtualMachine();

		void run();

	private:
		std::vector<std::unique_ptr<Node>> nodes;
	};
}