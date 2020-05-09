#pragma once

#include "AbstractExpressionNode.h"

class ImaginaryNumberNode : public AbstractExpressionNode {
public:
	explicit ImaginaryNumberNode() = default;

	std::string toString() const override;
};
