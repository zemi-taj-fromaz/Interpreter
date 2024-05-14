#pragma once

#include <string>

#include "token/Token.h"
#include <vector>

namespace ast
{
	struct Node
	{
		virtual std::string TokenLiteral() = 0; // for debugging and testing
	};

	struct Statement
	{
		virtual void statementNode() = 0;
	};

	struct Expression
	{
		virtual void expressionNode() = 0;
	};

	struct Identifier : public Expression
	{
		Identifier(Token::Token token, std::string literal) : Token(token), Value(literal) {}
		Token::Token Token;
		std::string Value;
		virtual void expressionNode() override {}
	};

	struct LetStatement : public Statement
	{
		Token::Token Token;
		Identifier* Name;
		Expression* Value;

		virtual void statementNode() override {};
	};

	struct ReturnStatement : public Statement
	{
		Token::Token Token;
		Expression* Value;

		virtual void statementNode() override {};
	};

	struct Program
	{
		std::vector<Statement*> Statements;
	};
}