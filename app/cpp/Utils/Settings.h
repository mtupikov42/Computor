#pragma once

#include <QSettings>

#include <memory>

using SettingsUPtr = std::unique_ptr<QSettings>;

namespace settings {

SettingsUPtr create();

} // end namespace settings
