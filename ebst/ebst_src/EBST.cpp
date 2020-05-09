#include "EBST.h"

#include "ExpressionException.h"

#include <cctype>
#include <stack>
#include <exception>
#include <algorithm>
#include <set>

namespace {

const auto equalSign = '=';
const auto equalToZero = " = 0";

std::vector<std::string> splitStringByEqualSign(const std::string& s) {
	auto next = 0U;
	auto last = 0U;
	std::vector<std::string> vec;

	while ((next = s.find(equalSign, last)) != std::string::npos) {
		vec.push_back(s.substr(last, next - last));
		last = next + 1;
	}
	vec.push_back(s.substr(last));

	return vec;
}

} // end anonymous namespace

EBST::EBST(const std::string& expressionString, bool containsEqualSign) : m_containsEqualSign(containsEqualSign) {
	if (m_containsEqualSign) {
		auto i = 0;
		for (const auto c : expressionString) {
			if (c == equalSign) {
				++i;
			}
		}

		if (i == 0) {
			throw ExpressionException(ExpressionError::NoEqualSign, 0);
		}

		if (i > 1) {
			throw ExpressionException(ExpressionError::TooManyEqualSigns, 0);
		}

		const auto exprVec = splitStringByEqualSign(expressionString);
		assert(exprVec.size() == 2);

		auto leftExp = parseExpression(exprVec[0]);
		auto leftRoot = buildTree(leftExp);

		auto rightExp = parseExpression(exprVec[1], exprVec[0].size() + 1);
		auto rightRoot = buildTree(rightExp);

		auto leftReducedTreeRootNode = buildReducedFormTree(leftRoot);
		SubtreesByDegree leftSubtrees;
		auto leftBalancedTreeRootNode = buildBalancedTree(leftReducedTreeRootNode, leftSubtrees);

		auto rightReducedTreeRootNode = buildReducedFormTree(rightRoot);
		SubtreesByDegree rightSubtrees;
		auto rightBalancedTreeRootNode = buildBalancedTree(rightReducedTreeRootNode, rightSubtrees);

		// mirror +- for right side
		for (auto& pair : rightSubtrees) {
			for (auto& subtree : pair.second) {
				mirrorNodeSign(subtree);
			}
		}

		// merge two maps with vectors
		auto first = rightSubtrees.begin();
		auto last = rightSubtrees.end();
		for (; first != last; ++first) {
			auto ins = leftSubtrees.insert(*first);
			if (!ins.second) {
				auto& rhsVec = first->second;
				auto& lhsVec = ins.first->second;
				lhsVec.insert(lhsVec.end(), rhsVec.begin(), rhsVec.end());
			}
		}

		const auto spreadedSubtrees = spreadSubtrees(leftSubtrees);
		m_rootNode = buildTreeFromVectorOfNodes(spreadedSubtrees);
	} else {
		auto expVec = parseExpression(expressionString);
		m_rootNode = buildTree(expVec);
	}

	m_reducedTreeRootNode = buildReducedFormTree(m_rootNode);
	m_balancedTreeRootNode = buildBalancedTree(m_reducedTreeRootNode, m_degreeSubtrees);

	if (!treeIsBalanced()) {
		// I don't think it actually will balance more, but who knows
		m_reducedTreeRootNode = buildReducedFormTree(m_balancedTreeRootNode);
		m_balancedTreeRootNode = buildBalancedTree(m_reducedTreeRootNode, m_degreeSubtrees);

		if (!treeIsBalanced()) {
			throw ExpressionException(ExpressionError::CannotBalance, 0);
		}
	}

	if (m_rootNode && m_containsEqualSign) {
		solveExpression();
	}
}

