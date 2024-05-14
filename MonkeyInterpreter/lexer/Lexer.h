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

		void readChar();
		Token::Token NextToken();

		std::string Input;
		int currentPosition;
		int readPosition;
		char ch;
	};

	void TestNextToken();
}