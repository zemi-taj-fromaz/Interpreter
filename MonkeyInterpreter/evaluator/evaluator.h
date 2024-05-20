#pragma once

#include "object/object.h"
#include "ast/ast.h"
#include "environment/environment.h"

namespace eval
{
	obj::Object* Evaluate(ast::Node* node, env::Environment& e);

	obj::Boolean* TRUE = new obj::Boolean(true);
	obj::Boolean* FALSE = new obj::Boolean(false);

	obj::Null* null = new obj::Null();


	obj::Object* evalStatements(std::vector<ast::Statement*> statements, env::Environment& e)
	{
		obj::Object* ret = nullptr;
		for (auto* s : statements)
		{
			ret = Evaluate(s, e);
			if (ret->Type() == obj::RETURN_OBJ) {
				auto* retObj = dynamic_cast<obj::ReturnValue*>(ret);
				return retObj->obj;
			}
			if (ret->Type() == obj::ERROR_OBJ) {
				//auto* retObj = dynamic_cast<obj::ErrorObject*>(ret);
				return ret;
			}
		}
		return ret;
	}

	obj::Object* evalBlockStatements(std::vector<ast::Statement*> statements, env::Environment& e)
	{
		obj::Object* ret = nullptr;
		for (auto* s : statements)
		{
			ret = Evaluate(s, e);
			if (ret->Type() == obj::RETURN_OBJ) {
				return ret;
			}
			if (ret->Type() == obj::ERROR_OBJ) {
				return ret;
			}
		}
		return ret;
	}

