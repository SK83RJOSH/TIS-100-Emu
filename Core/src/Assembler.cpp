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
				if (isalnum(c))
				{
					currentToken.text += c;
					if (isalpha(c))
						currentToken.type = Token::Type::IDENTIFIER;
					else
						currentToken.type = Token::Type::NUMBER;
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
				if (currentToken.type == Token::Type::NUMBER && isalpha(c))
					throw std::logic_error("Encountered alphabetical character "
						"while lexing number");
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

	Node::Destination parseDestination(std::queue<Token>& tokens)
	{
		auto token = tokens.front();

		if (token.type != Token::Type::IDENTIFIER)
			throw std::logic_error("parseDestination: Expected IDENTIFIER");

		tokens.pop();

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

	Instruction::Argument parseArgument(std::queue<Token>& tokens, bool destinationOnly = false)
	{
		Instruction::Argument ret;

		auto token = tokens.front();

		if (token.type == Token::Type::IDENTIFIER || destinationOnly)
		{
			ret.destination = parseDestination(tokens);
		}
		else if (token.type == Token::Type::NUMBER)
		{
			ret.offset = std::stoi(token.text);
			ret.isOffset = true;

			tokens.pop();
		}
		else
		{
			throw std::logic_error("parseArgument: Invalid token type");
		}

		return ret;
	}

	Instruction parseMov(std::queue<Token>& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::MOV;
		
		// Pop instruction token
		tokens.pop();

		instruction.arguments[0] = parseArgument(tokens);
		instruction.arguments[1] = parseArgument(tokens, true);

		return instruction;
	}

	Instruction parseSav(std::queue<Token>& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::SAV;
		
		// Pop instruction token
		tokens.pop();

		return instruction;
	}

	Instruction parseSwp(std::queue<Token>& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::SWP;
		
		// Pop instruction token
		tokens.pop();

		return instruction;
	}

	Instruction parseAdd(std::queue<Token>& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::ADD;
		
		// Pop instruction token
		tokens.pop();

		instruction.arguments[0] = parseArgument(tokens);

		return instruction;
	}

	Instruction parseSub(std::queue<Token>& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::SUB;
		
		// Pop instruction token
		tokens.pop();

		instruction.arguments[0] = parseArgument(tokens);

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

	Instruction parseNeg(std::queue<Token>& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::NEG;
		
		// Pop instruction token
		tokens.pop();

		return instruction;
	}

	Instruction parseJro(std::queue<Token>& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::JRO;
		
		// Pop instruction token
		tokens.pop();

		instruction.arguments[0] = parseArgument(tokens);

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
			else if (token.text == "SWP")
				instructions.push_back(parseSwp(tokens));
			else if (token.text == "SAV")
				instructions.push_back(parseSav(tokens));
			else if (token.text == "ADD")
				instructions.push_back(parseAdd(tokens));
			else if (token.text == "SUB")
				instructions.push_back(parseSub(tokens));
			else if (token.text == "NOP")
				instructions.push_back(parseNop(tokens));
			else if (token.text == "NEG")
				instructions.push_back(parseNeg(tokens));
			else if (token.text == "JRO")
				instructions.push_back(parseJro(tokens));
			else
				throw std::logic_error("Unhandled token! Text: " + token.text);
		}

		return instructions;
	}

} }