#include "Node.hpp"

namespace TIS {
	void Node::setState(State state)
	{
		this->state = state;
	}

	Node::State Node::getState()
	{
		return this->state;
	}
}