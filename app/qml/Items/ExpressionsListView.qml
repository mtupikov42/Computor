import QtQuick 2.12
import QtQuick.Controls 2.12

import "../Controls"

import SizeProvider 1.0

HistoryListView {
    id: root

    delegate: expressionItemDelegate

    signal expressionCopied(string expression)

    Component {
        id: expressionItemDelegate

        Column {
            spacing: SizeProvider.metric(5)

            ExpressionDelegate {
                width: root.width

                degree: degreeRole
                errorColumn: errorColumnRole
                solutions: solutionsRole
                rawExpressionString: rawExpressionStringRole
                infixString: infixStringRole
                infixWithParenthesesString: infixWithParenthesesStringRole
                postfixString: postfixStringRole
                prefixString: prefixStringRole
                errorString: errorStringRole
                wolframUrl: wolframUrlRole

                onExpressionCopied: root.expressionCopied(expression)
                onRequestRemove: root.model.remove(index)

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
