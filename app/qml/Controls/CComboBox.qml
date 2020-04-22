import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Templates 2.2 as Templates
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.2

import Theme 1.0
import SpecialSymbols 1.0
import SizeProvider 1.0
import QmlColorPalette 1.0

Templates.ComboBox {
    id: root

    leftPadding: SizeProvider.metric(4)
    rightPadding: SizeProvider.metric(4)
    bottomPadding: SizeProvider.metric(4)
    topPadding: SizeProvider.metric(4)
    font.family: Theme.defaultFontFamily
    font.pointSize: Theme.defaultFontSize
    implicitHeight: Theme.defaultButtonHeight
    implicitWidth: contentText.implicitWidth + leftPadding + rightPadding + comboboxIndicator.implicitWidth

    property color color: QmlColorPalette.normal.text
    property alias indicatorWidth: comboboxIndicator.width
    property bool borderEnabled: true
    property real radius: Theme.defaultBorderRadius

    background: Rectangle {
        id: controlBackground

        radius: root.radius
        color: QmlColorPalette.normal.background
        border.width: Theme.defaultBorderWidth
        border.color: QmlColorPalette.normal.border
    }

    popup: CComboBoxPopup {
        backgroundColor: controlBackground.color
        currentIndex: root.highlightedIndex
        model: root.popup.visible ? root.delegateModel : null
    }

    delegate: ItemDelegate {
        implicitWidth: root.width
        implicitHeight: Theme.defaultButtonHeight
        leftPadding: root.leftPadding
        highlighted: root.highlightedIndex === index

        contentItem: CText {
            text: modelData
            elide: Text.ElideRight
            color: {
                if (pressed) {
                    return QmlColorPalette.active.text
                }

                if (hovered) {
                    return QmlColorPalette.hovered.text
                }

                return QmlColorPalette.normal.text
            }
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }

        background: Item {
            visible: false
        }
    }

    contentItem: CText {
        id: contentText

        rightPadding: comboboxIndicator.width + Theme.defaultLayoutSpacing
        text: root.currentText
        font: root.font
        color: root.color
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
    }

    indicator: Rectangle {
        id: comboboxIndicator

        anchors.right: parent.right
        implicitHeight: Theme.defaultButtonHeight
        implicitWidth: Theme.defaultButtonHeight
        color: controlBackground.border.color

        CIconText {
            anchors.centerIn: parent
            text: SpecialSymbols.faCaretDown
            rotation: root.popup.visible ? -180 : 0
            color: QmlColorPalette.active.text

            Behavior on rotation {
                RotationAnimator {
                    duration: 200
                    easing.type: Easing.OutQuad
                }
            }
        }
    }

    Ripple {
        clipRadius: 1
        width: parent.width
        height: parent.height
        pressed: root.pressed
        anchor: root
        active: root.down || root.visualFocus || root.hovered
        color: Material.rippleColor
    }
}
