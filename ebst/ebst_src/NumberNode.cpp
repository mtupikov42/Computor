#include "NumberNode.h"

NumberNode::NumberNode(double value) : m_value(value) {}

double NumberNode::value() const {
	return m_value;
}

std::string NumberNode::toString() const {
	return trimDoubleToString(m_value);
}

std::string trimDoubleToString(double d) {
	// lol
	if (d == 0.0) {
		d = abs(d);
	}

	auto str = std::to_string(d);

	auto lastZeroPos = str.find_last_not_of('0');
	if (lastZeroPos + 1 != str.length()) {
		lastZeroPos += 2;
		str.erase(lastZeroPos, std::string::npos);
	}

	return str;
}
