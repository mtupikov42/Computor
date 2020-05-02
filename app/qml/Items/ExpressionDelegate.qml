import QtQuick 2.12
import QtQuick.Layouts 1.12

import "../Controls"

import SizeProvider 1.0
import SpecialSymbols 1.0

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
    readonly property real layoutMargin: SizeProvider.metric(10)

    signal expressionCopied(string expression)

    Loader {
        id: errorLoader

        width: root.width
        active: root.errorState
        sourceComponent: errorComponent
    }

    Loader {
        id: normalLoader

        width: root.width
        active: !root.errorState
        sourceComponent: normalComponent
    }

    Component {
        id: errorComponent

        RowLayout {
            spacing: SizeProvider.metric(5)

            CErrorText {
                id: errorText

                Layout.leftMargin: root.layoutMargin

                text: root.rawExpressionString
                errorColumn: root.errorColumn
                errorText: root.errorString
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.rightMargin: root.layoutMargin

                implicitWidth: copyNormalIcon.width

                CIconButton {
                    id: copyNormalIcon

                    text: SpecialSymbols.faClone
                    tooltip.text: qsTr("Copy expression")

                    onPressed: root.expressionCopied(errorText.text)
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }

    Component {
        id: normalComponent

        RowLayout {
            ColumnLayout {
                Layout.fillHeight: true
                Layout.leftMargin: root.layoutMargin

                implicitWidth: Math.max(expressionText.width, degreeText.width, solRepeater.width)
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
                    id: solRepeater

                    model: root.solutions
                    delegate: CText {
                        text: modelData
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.rightMargin: root.layoutMargin

                implicitWidth: copyNormalIcon.width

                CIconButton {
                    id: copyNormalIcon

                    text: SpecialSymbols.faClone
                    tooltip.text: qsTr("Copy expression")

                    onPressed: root.expressionCopied(expressionText.text)
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}
