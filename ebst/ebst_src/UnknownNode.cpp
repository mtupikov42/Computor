#include "UnknownNode.h"

UnknownNode::UnknownNode(char name) : m_name(name) {}

char UnknownNode::name() const {
	return m_name;
}

std::string UnknownNode::toString() const {
	return { m_name };
}
