import QtQuick 2.9
import QtQuick.Templates 2.2 as Templates

import SizeProvider 1.0
import QmlColorPalette 1.0

Templates.BusyIndicator {
    id: root

    width: SizeProvider.metric(25)
    height: SizeProvider.metric(25)

    property alias color: internal.color
    property alias breathing: internal.breathing
    property alias elementRadius: internal.elementR
    property alias speedFactor: internal.speedFactor
    property int elementsCount: 9

    QtObject {
        id: internal

        property color color: QmlColorPalette.normal.text
        property real indicatorRadius: content.width / 2
        property real elementR: SizeProvider.metric(2.5)
        property bool breathing: false
        property real speedFactor: 1.

        readonly property real elemenD: elementR * 2
    }

    contentItem: Item {
        id: content

        anchors.centerIn: parent
        implicitWidth: Math.min(parent.width, parent.height)
        implicitHeight: Math.min(parent.width, parent.height)

        RotationAnimator {
            target: content
            running: root.visible && root.running && (internal.speedFactor > 0.)
            from: 0
            to: 360
            loops: Animation.Infinite
            duration: (internal.speedFactor <= 0.) ? 0 : 1500 / internal.speedFactor
        }

        SequentialAnimation {
            loops: Animation.Infinite
            running: root.visible && root.running && internal.breathing

            NumberAnimation {
                target: internal
                property: "indicatorRadius"
                from: content.width * 0.4
                to: content.width * 0.5
                duration: 1250
                easing.type: Easing.OutCubic
            }

            NumberAnimation {
                target: internal
                property: "indicatorRadius"
                from: content.width * 0.5
                to: content.width * 0.4
                duration: 1500
                easing.type: Easing.OutCubic
            }

            PauseAnimation { duration: 150 }
        }

        Repeater {
            id: repeater

            model: root.elementsCount

            Rectangle {
                x: content.width / 2 - internal.elementR
                y: content.height / 2 - internal.elementR
                width: internal.elemenD
                height: internal.elemenD
                radius: internal.elementR
                color: root.color
                transform: [
                    Translate {
                        y: -internal.indicatorRadius + internal.elementR
                    },
                    Rotation {
                        angle: index / repeater.count * 360
                        origin.x: internal.elementR
                        origin.y: internal.elementR
                    }
                ]
            }
        }
    }
}
