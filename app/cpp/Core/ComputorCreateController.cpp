#include "ComputorCreateController.h"

ComputorCreateController::ComputorCreateController(QObject* parent) : QObject(parent) {}

void ComputorCreateController::createExpression(const QString& input) {
	auto model = QSharedPointer<ExpressionModel>::create(input);

	emit expressionModelCreated(model);
}

