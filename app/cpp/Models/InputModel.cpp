#include "InputModel.h"

#include <QRegularExpression>
#include <QLoggingCategory>

#include "Models/FunctionList.h"
#include "Models/VariableList.h"

namespace L {
Q_LOGGING_CATEGORY(input_model, "app.input.model", QtInfoMsg);
} // end namespace L

InputModel::InputModel(FunctionList* funcList, VariableList* varList, QObject* parent)
    : QObject(parent)
    , m_funcList(funcList)
    , m_varList(varList)
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
	const auto variableAssignPattern = QRegularExpression("^\\s*(\\w+)\\s*=(.*)$");

	const auto funcMatch = functionPattern.match(input);
	const auto resMatch = resolutionPattern.match(input);
	const auto varAssMatch = variableAssignPattern.match(input);

	if (varAssMatch.hasMatch()) {
		const auto varName = varAssMatch.captured(1).toLower();

		if (!nameIsUnique(m_funcList->validFunctionsNames(), varName)) {
			emit errorOccured(tr("Variable cannot be named as existing function"));
			return;
		}

		const auto resInput = varAssMatch.captured(2).toLower();
		const auto replacedInput = replaceVariablesWithValues(resInput);
		emit variableInserted(varName, replacedInput);
	} else if (resMatch.hasMatch()) {
		const auto resInput = resMatch.captured(1).toLower();
		qCDebug(L::input_model) << "Trying to resolve input:" << resInput;
		tryToResolve(resInput);
	} else if (funcMatch.hasMatch()) {
		const auto funcName = funcMatch.captured(1).toLower();

		if (!nameIsUnique(m_varList->validVariablesList().keys(), funcName)) {
			emit errorOccured(tr("Function cannot be named as existing variable"));
			return;
		}

		const auto varNameStr = funcMatch.captured(2).toLower();

		if (varNameStr.length() != 1) {
			emit errorOccured(tr("Function unknown variable name must consist of one character"));
			return;
		}

		const auto expr = funcMatch.captured(3).toLower();
		const auto replacedExpr = replaceVariablesWithValues(expr, { varNameStr });

		emit functionInserted(funcName, replacedExpr, varNameStr.toStdString().front());
		qCDebug(L::input_model) << "Function inserted:" << input;
	} else {
		const auto replacedInput = replaceVariablesWithValues(input);
		emit expressionInserted(replacedInput, false);
		qCDebug(L::input_model) << "Expression inserted:" << input;
	}
}

void InputModel::tryToResolve(const QString& input) {
	const auto funcNames = m_funcList->validFunctionsNames();
	QString resultExpression = replaceVariablesWithValues(input);

	for (auto i = 0; i < funcNames.size();) {
		const auto f = funcNames[i];
		QRegularExpressionMatch match;
		const QString regStr = f + "\\((\\-?\\d*\\.?\\d+)\\)";
		if (resultExpression.contains(QRegularExpression(regStr), &match)) {
			bool ok;
			const auto capturedNumberStr = match.captured(1);
			const auto capturedNumber = QString::number(capturedNumberStr.toDouble(&ok));

			if (!ok) {
				emit errorOccured(tr("Invalid variable or number in function ") + f);
				return;
			}

			const auto& exprPtr = m_funcList->at(i);
			auto exprStr = exprPtr->expressionString();
			const auto exprVar = exprPtr->expressionUnknownName();
			exprStr.replace(exprVar, capturedNumber, Qt::CaseInsensitive);

			const auto funcToReplace = QString("%1(%2)").arg(f).arg(capturedNumber);
			resultExpression.replace(funcToReplace, exprStr, Qt::CaseInsensitive);

			continue;
		}

		++i;
	}

	emit expressionInserted(resultExpression, true);
	qCDebug(L::input_model) << "Expression resolved and inserted:" << resultExpression;
}

QString InputModel::replaceVariablesWithValues(const QString& input, const QStringList& except) {
	const auto variableMap = m_varList->validVariablesList();

	QString resInput = input;
	for (const auto& var : variableMap.keys()) {
		const auto varRes = variableMap[var];

		if (!except.contains(var)) {
			resInput.replace(var, varRes, Qt::CaseInsensitive);
		}
	}

	return resInput;
}

bool InputModel::nameIsUnique(const QStringList& names, const QString& checkName) {
	for (const auto& name : names) {
		if (checkName == name) {
			return false;
		}
	}

	return true;
}
