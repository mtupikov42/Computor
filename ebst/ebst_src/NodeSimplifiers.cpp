#include "EBST.h"

#include "ExpressionException.h"

EBST::NodePtr EBST::simplifySubtree(NodePtr& node) const {
	const auto castedNode = getExpressionNode(node)->castToOperatorNode();
	if (!castedNode) {
		return node;
	}

	const auto nodeOp = castedNode->type();
	switch (nodeOp) {
	case OperatorType::Addition: return simplifyAddition(node);
	case OperatorType::Substitution: return simplifySubstitution(node);
	case OperatorType::Multiplication: return simplifyMultiplication(node);
	case OperatorType::Modulo:
	case OperatorType::Division: return simplifyDivision(node);
	case OperatorType::Power: return simplifyPower(node);
	default: return node;
	}
}

EBST::NodePtr EBST::simplifyAddition(NodePtr& node) const {
	auto& left = node->m_left;
	auto& right = node->m_right;

	const auto leftRule = getRuleForSubtree(left);
	const auto rightRule = getRuleForSubtree(right);
	const auto areEqual = leftRule == rightRule;

	const auto isSimpleRule = [](NodeRule rule) {
		return rule == NodeRule::UnknownVar || rule == NodeRule::NumberVar;
	};

	const auto resetLeftRight = [&left, &right] {
		left.reset();
		right.reset();
	};

	const auto isOpType = [this](const NodePtr& node, OperatorType type) {
		const auto opNode = getExpressionNode(node)->castToOperatorNode();
		return opNode && opNode->type() == type;
	};

	if (areEqual) {
		const auto leftLeftIsUnknown = getRuleForSubtree(left->m_left) == EBST::NodeRule::UnknownVar;
		auto& leftUnknownNode = leftLeftIsUnknown ? left->m_left : left->m_right;
		auto& leftNumberNode = leftLeftIsUnknown ? left->m_right : left->m_left;
		auto& leftNumberOperand = leftNumberNode->m_keyValue.first;

		const auto rightLeftIsUnknown = getRuleForSubtree(right->m_left) == EBST::NodeRule::UnknownVar;
		auto& rightNumberNode = rightLeftIsUnknown ? right->m_right : right->m_left;
		auto& rightNumberOperand = rightNumberNode->m_keyValue.first;

		switch (leftRule) {
		case UnknownAndNumberMul: {
			auto newNumberNode = add(leftNumberOperand->castToNumberNode(), rightNumberOperand->castToNumberNode());
			node->m_left = leftUnknownNode;
			node->m_right = newNumberNode;
			node->m_keyValue.first = std::make_shared<OperatorNode>(OperatorType::Multiplication);

			break;
		}
		case UnknownAndNumberAddSub: {
			auto leftNumber = leftNumberOperand->castToNumberNode()->value();
			if (isOpType(left, OperatorType::Substitution) && leftLeftIsUnknown) {
				leftNumber *= -1;
			}

			auto rightNumber = rightNumberOperand->castToNumberNode()->value();
			if (isOpType(right, OperatorType::Substitution) && rightLeftIsUnknown) {
				rightNumber *= -1;
			}

			const auto resNumber = leftNumber + rightNumber;
			const auto resExpr = std::make_shared<NumberNode>(resNumber);

			if (isOpType(left, OperatorType::Substitution) && !leftLeftIsUnknown && rightLeftIsUnknown) {
				node->m_keyValue.first = resExpr;
				resetLeftRight();
				break;
			} else if (leftLeftIsUnknown && rightLeftIsUnknown) {
				auto newLeftNode = allocateNode(std::make_shared<OperatorNode>(OperatorType::Multiplication));
				newLeftNode->m_left = allocateNode(std::make_shared<NumberNode>(2.0));
				newLeftNode->m_right = leftUnknownNode;

				node->m_left = newLeftNode;
				node->m_right = allocateNode(resExpr);

				break;
			} else if (isOpType(left, OperatorType::Substitution) && !leftLeftIsUnknown
			            && isOpType(right, OperatorType::Substitution) && !rightLeftIsUnknown) {
				auto newLeftNode = allocateNode(std::make_shared<OperatorNode>(OperatorType::Multiplication));
				newLeftNode->m_left = allocateNode(std::make_shared<NumberNode>(-2.0));
				newLeftNode->m_right = leftUnknownNode;

				node->m_left = newLeftNode;
				node->m_right = allocateNode(resExpr);

				break;
			}

			break;
		}
		case UnknownAndNumberPow: {
			const auto leftNumber = leftNumberOperand->castToNumberNode()->value();
			const auto rightNumber = rightNumberOperand->castToNumberNode()->value();

			if (leftNumber == rightNumber && leftLeftIsUnknown && rightLeftIsUnknown) {
				node->m_left = allocateNode(std::make_shared<NumberNode>(2.0));
				node->m_keyValue.first = std::make_shared<OperatorNode>(OperatorType::Multiplication);
			}

			break;
		}
		case NumberAndSubtreeMul: {
			const auto leftLeftIsNumber = getRuleForSubtree(left->m_left) == EBST::NodeRule::NumberVar;
			auto& leftUnknownNode = leftLeftIsNumber ? left->m_right : left->m_left;
			auto& leftNumberNode = leftLeftIsNumber ? left->m_left : left->m_right;

			const auto rightLeftIsNumber = getRuleForSubtree(right->m_left) == EBST::NodeRule::NumberVar;
			auto& rightNumberNode = rightLeftIsNumber ? right->m_left : right->m_right;
			auto& rightUnknownNode = leftLeftIsNumber ? right->m_right : right->m_left;

			if (subTreesAreEqual(leftUnknownNode, rightUnknownNode)) {
				auto newNumNode = add(getExpressionNode(leftNumberNode)->castToNumberNode(), getExpressionNode(rightNumberNode)->castToNumberNode());
				node->m_keyValue.first = std::make_shared<OperatorNode>(OperatorType::Multiplication);
				node->m_left = leftUnknownNode;
				node->m_right = newNumNode;
			}
			break;
		}
		default: break;
		}
	} else if (isSimpleRule(leftRule) || isSimpleRule(rightRule)) {
		const auto leftIsSimple = isSimpleRule(leftRule);
		const auto simpleRule = leftIsSimple ? leftRule : rightRule;
		const auto simpleRuleIsNumber = simpleRule == NodeRule::NumberVar;
		const auto complexRule = leftIsSimple ? rightRule : leftRule;

		const auto simpleNode = leftIsSimple ? left : right;
		const auto simpleNodeNum = getExpressionNode(simpleNode)->castToNumberNode();
		const auto simpleNodeNumValue = simpleNodeNum ? simpleNodeNum->value() : 0.0;
		const auto complexNode = leftIsSimple ? right : left;
		const auto complexLeftIsUnknown = getRuleForNode(complexNode->m_left) == NodeRule::UnknownVar;
		const auto complexNumNode = complexLeftIsUnknown ? complexNode->m_right : complexNode->m_left;
		const auto complexUnkNode = complexLeftIsUnknown ? complexNode->m_left : complexNode->m_right;
		const auto complexNodeNum = getExpressionNode(complexNumNode)->castToNumberNode();
		const auto complexNodeNumValue = complexNodeNum ? complexNodeNum->value() : 0.0;

		const auto complexOpIsAdd = getExpressionNode(complexNode)->castToOperatorNode()->type() == OperatorType::Addition;

		switch (complexRule) {
		case UnknownAndNumberAddSub: {
			if (simpleRuleIsNumber) {
				double resNum;
				if (!complexOpIsAdd && complexLeftIsUnknown) {
					resNum = simpleNodeNumValue - complexNodeNumValue;
				} else {
					resNum = complexNodeNumValue + simpleNodeNumValue;
				}

				if (resNum == 0.0) {
					node = complexUnkNode;
					break;
				}

				const auto newNumNode = allocateNode(std::make_shared<NumberNode>(resNum));
				node->m_left = complexUnkNode;
				node->m_right = newNumNode;

				break;
			} else {
				if (!complexOpIsAdd && !complexLeftIsUnknown) {
					node->m_keyValue.first = std::make_shared<NumberNode>(complexNodeNumValue);
					resetLeftRight();

					break;
				} else {
					auto newNode = allocateNode(std::make_shared<OperatorNode>(OperatorType::Multiplication));
					newNode->m_left = allocateNode(std::make_shared<NumberNode>(2.0));
					newNode->m_right = complexUnkNode;

					node->m_left = newNode;
					node->m_right = complexNumNode;

					break;
				}
			}
		}
		case UnknownAndNumberMul: {
			if (!simpleRuleIsNumber) {
				const auto newNum = complexNodeNumValue + 1;
				node->m_keyValue.first = std::make_shared<OperatorNode>(OperatorType::Multiplication);
				node->m_left = complexUnkNode;
				node->m_right = allocateNode(std::make_shared<NumberNode>(newNum));
			}

			break;
		}
		case UnknownAndNumberDivMod:
		case UnknownAndNumberPow: {
			if (simpleNodeNumValue == 0.0) {
				node = complexNode;

				break;
			}
			break;
		}
		default: break;
		}
	}

	switchLeftRightIfNumberOnRight(node);
	return node;
}

