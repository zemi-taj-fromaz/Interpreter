#pragma once

#include <string>
#include <map>


namespace Token
{
	static  std::string ILLEGAL = "ILLEGAL";
	static  std::string _EOF = "EOF";
	
	static  std::string IDENT = "IDENT";
	static  std::string INT = "INT";
	 	 
	static  std::string ASSIGN = "ASSIGN";
	static  std::string PLUS = "PLUS";
	 	 
	static  std::string COMMA = "COMMA";
	static  std::string SEMICOLON = "SEMICOLON";
	 	 
	static  std::string LPAREN = "LPAREN";
	static  std::string RPAREN = "RPAREN";
	static  std::string LBRACE = "LBRACE";
	static  std::string RBRACE = "RBRACE";
	 	 
	static  std::string FUNCTION = "FUNCTION";
	static  std::string LET = "LET";

	const std::map<std::string, std::string> KEYWORDS{
		{ "fn" , FUNCTION },
		{ "let", LET }
	};

	struct Token
	{
		Token(){}
		Token(std::string type, std::string Literal) : Type(type), Literal(Literal) {}


		std::string LookupIdent(std::string ident)
		{
			auto it = KEYWORDS.find(ident);
			if (it != KEYWORDS.end())
			{
				return it->second;
			}

			return IDENT;
		}

		std::string Type;		//which class of token does it belong to
		std::string Literal;	// actual characters representing the token
	};




}