std::string EBST::toString(OutputType type) const {
	std::string output;

    switch (type) {
    case OutputType::InfixWithParentheses:
	case OutputType::Infix: output = outputInfix(m_balancedTreeRootNode, type == OutputType::InfixWithParentheses); break;
	case OutputType::Postfix: output = outputPostfix(m_balancedTreeRootNode); break;
	case OutputType::Prefix: output = outputPrefix(m_balancedTreeRootNode); break;
	}

	const auto suffix = m_containsEqualSign ? equalToZero : "";
	return output + suffix;
}

int EBST::maxDegree() const {
	return m_maxDegree;
}

std::string EBST::unknownOperandName() const {
	return m_unknownOperandName;
}

ExpressionSolution EBST::solution() const {
	return m_solution;
}

EBST::NodePtr EBST::buildTree(const std::vector<AbstractExpressionNode::Ptr>& expr) {
    std::stack<EBST::NodePtr> stack;
	NodePtr t;
	NodePtr root;

    for (const auto& node : expr) {
        auto pair = std::make_pair(node, false);
        t = std::make_shared<EBST::Node>(pair);
		if (node->castToOperatorNode()) {
            auto t1 = stack.top();
            stack.pop();
            auto t2 = stack.top();
            stack.pop();

            t->m_left = t2;
            t->m_right = t1;
        }

        stack.push(t);
    }

	root = stack.top();
    stack.pop();

	return root;
}

