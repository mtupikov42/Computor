#pragma once

#include "AbstractExpressionNode.h"

class ImaginaryNumberNode : public AbstractExpressionNode {
public:
	static const char imaginaryUnit = 'i';

	explicit ImaginaryNumberNode() = default;

	std::string toString() const override;
};
