#include "EBST.h"

#include "ExpressionException.h"

EBST::NodePtr EBST::add(const NumberNode* n1, const NumberNode* n2) const {
	const auto result = n1->value() + n2->value();
	return EBST::allocateNode(std::make_shared<NumberNode>(result));
}

EBST::NodePtr EBST::subtract(const NumberNode* n1, const NumberNode* n2) const {
	const auto result = n1->value() - n2->value();
	return EBST::allocateNode(std::make_shared<NumberNode>(result));
}

EBST::NodePtr EBST::divide(const NumberNode* n1, const NumberNode* n2) const {
	if (n2->value() == 0.0) {
		throw ExpressionException(ExpressionError::DivisionByZero, 0);
	}

	const auto result = n1->value() / n2->value();
	return EBST::allocateNode(std::make_shared<NumberNode>(result));
}

EBST::NodePtr EBST::modulo(const NumberNode* n1, const NumberNode* n2) const {
	if (n2->value() == 0.0) {
		throw ExpressionException(ExpressionError::DivisionByZero, 0);
	}

	const auto result = std::remainder(n1->value(), n2->value());
	return EBST::allocateNode(std::make_shared<NumberNode>(result));
}

EBST::NodePtr EBST::multiply(const NumberNode* n1, const NumberNode* n2) const {
	const auto result = n1->value() * n2->value();
	return EBST::allocateNode(std::make_shared<NumberNode>(result));
}

EBST::NodePtr EBST::power(const NumberNode* n1, const NumberNode* n2) const {
	const auto result = std::pow(n1->value(), n2->value());
	return EBST::allocateNode(std::make_shared<NumberNode>(result));
}
