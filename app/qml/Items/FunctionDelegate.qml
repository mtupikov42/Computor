import QtQuick 2.12
import QtQuick.Layouts 1.12

import "../Controls"

import SizeProvider 1.0
import SpecialSymbols 1.0
import ComputorQmlPlugin 1.0

Item {
    id: root

    height: currentLoader.item.height

    property int errorColumn
    property string errorString
    property string functionString

    readonly property Loader currentLoader: errorState ? errorLoader : normalLoader
    readonly property bool errorState: errorColumn >= 0
    readonly property real layoutMargin: SizeProvider.metric(10)

    signal functionCopied(string func)
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

                text: root.functionString
                errorColumn: root.errorColumn
                errorText: root.errorString
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.rightMargin: root.layoutMargin

                implicitHeight: copyErrorIcon.height

                Item {
                    Layout.fillWidth: true
                }

                CIconButton {
                    id: copyErrorIcon

                    text: SpecialSymbols.faClone
                    tooltip.text: qsTr("Copy function")

                    onPressed: root.functionCopied(errorText.text)
                }


                CIconButton {
                    id: removeErrorIcon

                    text: SpecialSymbols.faTimes
                    tooltip.text: qsTr("Delete")

                    onPressed: root.requestRemove()
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

                    text: root.functionString
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.rightMargin: root.layoutMargin

                implicitHeight: copyNormalIcon.height

                Item {
                    Layout.fillWidth: true
                }

                CIconButton {
                    id: copyNormalIcon

                    Layout.alignment: Qt.AlignRight

                    text: SpecialSymbols.faClone
                    tooltip.text: qsTr("Copy function")

                    onPressed: root.functionCopied(expressionText.text)
                }

                CIconButton {
                    id: removeNormalIcon

                    Layout.alignment: Qt.AlignRight

                    text: SpecialSymbols.faTimes
                    tooltip.text: qsTr("Delete")

                    onPressed: root.requestRemove()
                }
            }
        }
    }
}
