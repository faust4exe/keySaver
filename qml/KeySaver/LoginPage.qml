import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: page1
    tools: commonTools

    Label {
        id: label
        height: 50
        text: "Please indicate the key\n and press Login"
        horizontalAlignment: Text.AlignHCenter
        anchors.right: parent.right
        anchors.rightMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.top: parent.top
        anchors.topMargin: 25
        wrapMode: Text.WordWrap
    }

    Button{
        id: button1
        width: 250
        height: 50
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: passField.bottom
            topMargin: 30
        }
        text: qsTr("Login")
        anchors.horizontalCenterOffset: 0
        onClicked: {
            // here change the page and save the password
            mainPassword = passField.text
            passField.text = ""

            cryptor.setKey(mainPassword)
            cryptor.loadItemsWithKey(mainPassword)
            appWindow.pageStack.push(mainPage);
        }

    }

    TextField {
        id: passField
        echoMode: TextInput.Password
        height: 50
        platformEnableEditBubble: false
        errorHighlight: false
        anchors.right: parent.right
        anchors.rightMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.top: label.bottom
        anchors.topMargin: 30


    }

    Label {
        id: label1
        x: 31
        y: 283
        width: 310
        height: 30
//        height: 25

        color: "#ff0000"
        text: "Warning !!!"
        anchors.top: button1.bottom
        anchors.topMargin: 50
        wrapMode: Text.WordWrap
        anchors.rightMargin: 25
        anchors.right: parent.right
        anchors.leftMargin: 50
        horizontalAlignment: Text.AlignLeft
        anchors.left: parent.left
    }

    Label {
        id: label2
        x: 31
        y: 313
        width: 310
        height: 76
        text: "\nKeep in mind that the key is used for decryption and it will not fail in case of wrong key, you just will get wrong information"
        anchors.top: label1.bottom
        anchors.topMargin: 0
        wrapMode: Text.WordWrap
        anchors.rightMargin: 25
        anchors.right: parent.right
        horizontalAlignment: Text.AlignHCenter
        anchors.leftMargin: 25
        anchors.left: parent.left
    }

}
