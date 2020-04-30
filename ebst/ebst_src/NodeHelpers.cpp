#include "EBST.h"

namespace {

OperatorType switchAddSubOp(OperatorType type) {
	switch (type) {
	case OperatorType::Addition: return OperatorType::Substitution;
	case OperatorType::Substitution: return OperatorType::Addition;
	default: return type;
	}
}

} // end anonymous namespace

EBST::NodePtr EBST::allocateNode(const ExpressionNode& node) const {
	auto pair = std::make_pair(node, false);
	return std::make_shared<EBST::Node>(pair);
}

EBST::NodePtr EBST::createNodeByDegreeAndValue(double value, int degree) const {
	const auto allocatePowNode = [this](int degree) {
		const auto degreeNode = allocateNode(ExpressionNode(static_cast<double>(degree)));
		const auto unknownNode = allocateNode(ExpressionNode(unknownOperandName().front()));
		auto operatorNode = allocateNode(ExpressionNode(OperatorType::Power));
		operatorNode->m_left = unknownNode;
		operatorNode->m_right = degreeNode;

		return operatorNode;
	};

	switch (degree) {
	case 0: {
		return allocateNode(ExpressionNode(value));
	}
	case 1: {
		if (value == 1.0) {
			return allocateNode(ExpressionNode(unknownOperandName().front()));
		} else if (value == 0.0) {
			return allocateNode(ExpressionNode(0.0));
		}

		const auto valueNode = allocateNode(ExpressionNode(value));
		const auto unknownNode = allocateNode(ExpressionNode(unknownOperandName().front()));
		auto operatorNode = allocateNode(ExpressionNode(OperatorType::Multiplication));
		operatorNode->m_left = valueNode;
		operatorNode->m_right = unknownNode;

		return operatorNode;
	}
	case 2:
	case 3: {
		if (value == 1.0) {
			return allocatePowNode(degree);
		} else if (value == 0.0) {
			return allocateNode(ExpressionNode(0.0));
		}

		const auto valueNode = allocateNode(ExpressionNode(value));
		const auto powNode = allocatePowNode(degree);
		auto operatorNode = allocateNode(ExpressionNode(OperatorType::Multiplication));
		operatorNode->m_left = valueNode;
		operatorNode->m_right = powNode;

		return operatorNode;
	}
	}

	return nullptr;
}

ExpressionNode EBST::getExpressionNode(const NodePtr& ptr) const {
	return ptr->m_keyValue.first;
}

bool EBST::subTreesAreEqual(const NodePtr& n1, const NodePtr& n2) const {
	if (!n1 && !n2) {
		return true;
	}

	const auto getExpr = [](const NodePtr& ptr) {
		return ptr->m_keyValue.first;
	};

	const auto parent1Expr = getExpr(n1);
	const auto parent2Expr = getExpr(n2);

	return parent1Expr == parent2Expr
	       && subTreesAreEqual(n1->m_left, n2->m_left)
	       && subTreesAreEqual(n1->m_right, n2->m_right);
}

bool EBST::nodeHasUnknownExpr(const NodePtr& ptr) const {
	if (!nodeHasChildren(ptr)) {
		return false;
	}

	auto left = ptr->m_left;
	auto right = ptr->m_right;

	const auto leftExpr = getExpressionNode(left);
	const auto rightExpr = getExpressionNode(right);
	const auto operandsUnknown = isOperandUnknown(leftExpr.operandValue()) || isOperandUnknown(rightExpr.operandValue());
	const auto areOperators = isOperator(leftExpr) || isOperator(rightExpr);

	return operandsUnknown && !areOperators;
}

