#include "Node.hpp"

namespace TIS {
	Node::Node() : state(State::IDLE)
	{
		this->setPort(Destination::UP, new Port());
		this->setPort(Destination::DOWN, new Port());
		this->setPort(Destination::LEFT, new Port());
		this->setPort(Destination::RIGHT, new Port());
		this->setPort(Destination::ANY, new Port());
		this->setPort(Destination::LAST, new Port());
	}

	void Node::setState(State state)
	{
		this->state = state;
	}

	Node::State Node::getState()
	{
		return this->state;
	}

	Port* Node::getPort(Destination destination)
	{
		return this->ports[destination];
	}

	void Node::setPort(Destination destination, Port* port)
	{
		this->ports[destination] = port;
	}
}