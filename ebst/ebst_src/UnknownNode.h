#pragma once

#include "AbstractExpressionNode.h"

const auto invalidUnknownNodeName = '~';

class UnknownNode : public AbstractExpressionNode {
public:
	explicit UnknownNode(char name);

	char name() const;
	std::string toString() const override;

private:
	char m_name = invalidUnknownNodeName;
};
