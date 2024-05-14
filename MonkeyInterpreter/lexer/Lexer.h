#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "token/Token.h"

namespace Lexer
{
	struct Lexer
	{
		Lexer(std::string input) : Input(input), readPosition(0), currentPosition(0)
		{
			readChar();
		}

		static void TestNextToken();

		void readChar();
		Token::Token NextToken();
		void skipWhitespace();

		bool isLetter(char c);
		bool isDigit(char c);

		bool isCompareOp(char c);

		char peekChar();

		std::string readNumber();
		std::string readIdentifier();

		std::string Input;
		int currentPosition;
		int readPosition;
		char ch;
	};

}
