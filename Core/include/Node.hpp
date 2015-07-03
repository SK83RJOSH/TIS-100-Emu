#pragma once

#include <map>
#include <string>

namespace TIS
{
	class Node
	{
	public:
		Node();
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
			INITIAL,
			IDLE,
			READ,
			WRITE,
			DEADLOCK
		};

		State getState() const;
		char const* getStateString() const;
		virtual void step() = 0;

		// Called on the port *receiving*
		bool getPortValue(short& portValue);
		void setPortValue(short portValue);

		Node* up = nullptr;
		Node* down = nullptr;
		Node* left = nullptr;
		Node* right = nullptr;

	protected:
		void setState(State state);

		// This buffer exists as transmission is a step in itself
		short oldPortValue = 0;
		bool oldHasPortValue = false;

		short portValue = 0;
		bool hasPortValue = false;

	private:
		State state = State::IDLE;
	};
}