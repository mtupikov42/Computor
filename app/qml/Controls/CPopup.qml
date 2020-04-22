import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Templates 2.12 as Templates
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.2
import QtGraphicalEffects 1.0

import SizeProvider 1.0
import Theme 1.0
import QmlColorPalette 1.0

Templates.Popup {
    id: root

    implicitWidth: Math.max(background.implicitWidth,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background.implicitHeight,
                             contentHeight + topPadding + bottomPadding)

    contentWidth: contentItem.implicitWidth || (contentChildren.length === 1 ? contentChildren[0].implicitWidth : 0)
    contentHeight: contentItem.implicitHeight || (contentChildren.length === 1 ? contentChildren[0].implicitHeight : 0)

    enter: Transition {
        NumberAnimation { property: "scale"; from: 0.9; to: 1.0; easing.type: Easing.OutQuint; duration: 220 }
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; easing.type: Easing.OutCubic; duration: 150 }
    }

    exit: Transition {
        NumberAnimation { property: "scale"; from: 1.0; to: 0.9; easing.type: Easing.OutQuint; duration: 220 }
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; easing.type: Easing.OutCubic; duration: 150 }
    }

    background: Rectangle {
        radius: Theme.defaultBorderRadius
        color: QmlColorPalette.generic.windowHeader
        border.color: QmlColorPalette.normal.border
        border.width: Theme.defaultBorderWidth
    }

    DropShadow {
        width: parent.width
        height: parent.height
        x: -leftPadding
        y: -topPadding

        source: root.background
        color: QmlColorPalette.generic.black10Percent
        verticalOffset: 8
        horizontalOffset: 8
    }
}