EBST::NodePtr EBST::simplifySubstitution(NodePtr& node) const {
	auto& left = node->m_left;
	auto& right = node->m_right;

	const auto leftRule = getRuleForSubtree(left);
	const auto rightRule = getRuleForSubtree(right);
	const auto areEqual = leftRule == rightRule;

	const auto isSimpleRule = [](NodeRule rule) {
		return rule == NodeRule::UnknownVar || rule == NodeRule::NumberVar;
	};

	const auto resetLeftRight = [&left, &right] {
		left.reset();
		right.reset();
	};

	const auto isOpType = [this](const NodePtr& node, OperatorType type) {
		const auto opNode = getExpressionNode(node)->castToOperatorNode();
		return opNode && opNode->type() == type;
	};

	if (areEqual) {
		const auto leftLeftIsUnknown = getRuleForSubtree(left->m_left) == EBST::NodeRule::UnknownVar;
		auto& leftUnknownNode = leftLeftIsUnknown ? left->m_left : left->m_right;
		auto& leftNumberNode = leftLeftIsUnknown ? left->m_right : left->m_left;
		auto& leftNumberOperand = leftNumberNode->m_keyValue.first;

		const auto rightLeftIsUnknown = getRuleForSubtree(right->m_left) == EBST::NodeRule::UnknownVar;
		auto& rightNumberNode = rightLeftIsUnknown ? right->m_right : right->m_left;
		auto& rightNumberOperand = rightNumberNode->m_keyValue.first;

		switch (leftRule) {
		case UnknownAndNumberMul: {
			auto newNumberNode = subtract(leftNumberOperand->castToNumberNode(), rightNumberOperand->castToNumberNode());
			node->m_left = leftUnknownNode;
			node->m_right = newNumberNode;
			node->m_keyValue.first = std::make_shared<OperatorNode>(OperatorType::Multiplication);

			break;
		}
		case UnknownAndNumberAddSub: {
			auto leftNumber = leftNumberOperand->castToNumberNode()->value();
			if (isOpType(left, OperatorType::Substitution) && leftLeftIsUnknown) {
				leftNumber *= -1;
			}

			auto rightNumber = rightNumberOperand->castToNumberNode()->value();
			if (isOpType(right, OperatorType::Substitution) && rightLeftIsUnknown) {
				rightNumber *= -1;
			}

			const auto resNumber = leftNumber - rightNumber;
			const auto resExpr = std::make_shared<NumberNode>(resNumber);

			if ((isOpType(left, OperatorType::Addition) && !leftLeftIsUnknown && rightLeftIsUnknown)
			    || (leftLeftIsUnknown && rightLeftIsUnknown)) {
				node->m_keyValue.first = resExpr;
				resetLeftRight();
			} else if (leftLeftIsUnknown && isOpType(right, OperatorType::Substitution) && !rightLeftIsUnknown) {
				auto newLeftNode = allocateNode(std::make_shared<OperatorNode>(OperatorType::Multiplication));
				newLeftNode->m_left = allocateNode(std::make_shared<NumberNode>(2.0));
				newLeftNode->m_right = leftUnknownNode;

				node->m_left = newLeftNode;
				node->m_right = allocateNode(resExpr);
			} else if (isOpType(left, OperatorType::Substitution) && !leftLeftIsUnknown && rightLeftIsUnknown) {
				auto newLeftNode = allocateNode(std::make_shared<OperatorNode>(OperatorType::Multiplication));
				newLeftNode->m_left = allocateNode(std::make_shared<NumberNode>(-2.0));
				newLeftNode->m_right = leftUnknownNode;

				node->m_left = newLeftNode;
				node->m_right = allocateNode(resExpr);
			}

			break;
		}
		case UnknownAndNumberPow: {
			const auto leftNumber = leftNumberOperand->castToNumberNode()->value();
			const auto rightNumber = rightNumberOperand->castToNumberNode()->value();

			if (leftNumber == rightNumber && leftLeftIsUnknown && rightLeftIsUnknown) {
				node->m_keyValue.first = std::make_shared<NumberNode>(0.0);
				resetLeftRight();
			}

			break;
		}
		case NumberAndSubtreeMul: {
			const auto leftLeftIsNumber = getRuleForSubtree(left->m_left) == EBST::NodeRule::NumberVar;
			auto& leftUnknownNode = leftLeftIsNumber ? left->m_right : left->m_left;
			auto& leftNumberNode = leftLeftIsNumber ? left->m_left : left->m_right;

			const auto rightLeftIsNumber = getRuleForSubtree(right->m_left) == EBST::NodeRule::NumberVar;
			auto& rightNumberNode = rightLeftIsNumber ? right->m_left : right->m_right;
			auto& rightUnknownNode = leftLeftIsNumber ? right->m_right : right->m_left;

			if (subTreesAreEqual(leftUnknownNode, rightUnknownNode)) {
				auto newNumNode = subtract(getExpressionNode(leftNumberNode)->castToNumberNode(), getExpressionNode(rightNumberNode)->castToNumberNode());
				node->m_keyValue.first = std::make_shared<OperatorNode>(OperatorType::Multiplication);
				node->m_left = leftUnknownNode;
				node->m_right = newNumNode;
			}

			break;
		}
		default: break;
		}
	} else if (isSimpleRule(leftRule) || isSimpleRule(rightRule)) {
		const auto leftIsSimple = isSimpleRule(leftRule);
		const auto simpleRule = leftIsSimple ? leftRule : rightRule;
		const auto simpleRuleIsNumber = simpleRule == NodeRule::NumberVar;
		const auto complexRule = leftIsSimple ? rightRule : leftRule;

		const auto simpleNode = leftIsSimple ? left : right;
		const auto simpleNodeNum = getExpressionNode(simpleNode)->castToNumberNode();
		const auto simpleNodeNumValue = simpleNodeNum ? simpleNodeNum->value() : 0.0;
		const auto complexNode = leftIsSimple ? right : left;
		const auto complexLeftIsUnknown = getRuleForNode(complexNode->m_left) == NodeRule::UnknownVar;
		const auto complexNumNode = complexLeftIsUnknown ? complexNode->m_right : complexNode->m_left;
		const auto complexUnkNode = complexLeftIsUnknown ? complexNode->m_left : complexNode->m_right;
		const auto complexNodeNum = getExpressionNode(complexNumNode)->castToNumberNode();
		const auto complexNodeNumValue = complexNodeNum ? complexNodeNum->value() : 0.0;
		const auto complexOpIsAdd = getExpressionNode(complexNode)->castToOperatorNode()->type() == OperatorType::Addition;

		switch (complexRule) {
		case UnknownAndNumberAddSub: {
			if (simpleRuleIsNumber) {
				double resNum;
				if (!complexOpIsAdd) {
					resNum = -1 * complexNodeNumValue - simpleNodeNumValue;
				} else {
					resNum = complexNodeNumValue - simpleNodeNumValue;
				}

				if (resNum == 0.0) {
					node = complexUnkNode;
					break;
				}

				const auto newNumNode = allocateNode(std::make_shared<NumberNode>(resNum));
				node->m_left = complexUnkNode;
				node->m_right = newNumNode;

				break;
			}

			if (complexOpIsAdd && !complexLeftIsUnknown) {
				node->m_keyValue.first = std::make_shared<NumberNode>(complexNodeNumValue);
				resetLeftRight();
			} else {
				auto newNode = allocateNode(std::make_shared<OperatorNode>(OperatorType::Multiplication));
				newNode->m_left = allocateNode(std::make_shared<NumberNode>(-2.0));
				newNode->m_right = complexUnkNode;

				node->m_left = newNode;
				node->m_right = complexNumNode;
			}

			break;
		}
		case UnknownAndNumberMul: {
			if (!simpleRuleIsNumber) {
				const auto newNum = complexNodeNumValue - 1;
				node->m_keyValue.first = std::make_shared<OperatorNode>(OperatorType::Multiplication);
				node->m_left = complexUnkNode;
				node->m_right = allocateNode(std::make_shared<NumberNode>(newNum));
			}

			break;
		}
		case UnknownAndNumberDivMod:
		case UnknownAndNumberPow: {
			if (simpleNodeNumValue == 0.0) {
				node = complexNode;
			}

			break;
		}
		default: break;
		}
	}

	switchLeftRightIfNumberOnRight(node);
	return node;
}