std::vector<AbstractExpressionNode::Ptr> EBST::parseExpression(const std::string& expr, int preExprLength) {
	std::vector<AbstractExpressionNode::Ptr> output;
	std::stack<AbstractExpressionNode::Ptr> stack;
	std::set<char> unknownOperands;

	const auto getErrorColumn = [preExprLength](int currCol) {
		return preExprLength + currCol;
	};

	const auto castedDistance = [](const auto& begin, const auto& end) {
		return static_cast<int>(std::distance(begin, end));
	};

	const auto readToken = [&expr,
	                        &unknownOperands,
	                        &stack,
	                        &castedDistance,
	                        &getErrorColumn] (auto& it, const AbstractExpressionNode::Ptr& lastExpr) -> AbstractExpressionNode::Ptr {
        auto dotFound = false;

        if (std::isdigit(*it) || *it == '-' || *it == '+') {
            auto begin = it;

			const auto lastOperator = lastExpr->castToOperatorNode();
			if (lastExpr && *it == '-' && lastOperator
			    && (lastOperator->type() == OperatorType::Substitution || lastOperator->type() == OperatorType::Addition)) {
				auto lastOpValue = lastOperator->type();

				if (lastOpValue == OperatorType::Substitution) {
					stack.pop();
					stack.push(std::make_shared<OperatorNode>(OperatorType::Addition));
					++begin;
				} else if (lastOpValue == OperatorType::Addition) {
					stack.pop();
					stack.push(std::make_shared<OperatorNode>(OperatorType::Substitution));
					++begin;
				}
			}

            ++it;
            while (it != expr.cend() && (std::isdigit(*it) || (*it == '.' && !dotFound))) {
                if (*it == '.') {
                    dotFound = true;
                }

                ++it;
            }

            const auto strNum = std::string(begin, it);
            --it;

			return parseExpressionFromString(strNum);
        } else if (std::isalpha(*it)) {
            auto begin = it;
            ++it;
            while (it != expr.cend() && (std::isalpha(*it))) {
                ++it;
            }

            const auto strVal = std::string(begin, it);
            --it;

            if (strVal.length() > 1) {
				throw ExpressionException(ExpressionError::UnknownOperandSize, getErrorColumn(castedDistance(expr.cbegin(), it)));
            }

			const auto ch = static_cast<char>(std::tolower(strVal.front()));
			unknownOperands.insert(ch);
			if (unknownOperands.size() > 1) {
				throw ExpressionException(ExpressionError::MultipleUnknownOperands, getErrorColumn(castedDistance(expr.cbegin(), it)));
			}

			return parseExpressionFromString({ ch });
        }

		throw ExpressionException(ExpressionError::InvalidToken, getErrorColumn(castedDistance(expr.cbegin(), it)));
		return nullptr;
    };

    const auto nextTokenIsNumber = [&expr](auto it) {
        const auto next = std::next(it, 1);
        return next != expr.cend() && std::isdigit(*next);
    };

	const auto nextTokenIsAlpha = [&expr](auto it) {
		const auto next = std::next(it, 1);
		return next != expr.cend() && std::isalpha(*next);
	};

	const auto expressionIsOperator = [](const AbstractExpressionNode::Ptr& node) {
		return (node && node->castToOperatorNode()) || !node;
    };

	AbstractExpressionNode::Ptr lastExpressionNode;
    for (auto it = expr.cbegin(); it < expr.cend(); ++it) {
        const auto c = *it;

		if (c < -1) {
			throw ExpressionException(ExpressionError::InvalidToken, getErrorColumn(castedDistance(expr.cbegin(), it)));
		}

        if (std::isspace(c)) {
            continue;
        }

		auto pOp = parseExpressionFromChar(c);

		const auto nextTokenIsNumberAndPrevIsOperator = nextTokenIsNumber(it) && expressionIsOperator(lastExpressionNode);

		if (pOp && !isBracket(pOp)
		    && !nextTokenIsNumberAndPrevIsOperator) {

			if (!lastExpressionNode && nextTokenIsAlpha(it)) {
				throw ExpressionException(ExpressionError::InvalidToken, getErrorColumn(castedDistance(expr.cbegin(), it)));
			}

			if (expressionIsOperator(lastExpressionNode) && !isBracket(lastExpressionNode)) {
				throw ExpressionException(ExpressionError::OperatorAfterOperator, getErrorColumn(castedDistance(expr.cbegin(), it)));
			}

            if (!stack.empty()) {
                auto top = stack.top();

				while (!isBracket(top) && (pOp->castToOperatorNode()->precedence() <= top->castToOperatorNode()->precedence())) {
                    stack.pop();
                    output.push_back(top);

                    if (!stack.empty()) {
                        top = stack.top();
                    } else {
                        break;
                    }
                }
            }

			stack.push(pOp);
        } else if (c == '(') {
			if (pOp) {
				stack.push(pOp);
            } else {
				throw ExpressionException(ExpressionError::LeftBracketError, getErrorColumn(castedDistance(expr.cbegin(), it)));
            }
        } else if (c == ')') {
            auto top = stack.top();
			while (stack.top()->castToOperatorNode()->type() != OperatorType::BracketLeft) {
                output.push_back(top);
                stack.pop();
                if (stack.empty()) {
					throw ExpressionException(ExpressionError::MissingLeftParentheses, getErrorColumn(castedDistance(expr.cbegin(), it)));
                }
                top = stack.top();
            }

			if (!stack.empty() && stack.top()->castToOperatorNode()->type() == OperatorType::BracketLeft) {
                stack.pop();
            }
        } else {
			pOp = readToken(it, lastExpressionNode);
			if (pOp) {
				output.push_back(pOp);
            } else {
				throw ExpressionException(ExpressionError::InvalidToken, getErrorColumn(castedDistance(expr.cbegin(), it)));
            }

			if (lastExpressionNode
			    && (lastExpressionNode->castToUnknownNode()
			    || lastExpressionNode->castToNumberNode()
			    || lastExpressionNode->castToImaginaryNumberNode())) {
				throw ExpressionException(ExpressionError::MissingOperator, getErrorColumn(castedDistance(expr.cbegin(), it)));
			}
        }

		lastExpressionNode = pOp;
    }

    while (!stack.empty()) {
        auto rToken = stack.top();
		if (isBracket(rToken)) {
			throw ExpressionException(ExpressionError::MissingRightParentheses, getErrorColumn(castedDistance(expr.cbegin(), expr.cend())));
        }
        output.push_back(rToken);
        stack.pop();
    }

	assert(unknownOperands.size() <= 1 && "missed multiple unknown operands");
	if (unknownOperands.size() == 1) {
		const auto operandName = static_cast<char>(*unknownOperands.cbegin());
		const auto currentOperandName = m_unknownOperandName.front();

		if (currentOperandName != invalidUnknownNodeName && currentOperandName != operandName) {
			throw ExpressionException(ExpressionError::MultipleUnknownOperands, getErrorColumn(0));
		}

		m_unknownOperandName = operandName;
	}

	if (output.empty()) {
		throw ExpressionException(ExpressionError::InvalidToken, getErrorColumn(0));
	}

	return output;
}

