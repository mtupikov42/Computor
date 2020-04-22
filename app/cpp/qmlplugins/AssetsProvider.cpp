#include "AssetsProvider.h"

#include <QQmlEngine>

void AssetsProvider::RegisterType() {
	static const auto instantiator = [](QQmlEngine* engine, QJSEngine*) -> QObject* {
		return new AssetsProvider(engine);
	};

	qmlRegisterSingletonType<AssetsProvider>("AssetsProvider", 1, 0, "AssetsProvider", instantiator);
}

QString AssetsProvider::appIcon() {
	return m_appIcon;
}

AssetsProvider::AssetsProvider(QObject* parent)
    : QObject(parent)
{}

const QString AssetsProvider::m_appIcon = ":/assets/icons/application/app.ico";

AssetsProvider* AssetsProvider::m_instance = nullptr;
