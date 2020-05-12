#pragma once

#include <vector>
#include <exception>
#include <complex>
#include <optional>
#include <map>

#include <AbstractBST.h>

#include "OperatorNode.h"
#include "NumberNode.h"
#include "ImaginaryNumberNode.h"
#include "UnknownNode.h"

struct ExpressionResult {
	std::string varName;
	std::string varResult;
};

struct ExpressionSolution {
	std::vector<ExpressionResult> solutions;
	std::optional<double> discriminant;
};

enum class ExpressionType {
	FullExpression,
	PartialExpression,
	OnlyNumericExpression
};

class EBST final : public AbstractBST<AbstractExpressionNode::Ptr, bool> {
public:
	using AbstractBaseTree = AbstractBST<AbstractExpressionNode::Ptr, bool>;
	using AbstractBaseTree::find;

	enum class OutputType {
		Infix,
        InfixWithParentheses,
        Postfix,
		Prefix
	};

	explicit EBST(const std::string& expressionString, ExpressionType type);

	std::string toString(OutputType type = OutputType::Infix) const;
	int maxDegree() const;
	std::string unknownOperandName() const;
	ExpressionSolution solution() const;

private:
	struct Node final : public AbstractBaseTree::AbstractNode {
		Node(const typename AbstractBaseTree::KVPair& keyValue);

		typename AbstractBaseTree::AbstractNode::Ptr next() const override;
	};
	using NodePtr = typename Node::Ptr;

	struct SubtreeWithOperator {
		NodePtr subtree;
		OperatorType op = OperatorType::Invalid;
		bool isLeft = false;
	};

	enum NodeRule {
		Subtree = 1 << 0, // A
		UnknownVar = 1 << 1, // x
		NumberVar = 1 << 2, // n
		Multiplication = 1 << 3, // *
		AdditionSubstitution = 1 << 4, // +-
		DivisionModulo = 1 << 5, // /%
		Power = 1 << 6, // ^
		ImaginaryVar = 1 << 7, // i

		NoRule = 1 << 10,

		UnknownAndNumberMul = UnknownVar | NumberVar | Multiplication, // (x * n)
		UnknownAndNumberAddSub = UnknownVar | NumberVar | AdditionSubstitution, // (x +- n)
		UnknownAndNumberDivMod = UnknownVar | NumberVar | DivisionModulo, // (x /% n)
		UnknownAndNumberPow = UnknownVar | NumberVar | Power, // (x ^ n)

		UnknownAndSubtree = UnknownVar | Subtree, // (x ? A)
		NumberAndSubtree = NumberVar | Subtree, // (n ? A)

		UnknownAndSubtreeMul = UnknownAndSubtree | Multiplication, // (x * A)
		UnknownAndSubtreeAddSub = UnknownAndSubtree | AdditionSubstitution, // (x +- A)
		NumberAndSubtreeMul = NumberAndSubtree | Multiplication, // (n * A)
		NumberAndSubtreeAddSub = NumberAndSubtree | AdditionSubstitution, // (n +- A)

		Rule1 = Subtree * Multiplication * UnknownAndSubtreeMul, // ((x * A) * B) -> (x * (A * B))
		Rule2 = Subtree * Multiplication * NumberAndSubtreeMul, // ((n * A) * B) -> (n * (A * B))
		Rule3 = UnknownAndSubtreeMul * UnknownAndSubtreeMul * AdditionSubstitution, // ((x * A) +- (x * B)) -> (x * (A +- B))
		Rule4 = NumberAndSubtreeMul * NumberAndSubtreeMul * AdditionSubstitution, // ((n * A) +- (n * B)) -> (n * (A +- B))
		Rule5 = Subtree * UnknownAndSubtreeAddSub * AdditionSubstitution, // (A +- (x +- B)) -> (x +- (A +- B))
		Rule6 = Subtree * NumberAndSubtreeAddSub * AdditionSubstitution, // (A +- (n +- B)) -> (n +- (A +- B))
	};

	using SubtreesByDegree = std::map<int, std::vector<SubtreeWithOperator>>;

	friend NodeRule operator|(NodeRule a, NodeRule b);
	std::string toString(const NodeRule rule) const;

	std::vector<AbstractExpressionNode::Ptr> parseExpression(const std::string& expressionString, int preExprLength = 0);

	NodePtr buildTree(const std::vector<AbstractExpressionNode::Ptr>& expressionString);

	// NodeMath.cpp
	NodePtr add(const NumberNode* n1, const NumberNode* n2) const;
	NodePtr subtract(const NumberNode* n1, const NumberNode* n2) const;
	NodePtr divide(const NumberNode* n1, const NumberNode* n2) const;
	NodePtr modulo(const NumberNode* n1, const NumberNode* n2) const;
	NodePtr multiply(const NumberNode* n1, const NumberNode* n2) const;
	NodePtr power(const NumberNode* n1, const NumberNode* n2) const;

	// NodeReduce.cpp
	NodePtr buildReducedFormTree(const NodePtr& node);
	NodePtr reduceNode(const NodePtr& parent) const;