EBST::NodeRule operator|(EBST::NodeRule a, EBST::NodeRule b) {
	auto lhsNum = static_cast<int>(a);
	auto rhsNum = static_cast<int>(b);
	return static_cast<EBST::NodeRule>(lhsNum | rhsNum);
}

std::string EBST::toString(const NodeRule rule) const {
	std::map<NodeRule, std::string> ruleToStringMap {
		{ NodeRule::Subtree, "Subtree" },
		{ NodeRule::UnknownVar, "UnknownVar" },
		{ NodeRule::NumberVar, "NumberVar" },
		{ NodeRule::Multiplication, "Multiplication" },
		{ NodeRule::AdditionSubstitution, "AdditionSubstitution" },
		{ NodeRule::DivisionModulo, "DivisionModulo" },
		{ NodeRule::Power, "Power" },
		{ NodeRule::NoRule, "NoRule" },
		{ NodeRule::UnknownAndNumberMul, "UnknownAndNumberMul" },
		{ NodeRule::UnknownAndNumberAddSub, "UnknownAndNumberAddSub" },
		{ NodeRule::UnknownAndNumberDivMod, "UnknownAndNumberDivMod" },
		{ NodeRule::UnknownAndNumberPow, "UnknownAndNumberPow" },
		{ NodeRule::UnknownAndSubtree, "UnknownAndSubtree" },
		{ NodeRule::NumberAndSubtree, "NumberAndSubtree" },
		{ NodeRule::UnknownAndSubtreeMul, "UnknownAndSubtreeMul" },
		{ NodeRule::UnknownAndSubtreeAddSub, "UnknownAndSubtreeAddSub" },
		{ NodeRule::NumberAndSubtreeMul, "NumberAndSubtreeMul" },
		{ NodeRule::NumberAndSubtreeAddSub, "NumberAndSubtreeAddSub" },
		{ NodeRule::Rule1, "Rule1" },
		{ NodeRule::Rule2, "Rule2" },
		{ NodeRule::Rule3, "Rule3" },
		{ NodeRule::Rule4, "Rule4" },
		{ NodeRule::Rule5, "Rule5" },
		{ NodeRule::Rule6, "Rule6" }
	};

	const auto it = ruleToStringMap.find(rule);
	if (it != ruleToStringMap.end()) {
		return ruleToStringMap.at(rule);
	}

	return {};
}

// unused stuff

void EBST::insert(const AbstractExpressionNode::Ptr&, const bool &) {}

EBST::NodePtr EBST::insert(EBST::NodePtr&, const AbstractBST::KVPair&) {
    return EBST::NodePtr();
}

bool EBST::remove(const AbstractExpressionNode::Ptr&) {
    return false;
}

EBST::NodePtr EBST::remove(EBST::NodePtr&, const AbstractExpressionNode::Ptr&) {
    return EBST::NodePtr();
}

EBST::NodePtr EBST::find(const EBST::NodePtr&, const AbstractExpressionNode::Ptr&) const {
	return EBST::NodePtr();
}

EBST::iterator EBST::find(const AbstractExpressionNode::Ptr&) const {
	return iterator();
}

EBST::iterator EBST::begin() const {
	return AbstractBaseTree::begin();
}

EBST::Node::Node(const KVPair& keyValue) : AbstractNode(keyValue) {}

EBST::NodePtr EBST::Node::next() const {
    return NodePtr();
}
