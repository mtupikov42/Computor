import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

import SizeProvider 1.0
import ComputorQmlPlugin 1.0
import Theme 1.0

import "../Controls"

Item {
    id: root

    property alias contentLoaderIsActive: mainContentLoader.active
    property alias contentComponent: mainContentLoader.sourceComponent
    property int visibilityBeforeMinimized: Window.Windowed

    readonly property real titleHeight: Theme.windowTitleHeight
    readonly property int minimumWindowHeight: SizeProvider.metric(680)
    readonly property int minimumWindowWidth: SizeProvider.metric(1280)
    readonly property int windowVisibility: customWindow.visibility

    function setWindowInCenter() {
        customWindow.windowX = (Screen.width - customWindow.windowWidth) / 2
        customWindow.windowY = (Screen.height - customWindow.windowHeight) / 2
    }

    function requestWindowRaise() {
        customWindow.raise()
    }

    WindowHeader {
        id: windowHeader

        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
        }
        implicitHeight: root.titleHeight
        visibility: customWindow.visibility

        onClose: customWindow.close()
        onMinimize: {
            root.visibilityBeforeMinimized = customWindow.visibility
            customWindow.visibility = Window.Minimized
        }
        onMaximize: customWindow.visibility = Window.Maximized
        onRestore: customWindow.visibility = root.visibilityBeforeMinimized
    }

    Loader {
        id: mainContentLoader

        anchors {
            top: windowHeader.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }

    CustomWindow {
        id: customWindow

        minimumWidth: root.minimumWindowWidth
        minimumHeight: root.minimumWindowHeight
        windowWidth: Math.min(minimumWidth, Screen.desktopAvailableWidth)
        windowHeight: Math.min(minimumHeight, Screen.desktopAvailableHeight)
        titleHeight: windowHeader.height
        borderWidth: Theme.windowBorderSize
        titleButtonsArea: windowHeader.buttonsRowRect

        readonly property var wsl: WindowStateLoader {}

        onClosing: {
            wsl.saveWindowState(customWindow.saveWindowState())
            UIController.quit()
        }
        onTitleAreaClicked: mainContentLoader.item.gainFocus()

        Component.onCompleted: customWindow.restoreWindowState(wsl.restoreWindowState())
    }
}
