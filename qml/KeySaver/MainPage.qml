import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: page1
    tools: commonTools

    Label {
        id: label

        width: 314
        height: 25
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 10
        }
        text: "Key : "  + appWindow.mainPassword
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenterOffset: 0
        visible: true
    }

    Button{
        id: button1
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 25 //+ commonTools.height
        }
        height: 50
        width: 300
        text: qsTr("Add new pair")
        onClicked: {
            // open page with new keys data
            newItemPage.showFieldsToFill()
            appWindow.pageStack.push(newItemPage)
        }
    }

    Rectangle {

        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.bottom: button1.top
        anchors.bottomMargin: 30
        anchors.top: label.bottom
        anchors.topMargin: 30

        border.width: 1
        border.color: "black"

        ListView {
            id: pairsView

            anchors.fill: parent

            spacing: 15

            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                     focus: true

            delegate: Item {
                id: item
                x: 5
                height: row1.height
                Column {
                    id: row1
                    spacing: 0

                    Text {
                        text: description
                        font.pointSize: 20

                    }

//                    Row {
                        Text {
                            text: login// + " :"
//                            font.italic: true
                            font.pointSize: 24
                        }

//                        spacing: 25

                        Text {
                            text: password
//                            font.bold: true
                            horizontalAlignment: Text.AlignRight
                            font.pointSize: 20
                            visible: false//pairsView.currentIndex == index
                            width: pairsView.width - 10

                        }
//                    }


                }

                Rectangle {
                    x: 0
                    y: 0
                    height: row1.height
                    width: row1.width
                    border.width: 1
                    border.color: "red"
                    opacity: 0.1
                }

                MouseArea {
//                    anchors.fill: parent

                    x: 0
                    y: 0
                    height: row1.height
                    width: row1.width

                    onClicked: {
                        console.debug("Press on item : " + index)
                        pairsView.currentIndex = index

                        // open page with details of pair
                        newItemPage.showDetails(login, password, description)
                        appWindow.pageStack.push(newItemPage)
                    }
                }
            }
            model: itemsModel

            clip: true
        }

    }
}
