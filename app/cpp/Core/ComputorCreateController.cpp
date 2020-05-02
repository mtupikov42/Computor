#include "ComputorCreateController.h"

ComputorCreateController::ComputorCreateController(QObject* parent) : QObject(parent) {}

void ComputorCreateController::createExpression(const QString& input) {
	auto model = QSharedPointer<ExpressionModel>::create(input, false);

	emit expressionModelCreated(model);
}

void ComputorCreateController::createFunction(
    const QString& functionName,
    const QString& expression,
    char expressionUnknown
) {
	auto model = QSharedPointer<FunctionModel>::create(functionName, expression, expressionUnknown);

	emit functionModelCreated(model);
}
