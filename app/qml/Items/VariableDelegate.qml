import QtQuick 2.12
import QtQuick.Layouts 1.12

import "../Controls"

import SizeProvider 1.0
import SpecialSymbols 1.0
import ComputorQmlPlugin 1.0

Item {
    id: root

    height: currentLoader.item.height

    property string variableName
    property string resultString
    property string errorString
    property string type

    readonly property Loader currentLoader: errorState ? errorLoader : normalLoader
    readonly property bool errorState: errorString.length !== 0
    readonly property real layoutMargin: SizeProvider.metric(10)

    signal variableCopied(string variable)
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

                text: root.resultString
                errorColumn: 0
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
                    tooltip.text: qsTr("Copy variable")

                    onPressed: root.variableCopied(errorText.text)
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
                    id: typeText

                    text: qsTr("Type: ") + root.type
                }

                RowLayout {
                    spacing: SizeProvider.metric(2)

                    CText {
                        id: variableNameText

                        text: root.variableName + ":"
                    }

                    CText {
                        id: resultText

                        text: root.resultString
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
                    tooltip.text: qsTr("Copy variable")

                    onPressed: root.variableCopied(resultText.text)
                }

                CIconButton {
                    id: removeNormalIcon

                    Layout.alignment: Qt.AlignRight

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
}
