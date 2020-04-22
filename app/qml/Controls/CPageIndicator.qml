import QtQuick 2.12
import QtQuick.Templates 2.12 as Templates

import SizeProvider 1.0
import Theme 1.0
import QmlColorPalette 1.0

Templates.PageIndicator {
    id: root

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    spacing: SizeProvider.metric(5)

    delegate: Rectangle {
        implicitWidth: SizeProvider.metric(10)
        implicitHeight: SizeProvider.metric(10)
        scale: index === currentIndex ? 1.2 : 1.0
        radius: width / 2
        color: {
            if (!root.enabled) {
                return QmlColorPalette.disabled.indication
            }

            if (index === currentIndex) {
                return QmlColorPalette.generic.accent
            }

            return QmlColorPalette.normal.indication
        }

        opacity: {
            if (index === currentIndex) {
                return 0.9
            }

            if (pressed) {
                return 0.7
            }

            return 0.4
        }

        Behavior on scale { ScaleAnimator { duration: 100 } }
        Behavior on opacity { OpacityAnimator { duration: 100 } }
    }

    contentItem: Row {
        spacing: root.spacing

        Repeater {
            model: root.count
            delegate: root.delegate
        }
    }
}
