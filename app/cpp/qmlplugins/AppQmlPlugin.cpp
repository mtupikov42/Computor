#include "AppQmlPlugin.h"

#include "AssetsProvider.h"
#include "CustomWindow.h"
#include "QmlColorPalette.h"
#include "QmlListModel.h"
#include "SizeProvider.h"
#include "SpecialSymbols.h"
#include "Theme.h"

#include <QLoggingCategory>

namespace L {
Q_LOGGING_CATEGORY(qml_plugin, "app.qml_plugin.register", QtInfoMsg)
} // end namespace L


AppQmlPlugin::AppQmlPlugin(QObject* parent) : QQmlExtensionPlugin(parent) {}

void AppQmlPlugin::registerTypes(const char* uri) {
	qCDebug(L::qml_plugin) << "Registering under " << uri;

	CustomWindow::RegisterType(uri);
	QmlListModel::RegisterType(uri);
	AssetsProvider::RegisterType();
	QmlColorPalette::RegisterType();
	SizeProvider::RegisterType();
	SpecialSymbols::RegisterType();
	Theme::RegisterType();
}
