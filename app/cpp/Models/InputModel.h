#pragma once

#include <QObject>
#include <QString>
#include <QPointer>

class FunctionList;

class InputModel : public QObject {
	Q_OBJECT

public:
	explicit InputModel(FunctionList* funcList, QObject* parent = nullptr);

	Q_INVOKABLE void readInput(const QString& input);

signals:
	void expressionInserted(const QString& input, bool inFunction);
	void functionInserted(
	    const QString& functionName,
	    const QString& expression,
	    char expressionUnknown
	);
	void errorOccured(const QString& error);

private:
	void deductInput(const QString& input);
	void tryToResolve(const QString& input);

	QPointer<FunctionList> m_funcList;
};
