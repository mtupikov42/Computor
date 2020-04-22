#pragma once

#include <QObject>

class UIController final : public QObject {
	Q_OBJECT

public:
	explicit UIController(QObject* parent = nullptr);

public slots:
	void quit();
};
