#pragma once

#include "lexer/Lexer.h"

namespace Lexer
{

	void Lexer::readChar()
	{
		if (this->readPosition >= this->Input.size())
		{
			this->ch = 0;
		}
		else
		{
			this->ch = this->Input[this->readPosition];
		}
		this->currentPosition = readPosition;
		this->readPosition++;
	}

	Token::Token Lexer::NextToken()
	{
		Token::Token token;

		switch (ch) {
		case '=':
			token = Token::Token(Token::ASSIGN, std::string(1, ch));
			break;
		case ';':
			token = Token::Token(Token::SEMICOLON, std::string(1, ch));
			break;
		case '(':
			token = Token::Token(Token::LPAREN, std::string(1, ch));
			break;
		case ')':
			token = Token::Token(Token::RPAREN, std::string(1, ch));
			break;
		case ',':
			token = Token::Token(Token::COMMA, std::string(1, ch));
			break;
		case '+':
			token = Token::Token(Token::PLUS, std::string(1, ch));
			break;
		case '{':
			token = Token::Token(Token::LBRACE, std::string(1, ch));
			break;
		case '}':
			token = Token::Token(Token::RBRACE, std::string(1, ch));
			break;
		case 0:
			token.Literal = "";
			token.Type = Token::_EOF;
			break;
		default:
			token.Literal = "";
			token.Type = Token::ILLEGAL;
			break;
		}

		this->readChar();

		return token;
	}


	void TestNextToken()
	{
		std::string input = 
			"let five = 5;				"	 
			"let ten = 10;				"
			"let add = fn(x, y) {		"
			"	x + y;					"
			"};							"
			"let result = add(five, ten); ";

		std::vector<Token::Token> testResult(
			{
				{Token::ASSIGN, "=" },
				{Token::PLUS, "+" },
				{Token::LPAREN, "(" },
				{Token::RPAREN , ")"},
				{Token::LBRACE, "{"},
				{Token::RBRACE, "}"},
				{Token::COMMA, ","},
				{Token::SEMICOLON, ";"},
				{Token::_EOF, ""},
			}
		);

		Lexer l = Lexer(input);

		for (Token::Token& token : testResult)
		{
			Token::Token nextToken = l.NextToken();

			if (token.Type != nextToken.Type || token.Literal != nextToken.Literal)
			{
				throw new std::invalid_argument("Token " + nextToken.Literal + " not parsed correctly");
			}
		}

		std::cout << "TOKENIZED CORRECTLY" << std::endl;
	}
}