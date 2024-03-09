import QtQuick 6.5
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

ApplicationWindow {
    id: window
    flags: Qt.Window | Qt.FramelessWindowHint
    width: parent.width
    height: parent.height
    Component.onCompleted: {
        window.showMaximized()
    }

    Rectangle {
        id: titlebar_handler
        x: 0
        y: 0
        width: 1920
        height: 50
        border.color: "#414450"
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#32505e"
            }
            orientation: Gradient.Horizontal
            GradientStop {
                position: 1
                color: "#414450"
            }

        }
        antialiasing: true

        Image {
            id: codeEditorIcon
            x: 8
            y: 4
            width: 65
            height: 42
            fillMode: Image.PreserveAspectFit
        }

        ToolSeparator {
            id: toolSeparator
            x: 79
            y: 8
            height: 36
        }

        Label {
            id: projectName
            x: 100
            y: 11
            width: 216
            height: 36
            font.family: "Source Han Sans"
            color: "#8cb2b0"
            text: qsTr("[Project  Name]")
            styleColor: "#ffffff"
            font.pointSize: 15
        }

        ToolSeparator {
            id: toolSeparator1
            x: 388
            y: 8
            height: 36
        }

        Button {
            id: navFile
            x: 420
            y: 5
            width: 50
            height: 40
            text: qsTr("File")
            autoRepeat: false
            highlighted: false
            font.family: "Source Han Sans"
            font.bold: true
            font.pointSize: 12

            flat: true
            checkable: false
        }

        Button {
            id: navEdit
            x: 470
            y: 5
            width: 50
            height: 40
            text: qsTr("Edit")
            font.family: "Source Han Sans"
            font.pointSize: 12
            font.bold: true
            flat: true
            checkable: false
        }

        Button {
            id: navView
            x: 520
            y: 5
            width: 50
            height: 40
            text: qsTr("View")
            font.family: "Source Han Sans"
            font.pointSize: 12
            font.bold: true
            flat: true
            checkable: false
        }

        Button {
            id: navBuildAndRun
            x: 566
            y: 5
            width: 135
            height: 40
            text: qsTr("Build and Run")
            font.family: "Source Han Sans"
            font.pointSize: 12
            font.bold: true
            flat: true
            checkable: false
        }

        Button {
            id: navTool
            x: 695
            y: 5
            width: 50
            height: 40
            text: qsTr("Tool")
            font.family: "Source Han Sans"
            font.pointSize: 12
            font.bold: true
            flat: true
            checkable: false
        }

        Button {
            id: navGit
            x: 740
            y: 5
            width: 50
            height: 40
            text: qsTr("Git")
            font.family: "Source Han Sans"
            font.pointSize: 12
            font.bold: true
            flat: true
            checkable: false
        }

        Button {
            id: navHelp
            x: 785
            y: 5
            width: 50
            height: 40
            text: qsTr("Help")
            font.family: "Source Han Sans"
            font.pointSize: 12
            font.bold: true
            flat: true
            checkable: false
        }

        ToolSeparator {
            id: toolSeparator2
            x: 1099
            y: 9
            height: 36
        }

        ComboBox {
            id: branchDisplayer
            x: 1125
            y: 10
            width: 430
            height: 34
            font.pointSize: 12
            font.family: "Source Han Sans"
            displayText: qsTr("VCS Branch Displayed Here..")
            antialiasing: true
            flat: true
        }

        Button {
            id: minimize
            x: 1628
            y: 5
            width: 75
            height: 40
            text: qsTr("Min")
            font.family: "Source Han Sans"
            font.pointSize: 15
            font.bold: true
            flat: true
            checkable: false
        }

        Button {
            id: maximize
            x: 1725
            y: 5
            width: 75
            height: 40
            text: qsTr("Max")
            font.family: "Source Han Sans"
            font.pointSize: 15
            font.bold: true
            flat: true
            checkable: false
        }

        Button {
            id: close
            x: 1825
            y: 5
            width: 75
            height: 40
            text: qsTr("Close")
            font.family: "Source Han Sans"
            font.pointSize: 15
            font.bold: true
            flat: true
            checkable: false

            onClicked: {
                window.close()
            }
        }

        ToolSeparator {
            id: toolSeparator3
            x: 1569
            y: 8
            height: 36
        }
    }
}