EBST::NodePtr EBST::simplifyMultiplication(NodePtr& node) const {
	auto& left = node->m_left;
	auto& right = node->m_right;

	const auto leftRule = getRuleForSubtree(left);
	const auto rightRule = getRuleForSubtree(right);

	const auto isSimpleRule = [](NodeRule rule) {
		return rule == NodeRule::UnknownVar || rule == NodeRule::NumberVar;
	};

	if (isSimpleRule(leftRule) || isSimpleRule(rightRule)) {
		const auto leftIsSimple = isSimpleRule(leftRule);
		const auto simpleRule = leftIsSimple ? leftRule : rightRule;
		const auto simpleRuleIsNumber = simpleRule == NodeRule::NumberVar;
		const auto complexRule = leftIsSimple ? rightRule : leftRule;

		const auto simpleNode = leftIsSimple ? left : right;
		const auto simpleNodeNum = getExpressionNode(simpleNode)->castToNumberNode();
		const auto simpleNodeNumValue = simpleNodeNum ? simpleNodeNum->value() : 0.0;
		const auto complexNode = leftIsSimple ? right : left;
		const auto complexLeftIsUnknown = getRuleForNode(complexNode->m_left) == NodeRule::UnknownVar;
		const auto complexNumNode = complexLeftIsUnknown ? complexNode->m_right : complexNode->m_left;
		const auto complexUnkNode = complexLeftIsUnknown ? complexNode->m_left : complexNode->m_right;
		const auto complexNodeNum = getExpressionNode(complexNumNode)->castToNumberNode();
		const auto complexNodeNumValue = complexNodeNum ? complexNodeNum->value() : 0.0;

		if (simpleRuleIsNumber && simpleNodeNumValue == 0.0) {
			return allocateNode(std::make_shared<NumberNode>(0.0));
		}

		switch (complexRule) {
		case UnknownAndNumberAddSub: {
			if (simpleRuleIsNumber) {
				node = complexNode;

				auto newUnknNode = allocateNode(std::make_shared<OperatorNode>(OperatorType::Multiplication));
				newUnknNode->m_left = complexUnkNode;
				newUnknNode->m_right = allocateNode(std::make_shared<NumberNode>(simpleNodeNumValue));

				auto newNumNode = allocateNode(std::make_shared<NumberNode>(simpleNodeNumValue * complexNodeNumValue));

				node->m_left = newUnknNode;
				node->m_right = newNumNode;

				break;
			}

			node = complexNode;

			auto newUnknPowNode = allocateNode(std::make_shared<OperatorNode>(OperatorType::Power));
			newUnknPowNode->m_left = complexUnkNode;
			newUnknPowNode->m_right = allocateNode(std::make_shared<NumberNode>(2.0));

			auto newUnknNode = allocateNode(std::make_shared<OperatorNode>(OperatorType::Multiplication));
			newUnknNode->m_left = allocateNode(std::make_shared<UnknownNode>(getExpressionNode(complexUnkNode)->castToUnknownNode()->name()));
			newUnknNode->m_right = allocateNode(std::make_shared<NumberNode>(complexNodeNumValue));

			node->m_left = newUnknPowNode;
			node->m_right = newUnknNode;

			break;
		}
		case UnknownAndNumberMul: {
			if (simpleRuleIsNumber) {
				node = complexNode;
				complexNumNode->m_keyValue.first = std::make_shared<NumberNode>(simpleNodeNumValue * complexNodeNumValue);

				break;
			}

			node = complexNode;

			auto newUnknPowNode = allocateNode(std::make_shared<OperatorNode>(OperatorType::Power));
			newUnknPowNode->m_left = complexUnkNode;
			newUnknPowNode->m_right = allocateNode(std::make_shared<NumberNode>(2.0));

			node->m_left = newUnknPowNode;
			node->m_right = complexNumNode;

			break;
		}
		case UnknownAndNumberPow: {
			if (!simpleRuleIsNumber) {
				node = complexNode;
				complexNumNode->m_keyValue.first = std::make_shared<NumberNode>(complexNodeNumValue + 1);
			}

			break;
		}
		default: break;
		}
	}

	switchLeftRightIfNumberOnRight(node);
	return node;
}

