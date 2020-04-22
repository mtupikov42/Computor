#include "StandardPaths.h"

#include <QCoreApplication>
#include <QDir>
#include <QVector>

namespace StandardPaths {

QString writableLocation(QStandardPaths::StandardLocation location) {
	const QVector<QStandardPaths::StandardLocation> appendedLocations {
		QStandardPaths::StandardLocation::GenericDataLocation
	};

	if (!appendedLocations.contains(location)) {
		return QStandardPaths::writableLocation(location);
	}

	QString path;
	path = QDir::toNativeSeparators(QStandardPaths::writableLocation(location));
	path.append(QDir::separator())
	    .append(QCoreApplication::organizationName()).append(QDir::separator())
	    .append(QCoreApplication::applicationName());
	return path;
}

} // end namespace StandardPaths
