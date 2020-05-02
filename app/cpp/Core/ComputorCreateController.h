#pragma once

#include <QObject>

#include "Models/ExpressionModel.h"
#include "Models/FunctionModel.h"

class ComputorCreateController final : public QObject {
	Q_OBJECT

public:
	explicit ComputorCreateController(QObject* parent = nullptr);

public slots:
	void createExpression(const QString& input);
	void createFunction(
	    const QString& functionName,
	    const QString& expression,
	    char expressionUnknown
	);

signals:
	void expressionModelCreated(const ExpressionModel::Ptr& model);
	void functionModelCreated(const FunctionModel::Ptr& model);
};