EBST::NodePtr EBST::simplifyDivision(NodePtr& node) const {
	auto& left = node->m_left;
	auto& right = node->m_right;

	const auto leftRule = getRuleForSubtree(left);
	const auto rightRule = getRuleForSubtree(right);

	const auto isSimpleRule = [](NodeRule rule) {
		return rule == NodeRule::UnknownVar || rule == NodeRule::NumberVar;
	};

	if (isSimpleRule(leftRule) || isSimpleRule(rightRule)) {
		const auto leftIsSimple = isSimpleRule(leftRule);
		const auto simpleRule = leftIsSimple ? leftRule : rightRule;
		const auto simpleRuleIsNumber = simpleRule == NodeRule::NumberVar;

		const auto simpleNode = leftIsSimple ? left : right;
		const auto simpleNodeNum = getExpressionNode(simpleNode)->castToNumberNode();
		const auto simpleNodeNumValue = simpleNodeNum ? simpleNodeNum->value() : 0.0;

		if (!leftIsSimple && simpleRuleIsNumber && simpleNodeNumValue == 0.0) {
			throw ExpressionException(ExpressionError::DivisionByZero, 0);
		}
	}

	if (rightRule != NodeRule::NumberVar) {
		throw ExpressionException(ExpressionError::TooComplexDivision, 0);
	}

	return node;
}

