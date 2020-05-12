#include "AbstractExpressionNode.h"

#include <regex>
#include <cassert>
#include <map>

#include "NumberNode.h"
#include "ImaginaryNumberNode.h"
#include "UnknownNode.h"
#include "OperatorNode.h"

namespace {

const std::string variableRegex{"^([[:alpha:]])$"};
const std::string numberRegex{"^[+-]?(?=[.]?[0-9])[0-9]*(?:[.][0-9]*)?(?:[Ee][+-]?[0-9]+)?$"};

} // end anonymous namespace

NumberNode* AbstractExpressionNode::castToNumberNode() {
	return dynamic_cast<NumberNode*>(this);
}

UnknownNode* AbstractExpressionNode::castToUnknownNode() {
	return dynamic_cast<UnknownNode*>(this);
}

OperatorNode* AbstractExpressionNode::castToOperatorNode() {
	return dynamic_cast<OperatorNode*>(this);
}

ImaginaryNumberNode* AbstractExpressionNode::castToImaginaryNumberNode() {
	return dynamic_cast<ImaginaryNumberNode*>(this);
}

AbstractExpressionNode::Ptr parseExpressionFromString(const std::string& str) {
	std::smatch sm;
	if (std::regex_match(str, sm, std::regex(variableRegex))) {
		assert(sm.size() == 2);

		const auto varName = sm[1].str();
		assert(varName.length() == 1);

		if (varName[0] == ImaginaryNumberNode::imaginaryUnit) {
			return std::make_shared<ImaginaryNumberNode>();
		}

		return std::make_shared<UnknownNode>(varName[0]);
	} else if (std::regex_match(str, sm, std::regex(numberRegex))) {
		assert(!sm.empty());

		const auto strNum = sm[0].str();
		const auto num = std::stod(strNum);
		return std::make_shared<NumberNode>(num);
	}

	return nullptr;
}

AbstractExpressionNode::Ptr parseExpressionFromChar(char c) {
	const std::map<char, OperatorType> charToOperandType {
		{ '+', OperatorType::Addition },
		{ '-', OperatorType::Substitution },
		{ '/', OperatorType::Division },
		{ '*', OperatorType::Multiplication },
		{ '%', OperatorType::Modulo },
		{ '^', OperatorType::Power },
		{ '(', OperatorType::BracketLeft },
		{ ')', OperatorType::BracketRight }
	};

	const auto it = charToOperandType.find(c);
	if (it != charToOperandType.cend()) {
		return std::make_shared<OperatorNode>(charToOperandType.at(c));
	}

	return nullptr;
}
