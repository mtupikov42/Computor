#include "ExpressionList.h"

ExpressionList::ExpressionList(QObject* parent) : QmlListModel(parent) {

}

QVariant ExpressionList::data(const QModelIndex& index, int role) const {
	if (!isValid(index)) {
		return {};
	}

	const auto idx = static_cast<size_t>(index.row());
	const auto& d = m_expModels[idx];

	switch (role) {
	case Role::Degree: return d->maxDegree();
	case Role::ErrorColumn: return d->errorColumn();
	case Role::Solutions: return d->solutionList();
	case Role::RawExpressionString: return d->rawExpression();
	case Role::InfixString: return d->toString(ExpressionModel::OutputType::Infix);
	case Role::InfixWithParenthesesString: return d->toString(ExpressionModel::OutputType::InfixWithParentheses);
	case Role::PostfixString: return d->toString(ExpressionModel::OutputType::Postfix);
	case Role::PrefixString: return d->toString(ExpressionModel::OutputType::Prefix);
	case Role::ReducedInfixString: return d->toString(ExpressionModel::OutputType::ReducedInfix);
	case Role::ErrorString: return d->errorString();
	default:
		qFatal("unhandled role");
		break;
	}

	return {};
}

bool ExpressionList::setData(const QModelIndex&, const QVariant&, int) {
	return false;
}

int ExpressionList::rowCount(const QModelIndex&) const {
	return static_cast<int>(m_expModels.size());
}

QHash<int, QByteArray> ExpressionList::roleNames() const {
	static const decltype(roleNames()) role_names{
		{ Role::Degree, "degreeRole" },
		{ Role::ErrorColumn, "errorColumnRole" },
		{ Role::Solutions, "solutionsRole" },
		{ Role::RawExpressionString, "rawExpressionStringRole" },
		{ Role::InfixString, "infixStringRole" },
		{ Role::InfixWithParenthesesString, "infixWithParenthesesStringRole" },
		{ Role::PostfixString, "postfixStringRole" },
		{ Role::PrefixString, "prefixStringRole" },
		{ Role::ReducedInfixString, "reducedInfixStringRole" },
		{ Role::ErrorString, "errorStringRole" }
	};

	return role_names;
}

void ExpressionList::add(const ExpressionModel::Ptr& model) {
	beginInsertRows({}, rowCount(), rowCount());
	m_expModels.push_back(model);
	endInsertRows();
}

void ExpressionList::resetModelData() {}