EBST::NodePtr EBST::simplifyPower(NodePtr& node) const {
	const auto rightRule = getRuleForSubtree(node->m_right);
	if (rightRule != NodeRule::NumberVar) {
		throw ExpressionException(ExpressionError::TooComplexDegree, 0);
	}

	return node;
}

EBST::NodePtr EBST::simplifyTwoNumbers(const NodePtr& node, const AbstractExpressionNode::Ptr& leftExp, const AbstractExpressionNode::Ptr& rightExp) const {
	const auto opNode = getExpressionNode(node)->castToOperatorNode();
	const auto leftNode = leftExp->castToNumberNode();
	const auto rightNode = rightExp->castToNumberNode();
	assert(opNode && leftNode && rightNode);

	switch (opNode->type()) {
	case OperatorType::Substitution: return subtract(leftNode, rightNode);
	case OperatorType::Addition: return add(leftNode, rightNode);
	case OperatorType::Multiplication: return multiply(leftNode, rightNode);
	case OperatorType::Division: return divide(leftNode, rightNode);
	case OperatorType::Modulo: return modulo(leftNode, rightNode);
	case OperatorType::Power: return power(leftNode, rightNode);
	default: assert(false && "invalid operator");
	}

	return NodePtr();
}

EBST::NodePtr EBST::simplifyOperatorAndNumber(NodePtr& node, const AbstractExpressionNode::Ptr& num, bool leftIsOp) const {
	const auto opNode = getExpressionNode(node)->castToOperatorNode();
	const auto numNode = num->castToNumberNode();

	const auto numberIsZero = numNode->value() == 0.0;
	const auto numberIsOne = numNode->value() == 1.0;

	auto& left = node->m_left;
	auto& right = node->m_right;

	const auto rightRule = getRuleForNode(right);
	const auto rightIsNumber = rightRule == NodeRule::NumberVar;

	const auto resetLeftRight = [&left, &right] {
		left.reset();
		right.reset();
	};

	switch (opNode->type()) {
	case OperatorType::Addition:
	case OperatorType::Substitution: {
		if (numberIsZero) {
			node = leftIsOp ? node->m_left : node->m_right;
		}

		switchLeftRightIfNumberOnRight(node);
		return node;
	}
	case OperatorType::Multiplication: {
		if (numberIsZero) {
			resetLeftRight();
			node->m_keyValue.first = std::make_shared<NumberNode>(0.0);
		} else if (numberIsOne) {
			node = leftIsOp ? left : right;
		}

		switchLeftRightIfNumberOnRight(node);
		return node;
	}
	case OperatorType::Division:
	case OperatorType::Modulo: {
		if (numberIsZero) {
			if (leftIsOp) {
				throw ExpressionException(ExpressionError::DivisionByZero, 0);
			} else {
				resetLeftRight();
				node->m_keyValue.first = std::make_shared<NumberNode>(0.0);
			}
		} else if (numberIsOne && leftIsOp) {
			node = leftIsOp ? left : right;
		} else if (!rightIsNumber) {
			throw ExpressionException(ExpressionError::TooComplexDivision, 0);
		}
		return node;
	}
	case OperatorType::Power: {
		if (numberIsZero) {
			if (leftIsOp) {
				resetLeftRight();
				node->m_keyValue.first = std::make_shared<NumberNode>(1.0);
			} else {
				resetLeftRight();
				node->m_keyValue.first = std::make_shared<NumberNode>(0.0);
			}
		} else if (numberIsOne) {
			if (leftIsOp) {
				node = leftIsOp ? left : right;
			} else {
				resetLeftRight();
				node->m_keyValue.first = std::make_shared<NumberNode>(1.0);
			}
		} else if (!rightIsNumber) {
			throw ExpressionException(ExpressionError::TooComplexDegree, 0);
		}
		return node;
	}
	default: assert(false && "invalid operator");
	}

	return node;
}

