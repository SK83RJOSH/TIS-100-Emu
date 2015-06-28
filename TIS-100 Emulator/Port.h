#pragma once

namespace TIS
{
	class Node;

	class Port
	{
	public:
		enum State
		{
			EMPTY,
			WAITING_FOR_DATA,
			HAS_DATA
		};

		Port() : state(State::EMPTY) {};
		void write(Node* node, short value);
		bool read(Node* node, short& value);
	private:
		Node* lastNode;
		State state;
		short value;
	};
}