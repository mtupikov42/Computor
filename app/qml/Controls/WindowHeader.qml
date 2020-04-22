import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

import SizeProvider 1.0
import QmlColorPalette 1.0
import Theme 1.0
import SpecialSymbols 1.0

Rectangle {
    id: root

    color: QmlColorPalette.generic.windowHeader

    property int visibility
    readonly property rect buttonsRowRect: Qt.rect(
                                               buttonsRow.x,
                                               buttonsRow.y + Theme.windowBorderSize,
                                               buttonsRow.width - Theme.windowBorderSize,
                                               buttonsRow.height
                                            )

    signal close()
    signal maximize()
    signal minimize()
    signal restore()

    RowLayout {
        id: buttonsRow

        anchors {
            top: parent.top
            right: parent.right
        }
        spacing: 0
        layoutDirection: Qt.RightToLeft

        CWindowHeaderButton {
            id: closeButton

            text: SpecialSymbols.faWindowClose
            tooltip.text: qsTr("Close")

            onClicked: root.close()
        }

        CWindowHeaderButton {
            id: maximizeButton

            visible: root.visibility === Window.Windowed
            text: SpecialSymbols.faWindowMaximize
            tooltip.text: qsTr("Maximize")

            onClicked: root.maximize()
        }

        CWindowHeaderButton {
            id: restoreButton

            visible: !maximizeButton.visible
            text: SpecialSymbols.faWindowRestore
            tooltip.text: qsTr("Restore down")

            onClicked: root.restore()
        }

        CWindowHeaderButton {
            id: minimizeButton

            text: SpecialSymbols.faWindowMinimize
            tooltip.text: qsTr("Minimize")

            onClicked: root.minimize()
        }
    }
}