int EBST::getMaximumPowerOfSubtree(const EBST::NodePtr& node) const {
	if (nodeHasChildren((node))) {
		auto left = node->m_left;
		auto right = node->m_right;

		if (isOperator(node->m_keyValue.first) && node->m_keyValue.first.operatorType() == OperatorType::Power) {
			return static_cast<int>(right->m_keyValue.first.operandValue().value);
		}

		const auto leftPower = getMaximumPowerOfSubtree(left);
		const auto rightPower = getMaximumPowerOfSubtree(right);

		return std::max(leftPower, rightPower);
	}

	const auto nodeIsUnknownVar = isOperandUnknown(node->m_keyValue.first.operandValue());
	return nodeIsUnknownVar ? 1 : 0;
}

int EBST::countUnknownVars(const NodePtr& node) const {
	if (nodeHasChildren(node)) {
		auto left = node->m_left;
		auto right = node->m_right;

		const auto leftCount = countUnknownVars(left);
		const auto rightCount = countUnknownVars(right);

		return leftCount + rightCount;
	}

	const auto nodeIsUnknownVar = isOperandUnknown(node->m_keyValue.first.operandValue());
	return nodeIsUnknownVar ? 1 : 0;
}

bool EBST::nodeHasChildren(const NodePtr& node) const {
	return node->m_left && node->m_right;
}

double EBST::retrieveNumberFromNode(const NodePtr& node, OperatorType prevOp, bool isFirst) const {
	const auto rule = getRuleForNode(node);

	const auto modifier = prevOp == OperatorType::Substitution && !isFirst ? -1.0 : 1.0;

	if (rule == NodeRule::NumberVar) {
		return node->m_keyValue.first.operandValue().value * modifier;
	} else if (rule == NodeRule::Multiplication) {
		return node->m_left->m_keyValue.first.operandValue().value * modifier;
	} else if (rule == NodeRule::DivisionModulo) {
		if (node->m_keyValue.first.operatorType() == OperatorType::Division) {
			return modifier / node->m_right->m_keyValue.first.operandValue().value;
		}

		const auto expr = ExpressionNode(modifier) % node->m_right->m_keyValue.first;
		return expr.operandValue().value;
	}

	return modifier;
}

void EBST::mirrorNodeSign(SubtreeWithOperator& sub) {
	const auto subtreeRule = getRuleForSubtree(sub.subtree);

	switch (subtreeRule) {
	case NodeRule::NumberVar: {
		sub.subtree->m_keyValue.first = ExpressionNode(-sub.subtree->m_keyValue.first.operandValue().value);
		return;
	}
	case NodeRule::NumberAndSubtreeMul: {
		auto& numberNode = sub.subtree->m_left;
		const auto value = numberNode->m_keyValue.first.operandValue().value;
		numberNode->m_keyValue.first = ExpressionNode(-value);
		return;
	}
	case NodeRule::NumberAndSubtreeAddSub: {
		auto& numberNode = sub.subtree->m_left;
		const auto value = numberNode->m_keyValue.first.operandValue().value;
		numberNode->m_keyValue.first = ExpressionNode(-value);
		const auto opType = sub.subtree->m_keyValue.first.operatorType();
		sub.subtree->m_keyValue.first = ExpressionNode(switchAddSubOp(opType));
		return;
	}
	default: break;
	}

	sub.op = switchAddSubOp(sub.op);
}

void EBST::mirrorNodeSignByPrevOp(NodePtr& node, OperatorType& op) {
	const auto nodeRule = getRuleForSubtree(node);

	NodePtr valueNode;
	switch (nodeRule) {
	case NodeRule::NumberVar: {
		valueNode = node;
		break;
	}
	case NodeRule::UnknownAndNumberMul:
	case NodeRule::NumberAndSubtreeMul: {
		valueNode = node->m_left;
		break;
	}
	default: return;
	}

	const auto value = valueNode->m_keyValue.first.operandValue().value;
	const auto valueOp = value >= 0 ? OperatorType::Addition : OperatorType::Substitution;

	if (valueOp == OperatorType::Addition) {
		return;
	}

	valueNode->m_keyValue.first = ExpressionNode(-value);
	op = switchAddSubOp(op);
}
