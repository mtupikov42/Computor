#include "InputModel.h"

#include <QRegularExpression>
#include <QLoggingCategory>

namespace L {
Q_LOGGING_CATEGORY(input_model, "app.input_model", QtInfoMsg);
} // end namespace L

InputModel::InputModel(QObject* parent) : QObject(parent) {}

void InputModel::readInput(const QString& input) {
	deductInput(input);
}

void InputModel::deductInput(const QString& input) {
	const auto functionPattern = QRegularExpression("^\\s*(\\w+)\\((\\w+)\\)\\s*=(.+$)");
	const auto funcMatch = functionPattern.match(input);

	if (funcMatch.hasMatch()) {
		const auto funcName = funcMatch.captured(1);
		const auto varNameStr = funcMatch.captured(2);
		const auto expr = funcMatch.captured(3);

		if (varNameStr.length() != 1) {
			emit invalidFunctionUnknownVar();
			return;
		}

		emit functionInserted(funcName, expr, varNameStr.toStdString().front());
		qCDebug(L::input_model) << "Function inserted:" << input;
	} else {
		emit expressionInserted(input);
		qCDebug(L::input_model) << "Expression inserted:" << input;
	}
}
