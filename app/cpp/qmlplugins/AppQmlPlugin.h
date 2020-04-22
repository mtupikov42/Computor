#pragma once

#include <QtQml>

class AppQmlPlugin : public QQmlExtensionPlugin {
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "AppQmlPlugin")

public:
	explicit AppQmlPlugin(QObject *parent = nullptr);

	void registerTypes(const char* uri) override;
};
