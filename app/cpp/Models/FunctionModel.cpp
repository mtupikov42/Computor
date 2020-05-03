#include "FunctionModel.h"

#include <QQmlEngine>

void FunctionModel::RegisterType(const char* uri) {
	qmlRegisterUncreatableType<FunctionModel>(uri, 1, 0, "FunctionModel", "FunctionModel must be created on C++ side");
}

FunctionModel::FunctionModel(
    const QString& functionName,
    const QString& expression,
    char expressionUnknown,
    QObject* parent
)
    : QObject(parent)
    , m_expressionModel(new ExpressionModel(expression, true, this))
    , m_expressionUnknown(expressionUnknown)
    , m_functionName(functionName)
{
}

QString FunctionModel::expressionUnknownName() const {
	return m_expressionUnknown;
}

QString FunctionModel::expressionString() const {
	return m_expressionModel->toString(ExpressionModel::OutputType::Infix);
}

QString FunctionModel::functionName() const {
	return m_functionName;
}

QString FunctionModel::toString() const {
	return QString("%1(%2) = %3").arg(functionName()).arg(expressionUnknownName()).arg(expressionString());
}

QString FunctionModel::errorString() const {
	return m_expressionModel->errorString();
}

int FunctionModel::errorColumn() const {
	if (m_expressionModel->errorColumn() < 0) {
		return m_expressionModel->errorColumn();
	}

	QString temp = "%1(%2) = ";
	temp.arg(functionName()).arg(expressionUnknownName());

	return temp.length() + m_expressionModel->errorColumn();
}
