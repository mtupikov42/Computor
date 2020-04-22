#pragma once

#include <QStandardPaths>
#include <QString>

namespace StandardPaths {

QString writableLocation(QStandardPaths::StandardLocation location);

} // end namespace StandardPaths
