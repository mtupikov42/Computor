#pragma once

#include <QObject>
#include <QString>
#include <QPointer>

class FunctionList;
class VariableList;

class InputModel : public QObject {
	Q_OBJECT

public:
	explicit InputModel(FunctionList* funcList, VariableList* varList, QObject* parent = nullptr);

	Q_INVOKABLE void readInput(const QString& input);

signals:
	void expressionInserted(const QString& input, bool inFunction);
	void functionInserted(
	    const QString& functionName,
	    const QString& expression,
	    char expressionUnknown
	);
	void variableInserted(
	    const QString& varName,
	    const QString& input
	);
	void errorOccured(const QString& error);

private:
	void deductInput(const QString& input);
	void tryToResolve(const QString& input);
	QString replaceVariablesWithValues(const QString& input, const QStringList& except = {});
	bool nameIsUnique(const QStringList& names, const QString& name);

	QPointer<FunctionList> m_funcList;
	QPointer<VariableList> m_varList;
};
