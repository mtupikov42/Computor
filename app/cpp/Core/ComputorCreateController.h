#pragma once

#include <QObject>

#include "Models/ExpressionModel.h"
#include "Models/FunctionModel.h"
#include "Models/VariableModel.h"

class ComputorCreateController final : public QObject {
	Q_OBJECT

public:
	explicit ComputorCreateController(QObject* parent = nullptr);

public slots:
	void createExpression(const QString& input, bool inFunction);
	void createFunction(
	    const QString& functionName,
	    const QString& expression,
	    char expressionUnknown
	);
	void createVariable(
	    const QString& varName,
	    const QString& input
	);

signals:
	void expressionModelCreated(const ExpressionModel::Ptr& model);
	void functionModelCreated(const FunctionModel::Ptr& model);
	void variableModelCreated(const VariableModel::Ptr& model);
};
