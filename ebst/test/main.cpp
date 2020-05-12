#include "EBST.h"
#include "ExpressionException.h"

#include "OperatorNode.h"
#include "NumberNode.h"
#include "ImaginaryNumberNode.h"
#include "UnknownNode.h"

#include <assert.h>
#include <iostream>

void operandTest() {
	{
		auto t = parseExpressionFromString("x");
		assert(t);
		const auto u = t->castToUnknownNode();
		assert(u);
		assert(u->name() == 'x');
	}

	{
		auto t = parseExpressionFromString("xy");
		assert(!t);
	}

	{
		auto t = parseExpressionFromString("3.14567");
		assert(t);
		const auto u = t->castToNumberNode();
		assert(u);
		auto tRes = u->value();
		assert(tRes == 3.14567);
	}

	{
		auto t = parseExpressionFromString("4.");
		assert(t);
		const auto u = t->castToNumberNode();
		assert(u);
		auto tRes = u->value();
		assert(tRes == 4.);
	}

	{
		auto t = parseExpressionFromString("+24434312");
		assert(t);
		const auto u = t->castToNumberNode();
		assert(u);
		auto tRes = u->value();
		assert(tRes == 24434312);
	}

	{
		auto t = parseExpressionFromString("-100500");
		assert(t);
		const auto u = t->castToNumberNode();
		assert(u);
		auto tRes = u->value();
		assert(tRes == -100500);
	}

	{
		auto t = parseExpressionFromString("-24434312.49328409234932");
		assert(t);
		const auto u = t->castToNumberNode();
		assert(u);
		auto tRes = u->value();
		assert(tRes == -24434312.49328409234932);
	}

	std::cout << "Operands OK" << std::endl;
}

void operatorTest() {
	{
		auto o = parseExpressionFromChar('a');
		assert(!o);
	}

	{
		auto o = parseExpressionFromChar('+');
		assert(o);
		const auto u = o->castToOperatorNode();
		assert(u);
		assert(u->type() == OperatorType::Addition);
	}

	std::cout << "Operators OK" << std::endl;
}

