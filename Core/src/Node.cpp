#include "Node.hpp"

namespace TIS {
	Node::Node()
	{
	}

	bool Node::getPortValue(short& portValue)
	{
		if (this->getState() != State::READ)
			return false;

		bool ret = false;

		if (this->oldHasPortValue)
		{
			portValue = this->oldPortValue;
			this->hasPortValue = false;
			ret = true;
		}

		this->oldHasPortValue = this->hasPortValue;
		this->oldPortValue = this->portValue;

		return ret;
	}

	void Node::setPortValue(short portValue)
	{
		this->portValue = portValue;
		this->hasPortValue = true;
	}

	void Node::setState(State state)
	{
		this->state = state;
	}

	Node::State Node::getState() const
	{
		return this->state;
	}

	char const* Node::getStateString() const
	{
		static char const* strings[] = 
			{"INITIAL", "IDLE", "READ", "WRITE", "DEADLOCK"};

		return strings[static_cast<int>(this->getState())];
	}
}