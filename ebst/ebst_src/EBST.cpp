#include "EBST.h"

#include "ExpressionException.h"

#include <cctype>
#include <stack>
#include <exception>
#include <algorithm>
#include <set>

EBST::EBST(const std::string& expressionString) {
	const auto exprVec = parseExpression(expressionString);
	m_rootNode = buildTree(exprVec);
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

	if (m_rootNode) {
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

	return output;
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

EBST::NodePtr EBST::buildTree(const std::vector<ExpressionNode>& expr) {
    std::stack<EBST::NodePtr> stack;
	NodePtr t;
	NodePtr root;

    for (const auto& node : expr) {
        auto pair = std::make_pair(node, false);
        t = std::make_shared<EBST::Node>(pair);
        if (node.type() == ExpressionType::Operator) {
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

std::vector<ExpressionNode> EBST::parseExpression(const std::string& expr, int preExprLength) {
    std::vector<ExpressionNode> output;
    std::stack<ExpressionNode> stack;
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
	                        &getErrorColumn] (auto& it, const std::optional<ExpressionNode>& lastExpr) -> std::optional<ExpressionNode> {
        auto dotFound = false;

        if (std::isdigit(*it) || *it == '-' || *it == '+') {
            auto begin = it;

			if (lastExpr.has_value() && *it == '-'
			    && (lastExpr.value().operatorType() == OperatorType::Substitution || lastExpr.value().operatorType() == OperatorType::Addition)) {
				auto lastOp = lastExpr.value().operatorType();

				if (lastOp == OperatorType::Substitution) {
					stack.pop();
					stack.push(ExpressionNode(OperatorType::Addition));
					++begin;
				} else if (lastOp == OperatorType::Addition) {
					stack.pop();
					stack.push(ExpressionNode(OperatorType::Substitution));
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

            return parseOperandNodeFromString(strNum);
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

			return parseOperandNodeFromString({ ch });
        }

		throw ExpressionException(ExpressionError::InvalidToken, getErrorColumn(castedDistance(expr.cbegin(), it)));
        return std::nullopt;
    };

    const auto nextTokenIsNumber = [&expr](auto it) {
        const auto next = std::next(it, 1);
        return next != expr.cend() && std::isdigit(*next);
    };

	const auto nextTokenIsAlpha = [&expr](auto it) {
		const auto next = std::next(it, 1);
		return next != expr.cend() && std::isalpha(*next);
	};

	const auto expressionNodeIsOperator = [](const std::optional<ExpressionNode>& op) {
		return (op.has_value() && op.value().type() == ExpressionType::Operator) || !op.has_value();
    };

    std::optional<ExpressionNode> lastExpressionNode;
    for (auto it = expr.cbegin(); it < expr.cend(); ++it) {
        const auto c = *it;

		if (c < -1) {
			throw ExpressionException(ExpressionError::InvalidToken, getErrorColumn(castedDistance(expr.cbegin(), it)));
		}

        if (std::isspace(c)) {
            continue;
        }

        auto pOp = parseOperatorNodeFromChar(c);

		const auto nextTokenIsNumberAndPrevIsOperator = nextTokenIsNumber(it)
		                                                && expressionNodeIsOperator(lastExpressionNode);

        if (pOp.has_value() && !isBracket(pOp.value())
		    && !nextTokenIsNumberAndPrevIsOperator) {

			if (!lastExpressionNode.has_value() && nextTokenIsAlpha(it)) {
				throw ExpressionException(ExpressionError::InvalidToken, getErrorColumn(castedDistance(expr.cbegin(), it)));
			}

			if (expressionNodeIsOperator(lastExpressionNode) && !isBracket(lastExpressionNode.value())) {
				throw ExpressionException(ExpressionError::OperatorAfterOperator, getErrorColumn(castedDistance(expr.cbegin(), it)));
			}

			auto op = pOp.value();

            if (!stack.empty()) {
                auto top = stack.top();

                while (!isBracket(top) && (op <= top)) {
                    stack.pop();
                    output.push_back(top);

                    if (!stack.empty()) {
                        top = stack.top();
                    } else {
                        break;
                    }
                }
            }

            stack.push(op);
        } else if (c == '(') {
            if (pOp.has_value()) {
                stack.push(pOp.value());
            } else {
				throw ExpressionException(ExpressionError::LeftBracketError, getErrorColumn(castedDistance(expr.cbegin(), it)));
            }
        } else if (c == ')') {
            auto top = stack.top();
            while (stack.top().operatorType() != OperatorType::BracketLeft) {
                output.push_back(top);
                stack.pop();
                if (stack.empty()) {
					throw ExpressionException(ExpressionError::MissingLeftParentheses, getErrorColumn(castedDistance(expr.cbegin(), it)));
                }
                top = stack.top();
            }

            if (!stack.empty() && stack.top().operatorType() == OperatorType::BracketLeft) {
                stack.pop();
            }
        } else {
			pOp = readToken(it, lastExpressionNode);
            if (pOp.has_value()) {
                output.push_back(pOp.value());
            } else {
				throw ExpressionException(ExpressionError::InvalidToken, getErrorColumn(castedDistance(expr.cbegin(), it)));
            }

			if (lastExpressionNode.has_value() && lastExpressionNode.value().type() == ExpressionType::Operand) {
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

		if (currentOperandName != invalidOperandVarName && currentOperandName != operandName) {
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

void EBST::insert(const ExpressionNode&, const bool &) {}

EBST::NodePtr EBST::insert(EBST::NodePtr&, const AbstractBST::KVPair&) {
    return EBST::NodePtr();
}

bool EBST::remove(const ExpressionNode&) {
    return false;
}

EBST::NodePtr EBST::remove(EBST::NodePtr&, const ExpressionNode&) {
    return EBST::NodePtr();
}

EBST::NodePtr EBST::find(const EBST::NodePtr&, const ExpressionNode&) const {
	return EBST::NodePtr();
}

EBST::iterator EBST::find(const ExpressionNode&) const {
	return iterator();
}

EBST::iterator EBST::begin() const {
	return AbstractBaseTree::begin();
}

EBST::Node::Node(const KVPair& keyValue) : AbstractNode(keyValue) {}

EBST::NodePtr EBST::Node::next() const {
    return NodePtr();
}
