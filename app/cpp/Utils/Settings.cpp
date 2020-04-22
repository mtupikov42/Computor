#include "Settings.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>

namespace {

QString getSettingsPath() {
	static QString path;
	if (path.isEmpty()) {
		const QFileInfo defPathInfo = QFileInfo(QSettings().fileName());
		QDir dir = defPathInfo.absoluteDir();
		dir.cdUp();

		path = dir.absolutePath();
		path += QDir::separator();
		path += QString::fromLatin1("settings.");
		path += defPathInfo.suffix();
	}

	return path;
}

} // end anonymous namespace

namespace settings {

SettingsUPtr create() {
	return std::unique_ptr<QSettings>(new QSettings(getSettingsPath(), QSettings::IniFormat));
}

} // end namespace settings
