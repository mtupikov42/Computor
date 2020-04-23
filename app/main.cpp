#include <QSettings>
#include <QDir>
#include <QQuickWindow>
#include <QGuiApplication>
#include <QDirIterator>
#include <QFontDatabase>
#include <QLoggingCategory>

#include <app_config.h>
#include <StandardPaths.h>

#include "EBST.h"
#include "ExpressionException.h"

#include "Core/UIManager.h"
#include "Core/Logger.h"
#include "Utils/MetadataInitializer.h"
#include "qmlplugins/AssetsProvider.h"

namespace L {
Q_LOGGING_CATEGORY(init, "app.main.init", QtInfoMsg);
} // end namespace L

void setupApplicationInfo() {
	QGuiApplication::setOrganizationName(app_cfg::company_name);
	QGuiApplication::setApplicationDisplayName(app_cfg::application_name);
	QGuiApplication::setApplicationName(app_cfg::application_name);
	QGuiApplication::setApplicationVersion(app_cfg::version_string);
}

void setupAppStorages() {
	const auto setupDir = [](QStandardPaths::StandardLocation path, QString appendedPath = {}) {
		auto wpath = StandardPaths::writableLocation(path);
		if (!appendedPath.isEmpty()) {
			wpath.append(QDir::separator())
			     .append(appendedPath);
		}
		QDir dir(QDir::toNativeSeparators(wpath));
		if (!dir.exists()) {
			dir.mkpath(dir.absolutePath());
		}
	};

	setupDir(QStandardPaths::AppDataLocation);
	setupDir(QStandardPaths::GenericDataLocation, Logger::logsDirName());
}

void setupSettings() {
	QSettings::setDefaultFormat(QSettings::IniFormat);
	const auto settingsLocation = StandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, settingsLocation);
}

void setupApplicationFonts() {
	QDirIterator it(":/fonts", { "*.otf", "*.ttf" }, QDir::Files, QDirIterator::Subdirectories);

	while (it.hasNext()) {
		it.next();

		const auto fontId = QFontDatabase::addApplicationFont(it.filePath());
		if (fontId < 0) {
			qCCritical(L::init) << "Unable to load font" << it.fileName();
		}
	}
}

void setupGraphics() {
#ifdef Q_OS_WIN
	QGuiApplication::setAttribute(Qt::AA_UseOpenGLES);
#endif
}

int main(int argc, char** argv) {
	setupApplicationInfo();
	setupAppStorages();

	Logger logger;
	MetadataInitializer::registerAllMeta();

	try {
		EBST ebst("x^2 + 10 * x - 50.3");
		const auto sols = ebst.solution().solutions;
		for (const auto& s : sols) {
			qCInfo(L::init) << QString::fromStdString(s.varName) << QString::fromStdString(s.varResult);
		}
	} catch (ExpressionException& ex) {
		qCCritical(L::init) << QString::fromStdString(ex.toString()) << ex.column();
	}

	setupGraphics();
	setupSettings();

	auto appInstance = QGuiApplication(argc, argv);
	appInstance.setWindowIcon(QIcon(AssetsProvider::appIcon()));

	qCInfo(L::init) << QGuiApplication::applicationName() << QGuiApplication::organizationName() << QGuiApplication::applicationVersion();

	setupApplicationFonts();

	UIManager uiManager;
	uiManager.initEngine();

	return appInstance.exec();
}