void ebstTest() {
	try {
		const auto tree = EBST("x - x + x * x - 10 * (10 * x) = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("-100.0 * x + x ^ 2.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 2);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 10000);

		auto res1 = result[0];
		auto res2 = result[1];
		assert(res1.varName == "x1");
		assert(res1.varResult == trimDoubleToString(0.0));
		assert(res2.varName == "x2");
		assert(res2.varResult == trimDoubleToString(100.0));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("x ^ 2 - 10 * x + 5 * x ^ 2 = -10 + 5 * x + x ^ 2 - x", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("10.0 - 14.0 * x + 5.0 * x ^ 2.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 2);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == -4);

		auto res1 = result[0];
		auto res2 = result[1];
		assert(res1.varName == "x1");
		assert(res1.varResult == "1.40-0.20i");
		assert(res2.varName == "x2");
		assert(res2.varResult == "1.40+0.20i");
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("(x^2 + (-10.123450 * 660000) + x / 100) = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("-6681477.0 + 0.010 * x + x ^ 2.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 2);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 26725908.0001);

		auto res1 = result[0];
		auto res2 = result[1];
		assert(res1.varName == "x1");
		assert(res1.varResult == trimDoubleToString(-2584.860315));
		assert(res2.varName == "x2");
		assert(res2.varResult == trimDoubleToString(2584.850315));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("-10 * x^2 - -4 * x + 7 + x = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("7.0 + 5.0 * x - 10.0 * x ^ 2.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 2);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 305);

		auto res1 = result[0];
		auto res2 = result[1];
		assert(res1.varName == "x1");
		assert(res1.varResult == trimDoubleToString(1.123212));
		assert(res2.varName == "x2");
		assert(res2.varResult == trimDoubleToString(-0.623212));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("x ^ 2 = x ^ 2", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("0.0 = 0") == 0);
		assert(tree.solution().solutions.size() == 0);
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("(x ^ 2) * 11 - (x ^ 2) * 2 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("9.0 * x ^ 2.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 1);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 0);

		auto res = result[0];
		assert(res.varName == "x");
		assert(res.varResult == trimDoubleToString(0.0));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("x ^ 2 + 7.3 * x = -1 * x ^ 2 + 10 - 5 * x", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("-10.0 + 12.30 * x + 2.0 * x ^ 2.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 2);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 231.29000000000002);

		auto res1 = result[0];
		auto res2 = result[1];
		assert(res1.varName == "x1");
		assert(res1.varResult == trimDoubleToString(-6.877055));
		assert(res2.varName == "x2");
		assert(res2.varResult == trimDoubleToString(0.727055));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("5 * x ^ 0 + 4 * x^1 - 9.3 * x ^ 2 - 1 * x^0 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("4.0 + 4.0 * x - 9.30 * x ^ 2.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 2);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 164.80);

		auto res1 = result[0];
		auto res2 = result[1];
		assert(res1.varName == "x1");
		assert(res1.varResult == trimDoubleToString(0.905239));
		assert(res2.varName == "x2");
		assert(res2.varResult == trimDoubleToString(-0.475131));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("x * 4 - x ^ 2 * 4 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("4.0 * x - 4.0 * x ^ 2.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 2);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 16);

		auto res1 = result[0];
		auto res2 = result[1];
		assert(res1.varName == "x1");
		assert(res1.varResult == trimDoubleToString(1.0));
		assert(res2.varName == "x2");
		assert(res2.varResult == trimDoubleToString(0.0));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("5 * x^0 + 4 * x^1 - 4 * x^0 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("1.0 + 4.0 * x = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 1);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 16);

		auto res = result.front();
		assert(res.varName == "x");
		assert(res.varResult == trimDoubleToString(-0.25));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("5 + 4 * x + x^2- x^2 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("5.0 + 4.0 * x = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 1);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 16);

		auto res = result.front();
		assert(res.varName == "x");
		assert(res.varResult == trimDoubleToString(-5.0 / 4.0));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("-3.14 * x ^ 3 - x ^ 2 + 10 * x - 5 = 3.6 - 2.7 - 2.98 * x + 0.001 * x ^ 2 - x ^ 3", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("-5.90 + 12.980 * x - 1.0010 * x ^ 2.0 - 2.140 * x ^ 3.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 3);

		const auto disc = tree.solution().discriminant;
		assert(!disc.has_value());

		auto res1 = result[0];
		auto res2 = result[1];
		auto res3 = result[2];
		assert(res1.varName == "x1");
		assert(res1.varResult == trimDoubleToString(1.932575));
		assert(res2.varName == "x2");
		assert(res2.varResult == trimDoubleToString(-2.893387));
		assert(res3.varName == "x3");
		assert(res3.varResult == trimDoubleToString(0.493055));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("5 * x^0 - 5 * x^0 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("0.0 = 0") == 0);
		assert(tree.solution().solutions.size() == 0);
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("5 * x^0 - 4 * x^0 - 7 * x^1 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("1.0 - 7.0 * x = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 1);
		auto res = result.front();

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 49);

		assert(res.varName == "x");
		assert(res.varResult == trimDoubleToString(1.0 / 7.0));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("5 * x^0 + 13 * x^1 + 3 * x^2 - 1* x^0 - 1 * x^1 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("4.0 + 12.0 * x + 3.0 * x ^ 2.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 2);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 96);

		auto res1 = result[0];
		auto res2 = result[1];
		assert(res1.varName == "x1");
		assert(res1.varResult == trimDoubleToString(-3.632993));
		assert(res2.varName == "x2");
		assert(res2.varResult == trimDoubleToString(-0.367007));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("6 * x^0 + 11 * x^1 + 5 * x^2 - 1 * x^0 - 1 * x^1 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("5.0 + 10.0 * x + 5.0 * x ^ 2.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 1);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == 0);

		auto res = result[0];
		assert(res.varName == "x");
		assert(res.varResult == trimDoubleToString(-1.0));
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("5 * x^0 + 3 * x^1 + 3 * x^2 - 1 * x^0 + 0 * x^1 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("4.0 + 3.0 * x + 3.0 * x ^ 2.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 2);

		const auto disc = tree.solution().discriminant;
		assert(disc.has_value());
		assert(disc.value() == -39);

		auto res1 = result[0];
		auto res2 = result[1];
		assert(res1.varName == "x1");
		assert(res1.varResult == "-0.50-1.040833i");
		assert(res2.varName == "x2");
		assert(res2.varResult == "-0.50+1.040833i");
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("8 * x^0 - 6 * x^1 + 0 * x^2 - 5.6 * x^3 - 3 * x^0 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("5.0 - 6.0 * x - 5.60 * x ^ 3.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 3);

		const auto disc = tree.solution().discriminant;
		assert(!disc.has_value());

		auto res1 = result[0];
		auto res2 = result[1];
		auto res3 = result[2];
		assert(res1.varName == "x1");
		assert(res1.varResult == "0.615598");
		assert(res2.varName == "x2");
		assert(res2.varResult == "-0.307799+1.164324i");
		assert(res3.varName == "x3");
		assert(res3.varResult == "-0.307799-1.164324i");
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	try {
		const auto tree = EBST("4 * x ^ 3 - 10 * x ^ 2 + 9.46 * x - 90.5 = 0", ExpressionType::FullExpression);
		const auto treeToString = tree.toString(EBST::OutputType::Infix);
		assert(treeToString.compare("-90.50 + 9.460 * x - 10.0 * x ^ 2.0 + 4.0 * x ^ 3.0 = 0") == 0);

		const auto result = tree.solution().solutions;
		assert(result.size() == 3);

		const auto disc = tree.solution().discriminant;
		assert(!disc.has_value());

		auto res1 = result[0];
		auto res2 = result[1];
		auto res3 = result[2];
		assert(res1.varName == "x1");
		assert(res1.varResult == "3.593748");
		assert(res2.varName == "x2");
		assert(res2.varResult == "-0.546874+2.448792i");
		assert(res3.varName == "x3");
		assert(res3.varResult == "-0.546874-2.448792i");
	} catch (const ExpressionException& ex) {
		std::cout << ex.toString() << "; column: " << ex.column() << std::endl;
		assert(false);
	}

	// invalid scenarios
	try {
		const auto tree = EBST("x = ", ExpressionType::FullExpression);
		assert(false && "did not caught empty expression");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::InvalidToken);
	}

	try {
		const auto tree = EBST("x^2 + 5 * x", ExpressionType::FullExpression);
		assert(false && "did not caught no equal sign");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::NoEqualSign);
	}

	try {
		const auto tree = EBST("x^2 = 4 * x = 3 - x", ExpressionType::FullExpression);
		assert(false && "did not caught multiple equal signs");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::TooManyEqualSigns);
	}

	try {
		const auto tree = EBST("x^2 - 4 * y + 5 * x = 0", ExpressionType::FullExpression);
		assert(false && "did not caught multiple unknowns");
	} catch (const ExpressionException&) {}

    try {
		const auto tree = EBST("x^2 - 4x + 7 + x = 0", ExpressionType::FullExpression);
        assert(false && "did not caught missing operator");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::MissingOperator);
	}

    try {
		const auto tree = EBST("xd^2 - 4 * x + 7 + x = 0", ExpressionType::FullExpression);
        assert(false && "did not caught multiple char variable");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::UnknownOperandSize);
	}

    try {
		const auto tree = EBST("(x + 10 = 0", ExpressionType::FullExpression);
        assert(false && "did not caught missing ')' bracket");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::MissingRightParentheses);
	}

    try {
		const auto tree = EBST("x + 10) = 0", ExpressionType::FullExpression);
        assert(false && "did not caught missing '(' bracket");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::MissingLeftParentheses);
	}

    try {
		const auto tree = EBST("10 ? 2 = 0", ExpressionType::FullExpression);
        assert(false && "did not caught invalid operator");
	} catch (const ExpressionException&) {}

    try {
		const auto tree = EBST("10.312.312.312 + 543.534543.543 = 0", ExpressionType::FullExpression);
        assert(false && "did not caught invalid operand");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::InvalidToken);
	}

	try {
		const auto tree = EBST("10 / (x - 2) + 10 * x ^ 2 = 0", ExpressionType::FullExpression);
		assert(false && "did not caught too complex division");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::TooComplexDivision);
	}

	try {
		const auto tree = EBST("3 ^ (x - 20) + 20 * x = 0", ExpressionType::FullExpression);
		assert(false && "did not caught too complex degree");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::TooComplexDegree);
	}

	try {
		const auto tree = EBST("(x / 100) / (x - 2) + 10 * x ^ 2 = 0", ExpressionType::FullExpression);
		assert(false && "did not caught too complex division");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::TooComplexDivision);
	}

	try {
		const auto tree = EBST("(x / 100) ^ (x - 20) + 20 * x = 0", ExpressionType::FullExpression);
		assert(false && "did not caught too complex degree");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::TooComplexDegree);
	}

	try {
		const auto tree = EBST("4 * x^0 - 8 * x^0 = 0", ExpressionType::FullExpression);
		assert(false && "did not caught unsolving expresion");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::Unsolvable);
	}

	try {
		const auto tree = EBST("8 * x^0 - 6 * x^1 + 0 * x^2 - 5.6 * x^4 - 3 * x^3 = 0", ExpressionType::FullExpression);
		assert(false && "uncought degree higher than three");
	} catch (const ExpressionException& ex) {
		assert(ex.errorType() == ExpressionError::DegreeHigherThanThree);
	}

    std::cout << "Tree OK" << std::endl;
}

int main() {
	operandTest();
	operatorTest();
    ebstTest();

	return 0;
}
