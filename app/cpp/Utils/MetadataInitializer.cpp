#include "Utils/MetadataInitializer.h"

#include "AppQmlPlugin.h"
#include "Core/WindowStateLoader.h"

#include <QWindow>

namespace MetadataInitializer {

namespace {

const auto appQmlPluginUri = "AppQmlPlugin";

} // end anonymous namespace

void registerQmlTypes() {
	static AppQmlPlugin qmlPlugin;
	qmlPlugin.registerTypes(appQmlPluginUri);

	WindowStateLoader::RegisterType(appQmlPluginUri);
}

void registerSignalsTypes() {
	qRegisterMetaType<QWindow::Visibility>("QWindow::Visibility");
}

void registerAllMeta() {
    registerQmlTypes();
    registerSignalsTypes();
}

} // end namespace MetadataInitializer
