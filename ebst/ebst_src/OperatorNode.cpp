#include "OperatorNode.h"

#include <map>
#include <cassert>

OperatorNode::OperatorNode(OperatorType type) : m_type(type) {}

OperatorType OperatorNode::type() const {
	return m_type;
}

int OperatorNode::precedence() const {
	const std::map<OperatorType, int> operatorToPrecedence {
		{ OperatorType::Addition, 2 },
		{ OperatorType::Substitution, 2 },
		{ OperatorType::Division, 3 },
		{ OperatorType::Multiplication, 3 },
		{ OperatorType::Modulo, 3 },
		{ OperatorType::Power, 4 },
		{ OperatorType::BracketLeft, 5 },
		{ OperatorType::BracketRight, 5 },
	};

	const auto it = operatorToPrecedence.find(m_type);
	if (it != operatorToPrecedence.cend()) {
		return operatorToPrecedence.at(m_type);
	}

	assert(false && "invalid operator type");
	return -1;
}

std::string OperatorNode::toString() const {
	const std::map<OperatorType, char> operatorToChar {
		{ OperatorType::Addition, '+' },
		{ OperatorType::Substitution, '-' },
		{ OperatorType::Division, '/' },
		{ OperatorType::Multiplication, '*' },
		{ OperatorType::Modulo, '%' },
		{ OperatorType::Power, '^' },
		{ OperatorType::BracketLeft, '(' },
		{ OperatorType::BracketRight, ')' }
	};

	const auto it = operatorToChar.find(m_type);
	if (it != operatorToChar.cend()) {
		return { operatorToChar.at(m_type) };
	}

	assert(false && "invalid operator type");
	return {};
}

bool isBracket(const OperatorNode::Ptr& ex) {
	const auto casted = ex->castToOperatorNode();

	if (!casted) {
		return false;
	}

	const auto type = casted->type();
	return type == OperatorType::BracketLeft || type == OperatorType ::BracketRight;
}
