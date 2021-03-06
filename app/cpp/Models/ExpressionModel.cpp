#include "ExpressionModel.h"

#include <QMap>
#include <QQmlEngine>
#include <QLoggingCategory>

#include "ExpressionException.h"

namespace L {
Q_LOGGING_CATEGORY(expression_model, "app.expression.model", QtInfoMsg);
} // end namespace L

void ExpressionModel::RegisterType(const char* uri) {
	qmlRegisterUncreatableType<ExpressionModel>(uri, 1, 0, "ExpressionModel", "ExpressionModel must be created on C++ side");
}

ExpressionModel::ExpressionModel(const QString& rawExpression, bool inFunction, QObject* parent) : QObject(parent) {
	parseRawExpression(rawExpression, inFunction);
}

QString ExpressionModel::toString(OutputType type) const {
	const QMap<OutputType, EBST::OutputType> ebstOutputConverter {
		{ OutputType::Infix, EBST::OutputType::Infix },
		{ OutputType::Prefix, EBST::OutputType::Prefix },
		{ OutputType::Postfix, EBST::OutputType::Postfix },
		{ OutputType::InfixWithParentheses, EBST::OutputType::InfixWithParentheses },
	};

	if (!m_expressionTree.has_value()) {
		return m_rawExpression;
	}

	Q_ASSERT(ebstOutputConverter.contains(type));

	const auto convertedType = ebstOutputConverter[type];

	return QString::fromStdString(m_expressionTree->toString(convertedType));
}

int ExpressionModel::maxDegree() const {
	if (treeIsValid()) {
		return expressionTree().maxDegree();
	}

	return 0;
}

QStringList ExpressionModel::solutionList() const {
	if (!treeIsValid()) {
		return {};
	}

	const auto solution = expressionTree().solution();
	const auto hasDiscriminant = solution.discriminant.has_value();
	const auto solutions = solution.solutions;

	QStringList list;

	if (solutions.empty()) {
		list.append("Expression has no solutions");
		return list;
	}

	if (hasDiscriminant) {
		list.append("Discriminant: " + QString::number(solution.discriminant.value()));
	}

	for (const auto& sol : solutions) {
		const auto varName = QString::fromStdString(sol.varName);
		const auto varResult = QString::fromStdString(sol.varResult);
		const auto result = varName + ": " + varResult;
		list.append(result);
	}

	return list;
}

int ExpressionModel::errorColumn() const {
	return m_errorColumn;
}

bool ExpressionModel::treeIsValid() const {
	return m_expressionTree.has_value();
}

void ExpressionModel::parseRawExpression(const QString& rawExpression, bool inFunction) {
	m_rawExpression = rawExpression;
	ExpressionType type = inFunction ? ExpressionType::PartialExpression : ExpressionType::FullExpression;

	try {
		EBST ebst(m_rawExpression.toStdString(), type);
		m_expressionTree = std::move(ebst);
		qCDebug(L::expression_model) << "Tree was parsed successfully";
	} catch (const ExpressionException& ex) {
		m_errorColumn = ex.column();
		m_errorString = QString::fromStdString(ex.toString());
		qCInfo(L::expression_model) << "Error occured on" << m_errorColumn << ":" << m_errorString;
	}
}

QString ExpressionModel::errorString() const {
	return m_errorString;
}

QString ExpressionModel::unknownVariableName() const {
	return QString::fromStdString(m_expressionTree->unknownOperandName());
}

QString ExpressionModel::rawExpression() const {
	return m_rawExpression;
}

const EBST& ExpressionModel::expressionTree() const {
	return m_expressionTree.value();
}
