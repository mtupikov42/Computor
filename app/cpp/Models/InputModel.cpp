#include "InputModel.h"

InputModel::InputModel(QObject* parent) : QObject(parent) {}

void InputModel::readInput(const QString& input) {
	emit expressionInserted(input);
}