	// NodeBalancing.cpp
	NodePtr buildBalancedTree(const NodePtr& node, SubtreesByDegree& degreeSubtrees);
	void splitSubtreesByDegree(const NodePtr& root, SubtreesByDegree& degreeSubtrees);
	void distributeSubtrees(
	    const NodePtr& node,
	    SubtreesByDegree& degreeSubtrees,
	    OperatorType parentOp,
	    OperatorType subParentOp,
	    bool isLeft
	);
	void insertNodeIntoDegreeSubtreesMap(
	    const NodePtr& ptr,
	    SubtreesByDegree& degreeSubtrees,
	    int power,
	    OperatorType type,
	    bool isLeft
	);
	std::vector<SubtreeWithOperator> spreadSubtrees(SubtreesByDegree& degreeSubtrees);
	NodePtr uniteSubtrees(std::vector<SubtreeWithOperator>& vec, int degree, bool first);
	NodePtr buildTreeFromVectorOfNodes(
	    const std::vector<SubtreeWithOperator>& vec,
	    bool hasParentTree = false,
	    bool isBalancedFirst = false
	) const;
	bool treeIsBalanced() const;

	// NodeOutput.cpp
	std::string outputInfix(const NodePtr& ptr, bool withBrackets) const;
	std::string outputPostfix(const NodePtr& ptr) const;
	std::string outputPrefix(const NodePtr& ptr) const;

	// NodeSimplifiers.cpp
	NodePtr simplifySubtree(NodePtr& node) const;
	NodePtr simplifyAddition(NodePtr& node) const;
	NodePtr simplifySubstitution(NodePtr& node) const;
	NodePtr simplifyMultiplication(NodePtr& node) const;
	NodePtr simplifyDivision(NodePtr& node) const;
	NodePtr simplifyPower(NodePtr& node) const;
	NodePtr simplifyTwoNumbers(const NodePtr& node, const AbstractExpressionNode::Ptr& leftExp, const AbstractExpressionNode::Ptr& rightExp) const;
	NodePtr simplifyOperatorAndNumber(NodePtr& node, const AbstractExpressionNode::Ptr& op, bool leftIsOp) const;
	NodePtr simplifySubTreeWithUnknowns(const NodePtr& ptr) const;
	void switchLeftRightIfNumberOnRight(const NodePtr& ptr) const;

	// NodeRules.cpp
	NodePtr applyRulesToTree(NodePtr& parent) const;
	NodePtr applyRulesToSubTree(NodePtr& parent) const;
	NodePtr applyRule1ToSubTree(NodePtr& parent) const;
	NodePtr applyRule2ToSubTree(NodePtr& parent) const;
	NodePtr applyRule3ToSubTree(NodePtr& parent) const;
	NodePtr applyRule4ToSubTree(NodePtr& parent) const;
	NodePtr applyRule5ToSubTree(NodePtr& parent) const;
	NodePtr applyRule6ToSubTree(NodePtr& parent) const;
	NodeRule getRuleForSubtree(const NodePtr& node) const;
	NodeRule getRuleForNode(const NodePtr& node) const;
	NodeRule validateRules(NodeRule rule1, NodeRule rule2, NodeRule rule3) const;

	// NodeHelpers.cpp
	AbstractExpressionNode::Ptr getExpressionNode(const NodePtr& ptr) const;
	bool nodeHasChildren(const NodePtr& node) const;
	bool nodeHasUnknownExpr(const NodePtr& ptr) const;
	bool subTreesAreEqual(const NodePtr& n1, const NodePtr& n2) const;
	double retrieveNumberFromNode(const NodePtr& node, OperatorType prevOp, bool isFirst) const;
	int calculateMaxDegree() const;
	int countUnknownVars(const NodePtr& node) const;
	int getMaximumPowerOfSubtree(const NodePtr& node) const;
	NodePtr allocateNode(const AbstractExpressionNode::Ptr& node) const;
	NodePtr createNodeByDegreeAndValue(double value, int degree) const;
	void mirrorNodeSign(SubtreeWithOperator& ptr);
	void mirrorNodeSignByPrevOp(NodePtr& node, OperatorType& op);

	// NodeSolver.cpp
	void solveExpression();
	void solveNumber();
	void solveLinear();
	void solveQuadratic();
	void solveCubic();
	double getNthDegreeUnknownValue(int degree, bool isFirst = false) const;

	// members
	NodePtr m_balancedTreeRootNode;
    NodePtr m_reducedTreeRootNode;
	SubtreesByDegree m_degreeSubtrees;
	ExpressionSolution m_solution;
	int m_maxDegree = 0;
	bool m_isBalanced = false;
	ExpressionType m_type;
	std::string m_unknownOperandName = { invalidUnknownNodeName };

	// unused stuff
	void insert(const AbstractExpressionNode::Ptr& key, const bool&) override;
    NodePtr insert(NodePtr& node, const typename AbstractBaseTree::KVPair& keyValue) override;
	bool remove(const AbstractExpressionNode::Ptr& key) override;
	NodePtr remove(NodePtr &p, const AbstractExpressionNode::Ptr &key) override;
	NodePtr find(const NodePtr& node, const AbstractExpressionNode::Ptr& key) const override;
	iterator find(const AbstractExpressionNode::Ptr& key) const override;
	iterator begin() const override;
};
