import QtQuick 2.12
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.2

import Theme 1.0
import QmlColorPalette 1.0
import SizeProvider 1.0
import SpecialSymbols 1.0

Templates.GroupBox {
    id: root

    implicitWidth: Math.max(background.implicitWidth,
                            label.implicitWidth + leftPadding + rightPadding,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background.implicitHeight, contentHeight + topPadding + bottomPadding)

    contentWidth: contentItem.implicitWidth || (contentChildren.length === 1 ? contentChildren[0].implicitWidth : 0)
    contentHeight: contentItem.implicitHeight || (contentChildren.length === 1 ? contentChildren[0].implicitHeight : 0)

    spacing: SizeProvider.metric(5)
    padding: SizeProvider.metric(12)
    topPadding: padding + label.implicitHeight + spacing

    label: CText {
        x: root.leftPadding
        width: root.availableWidth
        text: root.title
        color: root.enabled ? QmlColorPalette.normal.text : QmlColorPalette.disabled.text
        elide: Text.ElideRight
    }

    background: Rectangle {
        y: root.topPadding - root.padding
        width: parent.width
        height: parent.height - root.topPadding + root.padding

        radius: Theme.defaultBorderRadius
        color: QmlColorPalette.generic.transparent
        border.color: QmlColorPalette.normal.border
        border.width: Theme.defaultBorderWidth
    }
}
