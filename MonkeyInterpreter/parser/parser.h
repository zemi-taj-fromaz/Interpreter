#pragma once

#include "ast/ast.h"
#include "lexer/Lexer.h"

namespace parser
{
	struct Parser
	{
		Parser(Lexer::Lexer l) : lexer(l)
		{
			nextToken();
			nextToken(); // so currToken and peekToken are set
		}

		Lexer::Lexer lexer;
		Token::Token currToken;
		Token::Token peekToken;

		void nextToken() {
			currToken = peekToken;
			peekToken = lexer.NextToken();
		}

		bool expectPeek(std::string token)
		{
			if (peekToken.Type == token)
			{
				nextToken();
				return true;
			}
			return false;
		}

		ast::Program ParseProgram() { 
			ast::Program program;
			while (currToken.Type != Token::_EOF)
			{
				ast::Statement* statement = parseStatement();
				if (statement != nullptr)
				{
					program.Statements.push_back(statement);
				}
				nextToken();
			}
			return program;
		}

		ast::Statement* parseStatement()
		{
			if (currToken.Type == Token::LET)
			{
				return parseLetStatement();
			}
			else if (currToken.Type == Token::RETURN)
			{
				return parseReturnStatement();
			}
			throw std::exception("Unexpected Token in 'parseStatement()'");
		}

		ast::Statement* parseLetStatement()
		{
			if (!expectPeek(Token::IDENT)) {
				throw std::exception("Unexpected Token in 'parseLetStatement()'");
			}
			ast::LetStatement* ls = new ast::LetStatement();
			ls->Token = currToken;

			ls->Name = new ast::Identifier(currToken, currToken.Literal);

			//SKIP EXPRESSION UNTIL SEMICOLON (TODO)
			while (currToken.Type != Token::SEMICOLON) nextToken();
			
			return ls;
		}

		ast::Statement* parseReturnStatement()
		{
			ast::ReturnStatement* rs = new ast::ReturnStatement();
			rs->Token = currToken;

			//SKIP EXPRESSION UNTIL SEMICOLON (TODO)
			while (currToken.Type != Token::SEMICOLON) nextToken();

			return rs;
		}
	};
}
