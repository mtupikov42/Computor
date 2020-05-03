#pragma once

#include <QObject>
#include <QSharedPointer>

#include <optional>

#include "EBST.h"

class ExpressionModel : public QObject {
	Q_OBJECT

public:
	static void RegisterType(const char* uri);

	using Ptr = QSharedPointer<ExpressionModel>;

	enum class OutputType {
		Infix,
		InfixWithParentheses,
		Postfix,
		Prefix
	};
	Q_ENUM(OutputType)

	explicit ExpressionModel(const QString& rawExpression, bool inFunction, QObject* parent = nullptr);

	QString toString(OutputType type) const;
	int maxDegree() const;
	int errorColumn() const;
	QStringList solutionList() const;
	QString rawExpression() const;
	QString errorString() const;
	QString unknownVariableName() const;

private:
	bool treeIsValid() const;
	void parseRawExpression(const QString& rawExpression, bool inFunction);
	const EBST& expressionTree() const;

	std::optional<EBST> m_expressionTree;
	QString m_rawExpression;
	QString m_errorString;
	int m_errorColumn = -1;
};

using ExpressionModels = QVector<ExpressionModel::Ptr>;
