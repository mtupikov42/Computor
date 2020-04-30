#include "EBST.h"

#include <algorithm>

#include "ExpressionException.h"

EBST::NodePtr EBST::buildBalancedTree(const NodePtr& node, SubtreesByDegree& degreeSubtrees) {
	NodePtr balancedTree;
	if (node) {
		splitSubtreesByDegree(node, degreeSubtrees);
		const auto rootTreeVec = spreadSubtrees(degreeSubtrees);
		balancedTree = buildTreeFromVectorOfNodes(rootTreeVec, false, true);
	}

	m_isBalanced = true;

	// if all simplified to zero
	if (!balancedTree) {
		auto zero = allocateNode(ExpressionNode(0.0));
		degreeSubtrees[0].push_back({ zero, OperatorType::Addition, false });
		return zero;
	}

	splitSubtreesByDegree(balancedTree, degreeSubtrees);
	auto maxDegree = 0;
	for (const auto& pair : degreeSubtrees) {
		if (pair.second.size() != 1) {
			m_isBalanced = false;
		}

		maxDegree = std::max(maxDegree, pair.first);
	}

	m_maxDegree = maxDegree;

	return balancedTree;
}

void EBST::splitSubtreesByDegree(const NodePtr& root, SubtreesByDegree& degreeSubtrees) {
	degreeSubtrees.clear();
	distributeSubtrees(root, degreeSubtrees, OperatorType::Addition, OperatorType::Addition, false);
}

void EBST::distributeSubtrees(
    const NodePtr& node,
    SubtreesByDegree& degreeSubtrees,
    OperatorType parentOp,
    OperatorType subParentOp,
    bool isLeft
) {
	auto left = node->m_left;
	auto right = node->m_right;

	auto isAddSubOperator = [](const NodePtr& ptr) {
		const auto expr = ptr->m_keyValue.first;

		if (!isOperator(expr)) {
			return false;
		}

		const auto opType = expr.operatorType();
		return opType == OperatorType::Substitution || opType == OperatorType::Addition;
	};

	if (left && right) {
		const auto nodeIsAddSub = isAddSubOperator(node);

		if (!nodeIsAddSub) {
			auto resolvedOp = (parentOp == OperatorType::Substitution && !isLeft) ? parentOp : OperatorType::Addition;
			if (subParentOp == OperatorType::Substitution && isLeft) {
				resolvedOp = OperatorType::Substitution;
			}

			if (countUnknownVars(node) > 1) {
				if (countUnknownVars(left) > 1) {
					node->m_left = reduceNode(left);
				}

				if (countUnknownVars(right) > 1) {
					node->m_right = reduceNode(right);
				}

				auto reducedNode = reduceNode(node);
				if (!subTreesAreEqual(node, reducedNode)) {
					distributeSubtrees(reducedNode, degreeSubtrees, parentOp, subParentOp, isLeft);
					return;
				}
			}

			const auto subtreePower = getMaximumPowerOfSubtree(node);
			insertNodeIntoDegreeSubtreesMap(node, degreeSubtrees, subtreePower, resolvedOp, isLeft);
			return;
		}

		const auto nodeOp = node->m_keyValue.first.operatorType();

		distributeSubtrees(left, degreeSubtrees, nodeOp, parentOp, true);
		distributeSubtrees(right, degreeSubtrees, nodeOp, parentOp, false);
		return;
	}

	if (isOperator(node->m_keyValue.first)) {
		return;
	}

	const auto isUnknown = isOperandUnknown(node->m_keyValue.first.operandValue());

	if (!isUnknown && node->m_keyValue.first.operandValue().value == 0.0) {
		return;
	}

	const auto insertOp = isLeft ? OperatorType::Addition : parentOp;
	insertNodeIntoDegreeSubtreesMap(node, degreeSubtrees, isUnknown ? 1 : 0, insertOp, isLeft);
}

void EBST::insertNodeIntoDegreeSubtreesMap(
    const NodePtr& node,
    SubtreesByDegree& degreeSubtrees,
    int power,
    OperatorType type,
    bool isLeft
) {
	if (degreeSubtrees.count(power) == 0) {
		degreeSubtrees[power] = {};
	}

	auto& degreeVec = degreeSubtrees[power];
	degreeVec.push_back({ node, type, isLeft });
}

