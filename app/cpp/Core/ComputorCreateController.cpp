#include "ComputorCreateController.h"

ComputorCreateController::ComputorCreateController(QObject* parent) : QObject(parent) {}

void ComputorCreateController::createExpression(const QString& input, bool inFunction) {
	auto model = QSharedPointer<ExpressionModel>::create(input, inFunction);

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

void ComputorCreateController::createNumberVariable(
    const QString& varName,
    const QString& realPart,
    const QString& imagPart
) {
	auto model = QSharedPointer<VariableModel>::create(varName, realPart, imagPart);

	emit variableModelCreated(model);
}
