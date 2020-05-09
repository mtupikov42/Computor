import QtQuick 2.12
import QtQuick.Layouts 1.3

import "../Items"

import SizeProvider 1.0

Item {
    id: root

    implicitHeight: examplesColumn.height
    implicitWidth: examplesColumn.width

    signal exampleCopied(string example)

    ColumnLayout {
        id: examplesColumn

        readonly property real examplesWidth: SizeProvider.metric(300)
        readonly property real examplesHeight: SizeProvider.metric(220)

        ExamplesListView {
            id: functionExamples

            implicitHeight: examplesColumn.examplesHeight
            implicitWidth: examplesColumn.examplesWidth
            exampleTitle: qsTr("Function examples:")
            exampleModel: [
                "f1(x) = -10 * x^2 - -4 * x + 7 + x",
                "f2(y) = y * 4 - y^2 * 4",
                "f3(x) = 5 * x^0 + 4 * x^1 - 9.3 * x^2 - 1 * x^0",
                "f4(y) = -69 + y",
                "f5(x) = x^3 - 4 * x + 5 - 6 * x * 20",
                "f6(z) = 10.0 - 14.0 * z + 5.0 * z^2",
                "f7(x) = x - x + x * x - 10 * (10 * x)",
                "f8(c) = c - 9 * (101.101 - c)",
                "f9(x) = x - x + x * x - 10 * (10 * x)",
                "f10(t) = 66.6 * t ^ 3",
                "f11(x) = x * 4 * x - 10 * x ^ 2",
                "f12(m) = m - 100500.0505 * (65.3 * m)"
            ]

            onExampleCopied: {
                root.exampleCopied(example)
            }
        }

        ExamplesListView {
            id: expressionExamples

            implicitHeight: examplesColumn.examplesHeight
            implicitWidth: examplesColumn.examplesWidth
            exampleTitle: qsTr("Expression examples:")
            exampleModel: [
                "x^2 + 7.3 * x = -1 * x^2 + 10 - 5 * x",
                "5 + 4 * x + x^2- x^2 = 0",
                "5*x^0 - 5*x^0 = 0",
                "-3.14*x^3 - x^2 + 10*x - 5 = 2.7 - 2.98*x + x^2 - x^3",
                "5*x^0 - 4*x^0 - 7*x^1 = 0",
                "6 * x^0 + 11 * x^1 + 5 * x^2 - 1 * x^0 - 1 * x^1 = 0",
                "8 * x^0 - 6 * x^1 + 0 * x^2 - 5.6 * x^3 - 3 * x^0 = 0",
                "4 * x ^ 3 - 10 * x ^ 2 + 9.46 * x - 90.5 = 0",
                "5*x^0 + 13*x^1 + 3*x^2 - 1*x^0 - 1*x^1 = 0",
                "x ^ 2 = x ^ 2",
                "(x^2 + (-10.123450 * 660000) + x / 100) = 0",
                "x - x + x * x - 10 * (10 * x) = 0"
            ]

            onExampleCopied: {
                root.exampleCopied(example)
            }
        }
    }
}
