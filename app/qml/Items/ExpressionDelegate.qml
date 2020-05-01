import QtQuick 2.12
import QtQuick.Layouts 1.12

import "../Controls"

import SizeProvider 1.0

Item {
    id: root

    property int degree
    property int errorColumn
    property var solutions
    property string rawExpressionString
    property string infixString
    property string infixWithParenthesesString
    property string postfixString
    property string prefixString
    property string reducedInfixString
    property string errorString

    readonly property bool errorState: errorColumn >= 0

    Loader {
        id: errorLoader

        anchors.fill: parent
        active: root.errorState
        sourceComponent: Component {
            CErrorText {
                id: errorText

                text: root.rawExpressionString
                errorColumn: root.errorColumn
                errorText: root.errorString
            }
        }
    }

    Loader {
        id: normalLoader

        anchors.fill: parent
        active: !root.errorState
        sourceComponent: Component {
            CText {
                id: expressionText

                text: root.reducedInfixString
            }
        }
    }
}
