#include "Utils/MetadataInitializer.h"

#include "AppQmlPlugin.h"
#include "Core/WindowStateLoader.h"
#include "Models/ExpressionModel.h"

#include <QWindow>

namespace MetadataInitializer {

namespace {

const auto computorQmlPluginUri = "ComputorQmlPlugin";

} // end anonymous namespace

void registerQmlTypes() {
	static AppQmlPlugin qmlPlugin;
	qmlPlugin.registerTypes(computorQmlPluginUri);

	WindowStateLoader::RegisterType(computorQmlPluginUri);
	ExpressionModel::RegisterType(computorQmlPluginUri);
}

void registerSignalsTypes() {
	qRegisterMetaType<QWindow::Visibility>("QWindow::Visibility");
	qRegisterMetaType<ExpressionModel::Ptr>("ExpressionModel::Ptr");
}

void registerAllMeta() {
    registerQmlTypes();
    registerSignalsTypes();
}

} // end namespace MetadataInitializer
