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
		virtual std::string String() = 0;
	};

	struct Expression
	{

		virtual std::string String() = 0;
		virtual void expressionNode() = 0;
	};

	struct IntegerLiteral : public Expression
	{
		IntegerLiteral(Token::Token token, std::string literal) : Token(token), Value(std::stoi(literal)) {}
		Token::Token Token;
		int Value;

		std::string String() {
			return Token.Literal;
		}

		virtual void expressionNode() override {}
	};

	struct Boolean : public Expression
	{
		Boolean(Token::Token token) : Token(token) {
			if (token.Literal == "true") Value = true;
			else if (token.Literal == "false") Value = false;
			else throw std::exception("Bad Init of Boolean Expression + you're gay");
		}
		Token::Token Token;
		bool Value;

		std::string String() {
			return Token.Literal;
		}

		virtual void expressionNode() override {}
	};

	struct Identifier : public Expression
	{
		Identifier(Token::Token token, std::string literal) : Token(token), Value(literal) {}
		Token::Token Token;
		std::string Value;

		std::string String() { return Value; }

		virtual void expressionNode() override {}
	};

	struct PrefixExpression : public Expression
	{
		PrefixExpression(Token::Token token, std::string literal) : Token(token), Value(literal), Operator(token.Literal) {}
		Token::Token Token;
		std::string Value;
		std::string Operator;
		Expression* Right;

		std::string String() {
			return "(" + Operator + Right->String() + ")";
		}

		virtual void expressionNode() override {}
	};

	struct InfixExpression : public Expression
	{
		InfixExpression(Token::Token token) : Token(token), Operator(Token.Literal) {}
		InfixExpression(Token::Token token, Expression* left) : Token(token), Operator(Token.Literal), Left(left) {}
		Token::Token Token;
		std::string Operator;
		Expression* Left;
		Expression* Right;

		std::string String() {
			return "(" + Left->String() + " " + Operator +  " " + Right->String() + ")";
		}

		virtual void expressionNode() override {}
	};



	struct LetStatement : public Statement
	{
		Token::Token Token;
		Identifier* Name;
		Expression* Value;

		std::string String()
		{
			return Name->Value + " = " + (Value != nullptr ? Value->String() : "") + ";";
		}

		virtual void statementNode() override {};
	};

	struct ReturnStatement : public Statement
	{
		Token::Token Token;
		Expression* Value;

		std::string String()
		{
			return Token.Literal + " " + (Value != nullptr ? Value->String() : " ") + ";";
		}

		virtual void statementNode() override {};
	};

	struct ExpressionStatement : public Statement
	{
		Token::Token Token;
		Expression* Value;

		std::string String()
		{
			return Value != nullptr ? Value->String() : " ";
		}

		virtual void statementNode() override {};
	};

	struct BlockStatement : public Statement
	{
		BlockStatement(){}
		Token::Token Token;
		std::vector<Statement*> statements;

		std::string String() {
			std::string ret = "";
			ret += Token.Literal;
			for(auto* s : statements)
			{
				ret += s->String();
				ret += "\n";
			}
			return ret;
		}

		virtual void statementNode() override {};

	};

	struct IfExpression : public Expression
	{
		IfExpression(Token::Token token) : Token(token) {}
		IfExpression(Token::Token token, Expression* left) : Token(token), Condition(left) {}
		Token::Token Token;
		Expression* Condition;
		BlockStatement* Consequence;
		BlockStatement* Alternative{ nullptr };

		std::string String() {
			return "( if (" + Condition->String() + ") " + Consequence->String() + "} " + (Alternative != nullptr ? ("else" + Alternative->String()) : " ") + ")";
			//return "( )";
		}

		virtual void expressionNode() override {}
	};


	struct FunctionLiteral : public Expression
	{
		FunctionLiteral(Token::Token token) : Token(token) {}
		Token::Token Token; //fn
		std::vector<Identifier*> Parameters;
		BlockStatement* Body;

		std::string String() {
			std::string ret = "";
			ret += Token.Literal + "(";
			for (auto* p : Parameters)
			{
				ret += p->String() + ",";
			}
			ret += ") {";
			ret += Body->String();
			ret += "};";
			return ret;
		}

		virtual void expressionNode() override {}
	};


	struct Program
	{
		std::vector<Statement*> Statements;
	};
}