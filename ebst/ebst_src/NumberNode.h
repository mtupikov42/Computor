#pragma once

#include "AbstractExpressionNode.h"

class NumberNode : public AbstractExpressionNode {
public:
	explicit NumberNode(double value);

	double value() const;
	std::string toString() const override;

private:
	double m_value = 0.0;
};

std::string trimDoubleToString(double d);
