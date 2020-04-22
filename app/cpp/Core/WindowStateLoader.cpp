#include "WindowStateLoader.h"

#include <QQmlEngine>

#include "Utils/Settings.h"

namespace {

const auto windowSettingsGroup = "WindowSettings";
const auto dataKey = "data";

} // end anonymous namespace

void WindowStateLoader::RegisterType(const char* uri) {
	qmlRegisterType<WindowStateLoader>(uri, 1, 0, "WindowStateLoader");
}

WindowStateLoader::WindowStateLoader(QObject* parent) : QObject(parent) {}

QVariantMap WindowStateLoader::restoreWindowState() {
	auto settings = settings::create();

	settings->beginGroup(windowSettingsGroup);
	const auto data = settings->value(dataKey, QVariantMap()).value<QVariantMap>();
	settings->endGroup();

	return data;
}

void WindowStateLoader::saveWindowState(const QVariantMap& data) {
	auto settings = settings::create();

	settings->beginGroup(windowSettingsGroup);
	settings->setValue(dataKey, data);
	settings->endGroup();
}
