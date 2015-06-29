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
			IDLE,
			WAIT,
			DEADLOCK
		};

		State getState();
		Port* getPort(Destination destination);
		void setPort(Destination destination, Port* port);
		virtual void step() = 0;
	protected:
		std::map<Destination, Port*> ports;
		Node();
		void setState(State state);
	private:
		State state;
	};
}