#pragma once

#include <string>

namespace Token
{
	struct Token
	{
		Token(){}
		Token(std::string type, std::string Literal) : Type(type), Literal(Literal) {}
		std::string Type;		//which class of token does it belong to
		std::string Literal;	// actual characters representing the token
	};

	const std::string ILLEGAL = "ILLEGAL";
	const std::string _EOF = "EOF";

	const std::string IDENT = "IDENT";
	const std::string INT = "INT";

	const std::string ASSIGN = "ASSIGN";
	const std::string PLUS = "PLUS";

	const std::string COMMA = "COMMA";
	const std::string SEMICOLON = "SEMICOLON";

	const std::string LPAREN = "LPAREN";
	const std::string RPAREN = "RPAREN";
	const std::string LBRACE = "LBRACE";
	const std::string RBRACE = "RBRACE";

	const std::string FUNCTION = "FUNCTION";
	const std::string LET = "LET";
}






