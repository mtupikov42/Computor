#include "VariableModel.h"

#include <QQmlEngine>
#include <QLoggingCategory>
#include <QRegularExpression>

namespace {

QString complexToString(const std::complex<double>& c) {
	const auto realNum = c.real();
	const auto imagNum = c.imag();

	const auto realStr = QString::number(realNum, 'g', 3);
	const auto imagStr = QString::number(imagNum, 'g', 3);

	if (imagNum == 0.0) {
		return realStr;
	}

	const auto realIsPositive = imagNum > 0.0;
	const auto op = realIsPositive ? "+" : "";

	return realStr + op + imagStr + "i";
}

namespace L {
Q_LOGGING_CATEGORY(variable_model, "app.variable.model", QtInfoMsg);
} // end namespace L

} // end anonymous namespace

void VariableModel::RegisterType(const char* uri) {
	qmlRegisterUncreatableType<VariableModel>(uri, 1, 0, "VariableModel", "VariableModel must be created on C++ side");
}

VariableModel::VariableModel(const QString& varName, const QString& realPart, const QString& imagPart, QObject* parent)
    : QObject(parent)
    , m_name(varName)
{
	if (m_name == "i") {
		m_error = tr("Variable cannot be named `i` for obvious reasons");
	} else {
		parseRawInput(realPart, imagPart);
	}
}

QString VariableModel::name() const {
	return m_name;
}

QString VariableModel::resultString() const {
	return complexToString(m_result);
}

QString VariableModel::errorString() const {
	return m_error;
}

VariableModel::Type VariableModel::type() const {
	return m_type;
}

bool VariableModel::isValid() const {
	return m_error.size() == 0;
}

void VariableModel::parseRawInput(const QString& realPart, const QString& imagPart) {
	const auto doubleRegex = QString("([+-]?\\d*\\.?\\d+)");
	const auto realNumRegex = QRegularExpression("^" + doubleRegex + "$");
	const auto imagNumRegex = QRegularExpression("^" + doubleRegex + "i$");

	auto realNum = 0.0;
	auto complexNum = 0.0;

	const auto parseNumber = [this](
	    const QRegularExpression& regex,
	    const QString& input,
	    double& assignValue,
	    const QString& errorString
	) {
		const auto realNumMatch = regex.match(input);

		if (realNumMatch.hasMatch()) {
			const auto number = realNumMatch.captured(1);
			assignValue = number.toDouble();
			return true;
		}

		m_error = errorString;
		return false;
	};

	if (!parseNumber(realNumRegex, realPart, realNum, tr("Can't parse real number"))) {
		return;
	}

	if (imagPart.size() != 0) {
		parseNumber(imagNumRegex, imagPart, complexNum, tr("Can't parse imaginary number"));
		m_type = Type::Complex;
	} else {
		m_type = Type::Real;
	}

	m_result = std::complex<double>(realNum, complexNum);
	qCDebug(L::variable_model) << "Parsed" << m_type << "variable:" << resultString();
}
