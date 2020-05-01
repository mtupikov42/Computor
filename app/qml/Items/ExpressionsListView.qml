import QtQuick 2.12
import QtQuick.Controls 2.12

import "../Controls"

import SizeProvider 1.0

Item {
    id: root

    property var model: null

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

        Behavior on bottomMargin { NumberAnimation { duration: 400; easing.type: Easing.OutCubic } }
        Behavior on contentY { NumberAnimation { duration: 400; easing.type: Easing.OutCubic } }
    }

    Component {
        id: expressionItemDelegate

        Column {
            spacing: SizeProvider.metric(1)

            ExpressionDelegate {
                width: expressionsView.width
                height: errorState ? SizeProvider.metric(60) : SizeProvider.metric(30)

                degree: degreeRole
                errorColumn: errorColumnRole
                solutions: solutionsRole
                rawExpressionString: rawExpressionStringRole
                infixString: infixStringRole
                infixWithParenthesesString: infixWithParenthesesStringRole
                postfixString: postfixStringRole
                prefixString: prefixStringRole
                reducedInfixString: reducedInfixStringRole
                errorString: errorStringRole

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
