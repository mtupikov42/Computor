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

    CTabBar {
        id: tabBar

        anchors {
            top: parent.top
            left: parent.left
            margins: SizeProvider.metric(30)
        }

        width:stackRect.width

        CTabButton {
            text: "Controls 1"
        }

        CTabButton {
            text: "Controls 2"
        }

        CTabButton {
            text: "Controls 3"
        }
    }

    Rectangle {
        id: stackRect

        anchors {
            top: tabBar.bottom
            left: tabBar.left
        }
        width: SizeProvider.metric(220)
        height: SizeProvider.metric(500)
        color: QmlColorPalette.generic.transparent
        border.width: Theme.defaultBorderWidth
        border.color: QmlColorPalette.normal.border

        StackLayout {
            id: stack

            anchors.fill: parent
            anchors.margins: SizeProvider.metric(5)
            width: parent.width
            currentIndex: tabBar.currentIndex

            ColumnLayout {
                spacing: SizeProvider.metric(30)

                CBusyIndicator {
                    breathing: true
                    running: true
                }

                CButton {
                    text: "Button"
                }

                CDelayButton {
                    text: "Delay Button"
                }

                CCheckBox {
                    text: "Check Box"
                }

                CComboBox {
                    implicitWidth: SizeProvider.metric(100)
                    model: [ "Combo1", "Combo2", "Combo3" ]
                }

                CIconButton {
                    text: SpecialSymbols.faBell
                    tooltip.text: "ToolTipText"
                }

                CTextField {
                    implicitWidth: SizeProvider.metric(200)
                    placeholderText: "Text Field.."
                }
            }

            ColumnLayout {
                spacing: SizeProvider.metric(30)

                CSpinBox {
                    from: 0
                    to: 100
                    value: 50
                    editable: true
                }

                CSwitch {
                    text: "Switch"
                }

                CTextArea {
                    width: SizeProvider.metric(200)
                    height: SizeProvider.metric(100)
                    placeholderText: "Text area..."
                }

                CDial {
                    from: 0
                    to: 100
                    text: "Some long value name"
                }

                CSearchField {
                    implicitWidth: SizeProvider.metric(100)
                }
            }

            ColumnLayout {
                spacing: SizeProvider.metric(30)

                CGroupBox {
                    title: "Group box"

                    ColumnLayout {
                        anchors.fill: parent

                        CCheckBox { text: "Check 1" }
                        CCheckBox { text: "Check 2" }
                        CCheckBox { text: "Check 3" }
                    }
                }

                CButton {
                    text: "Open popup"
                    onClicked: cpopup.open()

                    CPopup {
                        id: cpopup

                        parent: root
                        x: (root.width - width) / 2
                        y: (root.height - height) / 2
                        width: SizeProvider.metric(200)
                        height: SizeProvider.metric(200)
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

                        CText {
                            anchors.centerIn: parent
                            text: "Popup text"
                        }
                    }
                }

                CPageIndicator {
                    interactive: true
                    count: 5
                    currentIndex: 3
                }

                CProgressBar {
                    value: 0.5
                }

                ColumnLayout {
                    spacing: SizeProvider.metric(5)

                    CRadioButton {
                        text: "Radio button 1"
                        checked: true
                    }

                    CRadioButton {
                        text: "Radio button 2"
                    }
                }

                CSlider {
                    from: 0
                    to: 100
                    value: 25
                }

                CRangeSlider {
                    from: 0
                    to: 100
                    first.value: 15
                    second.value: 85
                }
            }
        }
    }
}
