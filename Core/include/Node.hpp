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
		virtual ~Node() {}

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
		std::map<Destination, Port> ports;
		Node() : state(State::UNLOCKED) {};
		void setState(State state);
	private:
		State state;
	};
}