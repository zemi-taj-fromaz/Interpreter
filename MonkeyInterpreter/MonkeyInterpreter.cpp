#pragma once

#include <iostream>

#include "lexer/Lexer.h"
#include "parser/parser_test.h"

int main()
{
    std::string input;
	parser::TestReturnStatements();
 //   while (std::getline(std::cin, input)) 
	//{
	//	Lexer::Lexer l = Lexer::Lexer(input);

	//	for (Token::Token token = l.NextToken(); token.Type != Token::_EOF; token = l.NextToken())
	//	{
	//		std::cout << token.Type << " " << token.Literal << std::endl;
	//	}
 //   }
	//try {
	//	Lexer::Lexer::TestNextToken();
	//}
	//catch(std::exception& e)
	//{
	//	std::cout << e.what() << std::endl;
	//}
}