std::vector<EBST::SubtreeWithOperator> EBST::spreadSubtrees(SubtreesByDegree& degreeSubtrees) {
	const auto nodeIsNumberZero = [this](const NodePtr& node) {
		return getRuleForNode(node) == NumberVar && getExpressionNode(node).operandValue().value == 0.0;
	};

	auto countDeg = 0;
	std::vector<SubtreeWithOperator> rootTreeVec;
	for (auto& pair : degreeSubtrees) {
		const auto degree = pair.first;
		auto& vec = pair.second;
		std::partition(vec.begin(), vec.end(), [](const SubtreeWithOperator& s) {
			return s.isLeft && s.op == OperatorType::Addition;
		});
		auto degreeTree = uniteSubtrees(vec, degree, countDeg == 0);
		auto degTreeOp = vec.front().op;

		auto node = reduceNode(degreeTree);

		if (!nodeIsNumberZero(node)) {
			rootTreeVec.push_back({ node, degTreeOp, false });
			++countDeg;
		}
	}

	return rootTreeVec;
}

EBST::NodePtr EBST::uniteSubtrees(std::vector<SubtreeWithOperator>& vec, int degree, bool first) {
	auto resultValue = 0.0;
	auto count = 0;

	for (auto& sub : vec) {
		const auto node = sub.subtree;
		auto& op = sub.op;
		const auto nodeRule = getRuleForSubtree(node);
		double value = 0.0;

		switch (degree) {
		case 0: {
			value = node->m_keyValue.first.operandValue().value;
			break;
		}
		case 1: {
			if (nodeRule != NodeRule::UnknownVar) {
				if (getRuleForNode(node) == NodeRule::DivisionModulo
				    && getRuleForNode(node->m_left) == NodeRule::UnknownVar) {
					value = node->m_right->m_keyValue.first.operandValue().value;
					value = node->m_keyValue.first.operatorType() == OperatorType::Modulo
					        ? std::remainder(1.0, value)
					        : (1.0 / value);
				} else {
					value = node->m_left->m_keyValue.first.operandValue().value;
				}
			} else {
				value = 1.0;
			}
			break;
		}
		case 2:
		case 3: {
			if (nodeRule != NodeRule::UnknownAndNumberPow) {
				value = node->m_left->m_keyValue.first.operandValue().value;
			} else {
				value = 1.0;
			}
			break;
		}
		default: throw ExpressionException(ExpressionError::DegreeHigherThanThree, 0);
		}

		if (op == OperatorType::Substitution) {
			resultValue -= value;
		} else {
			resultValue += value;
		}

		count++;
	}

	if (resultValue < 0 && !first) {
		vec.front().op = OperatorType::Substitution;
		resultValue *= -1;
	}

	return createNodeByDegreeAndValue(resultValue, degree);
}

EBST::NodePtr EBST::buildTreeFromVectorOfNodes(
    const std::vector<SubtreeWithOperator>& vec,
    bool hasParentTree,
    bool isBalancedFirst
) const {
	for (auto it = vec.cbegin(); it < vec.cend();) {
		auto& subtree = it->subtree;

		auto next = std::next(it, 1);
		if (next != vec.cend()) {
			auto opNode = allocateNode(ExpressionNode(next->op));

			opNode->m_left = subtree;
			opNode->m_right = buildTreeFromVectorOfNodes(std::vector(next, vec.cend()), true);
			return opNode;
		}

		const auto nodeIsSimple = (nodeHasChildren(subtree)
		                          && !nodeHasChildren(subtree->m_left)
		                          && !nodeHasChildren(subtree->m_right))
		                          || !nodeHasChildren(subtree);

		if (!hasParentTree && it->op == OperatorType::Substitution && nodeIsSimple && isBalancedFirst && !it->isLeft) {
			auto op = allocateNode(ExpressionNode(OperatorType::Multiplication));
			op->m_left = allocateNode(ExpressionNode(-1.0));
			op->m_right = it->subtree;

			return op;
		}

		return it->subtree;
	}

	return {};
}

bool EBST::treeIsBalanced() const {
	return m_isBalanced;
}
