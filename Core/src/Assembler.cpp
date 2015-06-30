#include "Assembler.hpp"

#include <deque>
#include <algorithm>

namespace TIS { namespace Assembler {

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

	typedef std::deque<Token> Tokens;
	typedef std::map<std::string, short> Labels;

	bool isValidIdentifierCharacter(char c)
	{
		return isalnum(c) || (c == '_');
	}

	Tokens tokenize(std::string const& code)
	{
		Tokens tokens;
		Token currentToken;

		for (auto c : code)
		{
			if (currentToken.text.empty())
			{
				if (isValidIdentifierCharacter(c))
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
			else if (isValidIdentifierCharacter(c))
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
				tokens.push_back(currentToken);
				currentToken = Token();
			}
		}

		// Handle case where a newline is not present at the end
		if (!currentToken.text.empty())
			tokens.push_back(currentToken);

		return tokens;
	}

	Node::Destination parseDestination(Tokens& tokens)
	{
		auto token = tokens.front();

		if (token.type != Token::Type::IDENTIFIER)
			throw std::logic_error("parseDestination: Expected IDENTIFIER");

		tokens.pop_front();

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
			throw std::logic_error("parseDestination: Invalid destination: " + token.text);
	}

	Instruction::Argument parseArgument(Tokens& tokens, bool destinationOnly = false)
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

			tokens.pop_front();
		}
		else
		{
			throw std::logic_error("parseArgument: Invalid token type");
		}

		return ret;
	}

	Instruction::Argument parseLabel(Tokens& tokens, Labels& labels, size_t index)
	{
		Instruction::Argument ret;

		auto token = tokens.front();
		if (token.type != Token::Type::IDENTIFIER)
			throw std::logic_error("parseLabel: Expecting identifier");

		auto text = token.text;
		if (labels.find(text) == labels.end())
			throw std::logic_error("parseLabel: Couldn't find label");

		ret.isOffset = true;
		ret.offset = labels[text];

		tokens.pop_front();

		return ret;
	}

	Instruction parseMov(Tokens& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::MOV;
		
		// Pop instruction token
		tokens.pop_front();

		instruction.arguments[0] = parseArgument(tokens);
		instruction.arguments[1] = parseArgument(tokens, true);

		return instruction;
	}

	Instruction parseSav(Tokens& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::SAV;
		
		// Pop instruction token
		tokens.pop_front();

		return instruction;
	}

	Instruction parseSwp(Tokens& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::SWP;
		
		// Pop instruction token
		tokens.pop_front();

		return instruction;
	}

	Instruction parseAdd(Tokens& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::ADD;
		
		// Pop instruction token
		tokens.pop_front();

		instruction.arguments[0] = parseArgument(tokens);

		return instruction;
	}

	Instruction parseSub(Tokens& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::SUB;
		
		// Pop instruction token
		tokens.pop_front();

		instruction.arguments[0] = parseArgument(tokens);

		return instruction;
	}

	Instruction parseNop(Tokens& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::ADD;
		
		// Pop instruction token
		tokens.pop_front();

		instruction.arguments[0].destination = Node::Destination::NIL;

		return instruction;
	}

	Instruction parseNeg(Tokens& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::NEG;
		
		// Pop instruction token
		tokens.pop_front();

		return instruction;
	}

	Instruction parseJro(Tokens& tokens)
	{
		Instruction instruction;
		instruction.opcode = TIS::Opcode::JRO;
		
		// Pop instruction token
		tokens.pop_front();

		instruction.arguments[0] = parseArgument(tokens);

		return instruction;
	}

	Instruction parseJmp(Opcode opcode, Tokens& tokens, Labels& labels, size_t index)
	{
		Instruction instruction;
		instruction.opcode = opcode;
		
		// Pop instruction token
		tokens.pop_front();

		instruction.arguments[0] = parseLabel(tokens, labels, index);

		return instruction;
	}
	
	std::vector<Instruction> assemble(std::string const& code)
	{
		auto tokens = tokenize(code);
		std::vector<Instruction> instructions;
		std::map<size_t, size_t> labelLocations;
		Labels labelIndices;

		size_t index = 0;
		// Do an initial pass in order to create labels
		for (auto& token : tokens)
		{
			if (token.type == Token::Type::LABEL)
			{
				if (labelIndices.find(token.text) != labelIndices.end())
					throw std::logic_error("Attempted to redefine label: " + token.text);

				labelIndices[token.text] = index++;
				continue;
			}
		}

		// Do pass to synthesize instructions
		index = 0;
		while (tokens.size())
		{
			auto token = tokens.front();

			if (token.type == Token::Type::LABEL)
			{
				labelLocations[labelIndices[token.text]] = index;
				tokens.pop_front();
				continue;
			}

			TIS::Instruction instruction;

			// Handle instructions
			if (token.text == "MOV")
				instruction = parseMov(tokens);
			else if (token.text == "SWP")
				instruction = parseSwp(tokens);
			else if (token.text == "SAV")
				instruction = parseSav(tokens);
			else if (token.text == "ADD")
				instruction = parseAdd(tokens);
			else if (token.text == "SUB")
				instruction = parseSub(tokens);
			else if (token.text == "NOP")
				instruction = parseNop(tokens);
			else if (token.text == "NEG")
				instruction = parseNeg(tokens);
			else if (token.text == "JRO")
				instruction = parseJro(tokens);
			else if (token.text == "JMP")
				instruction = parseJmp(Opcode::JMP, tokens, labelIndices, index);
			else if (token.text == "JEZ")
				instruction = parseJmp(Opcode::JEZ, tokens, labelIndices, index);
			else if (token.text == "JNZ")
				instruction = parseJmp(Opcode::JNZ, tokens, labelIndices, index);
			else if (token.text == "JGZ")
				instruction = parseJmp(Opcode::JGZ, tokens, labelIndices, index);
			else if (token.text == "JLZ")
				instruction = parseJmp(Opcode::JLZ, tokens, labelIndices, index);
			else
				throw std::logic_error("Unhandled token! Text: " + token.text);

			instructions.push_back(instruction);
			++index;
		}

		auto opcodesBegin = LabelOpcodes.begin();
		auto opcodesEnd = LabelOpcodes.end();

		index = 0;
		// Final pass to remap label indices to label locations
		for (auto& instruction : instructions)
		{
			if (std::find(opcodesBegin, opcodesEnd, instruction.opcode) != opcodesEnd)
			{
				instruction.arguments[0].offset = 
					labelLocations[instruction.arguments[0].offset] - index;
			}
			++index;
		}

		return instructions;
	}

} }
