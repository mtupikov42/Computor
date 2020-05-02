import QtQuick 2.12
import QtQuick.Layouts 1.12

import "../Controls"

import SizeProvider 1.0
import SpecialSymbols 1.0
import ComputorQmlPlugin 1.0

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
    property string errorString

    readonly property Loader currentLoader: errorState ? errorLoader : normalLoader
    readonly property bool errorState: errorColumn >= 0
    readonly property real layoutMargin: SizeProvider.metric(10)

    signal expressionCopied(string expression)
    signal requestRemove()

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

                Layout.alignment: Qt.AlignTop
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

                implicitWidth: copyErrorIcon.width

                CIconButton {
                    id: copyErrorIcon

                    text: SpecialSymbols.faClone
                    tooltip.text: qsTr("Copy expression")

                    onPressed: root.expressionCopied(errorText.text)
                }


                CIconButton {
                    id: removeErrorIcon

                    text: SpecialSymbols.faTimes
                    tooltip.text: qsTr("Delete")

                    onPressed: root.requestRemove()
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
                Layout.alignment: Qt.AlignTop

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

                    Layout.alignment: Qt.AlignRight

                    text: SpecialSymbols.faClone
                    tooltip.text: qsTr("Copy expression")

                    onPressed: root.expressionCopied(expressionText.text)
                }

                CIconButton {
                    id: removeNormalIcon

                    Layout.alignment: Qt.AlignRight

                    text: SpecialSymbols.faTimes
                    tooltip.text: qsTr("Delete")

                    onPressed: root.requestRemove()
                }

                CComboBox {
                    id: exprType

                    Layout.alignment: Qt.AlignRight

                    implicitWidth: SizeProvider.metric(160)
                    model: [
                        "Infix",
                        "Infix with parentheses",
                        "Postfix",
                        "Prefix"
                    ]

                    onCurrentIndexChanged: {
                        if (currentIndex === 0) {
                            expressionText.text = root.infixString
                            return
                        }

                        if (currentIndex === 1) {
                            expressionText.text = root.infixWithParenthesesString
                            return
                        }

                        if (currentIndex === 2) {
                            expressionText.text = root.postfixString
                            return
                        }

                        if (currentIndex === 3) {
                            expressionText.text = root.prefixString
                            return
                        }

                        expressionText.text = root.rawExpressionString
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}
