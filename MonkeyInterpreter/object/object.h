#pragma once

#include <string>

namespace obj
{

	const std::string INTEGER_OBJ = "INTEGER";
	const std::string BOOLEAN_OBJ = "BOOLEAN";
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
}
