#pragma once

#include "Port.hpp"

#include <map>
#include <string>

namespace TIS
{
	class Node
	{
	friend class Port;
	public:
		enum class Destination : unsigned char
		{
			UP,
			DOWN,
			LEFT,
			RIGHT,
			ANY,
			LAST,
			ACC,
			NIL
		};

		enum class State : unsigned char
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
		std::map<Destination, Port> ports;
	};
}