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

        onVariableInserted: {
            historyArea.selectVariables()
        }
    }

    RowLayout {
        id: mainRow

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: layoutMargin
        }

        readonly property real layoutMargin: SizeProvider.metric(5)

        ColumnLayout {
            Layout.preferredWidth: parent.width - examples.width - mainRow.layoutMargin

            spacing: SizeProvider.metric(10)

            WidgetComputorHistoryArea {
                id: historyArea

                Layout.fillHeight: true

                implicitWidth: parent.width

                onContentCopied: inputField.pasteTextAndGainFocus(content)
            }

            CTextFieldWithHistory {
                id: inputField

                function pasteTextAndGainFocus(toPaste) {
                    inputField.textField.text = toPaste
                    inputField.gainTextFieldFocus()
                }

                implicitWidth: parent.width
                historyModel: InputHistoryList
                inputModel: InputModel
                textField.placeholderText: qsTr("Please enter function / variable / matrix..")
                textField.tooltip.visible: false
                textField.tooltip.color: QmlColorPalette.generic.error
                textField.tooltip.placement: Theme.Right
            }
        }

        ComputorExamples {
            id: examples

            Layout.alignment: Qt.AlignTop | Qt.AlignRight

            onExampleCopied: inputField.pasteTextAndGainFocus(example)
        }
    }
}