EBST::NodePtr EBST::simplifySubTreeWithUnknowns(const NodePtr& ptr) const {
	auto& left = ptr->m_left;
	auto& right = ptr->m_right;

	const auto parentExpr = getExpressionNode(ptr);
	const auto leftExpr = getExpressionNode(left);
	const auto rightExpr = getExpressionNode(right);
	const auto leftUnknown = leftExpr->castToUnknownNode();
	const auto rightUnknown = rightExpr->castToUnknownNode();
	const auto leftAndRightUnknown = leftUnknown && rightUnknown;
	const auto leftUnknownRightZero = leftUnknown && !rightUnknown && rightExpr->castToNumberNode()->value() == 0.0;
	const auto rightUnknownLeftZero = rightUnknown && !leftUnknown && leftExpr->castToNumberNode()->value() == 0.0;
	const auto leftUnknownRightOne = leftUnknown && !rightUnknown && rightExpr->castToNumberNode()->value() == 1.0;
	const auto rightUnknownLeftOne = rightUnknown && !leftUnknown && leftExpr->castToNumberNode()->value() == 1.0;

	const auto rightRule = getRuleForNode(ptr->m_right);
	const auto rightIsNumber = rightRule == NodeRule::NumberVar;

	const auto resetLeftRight = [&left, &right] {
		left.reset();
		right.reset();
	};

	switch (parentExpr->castToOperatorNode()->type()) {
	case OperatorType::Substitution: {
		if (leftAndRightUnknown) {
			resetLeftRight();
			ptr->m_keyValue.first = std::make_shared<NumberNode>(0.0);
		} else if (leftUnknownRightZero) {
			resetLeftRight();
			ptr->m_keyValue.first = leftExpr;
		} else if (rightUnknownLeftZero) {
			ptr->m_keyValue.first = std::make_shared<OperatorNode>(OperatorType::Multiplication);
			left->m_keyValue.first = std::make_shared<NumberNode>(-1.0);
			right->m_keyValue.first = rightExpr;
		}

		switchLeftRightIfNumberOnRight(ptr);
		break;
	}
	case OperatorType::Addition: {
		if (leftAndRightUnknown) {
			ptr->m_keyValue.first = std::make_shared<OperatorNode>(OperatorType::Multiplication);
			left->m_keyValue.first = std::make_shared<NumberNode>(2.0);
		} else if (leftUnknownRightZero || rightUnknownLeftZero) {
			resetLeftRight();
			ptr->m_keyValue.first = leftUnknown ? leftExpr : rightExpr;
		}

		switchLeftRightIfNumberOnRight(ptr);
		break;
	}
	case OperatorType::Multiplication: {
		if (leftAndRightUnknown) {
			ptr->m_keyValue.first = std::make_shared<OperatorNode>(OperatorType::Power);
			right->m_keyValue.first = std::make_shared<NumberNode>(2.0);
		} else if (leftUnknownRightZero || rightUnknownLeftZero) {
			resetLeftRight();
			ptr->m_keyValue.first = std::make_shared<NumberNode>(0.0);
		} else if (leftUnknownRightOne) {
			ptr->m_keyValue.first = leftExpr;
			resetLeftRight();
		} else if (rightUnknownLeftOne) {
			ptr->m_keyValue.first = rightExpr;
			resetLeftRight();
		}

		switchLeftRightIfNumberOnRight(ptr);
		break;
	}
	case OperatorType::Division:
	case OperatorType::Modulo: {
		if (!rightIsNumber) {
			throw ExpressionException(ExpressionError::TooComplexDivision, 0);
		} else if (leftAndRightUnknown) {
			resetLeftRight();
			ptr->m_keyValue.first = std::make_shared<NumberNode>(1.0);
		} else if (leftUnknownRightZero) {
			throw ExpressionException(ExpressionError::DivisionByZero, 0);
		} else if (rightUnknownLeftZero) {
			resetLeftRight();
			ptr->m_keyValue.first = std::make_shared<NumberNode>(0.0);
		} else if (leftUnknownRightOne) {
			ptr->m_keyValue.first = leftExpr;
			resetLeftRight();
		}
		break;
	}
	case OperatorType::Power: {
		if (!rightIsNumber) {
			throw ExpressionException(ExpressionError::TooComplexDegree, 0);
		} else if (leftAndRightUnknown) {
			break;
		} else if (leftUnknownRightZero) {
			resetLeftRight();
			ptr->m_keyValue.first = std::make_shared<NumberNode>(1.0);
		} else if (rightUnknownLeftZero) {
			resetLeftRight();
			ptr->m_keyValue.first = std::make_shared<NumberNode>(0.0);
		} else if (leftUnknownRightOne) {
			ptr->m_keyValue.first = leftExpr;
			resetLeftRight();
		} else if (rightUnknownLeftOne) {
			resetLeftRight();
			ptr->m_keyValue.first = std::make_shared<NumberNode>(1.0);
		}
		break;
	}
	default: assert(false && "invalid operator");
	}

	return ptr;
}

void EBST::switchLeftRightIfNumberOnRight(const NodePtr& ptr) const {
	if (!nodeHasChildren(ptr)) {
		return;
	}

	switchLeftRightIfNumberOnRight(ptr->m_left);
	switchLeftRightIfNumberOnRight(ptr->m_right);

	const auto rightRule = getRuleForNode(ptr->m_right);
	const auto leftRule = getRuleForNode(ptr->m_left);
	const auto rightIsNumber = rightRule == NodeRule::NumberVar;
	const auto leftIsNumber = leftRule == NodeRule::NumberVar;
	const auto rootRule = getRuleForNode(ptr);
	const auto ptrNode = getExpressionNode(ptr);
	const auto leftIsOpAndRootMul = ptrNode->castToOperatorNode() && rootRule == NodeRule::Multiplication;
	const auto rootNotValid = rootRule == NodeRule::Power || rootRule == NodeRule::DivisionModulo || rootRule == NodeRule::AdditionSubstitution;

	if (rightIsNumber
	    && (!leftIsNumber || leftIsOpAndRootMul)
	    && !rootNotValid) {
		auto r = ptr->m_right;
		auto l = ptr->m_left;
		ptr->m_left = r;
		ptr->m_right = l;
	}
}
