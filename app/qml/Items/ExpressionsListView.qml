import QtQuick 2.12
import QtQuick.Controls 2.12

import "../Controls"

import SizeProvider 1.0

Item {
    id: root

    property var model: null

    signal expressionCopied(string expression)

    ListView {
        id: expressionsView

        anchors.fill: parent
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        keyNavigationEnabled: true
        keyNavigationWraps: true
        delegate: expressionItemDelegate
        model: root.model
        spacing: SizeProvider.metric(10)
        ScrollBar.vertical: CScrollBar {
            policy: (expressionsView.contentHeight > expressionsView.height) ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
        }

        onCountChanged: currentIndex = count - 1

        Behavior on bottomMargin { NumberAnimation { duration: 400; easing.type: Easing.OutCubic } }
        Behavior on contentY { NumberAnimation { duration: 400; easing.type: Easing.OutCubic } }
    }

    Component {
        id: expressionItemDelegate

        Column {
            spacing: SizeProvider.metric(5)

            ExpressionDelegate {
                width: expressionsView.width

                degree: degreeRole
                errorColumn: errorColumnRole
                solutions: solutionsRole
                rawExpressionString: rawExpressionStringRole
                infixString: infixStringRole
                infixWithParenthesesString: infixWithParenthesesStringRole
                postfixString: postfixStringRole
                prefixString: prefixStringRole
                errorString: errorStringRole

                onExpressionCopied: root.expressionCopied(expression)
                onRequestRemove: expressionsView.model.remove(index)

                CDecorationLine {
                    anchors {
                        top: parent.bottom
                        horizontalCenter: parent.horizontalCenter
                    }
                    lineLength: parent.width - SizeProvider.metric(20)
                }
            }
        }
    }
}
