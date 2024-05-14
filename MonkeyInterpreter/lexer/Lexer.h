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

		void readChar()
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
		Token::Token NextToken()
		{
			Token::Token token;

			skipWhitespace();

			switch (ch) {
			case '=':
				if (isCompareOp(ch))
				{
					token.Literal = "==";
					token.Type = Token::EQUAL;
				}
				else
				{
					token = Token::Token(Token::ASSIGN, std::string(1, ch));
				}
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
			case '-':
				token = Token::Token(Token::MINUS, std::string(1, ch));
				break;
			case '!':
				if (isCompareOp(ch))
				{
					token.Literal = "!=";
					token.Type = Token::UNEQUAL;
				}
				else
				{
					token = Token::Token(Token::BANG, std::string(1, ch));
				}
				break;
			case '*':
				token = Token::Token(Token::ASTERISK, std::string(1, ch));
				break;
			case '/':
				token = Token::Token(Token::SLASH, std::string(1, ch));
				break;
			case '<':
				token = Token::Token(Token::LT, std::string(1, ch));
				break;
			case '>':
				token = Token::Token(Token::GT, std::string(1, ch));
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

		void TestNextToken()
		{
			std::string input =
				"let five = 5;				"
				"let ten = 10;				"
				"let add = fn(x, y) {		"
				"	x + y;					"
				"};							"
				"let result = add(five, ten); "
				"!-/*5;"
				"5 < 10 > 5;"
				"if( 5 < 10) {"
				"	return true;"
				"} else {"
				"	return false;"
				"}"
				" 10 == 10;"
				" 10 != 9;";

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


					{Token::BANG, "!"},
					{Token::MINUS, "-"},
					{Token::SLASH, "/"},
					{Token::ASTERISK, "*"},
					{Token::INT, "5"},
					{Token::SEMICOLON, ";"},

					{Token::INT, "5"},
					{Token::LT, "<"},
					{Token::INT, "10"},
					{Token::GT, ">"},
					{Token::INT, "5"},
					{Token::SEMICOLON, ";"},

					{Token::IF, "if"},
					{Token::LPAREN, "("},
					{Token::INT, "5"},
					{Token::LT, "<"},
					{Token::INT, "10"},
					{Token::RPAREN, ")"},
					{Token::LBRACE, "{"},
					{Token::RETURN, "return"},
					{Token::TRUE, "true"},
					{Token::SEMICOLON, ";"},
					{Token::RBRACE, "}"},
					{Token::ELSE, "else"},
					{Token::LBRACE, "{"},
					{Token::RETURN, "return"},
					{Token::FALSE, "false"},
					{Token::SEMICOLON, ";"},
					{Token::RBRACE, "}"},

					{Token::INT, "10"},
					{Token::EQUAL, "=="},
					{Token::INT, "10"},
					{Token::SEMICOLON, ";"},
					{Token::INT, "10"},
					{Token::UNEQUAL, "!="},
					{Token::INT, "9"},
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


		void skipWhitespace()
		{
			while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
				readChar();
			}
		}

		bool isLetter(char c)
		{
			return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
		}

		bool isDigit(char c)
		{
			return c >= '0' && c <= '9';
		}

		bool isCompareOp(char c)
		{
			if (peekChar() == '=')
			{
				readChar();
				return true;
			}
			return false;
		}

		char peekChar()
		{
			if (readPosition >= Input.size())
			{
				return 0;
			}
			return Input[readPosition];
		}

		std::string readNumber()
		{
			int number = ch - 48;
			while (isDigit(peekChar()))
			{
				number *= 10;
				readChar();
				number += ch - 48;
			}
			return std::to_string(number);
		}

		std::string readIdentifier()
		{
			std::string identifier = std::string(1, ch);
			while (isLetter(peekChar()))
			{
				readChar();
				identifier += std::string(1, ch);
			}
			return identifier;
		}

		std::string Input;
		int currentPosition;
		int readPosition;
		char ch;
	};

}
