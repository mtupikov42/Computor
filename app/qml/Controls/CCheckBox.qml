import QtQuick 2.9
import QtQuick.Templates 2.2 as Templates
import QtQuick.Layouts 1.3

import Theme 1.0
import SizeProvider 1.0
import QmlColorPalette 1.0

Templates.Control {
    id: root

    padding: 0
    implicitHeight: contentItem.implicitHeight + topPadding + bottomPadding
    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding

    property bool checked: false
    property alias orientation: contentLayout.layoutDirection
    property alias text: contentText.text

    signal toggled()

    contentItem: Item {
        implicitWidth: contentLayout.implicitWidth
        implicitHeight: contentLayout.implicitHeight

        RowLayout {
            id: contentLayout

            anchors.fill: parent
            spacing: SizeProvider.metric(5)
            layoutDirection: Qt.LeftToRight

            Rectangle {
                id: indicatorBackground

                implicitWidth: SizeProvider.metric(15)
                implicitHeight: SizeProvider.metric(15)
                color: QmlColorPalette.normal.background
                radius: Theme.defaultBorderRadius
                Layout.alignment: Qt.AlignVCenter

                Rectangle {
                    id: indicator

                    anchors.centerIn: parent
                    implicitWidth: SizeProvider.metric(7)
                    implicitHeight: SizeProvider.metric(7)
                    radius: SizeProvider.metric(2)
                    scale: root.checked ? 1. : 0.
                    color: {
                        if (!root.enabled) {
                            return QmlColorPalette.disabled.indication
                        }

                        if (controlMouseArea.pressed) {
                            return QmlColorPalette.active.indication
                        }

                        if (controlMouseArea.containsMouse) {
                            return QmlColorPalette.hovered.indication
                        }

                        return QmlColorPalette.normal.indication
                    }

                    Behavior on scale { ScaleAnimator { duration: 100 } }
                    Behavior on color { ColorAnimation { duration: 100 } }
                }
            }

            CText {
                id: contentText

                color: {
                    if (!root.enabled) {
                        return QmlColorPalette.disabled.text
                    }

                    if (controlMouseArea.pressed || root.checked) {
                        return QmlColorPalette.active.text
                    }

                    if (controlMouseArea.containsMouse) {
                        return QmlColorPalette.hovered.text
                    }

                    return QmlColorPalette.normal.text
                }

                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                Behavior on color { ColorAnimation { duration: 100 } }
            }
        }
    }

    MouseArea {
        id: controlMouseArea

        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            root.checked = !root.checked
            root.toggled()
        }
    }
}
