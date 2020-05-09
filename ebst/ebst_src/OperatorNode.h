#pragma once

#include "AbstractExpressionNode.h"

enum class OperatorType {
	Invalid,
	Addition,
	Substitution,
	Division,
	Multiplication,
	Modulo,
	Power,
	BracketLeft,
	BracketRight
};

class OperatorNode : public AbstractExpressionNode {
public:
	explicit OperatorNode(OperatorType type);

	OperatorType type() const;
	int precedence() const;
	std::string toString() const override;

private:
	OperatorType m_type = OperatorType::Invalid;
};
