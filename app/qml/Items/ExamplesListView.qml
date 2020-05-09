import QtQuick 2.12
import QtQuick.Layouts 1.3

import "../Controls"

import SizeProvider 1.0
import QmlColorPalette 1.0
import Theme 1.0

Item {
    id: root

    property alias exampleTitle: exampleHeader.text
    property alias exampleModel: exampleListView.model

    signal exampleCopied(string example)

    ColumnLayout {
        anchors.fill: parent
        spacing: SizeProvider.metric(2)

        CText {
            id: exampleHeader
        }

        Rectangle {
            id: examplesRect

            Layout.fillHeight: true
            Layout.fillWidth: true

            border.width: Theme.defaultBorderWidth
            border.color: QmlColorPalette.normal.border
            color: QmlColorPalette.generic.transparent

            HistoryListView {
                id: exampleListView

                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                    margins: examplesRect.border.width
                }
                verticalScrollBar.padding: SizeProvider.metric(2)
                spacing: 0
                delegate: CButton {
                    width: exampleListView.width
                    border.color: QmlColorPalette.generic.transparent
                    radius: 0
                    text: modelData
                    textHorizontalAlignment: Qt.AlignLeft
                    backgroundColor: {
                        if (!root.enabled) {
                            return QmlColorPalette.disabled.background
                        }

                        if (root.pressed || root.highlighted) {
                            return QmlColorPalette.active.background
                        }

                        if (index % 2 === 0) {
                            return QmlColorPalette.hovered.background
                        }

                        return QmlColorPalette.normal.background
                    }

                    onPressed: {
                        root.exampleCopied(text)
                    }
                }

                onCountChanged: currentIndex = 0 // do not roll down
            }
        }
    }
}
