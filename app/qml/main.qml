import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.2

import "Widgets"

import SizeProvider 1.0
import ComputorQmlPlugin 1.0
import QmlColorPalette 1.0

Window {
    id: root

    flags: Qt.Window
    visible: customWindowArea.windowVisibility

    Component.onCompleted: {
        customWindowArea.contentLoaderIsActive = true
        customWindowArea.setWindowInCenter()
        customWindowArea.requestWindowRaise()
    }

    Component {
        id: mainComponent

        WidgetMain {}
    }

    CustomWindowAreaWidget {
        id: customWindowArea

        anchors.fill: parent
        contentComponent: mainComponent
    }
}
