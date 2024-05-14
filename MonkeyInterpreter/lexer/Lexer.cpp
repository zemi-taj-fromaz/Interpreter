#pragma once

#include "lexer/Lexer.h"

namespace Lexer
{
	void Lexer::TestNextToken()
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
				{Token::LET, "let"},
				{Token::IDENT, "five"},
				{Token::ASSIGN, "="},
				{Token::INT, "5"},
				{Token::SEMICOLON, ";"},
				{Token::LET, "let"},
				{Token::IDENT, "ten"},
				{Token::ASSIGN, "="},
				{Token::INT, "10"},
				{Token::SEMICOLON, ";"},
				{Token::LET, "let"},
				{Token::IDENT, "add"},
				{Token::ASSIGN, "="},
				{Token::FUNCTION, "fn"},
				{Token::LPAREN, "("},
				{Token::IDENT, "x"},
				{Token::COMMA, ","},
				{Token::IDENT, "y"},
				{Token::RPAREN, ")"},
				{Token::LBRACE, "{"},
				{Token::IDENT, "x"},
				{Token::PLUS, "+"},
				{Token::IDENT, "y"},
				{Token::SEMICOLON, ";"},
				{Token::RBRACE, "}"},
				{Token::SEMICOLON, ";"},
				{Token::LET, "let"},
				{Token::IDENT, "result"},
				{Token::ASSIGN, "="},
				{Token::IDENT, "add"},
				{Token::LPAREN, "("},
				{Token::IDENT, "five"},
				{Token::COMMA, ","},
				{Token::IDENT, "ten"},
				{Token::RPAREN, ")"},
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

	void Lexer::skipWhitespace()
	{
		while(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
			readChar();
		}
	}

	bool Lexer::isLetter(char c)
	{
		return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
	}

	bool Lexer::isDigit(char c)
	{
		return c>= '0' && c <= '9';
	}

	std::string Lexer::readNumber()
	{
		int number = ch - 48;
		while (isDigit(Input[readPosition]))
		{
			number *= 10;
			readChar();
			number += ch - 48;
		}
		return std::to_string(number);
	}

	std::string Lexer::readIdentifier()
	{
		std::string identifier = std::string(1,ch);
		while (isLetter(Input[readPosition]))
		{
			readChar();
			identifier += std::string(1, ch);
		}
		return identifier;
	}

	Token::Token Lexer::NextToken()
	{
		Token::Token token;

		skipWhitespace();

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
		{
			token.Literal = "";
			token.Type = Token::_EOF;
			break;
		}
		default:
		{
			if (isLetter(ch))
			{
				token.Literal = this->readIdentifier();
				token.Type = token.LookupIdent(token.Literal);
			}
			else if (isDigit(ch))
			{
				token.Type = Token::INT;
				token.Literal = this->readNumber();
			}
			else
			{
				token = Token::Token(Token::ILLEGAL, std::string(1, ch));
			}
			break;
		}
		}

		this->readChar();

		return token;
	}


}