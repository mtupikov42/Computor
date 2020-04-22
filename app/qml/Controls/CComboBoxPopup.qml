import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Templates 2.2 as Templates

import Theme 1.0
import SizeProvider 1.0

Templates.Popup {
    id: root

    property alias backgroundColor: popupBackground.color
    property alias currentIndex: listView.currentIndex
    property alias model: listView.model

    y: parent.height
    width: parent.width
    implicitHeight: contentItem.implicitHeight
    padding: 0

    background: Rectangle {
        id: popupBackground

        anchors.fill: parent
        color: popupBackground.color
    }

    contentItem: ListView {
        id: listView
        clip: true
        implicitHeight: contentHeight
        ScrollIndicator.vertical: ScrollIndicator {}
    }
}
