#pragma once

#include <string>
#include <sstream>

namespace env
{
	struct Environment;
}


namespace obj
{

	const std::string INTEGER_OBJ = "INTEGER";
	const std::string RETURN_OBJ = "RETURN";
	const std::string BOOLEAN_OBJ = "BOOLEAN";
	const std::string FUNCTION_OBJ = "FUNCTION";
	const std::string ERROR_OBJ = "ERROR";
	const std::string NULL_OBJ = "NULL";

	struct Object
	{
		virtual std::string Type() = 0;
		virtual std::string Inspect() = 0;
	};

	struct Integer : public Object
	{
		Integer(uint64_t val) : Value(val) {}
		virtual std::string Type() { return INTEGER_OBJ; }
		virtual std::string Inspect() { return std::to_string(Value); }
		int64_t Value;
	};

	struct ReturnValue : public Object
	{
		ReturnValue(Object* object) : obj(object) {}
		virtual std::string Type() { return RETURN_OBJ; }
		virtual std::string Inspect() { return obj->Inspect(); }
		Object* obj;
	};

	struct ErrorObject : public Object
	{
		ErrorObject(std::string message) : msg(message) {}
		std::string msg;
		virtual std::string Type() { return ERROR_OBJ; }
		virtual std::string Inspect() { return msg; }
	};

	struct Boolean : public Object
	{
		Boolean(bool val) : Value(val) {}
		virtual std::string Type() { return BOOLEAN_OBJ; }
		virtual std::string Inspect() { return (Value ? "true" : "false"); }
		bool Value;
	};

	struct Null : public Object
	{
		virtual std::string Type() { return NULL_OBJ; }
		virtual std::string Inspect() { return "null"; }
	};

	struct Function : public Object
	{
		Function(ast::BlockStatement* body, std::vector<ast::Identifier*> p) : body(body), Parameters(p) {}
		ast::BlockStatement* body;
		env::Environment* env;
		std::vector<ast::Identifier*> Parameters;
		virtual std::string Type() { return FUNCTION_OBJ; }
		virtual std::string Inspect() {
			
			std::ostringstream os;
			
			for (auto it = Parameters.begin(); it != Parameters.end(); ++it) {
				if (it != Parameters.begin()) {
					os << ",";
				}
				os << *it;
			}
			std::string paramsString = os.str();
			
			std::string ret = "fn(" + paramsString + "){\n" + body->String() + "\n}";
			return ret;
		}
		bool Value;
	};
}
