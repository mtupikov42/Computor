import QtQuick 2.12

import Theme 1.0
import SpecialSymbols 1.0
import SizeProvider 1.0
import QmlColorPalette 1.0

CTextField {
    id: root

    placeholderText: qsTr("Search...")
    implicitHeight: Theme.defaultButtonHeight
    implicitWidth: contentWidth + leftPadding + rightPadding
    leftPadding: searchIndicator.implicitWidth

    signal trigger()

    function reset() {
        text = ""
        internal.handleSearchConfirmed()
    }

    onTextEdited: internal.handleSearchEdited(text)
    Keys.onEnterPressed: internal.handleSearchConfirmed()
    Keys.onReturnPressed: internal.handleSearchConfirmed()

    QtObject {
        id: internal

        readonly property int searchThreshold: 3
        readonly property int searchDelay: 400

        function handleSearchEdited(text) {
            if (text.length === 0) {
                handleSearchConfirmed()
            } else if (text.length >= searchThreshold) {
                delayTimer.restart()
            }
        }

        function handleSearchConfirmed() {
            delayTimer.stop()
            root.trigger()
        }
    }

    Timer {
        id: delayTimer

        running: false
        repeat: false
        interval: internal.searchDelay

        onTriggered: {
            internal.handleSearchConfirmed()
        }
    }

    CIconText {
        id: searchIndicator

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        text: SpecialSymbols.faSearch
        rightPadding: SizeProvider.metric(5)
        leftPadding: SizeProvider.metric(5)
        color: {
            if (root.activeFocus) {
                return QmlColorPalette.active.icon
            }

            if (!root.enabled) {
                return QmlColorPalette.disabled.icon
            }

            return QmlColorPalette.normal.icon
        }

        Behavior on color { ColorAnimation { duration: 100 } }
    }
}
