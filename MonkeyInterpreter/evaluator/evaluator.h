#pragma once

#include "object/object.h"
#include "ast/ast.h"

namespace eval
{

	obj::Boolean* TRUE = new obj::Boolean(true);
	obj::Boolean* FALSE = new obj::Boolean(false);

	obj::Null* null = new obj::Null();



	obj::Object* Evaluate(ast::Node* node);


	obj::Object* evalStatements(std::vector<ast::Statement*> statements)
	{
		obj::Object* ret = nullptr;
		for (auto* s : statements)
		{
			ret = Evaluate(s);
		}
		return ret;
	}

	obj::Object* evalBangOperator(obj::Object* obj)
	{
		if (obj->Type() == obj::INTEGER_OBJ)
		{
			return (obj->Inspect() == "0" ? TRUE : FALSE);
		}
		if (obj->Type() == obj::BOOLEAN_OBJ)
		{
			return (obj->Inspect() == "false" ? TRUE : FALSE);
		}
		if (obj->Type() == obj::NULL_OBJ)
		{
			return TRUE;
		}
		return FALSE;
	}

	obj::Object* evalMinusPrefixOperator(obj::Object* obj)
	{
		if (obj->Type() != obj::INTEGER_OBJ)
		{
			return null;
		}
		
		
		if (auto* integer = dynamic_cast<obj::Integer*>(obj))
		{
			integer->Value *= -1;
			return integer;
		}
		return null;
	}

	obj::Object* evaluatePrefixExpr(char oper, obj::Object* obj)
	{
		switch (oper)
		{
			case '!':
			{
				return evalBangOperator(obj);
			}
			case '-':
			{
				return evalMinusPrefixOperator(obj);
			}
			default:
				return null;
		}
	}

	obj::Object* evalNumericalInfixOperator(char oper, obj::Object* left, obj::Object* right)
	{
		auto* leftInt = dynamic_cast<obj::Integer*>(left);
		auto* rightInt = dynamic_cast<obj::Integer*>(right);
		if (!leftInt || !rightInt) return null;

		switch (oper)
		{
		case '+':
		{
			return new obj::Integer(leftInt->Value + rightInt->Value);
		}
		case '-':
		{
			return new obj::Integer(leftInt->Value - rightInt->Value);
		}
		case '*':
		{
			return new obj::Integer(leftInt->Value * rightInt->Value);

		}
		case '/':
		{
			if (rightInt->Value == 0) return null;
			return new obj::Integer(leftInt->Value / rightInt->Value);
		}
		default:
			return null;
		}
	}

	obj::Object* evalLogicalInfixOperator(std::string oper, obj::Object* left, obj::Object* right)
	{

		if (left->Type() == obj::INTEGER_OBJ && right->Type() == obj::INTEGER_OBJ)
		{
			auto* leftInt = dynamic_cast<obj::Integer*>(left);
			auto* rightInt = dynamic_cast<obj::Integer*>(right);
			if (!leftInt || !rightInt) return null;

			if (oper == "<") return new obj::Boolean(leftInt->Value < rightInt->Value);
			if (oper == ">") return new obj::Boolean(leftInt->Value > rightInt->Value);
			if (oper == "==") return new obj::Boolean(leftInt->Value == rightInt->Value);
			if (oper == "!=") return new obj::Boolean(leftInt->Value != rightInt->Value);

		} 
		else if (left->Type() == obj::BOOLEAN_OBJ && right->Type() == obj::BOOLEAN_OBJ)
		{
			auto* leftBool = dynamic_cast<obj::Boolean*>(left);
			auto* rightBool = dynamic_cast<obj::Boolean*>(right);
			if (!leftBool || !rightBool) return null;

			if (oper == "<") return null;
			if (oper == ">") return null;
			if (oper == "==") return new obj::Boolean(leftBool->Value == rightBool->Value);
			if (oper == "!=") return new obj::Boolean(leftBool->Value != rightBool->Value);
		}

		return null;
	}

	obj::Object* evaluateInfixExpr(std::string oper, obj::Object* left, obj::Object* right)
	{
		if (oper == "+" || oper == "-" || oper == "*" || oper == "/")
		{
			return evalNumericalInfixOperator(oper[0], left, right);

		}
		if (oper == ">" || oper == "<" || oper == "==" || oper == "!=")
		{
			return evalLogicalInfixOperator(oper, left, right);
		}
		return null;
	}

	obj::Object* Evaluate(ast::Node* node)
	{

		if (auto* program = dynamic_cast<ast::Program*>(node))
		{
			return evalStatements(program->Statements);
		}
		else if (auto* exprStmt = dynamic_cast<ast::ExpressionStatement*>(node))
		{
			return Evaluate(exprStmt->Value);
		} 
		else if (auto* intPtr = dynamic_cast<ast::IntegerLiteral*>(node))
		{
			return new obj::Integer(intPtr->Value);
		}
		else if (auto* boolPtr = dynamic_cast<ast::Boolean*>(node))
		{
			return boolPtr->Value ? TRUE : FALSE;
		}
		else if (auto* prefixExpr = dynamic_cast<ast::PrefixExpression*>(node))
		{
			auto* right = Evaluate(prefixExpr->Right);
			char OPER = prefixExpr->Operator[0];
			return evaluatePrefixExpr(OPER, right);
		}
		else if (auto* infixExpr = dynamic_cast<ast::InfixExpression*>(node))
		{
			auto* right = Evaluate(infixExpr->Right);
			auto* left = Evaluate(infixExpr->Left);
			return evaluateInfixExpr(infixExpr->Operator, left, right);
		}
		return nullptr;
	}



}