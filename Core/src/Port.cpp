#include "Port.hpp"
#include "Node.hpp"
#include "Util.hpp"

#include <algorithm>

namespace TIS
{
	Port::State Port::getState()
	{
		return this->state;
	}

	void Port::write(Node* node, short value)
	{
		if (this->state == State::EMPTY || this->state == State::WAITING_FOR_DATA && this->lastNode != node)
		{
			this->state = State::HAS_DATA;
			this->value = Util::clamp(value, -999, 999);
			this->lastNode = node;
		}
		else
		{
			this->lastNode->setState(Node::State::DEADLOCK);
			node->setState(Node::State::DEADLOCK);
		}
	}

	bool Port::read(Node* node, short& value)
	{
		if (this->state == State::HAS_DATA && this->lastNode != node)
		{
			this->state = State::EMPTY;
			value = this->value;

			return true;
		}
		else if (this->state == State::EMPTY)
		{
			this->state = State::WAITING_FOR_DATA;
			this->lastNode = node;
		}
		else if (this->state == State::HAS_DATA || this->state == State::WAITING_FOR_DATA && this->lastNode != node)
		{
			this->lastNode->setState(Node::State::DEADLOCK);
			node->setState(Node::State::DEADLOCK);
		}

		return false;
	}
}