#pragma once

#include "ast/ast.h"
#include "lexer/Lexer.h"

#include <map>
#include <functional>

namespace parser
{
	enum OperatorPresedence
	{
		LOWEST = 0,
		EQUALS = 1,
		LESSGREATER = 2,
		SUM = 3,
		PRODUCT = 4,
		PREFIX = 5,
		CALL = 6
	};

	struct Parser
	{
		Parser(Lexer::Lexer l) : lexer(l)
		{
			nextToken();
			nextToken(); // so currToken and peekToken are set

			prefixParseFunctions[Token::IDENT] = [this]() { return this->parseIdentifier(); };
			prefixParseFunctions[Token::TRUE] = [this]() { return this->parseBoolean(); };
			prefixParseFunctions[Token::FALSE] = [this]() { return this->parseBoolean(); };
			prefixParseFunctions[Token::INT] = [this]() { return this->parseIntegerLiteral(); };
			prefixParseFunctions[Token::BANG] = [this]() { return this->parsePrefixExpression(); };
			prefixParseFunctions[Token::MINUS] = [this]() { return this->parsePrefixExpression(); };
			prefixParseFunctions[Token::LPAREN] = [this]() { return this->parseGroupedExpression(); };
			prefixParseFunctions[Token::IF] = [this]() { return this->parseIfExpression(); };

			infixParseFunctions[Token::PLUS] = [this](ast::Expression* left) { return this->parseInfixExpression(left); };
			infixParseFunctions[Token::MINUS] = [this](ast::Expression* left) { return this->parseInfixExpression(left); };
			infixParseFunctions[Token::SLASH] = [this](ast::Expression* left) { return this->parseInfixExpression(left); };
			infixParseFunctions[Token::ASTERISK] = [this](ast::Expression* left) { return this->parseInfixExpression(left); };
			infixParseFunctions[Token::EQUAL] = [this](ast::Expression* left) { return this->parseInfixExpression(left); };
			infixParseFunctions[Token::UNEQUAL] = [this](ast::Expression* left) { return this->parseInfixExpression(left); };
			infixParseFunctions[Token::LT] = [this](ast::Expression* left) { return this->parseInfixExpression(left); };
			infixParseFunctions[Token::GT] = [this](ast::Expression* left) { return this->parseInfixExpression(left); };

			presedences[Token::EQUAL] = OperatorPresedence::EQUALS;
			presedences[Token::UNEQUAL] = OperatorPresedence::EQUALS;
			presedences[Token::LT] = OperatorPresedence::LESSGREATER;
			presedences[Token::GT] = OperatorPresedence::LESSGREATER;
			presedences[Token::PLUS] = OperatorPresedence::SUM;
			presedences[Token::MINUS] = OperatorPresedence::SUM;
			presedences[Token::SLASH] = OperatorPresedence::PRODUCT;
			presedences[Token::ASTERISK] = OperatorPresedence::PRODUCT;
		}

		Lexer::Lexer lexer;
		Token::Token currToken;
		Token::Token peekToken;

		std::map<std::string, std::function<ast::Expression* ()>> prefixParseFunctions;
		std::map<std::string, std::function<ast::Expression* (ast::Expression*)>> infixParseFunctions;
		std::map<std::string, int> presedences;

		int currPrecedence() {
			return presedences[currToken.Type];
		}

		int peekPrecedence() {
			return presedences[peekToken.Type];
		}

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
			else 
			{
				return parseExpressionStatement();
			}
			throw std::exception("Unexpected Token in 'parseStatement()'");

			return nullptr;
		}

		ast::Statement* parseLetStatement()
		{
			if (!expectPeek(Token::IDENT)) {
				throw std::exception("Unexpected Token in 'parseLetStatement()'");
			}
			ast::LetStatement* ls = new ast::LetStatement();
			ls->Token = currToken;

			ls->Name = new ast::Identifier(currToken, currToken.Literal);

			if (!expectPeek(Token::ASSIGN)) { throw std::exception("You're gay"); }

			nextToken();

			ls->Value = parseExpression(LOWEST);

			if (!expectPeek(Token::SEMICOLON)) { throw std::exception("You're gay"); }

			return ls;
		}

		ast::Statement* parseReturnStatement()
		{
			ast::ReturnStatement* rs = new ast::ReturnStatement();
			rs->Token = currToken;

			nextToken();

			rs->Value = parseExpression(LOWEST);

			if (!expectPeek(Token::SEMICOLON)) { throw std::exception("You're gay"); }


			return rs;
		}

		ast::BlockStatement* parseBlockStatement()
		{
			ast::BlockStatement* bs = new ast::BlockStatement();
			bs->Token = currToken;
			nextToken();
			while (currToken.Type != Token::RBRACE && currToken.Type != Token::_EOF)
			{
				auto* s = parseStatement();
				if(s) bs->statements.push_back(s);
				nextToken();
			}
			return bs;
		}

		ast::Statement* parseExpressionStatement()
		{
			ast::ExpressionStatement* rs = new ast::ExpressionStatement();

			rs->Value = parseExpression(LOWEST);

			expectPeek(Token::SEMICOLON);

			return rs;
		}

		ast::Expression* parseExpression(int operatorPrecedence)
		{
			ast::Expression* left = prefixParseFunctions[currToken.Type]();

			while (peekToken.Type != Token::SEMICOLON && operatorPrecedence < peekPrecedence())
			{
				nextToken();
				left = infixParseFunctions[currToken.Type](left);
			}

			return left;
		}

		ast::Expression* parseIdentifier()
		{
			return new ast::Identifier(currToken, currToken.Literal);
		}

		ast::Expression* parseBoolean()
		{
			return new ast::Boolean(currToken);
		}

		ast::Expression* parseIntegerLiteral()
		{
			return new ast::IntegerLiteral(currToken, currToken.Literal);
		}

		ast::Expression* parsePrefixExpression()
		{
			ast::PrefixExpression* expression = new ast::PrefixExpression(currToken, currToken.Literal);
			
			nextToken();
			expression->Right = parseExpression(PREFIX);
			return expression;
		}

		ast::Expression* parseGroupedExpression()
		{
			ast::Expression* e = parseExpression(LOWEST);
			if (!expectPeek(Token::RPAREN))
			{
				return nullptr;
			}
			return e;
		}

		ast::Expression* parseIfExpression()
		{
			ast::IfExpression* e = new ast::IfExpression(currToken);
			if (!expectPeek(Token::LPAREN)) return nullptr;
			nextToken();

			e->Condition = parseExpression(LOWEST);
			if (!expectPeek(Token::RPAREN)) return nullptr;
			if (!expectPeek(Token::LBRACE)) return nullptr;

			e->Consequence = parseBlockStatement();
		
			if (peekToken.Type == Token::ELSE)
			{
				nextToken();
				if (!expectPeek(Token::LBRACE)) return nullptr;
				e->Alternative = parseBlockStatement();
			}

			return e;
		}


		ast::Expression* parseInfixExpression(ast::Expression* left)
		{
			ast::InfixExpression* expression = new ast::InfixExpression(currToken, left);

			int presedence = currPrecedence();
			nextToken();
			expression->Right = parseExpression(presedence);
			return expression;
		}
	};
}
