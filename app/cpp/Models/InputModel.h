#pragma once

#include <QObject>
#include <QString>

class InputModel : public QObject {
	Q_OBJECT

public:
	explicit InputModel(QObject* parent = nullptr);

	Q_INVOKABLE void readInput(const QString& input);

signals:
	void expressionInserted(const QString& input);
};
