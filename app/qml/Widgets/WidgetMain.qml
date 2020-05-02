import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import SizeProvider 1.0
import QmlColorPalette 1.0
import Theme 1.0
import SpecialSymbols 1.0

import "../Controls"

Rectangle {
    id: root

    color: QmlColorPalette.generic.windowBackground

    function gainFocus() {
        mainFocus.forceActiveFocus()
    }

    MouseArea {
        id: mainFocus

        anchors.fill: parent
        cursorShape: Qt.ArrowCursor
        onClicked: forceActiveFocus()
    }

    ColumnLayout {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            margins: SizeProvider.metric(5)
        }

        width: SizeProvider.metric(500)
        spacing: SizeProvider.metric(10)

        WidgetComputorHistoryArea {
            id: historyArea

            Layout.fillHeight: true
            Layout.fillWidth: true

            onContentCopied: inputField.text = content
        }

        CTextField {
            id: inputField

            Layout.fillWidth: true

            function textIsWhitespaceOnly(text) {
                return text.replace(/\s/g, '').length === 0
            }

            onAccepted: {
                var textIsEmpty = text.length === 0

                if (!textIsEmpty && !textIsWhitespaceOnly(text)) {
                    InputModel.readInput(text)
                }

                text = ""
            }
        }
    }
}
