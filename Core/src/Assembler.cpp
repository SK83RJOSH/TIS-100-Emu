#include "Assembler.hpp"

#include <queue>

namespace TIS { namespace Assembler	{
	
	struct Token
	{
		enum class Type
		{
			IDENTIFIER,
			NUMBER,
			LABEL
		};

		Type type = Type::IDENTIFIER;
		std::string text;
	};

	std::queue<Token> tokenize(std::string const& code)
	{
		std::queue<Token> tokens;
		Token currentToken;

		for (auto c : code)
		{
			if (currentToken.text.empty())
			{
				if (isalpha(c))
				{
					currentToken.text += c;
				}
				else if (isspace(c))
				{
					// Ignore whitespace starting characters
					continue;
				}
				else
				{
					throw std::logic_error("Attempted to lex token with an "
						"invalid starting character");
				}
			}
			else if (isalnum(c))
			{
				currentToken.text += c;
			}
			else if (c == ':')
			{
				currentToken.type = Token::Type::LABEL;
			}
			else if (c == ',')
			{
				// Ignore commas
				continue;
			}
			else if (isspace(c) || c == '\n')
			{
				tokens.push(currentToken);
				currentToken = Token();
			}
		}

		// Handle case where a newline is not present at the end
		if (!currentToken.text.empty())
			tokens.push(currentToken);

		return tokens;
	}

	Node::Destination parseDestination(Token const& token)
	{
		if (token.type != Token::Type::IDENTIFIER)
			throw std::logic_error("parseDestination: Expected IDENTIFIER");

		if (token.text == "UP")
			return Node::Destination::UP;
		else if (token.text == "DOWN")
			return Node::Destination::DOWN;
		else if (token.text == "LEFT")
			return Node::Destination::LEFT;
		else if (token.text == "RIGHT")
			return Node::Destination::RIGHT;
		else if (token.text == "ANY")
			return Node::Destination::ANY;
		else if (token.text == "LAST")
			return Node::Destination::LAST;
		else if (token.text == "ACC")
			return Node::Destination::ACC;
		else if (token.text == "NIL")
			return Node::Destination::NIL;
		else
			throw std::runtime_error("parseDestination: Invalid destination: " + token.text);
	}

	Instruction parseMov(std::queue<Token>& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::MOV;
		
		// Pop instruction token
		tokens.pop();

		instruction.arguments[0].destination = parseDestination(tokens.front());
		tokens.pop();

		instruction.arguments[1].destination = parseDestination(tokens.front());
		tokens.pop();

		return instruction;
	}

	Instruction parseAdd(std::queue<Token>& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::ADD;
		
		// Pop instruction token
		tokens.pop();

		instruction.arguments[0].destination = parseDestination(tokens.front());
		tokens.pop();

		return instruction;
	}

	Instruction parseNop(std::queue<Token>& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::ADD;
		
		// Pop instruction token
		tokens.pop();

		instruction.arguments[0].destination = Node::Destination::NIL;

		return instruction;
	}

	std::vector<Instruction> assemble(std::string const& code)
	{
		auto tokens = tokenize(code);
		std::vector<Instruction> instructions;

		while (tokens.size())
		{
			auto token = tokens.front();

			if (token.text == "MOV")
				instructions.push_back(parseMov(tokens));
			else if (token.text == "ADD")
				instructions.push_back(parseAdd(tokens));
			else if (token.text == "NOP")
				instructions.push_back(parseNop(tokens));
			else
				throw std::logic_error("Unhandled token! Text: " + token.text);
		}

		return instructions;
	}

} }