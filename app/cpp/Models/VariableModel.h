#pragma once

#include <optional>
#include <complex>

#include <QObject>
#include <QSharedPointer>

class VariableModel : public QObject {
	Q_OBJECT

public:
	static void RegisterType(const char* uri);

	using Ptr = QSharedPointer<VariableModel>;

	enum class Type {
		Real,
		Complex
	};
	Q_ENUM(Type)

	explicit VariableModel(
	    const QString& varName,
	    const QString& realPart,
	    const QString& imagPart,
	    QObject* parent = nullptr
	);

	QString name() const;
	QString resultString() const;
	QString errorString() const;
	Type type() const;
	bool isValid() const;

private:
	void parseRawInput(const QString& realPart, const QString& imagPart);

	Type m_type;
	QString m_name;
	QString m_error;
	std::complex<double> m_result;
};

using VariableModels = QVector<VariableModel::Ptr>;
