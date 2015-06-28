#pragma once

#include "Port.hpp"

#include <map>
#include <string>

namespace TIS
{
	class Port;

	class Node
	{
	friend class Port;
	public:
		enum class Port
		{
			UP,
			DOWN,
			LEFT,
			RIGHT,
			ANY,
			LAST
		};

		enum class State
		{
			UNLOCKED,
			LOCKED,
			DEADLOCK
		};

		State getState();
		virtual void step() = 0;
	protected:
		Node();
		void setState(State state);
	private:
		State state;
		std::map<Port, TIS::Port> ports;
	};
}