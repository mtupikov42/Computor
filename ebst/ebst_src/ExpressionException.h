#pragma once

#include <string>

enum class ExpressionError {
	CannotBalance,
	DegreeHigherThanThree,
	DivisionByZero,
	InvalidToken,
	LeftBracketError,
	MissingLeftParentheses,
	MissingOperator,
	MissingRightParentheses,
	MultipleUnknownOperands,
	NoEqualSign,
	OperatorAfterOperator,
	TooComplexDegree,
	TooComplexDivision,
	TooManyEqualSigns,
	UnknownOperandSize,
	Unsolvable
};

class ExpressionException : public std::logic_error {
public:
	ExpressionException(ExpressionError err, int column);

	std::string toString() const;
	ExpressionError errorType() const;
	int column() const;

private:
	ExpressionError m_type;
	int m_column;
};
