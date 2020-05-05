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

    Connections {
        target: InputModel

        onErrorOccured: {
            inputField.tooltip.text = error
            inputField.tooltip.visible = true
        }

        onExpressionInserted: {
            historyArea.selectExpressions()
        }

        onFunctionInserted: {
            historyArea.selectFunctions()
        }
    }

    RowLayout {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            margins: SizeProvider.metric(5)
        }

        ColumnLayout {
            Layout.preferredWidth: SizeProvider.metric(500)

            spacing: SizeProvider.metric(10)

            WidgetComputorHistoryArea {
                id: historyArea

                Layout.fillHeight: true
                Layout.fillWidth: true

                onContentCopied: inputField.text = content
            }

            CTextFieldWithHistory {
                id: inputField

                Layout.fillWidth: true

                historyModel: InputHistoryList
                inputModel: InputModel
                textField.placeholderText: qsTr("Please enter function / variable / matrix..")
                textField.tooltip.visible: false
                textField.tooltip.color: QmlColorPalette.generic.error
                textField.tooltip.placement: Theme.Right
            }
        }
    }
}
