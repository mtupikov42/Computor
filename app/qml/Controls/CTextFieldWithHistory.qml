import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

import SizeProvider 1.0
import Theme 1.0
import QmlColorPalette 1.0
import SpecialSymbols 1.0

Item {
    id: root

    implicitHeight: Theme.defaultTextFieldHeight

    property alias textField: textField
    property alias historyModel: btnsView.model // InputHistoryList C++
    property var inputModel: null // InputModel C++
    property int historyPlacement: Theme.Top // only Top or Bottom

    Rectangle {
        id: historyArea

        visible: false
        width: root.width
        height: SizeProvider.metric(150)
        anchors {
            top: root.historyPlacement === Theme.Bottom ? mainRow.bottom : undefined
            bottom: root.historyPlacement === Theme.Top ? mainRow.top : undefined
            left: mainRow.left
            right: mainRow.right
            bottomMargin: root.historyPlacement === Theme.Top ? SizeProvider.metric(5) : 0
            topMargin: root.historyPlacement === Theme.Bottom ? SizeProvider.metric(5) : 0
        }            color: QmlColorPalette.normal.background
        radius: Theme.defaultBorderRadius

        MouseArea {
            anchors.fill: parent
            enabled: historyArea.visible
            preventStealing: true
            hoverEnabled: true
            onWheel: wheel.accepted = true
            onPressed: mouse.accepted = true
            onReleased: mouse.accepted = true
        }

        ListView {
            id: btnsView

            anchors.fill: parent
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            keyNavigationEnabled: true
            keyNavigationWraps: true
            ScrollBar.vertical: CScrollBar {
                policy: (btnsView.contentHeight > btnsView.height) ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
            }
            delegate: CButton {
                width: btnsView.width
                border.color: QmlColorPalette.generic.transparent
                radius: 0
                text: modelRole
                textHorizontalAlignment: Qt.AlignLeft
                backgroundColor: {
                    if (!root.enabled) {
                        return QmlColorPalette.disabled.background
                    }

                    if (root.pressed || root.highlighted) {
                        return QmlColorPalette.active.background
                    }

                    if (index % 2 === 0) {
                        return QmlColorPalette.hovered.background
                    }

                    return QmlColorPalette.normal.background
                }

                onPressed: {
                    historyArea.visible = false
                    textField.text = text
                    textField.forceActiveFocus()
                }
            }

            onCountChanged: currentIndex = count - 1
        }
    }

    RowLayout {
        id: mainRow

        anchors.fill: parent
        spacing: SizeProvider.metric(1)

        CTextField {
            id: textField

            Layout.fillWidth: true

            function textIsWhitespaceOnly(text) {
                return text.replace(/\s/g, '').length === 0
            }

            onAccepted: {
                var textIsEmpty = text.length === 0

                if (!textIsEmpty && !textIsWhitespaceOnly(text)) {
                    root.historyModel.add(text)
                    root.inputModel.readInput(text)
                }

                text = ""
            }

            onPressed: {
                historyArea.visible = false
            }
        }

        CIconButton {
            id: historyButton

            enabled: btnsView.count !== 0
            width: Theme.defaultButtonHeight
            height: Theme.defaultButtonHeight
            text: SpecialSymbols.faCaretLeft
            rotation: historyArea.visible ? 90 : 0
            color: QmlColorPalette.active.text
            tooltip.text: qsTr("Input history")

            onPressed: {
                historyArea.visible = !historyArea.visible
            }

            Behavior on rotation {
                RotationAnimator {
                    duration: 200
                    easing.type: Easing.OutQuad
                }
            }
        }
    }
}
