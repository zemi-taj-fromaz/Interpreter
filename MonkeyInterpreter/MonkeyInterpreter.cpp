#pragma once

#include <iostream>

#include "lexer/Lexer.h"
#include "parser/parser_test.h"

int main()
{
    std::string input;
	//parser::TestParsingInfix();
    while (std::getline(std::cin, input)) 
	{
		Lexer::Lexer l = Lexer::Lexer(input);
		parser::Parser p = parser::Parser(l);

		ast::Program program = p.ParseProgram();
		
		std::cout << program.String() << std::endl;
	
    }
	
}

