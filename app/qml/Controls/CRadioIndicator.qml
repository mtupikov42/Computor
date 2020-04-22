import QtQuick 2.12
import QtQuick.Layouts 1.3

import SizeProvider 1.0
import QmlColorPalette 1.0
import SpecialSymbols 1.0
import Theme 1.0

Rectangle {
    id: root

    implicitWidth: SizeProvider.metric(15)
    implicitHeight: SizeProvider.metric(15)
    color: QmlColorPalette.normal.background
    radius: Theme.defaultBorderRadius
    Layout.alignment: Qt.AlignVCenter

    property alias text: contentText.text
    property Item control

    CIconText {
        id: contentText

        implicitWidth: root.width
        implicitHeight: root.height
        scale: root.control.checked ? 1. : 0.
        text: SpecialSymbols.faChecked
        color: {
            if (!root.control.enabled) {
                return QmlColorPalette.disabled.indication
            }

            if (root.control.pressed || root.control.checked) {
                return QmlColorPalette.active.indication
            }

            if (root.control.hovered) {
                return QmlColorPalette.hovered.indication
            }

            return QmlColorPalette.normal.indication
        }

        property int easing: Easing.InBack

        Connections {
            target: root.control
            onCheckedChanged: contentText.easing = (root.control.checked ? Easing.OutBack : Easing.InBack)
        }

        Behavior on scale { ScaleAnimator { duration: 100; easing.type: contentText.easing } }
        Behavior on color { ColorAnimation { duration: 100 } }
    }
}