	obj::Object* evalBangOperator(obj::Object* obj, env::Environment& e)
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
		return new obj::ErrorObject("Cannot call ! operator on that type of obj");
	}

	obj::Object* evalMinusPrefixOperator(obj::Object* obj, env::Environment& e)
	{
		if (obj->Type() != obj::INTEGER_OBJ)
		{
			return new obj::ErrorObject("Cannot call - prefix operator on object that is not INT");
		}
		
		
		if (auto* integer = dynamic_cast<obj::Integer*>(obj))
		{
			integer->Value *= -1;
			return integer;
		}
		return new obj::ErrorObject("Cannot call - prefix operator on object that is not INT");
	}

	obj::Object* evaluatePrefixExpr(char oper, obj::Object* obj,env::Environment& e)
	{
		switch (oper)
		{
			case '!':
			{
				return evalBangOperator(obj, e);
			}
			case '-':
			{
				return evalMinusPrefixOperator(obj, e);
			}
			default:
				return new obj::ErrorObject("Invalid prefix operator");
		}
	}

	obj::Object* evalNumericalInfixOperator(char oper, obj::Object* left, obj::Object* right, env::Environment& e)
	{
		auto* leftInt = dynamic_cast<obj::Integer*>(left);
		auto* rightInt = dynamic_cast<obj::Integer*>(right);
		if (!leftInt || !rightInt) return new obj::ErrorObject("Cannot call infix numerical operator(*,+,-,/) on objects that are not INT");

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
			return new obj::ErrorObject("Invalid Infix operator");
		}
	}

	obj::Object* evalLogicalInfixOperator(std::string oper, obj::Object* left, obj::Object* right, env::Environment& e)
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

		return new obj::ErrorObject("Illegal argument types for infix operator");

	}

	obj::Object* evaluateInfixExpr(std::string oper, obj::Object* left, obj::Object* right, env::Environment& e)
	{
		if (oper == "+" || oper == "-" || oper == "*" || oper == "/")
		{
			return evalNumericalInfixOperator(oper[0], left, right, e);

		}
		if (oper == ">" || oper == "<" || oper == "==" || oper == "!=")
		{
			return evalLogicalInfixOperator(oper, left, right, e);
		}
		return new obj::ErrorObject("Unsupported infix operator");

	}

	obj::Object* evaluateIfExpr(ast::IfExpression* ifExpr, env::Environment& e)
	{
		auto* condition = Evaluate(ifExpr->Condition, e);

		if (condition->Type() == obj::INTEGER_OBJ)
		{
			if (auto* intCondition = dynamic_cast<obj::Integer*>(condition))
			{
				if (intCondition->Value == 0)
				{
					return (ifExpr->Alternative != nullptr ? Evaluate(ifExpr->Alternative, e) : null);
				}
				return Evaluate(ifExpr->Consequence, e);
			}
			return null;
		}

		if (condition->Type() == obj::BOOLEAN_OBJ)
		{
			if (auto* boolCondition = dynamic_cast<obj::Boolean*>(condition))
			{
				if (boolCondition->Value == false)
				{
					return (ifExpr->Alternative != nullptr ? Evaluate(ifExpr->Alternative, e) : null);
				}
				return Evaluate(ifExpr->Consequence, e);
			}
			return null;
		}
		return new obj::ErrorObject("Condition in if expression evaluates to null");

	}

	std::vector<obj::Object*> evaluateExpressions(std::vector<ast::Expression*> expressions, env::Environment& e)
	{
		std::vector<obj::Object*> ret;
		for (auto* expr : expressions)
		{
			auto* evaluatedExpr = Evaluate(expr, e);
			if (evaluatedExpr->Type() == obj::ERROR_OBJ) return { evaluatedExpr };
			ret.push_back(evaluatedExpr);
		}

		return ret;
	}

	env::Environment* extendFunctionEnv(obj::Function* function, std::vector<obj::Object*> args)
	{
		auto* newEnv = env::createEnclosedEnvironment(function->env);

		for (int i = 0; i < function->Parameters.size(); i++)
		{
			auto* p = function->Parameters[i];
			newEnv->set(p->Value, args[i]);
		}

		return newEnv;
	}

	obj::Object* unwrapReturnValue(obj::Object* wrapped)
	{
		if (auto* returnValue = dynamic_cast<obj::ReturnValue*>(wrapped))
		{
			return returnValue->obj;
		}
		return nullptr;
	}

	obj::Object* applyFunction(obj::Object* function, std::vector<obj::Object*> args, env::Environment& e)
	{
		auto* fn = dynamic_cast<obj::Function*>(function);
		if (!fn) return nullptr;
		fn->env = &e;
		auto* extendedFunctionEnv = extendFunctionEnv(fn, args);

		obj::Object* evaluated = Evaluate(fn->body, *extendedFunctionEnv);
		return unwrapReturnValue(evaluated);
	}

	obj::Object* Evaluate(ast::Node* node, env::Environment& e)
	{

		if (auto* program = dynamic_cast<ast::Program*>(node))
		{
			return evalStatements(program->Statements, e);
		}
		else if (auto* exprStmt = dynamic_cast<ast::ExpressionStatement*>(node))
		{
			return Evaluate(exprStmt->Value, e);
		} 
		else if (auto* blkStmt = dynamic_cast<ast::BlockStatement*>(node))
		{
			return evalBlockStatements(blkStmt->statements, e);
		}
		else if (auto* retStmt = dynamic_cast<ast::ReturnStatement*>(node))
		{
			obj::Object* o = Evaluate(retStmt->Value, e);
			if (o->Type() == obj::ERROR_OBJ) return new obj::ErrorObject("errror in return statement");
			//TODO what should this return?
			return new obj::ReturnValue(o);
		}
		else if (auto* function = dynamic_cast<ast::FunctionLiteral*>(node))
		{
			//TODO what should this return?
			return new obj::Function(function->Body, function->Parameters);
		}
		else if (auto* letStmt = dynamic_cast<ast::LetStatement*>(node))
		{
			obj::Object* o = Evaluate(letStmt->Value, e);
			if (o->Type() == obj::ERROR_OBJ) return new obj::ErrorObject("errror in return statement");
			e.set(letStmt->Name->Value, o);
			return new obj::ReturnValue(o);
		}
		else if (auto* identifier = dynamic_cast<ast::Identifier*>(node))
		{
			obj::Object* o = e.get(identifier->Value);
			return (o == nullptr ? new obj::ErrorObject("Identifier not found") : o);
		}
		else if (auto* ifExpr = dynamic_cast<ast::IfExpression*>(node))
		{
			return evaluateIfExpr(ifExpr, e);
		
		}
		else if (auto* intPtr = dynamic_cast<ast::IntegerLiteral*>(node))
		{
			return new obj::Integer(intPtr->Value);
		}
		else if (auto* boolPtr = dynamic_cast<ast::Boolean*>(node))
		{
			return boolPtr->Value ? TRUE : FALSE;
		}
		else if (auto* callExpr = dynamic_cast<ast::CallExpression*>(node))
		{
			auto* function = Evaluate(callExpr->Function, e);
			if (function->Type() == obj::ERROR_OBJ) return new obj::ErrorObject("errror while evaluating function");

			std::vector<obj::Object*> args = evaluateExpressions(callExpr->Arguments, e);



			return applyFunction(function, args, e);
		}
		else if (auto* prefixExpr = dynamic_cast<ast::PrefixExpression*>(node))
		{
			auto* right = Evaluate(prefixExpr->Right, e);
			if (right->Type() == obj::ERROR_OBJ) return new obj::ErrorObject("errror in perfix expr");
			char OPER = prefixExpr->Operator[0];
			return evaluatePrefixExpr(OPER, right, e);
		}
		else if (auto* infixExpr = dynamic_cast<ast::InfixExpression*>(node))
		{
			auto* left = Evaluate(infixExpr->Left, e);
			if (left->Type() == obj::ERROR_OBJ) return new obj::ErrorObject("errror in left infix expr");

			auto* right = Evaluate(infixExpr->Right, e);
			if (right->Type() == obj::ERROR_OBJ) return new obj::ErrorObject("errror in right infix expr");

			return evaluateInfixExpr(infixExpr->Operator, left, right, e);
		}
		return new obj::ErrorObject("Node not sup");

	}



}