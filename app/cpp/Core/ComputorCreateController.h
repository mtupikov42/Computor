#pragma once

#include <QObject>

#include "Models/ExpressionModel.h"

class ComputorCreateController final : public QObject {
	Q_OBJECT

public:
	explicit ComputorCreateController(QObject* parent = nullptr);

public slots:
	void createExpression(const QString& input);

signals:
	void expressionModelCreated(const ExpressionModel::Ptr& model);
};
