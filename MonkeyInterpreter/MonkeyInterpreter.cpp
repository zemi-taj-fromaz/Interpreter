#pragma once

#include <iostream>

#include "lexer/Lexer.h"
#include "parser/parser_test.h"
#include "evaluator/evaluator.h"

int main()
{
    std::string input;
	//parser::TestParsingInfix();
    while (std::getline(std::cin, input)) 
	{
		Lexer::Lexer l = Lexer::Lexer(input);
		parser::Parser p = parser::Parser(l);
		env::Environment e = env::Environment();

		ast::Program program = p.ParseProgram();
		
		auto* eval = eval::Evaluate(&program, &e);
		if (eval) {
			std::cout << eval->Inspect() << std::endl;
			std::cout << std::endl;
		}
    }
	
}

