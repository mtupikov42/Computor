#pragma once

#include <QObject>
#include <QString>
#include <QPointer>
#include <QSharedPointer>

#include "ExpressionModel.h"

class FunctionModel : public QObject {
	Q_OBJECT

public:
	static void RegisterType(const char* uri);

	using Ptr = QSharedPointer<FunctionModel>;

	explicit FunctionModel(
	    const QString& functionName,
	    const QString& expression,
	    char expressionUnknown,
	    QObject* parent = nullptr
	);

	QString expressionUnknownName() const;
	QString expressionString() const;
	QString functionName() const;
	QString toString() const;
	QString errorString() const;
	int errorColumn() const;
	bool isValid() const;

private:
	void checkErrorState();

	QPointer<ExpressionModel> m_expressionModel;
	QString m_expressionUnknown;
	QString m_functionName;
	QString m_errorString;
	int m_errorColumn = -1;
};

using FunctionModels = QVector<FunctionModel::Ptr>;
