import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../Controls"
import "../Items"

import SizeProvider 1.0
import Theme 1.0
import QmlColorPalette 1.0

Item {
    id: root

    signal contentCopied(string content)

    CTabBar {
        id: computorBar

        anchors {
            top: parent.top
            left: parent.left
        }

        width: parent.width

        CTabButton {
            text: "Expressions"
        }
    }

    Rectangle {
        id: stackRect

        anchors {
            top: computorBar.bottom
            left: computorBar.left
        }
        width: root.width
        height: root.height - computorBar.height
        color: QmlColorPalette.generic.transparent
        border.width: Theme.defaultBorderWidth
        border.color: QmlColorPalette.normal.border

        StackLayout {
            id: stack

            anchors.fill: parent
            anchors.margins: SizeProvider.metric(5)
            width: parent.width
            currentIndex: computorBar.currentIndex

            ExpressionsListView {
                id: expressionList

                model: ExpressionList

                onExpressionCopied: root.contentCopied(expression)
            }
        }
    }
}
