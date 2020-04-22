#pragma once

#include <QObject>

class QQmlEngine;

class AssetsProvider final : public QObject {
	Q_OBJECT

public:
	static void RegisterType();

	static QString appIcon();

private:
	AssetsProvider(QObject* parent = nullptr);

	static const QString m_appIcon;

	static AssetsProvider* m_instance;
};
