import QtQuick 2.12
import QtQuick.Controls 2.12

import "../Controls"

import SizeProvider 1.0

Item {
    id: root

    property alias model: historyView.model
    property alias delegate: historyView.delegate

    ListView {
        id: historyView

        anchors.fill: parent
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        keyNavigationEnabled: true
        keyNavigationWraps: true
        spacing: SizeProvider.metric(10)
        ScrollBar.vertical: CScrollBar {
            policy: (historyView.contentHeight > historyView.height) ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
        }

        onCountChanged: currentIndex = count - 1

        Behavior on bottomMargin { NumberAnimation { duration: 400; easing.type: Easing.OutCubic } }
        Behavior on contentY { NumberAnimation { duration: 400; easing.type: Easing.OutCubic } }
    }
}
