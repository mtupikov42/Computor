import QtQuick 2.9
import QtQuick.Controls 2.2

import Theme 1.0
import SizeProvider 1.0
import QmlColorPalette 1.0

ScrollBar {
    id: root

    padding: 0
    orientation: Qt.Vertical

    background: Rectangle {
        color: QmlColorPalette.generic.transparent
    }

    contentItem: Rectangle {
        implicitWidth: (orientation === Qt.Vertical) ? SizeProvider.metric(4) : 0
        implicitHeight: (orientation === Qt.Vertical) ? 0 : SizeProvider.metric(4)
        radius: Theme.defaultBorderRadius
        color: (root.pressed || root.hovered)
               ? QmlColorPalette.active.indication
               : QmlColorPalette.normal.indication

        Behavior on color { ColorAnimation { duration: 100 } }
    }

    Behavior on size { NumberAnimation { duration: 100 } }
    Behavior on position { enabled: !root.pressed; NumberAnimation { duration: 100 } }
}
