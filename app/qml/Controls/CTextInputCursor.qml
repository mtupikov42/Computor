import QtQuick 2.9

import Theme 1.0
import SizeProvider 1.0

Rectangle {
    id: root

    height: metrics.height
    width: SizeProvider.metric(1)
    visible: parent.focus && !parent.readOnly

    property font font: undefined
    property bool animated: true

    onAnimatedChanged: {
        animation.running = animated
        if (!animated) {
            opacity = 1.
        }
    }

    TextMetrics {
        id: metrics
        font: root.font
        text: "|"
    }

    SequentialAnimation {
        id: animation

        loops: Animation.Infinite
        running: root.visible

        PauseAnimation { duration: 300 }
        OpacityAnimator { target: root; duration: 300; from: 1.; to: 0.; easing.type: Easing.InCubic }
        PauseAnimation { duration: 100 }
        OpacityAnimator { target: root; duration: 200; from: 0.; to: 1.; easing.type: Easing.OutCubic }
    }
}
