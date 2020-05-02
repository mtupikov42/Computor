import QtQuick 2.12
import QtQuick.Controls 2.12

import "../Controls"

import SizeProvider 1.0

HistoryListView {
    id: root

    delegate: functionItemDelegate

    signal functionCopied(string func)

    Component {
        id: functionItemDelegate

        Column {
            spacing: SizeProvider.metric(5)

            FunctionDelegate {
                width: root.width

                functionString: functionStringRole
                errorColumn: errorColumnRole
                errorString: errorStringRole

                onFunctionCopied: root.functionCopied(func)
                onRequestRemove: root.model.remove(index)

                CDecorationLine {
                    anchors {
                        top: parent.bottom
                        topMargin: SizeProvider.metric(5)
                        horizontalCenter: parent.horizontalCenter
                    }
                    lineLength: parent.width - SizeProvider.metric(20)
                }
            }
        }
    }
}
