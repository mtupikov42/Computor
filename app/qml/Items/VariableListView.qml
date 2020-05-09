import QtQuick 2.12
import QtQuick.Controls 2.12

import "../Controls"

import SizeProvider 1.0

HistoryListView {
    id: root

    delegate: variableItemDelegate

    signal variableCopied(string variable)

    Component {
        id: variableItemDelegate

        Column {
            spacing: SizeProvider.metric(5)

            VariableDelegate {
                width: root.width

                variableName: varNameRole
                resultString: resultRole
                type: typeRole
                errorString: errorStringRole

                onVariableCopied: root.variableCopied(variable)
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
