import QtQuick 2.9

Image {
    id: root

    onStatusChanged: {
        if (status == Image.Loading) {
            opacity = 0.
        } else if (status == Image.Ready) {
            opacity = 1.
        }
    }

    Behavior on opacity {
        OpacityAnimator {
            duration: 200
            easing.type: Easing.InCubic
        }
    }
}
