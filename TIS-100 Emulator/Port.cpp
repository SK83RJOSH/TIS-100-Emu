#include "Port.h"
#include "Node.h"

#include <algorithm>

namespace TIS
{
	void Port::write(Node* node, short value)
	{
		if (this->state == State::HAS_DATA) {
			if (this->lastNode != node)
			{
				this->lastNode->setState(Node::State::DEADLOCK);
				node->setState(Node::State::DEADLOCK);
			}
		}
		else
		{
			this->state = State::HAS_DATA;
			this->value = std::min(std::max(value, static_cast<short>(-999)), static_cast<short>(999));
			this->lastNode = node;
		}
	}

	bool Port::read(Node* node, short& value)
	{
		if (this->state == State::HAS_DATA) {
			this->state = State::EMPTY;
			value = this->value;

			return true;
		}
		else
		{
			if (this->state == State::WAITING_FOR_DATA && this->lastNode != node)
			{
				this->lastNode->setState(Node::State::DEADLOCK);
				node->setState(Node::State::DEADLOCK);
			}

			this->state = State::WAITING_FOR_DATA;
		}

		return false;
	}
}