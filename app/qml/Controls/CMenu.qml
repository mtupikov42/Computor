import QtQuick 2.9
import QtQuick.Controls 2.2

import Theme 1.0
import SizeProvider 1.0
import QmlColorPalette 1.0

Menu {
    id: root

    background: Rectangle {
        implicitWidth: SizeProvider.metric(150)
        color: QmlColorPalette.normal.background
        border.color: QmlColorPalette.normal.border
        border.width: Theme.defaultBorderWidth
    }
}
