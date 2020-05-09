#pragma once

#include <string>
#include <memory>

class NumberNode;
class ImaginaryNumberNode;
class UnknownNode;
class OperatorNode;

class AbstractExpressionNode {
public:
	using Ptr = std::shared_ptr<AbstractExpressionNode>;

	virtual ~AbstractExpressionNode() = default;

	NumberNode* castToNumberNode();
	UnknownNode* castToUnknownNode();
	OperatorNode* castToOperatorNode();
	ImaginaryNumberNode* castToImaginaryNumberNode();

	virtual std::string toString() const = 0;
};

AbstractExpressionNode::Ptr parseExpressionFromString(const std::string& str);
AbstractExpressionNode::Ptr parseExpressionFromChar(char c);
