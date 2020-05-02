import QtQuick 2.12
import QtQuick.Layouts 1.12

import "../Controls"

import SizeProvider 1.0

Item {
    id: root

    height: currentLoader.item.height

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

    readonly property Loader currentLoader: errorState ? errorLoader : normalLoader
    readonly property bool errorState: errorColumn >= 0

    Loader {
        id: errorLoader

        active: root.errorState
        sourceComponent: errorComponent
    }

    Loader {
        id: normalLoader

        active: !root.errorState
        sourceComponent: normalComponent
    }

    Component {
        id: errorComponent

        RowLayout {
            spacing: SizeProvider.metric(5)

            CErrorText {
                id: errorText

                text: root.rawExpressionString
                errorColumn: root.errorColumn
                errorText: root.errorString
            }
        }
    }

    Component {
        id: normalComponent

        ColumnLayout {
            spacing: SizeProvider.metric(3)

            CText {
                id: expressionText

                text: root.reducedInfixString
            }

            CText {
                id: degreeText

                text: qsTr("Degree: ") + root.degree
            }

            Repeater {
                model: root.solutions
                delegate: CText {
                    text: modelData
                }
            }
        }
    }
}
