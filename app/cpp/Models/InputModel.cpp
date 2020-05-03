#include "InputModel.h"

#include <QRegularExpression>
#include <QLoggingCategory>

#include "Models/FunctionList.h"

namespace L {
Q_LOGGING_CATEGORY(input_model, "app.input_model", QtInfoMsg);
} // end namespace L

InputModel::InputModel(FunctionList* funcList, QObject* parent)
    : QObject(parent)
    , m_funcList(funcList)
{
	Q_ASSERT(m_funcList);
}

void InputModel::readInput(const QString& input) {
	deductInput(input);
}

void InputModel::deductInput(const QString& input) {
	const QString functionStringReg = "\\s*(\\w+)\\((\\w+)\\)\\s*";

	const auto functionPattern = QRegularExpression("^" + functionStringReg + "=(.+)$");
	const auto resolutionPattern = QRegularExpression("^\\s*(.+)\\s*=\\s*\\?\\s*$");

	const auto funcMatch = functionPattern.match(input);
	const auto resMatch = resolutionPattern.match(input);

	if (resMatch.hasMatch()) {
		const auto resInput = resMatch.captured(1).toLower();
		qCDebug(L::input_model) << "Trying to resolve input:" << resInput;
		tryToResolve(resInput);
	} else if (funcMatch.hasMatch()) {
		const auto funcName = funcMatch.captured(1).toLower();
		const auto varNameStr = funcMatch.captured(2).toLower();
		const auto expr = funcMatch.captured(3).toLower();

		if (varNameStr.length() != 1) {
			emit errorOccured(tr("Function unknown variable name must consist of one character"));
			return;
		}

		emit functionInserted(funcName, expr, varNameStr.toStdString().front());
		qCDebug(L::input_model) << "Function inserted:" << input;
	} else {
		emit expressionInserted(input, false);
		qCDebug(L::input_model) << "Expression inserted:" << input;
	}
}

void InputModel::tryToResolve(const QString& input) {
	const auto funcNames = m_funcList->functionsNames();

	auto i = 0;
	for (const auto& f : funcNames) {
		QRegularExpressionMatch match;
		const QString regStr = f + "\\((.+)\\)";
		if (input.contains(QRegularExpression(regStr), &match)) {
			bool ok;
			const auto funcVar = QString::number(match.captured(1).toDouble(&ok));

			if (!ok) {
				emit errorOccured(tr("Invalid variable or number in function ") + f);
				return;
			}

			const auto& exprPtr = m_funcList->at(i);
			auto exprStr = exprPtr->expressionString();
			const auto exprVar = exprPtr->expressionUnknownName();

			exprStr.replace(exprVar, funcVar, Qt::CaseInsensitive);

			emit expressionInserted(exprStr, true);
			qCDebug(L::input_model) << "Expression resolved and inserted:" << input;
		}

		++i;
	}
}
