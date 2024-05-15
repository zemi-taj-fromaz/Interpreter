#include "parser/parser.h"

namespace parser
{
	bool testLetStatement(ast::Statement* s, std::string name)
	{
		ast::LetStatement* ls;
		if (ast::LetStatement* f = dynamic_cast<ast::LetStatement*>(s)) {
			ls = f;
		}
		else
		{
			throw std::exception();
		}

		if (ls->Name->Value != name) return false;
		if (ls->Name->Token.Literal != name) return false;

		return true;
	}

	bool testReturnStatement(ast::Statement* s, std::string name)
	{
		ast::ReturnStatement* rs;
		if (ast::ReturnStatement* f = dynamic_cast<ast::ReturnStatement*>(s)) {
			rs = f;
		}
		else
		{
			throw std::exception();
		}

		if (rs->Token.Literal != name) return false;

		return true;
	}


	bool testExpressionStatement(ast::Statement* s, std::string name)
	{
		ast::ExpressionStatement* es;
		if (ast::ExpressionStatement* f = dynamic_cast<ast::ExpressionStatement*>(s)) {
			es = f;
		}
		else
		{
			throw std::exception();
		}

		if (es->Token.Literal != name) return false;

		return true;
	}

	void TestLetStatements()
	{
		std::string input =
			"let x = 5;"
			"let y = 10;"
			"let foobar = 838383;";

			Lexer::Lexer l = Lexer::Lexer(input);
			parser::Parser p = Parser(l);

			ast::Program program = p.ParseProgram();
			if (program.Statements.size() == 0) {
				std::cout << "program empty" << std::endl;
			}

			std::vector<std::string> expectedIdentifiers({ "x", "y", "foobar" });

			int i = 0;
			for (std::string& id : expectedIdentifiers)
			{
				auto* statement = program.Statements[i++];
				if (!testLetStatement(statement, id)) throw std::exception("You're gay");
			}
			std::cout << "PARSED CORRECTLY" << std::endl;
	}

	void TestReturnStatements()
	{
		std::string input =
			"return 5;"
			"return 10;"
			"return 838383;";

		Lexer::Lexer l = Lexer::Lexer(input);
		parser::Parser p = Parser(l);

		ast::Program program = p.ParseProgram();
		if (program.Statements.size() == 0) {
			std::cout << "program empty" << std::endl;
		}

		for (ast::Statement* s : program.Statements)
		{
			if (!testReturnStatement(s, "return")) throw std::exception("You're gay");
		}
		std::cout << "PARSED CORRECTLY" << std::endl;
	}

	void TestParsingInfix()
	{
		std::string input =
			"return zakon;";
			//"if( true ) { let mamu = 2; }";

		Lexer::Lexer l = Lexer::Lexer(input);
		parser::Parser p = Parser(l);

		ast::Program program = p.ParseProgram();
		if (program.Statements.size() == 0) {
			std::cout << "program empty" << std::endl;
		}

		for (ast::Statement* s : program.Statements)
		{
			//if (!testReturnStatement(s, "return")) throw std::exception("You're gay");
			ast::ReturnStatement* es = dynamic_cast<ast::ReturnStatement*>(s);
			if (!es) throw std::exception("You're gay");
			std::cout << es->String() << std::endl;
		}
		std::cout << "PARSED CORRECTLY" << std::endl;
	}
}