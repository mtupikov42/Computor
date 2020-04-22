import QtQuick 2.9

import QmlColorPalette 1.0
import SizeProvider 1.0

Rectangle {
    property bool vertical: false
    property int lineLength: SizeProvider.metric(1)

    implicitHeight: vertical ? lineLength : SizeProvider.metric(1)
    implicitWidth: vertical ? SizeProvider.metric(1) : lineLength
    color: QmlColorPalette.generic.line
}
