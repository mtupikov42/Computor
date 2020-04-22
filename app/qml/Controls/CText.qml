import QtQuick 2.9

import Theme 1.0
import QmlColorPalette 1.0

Item {
    id: root

    implicitWidth: internalText.implicitWidth
    implicitHeight: internalText.implicitHeight

    property alias font: internalText.font
    property alias text: internalText.text
    property alias color: internalText.color
    property alias elide: internalText.elide
    property alias padding: internalText.padding
    property alias leftPadding: internalText.leftPadding
    property alias rightPadding: internalText.rightPadding
    property alias bottomPadding: internalText.bottomPadding
    property alias topPadding: internalText.topPadding
    property alias horizontalAlignment: internalText.horizontalAlignment
    property alias verticalAlignment: internalText.verticalAlignment
    property alias wrapMode: internalText.wrapMode
    property alias lineHeight: internalText.lineHeight
    property alias lineHeightMode: internalText.lineHeightMode
    property alias textFormat: internalText.textFormat
    property alias textRotation: internalText.rotation
    property alias isTruncated: internalText.truncated

    Text {
        id: internalText

        padding: 0
        anchors.fill: parent
        font.family: Theme.defaultFontFamily
        font.pointSize: Theme.defaultFontSize
        color: QmlColorPalette.normal.text
        textFormat: Text.PlainText
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
