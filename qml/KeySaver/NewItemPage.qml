import QtQuick 1.1
import com.nokia.meego 1.0

Page{
    id: newItemPage

    property bool isPageEditable: true

    tools: commonTools

    function showDetails(login, password, description) {
        newItemPage.isPageEditable = false

        textfield1.text = login
        textfield2.text = password
        textfield3.text = description
    }

    function showFieldsToFill() {
        newItemPage.isPageEditable = true
    }


    Label {
        id: label
        height: 50
        text: "Please fill the fields with data"
        anchors.right: parent.right
        anchors.rightMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.top: parent.top
        anchors.topMargin: 25
    }

    TextField {
        id: textfield1
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.right: parent.right
        anchors.rightMargin: 25
        anchors.top: label1.bottom
        anchors.topMargin: 5

        readOnly: !newItemPage.isPageEditable
    }

    Label {
        id: label1
        height: 25
        text: "Login :"
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.top: label.bottom
        anchors.topMargin: 50
    }

    TextField {
        id: textfield2
        height: 50
        anchors.right: parent.right
        anchors.rightMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: label2.bottom
        anchors.topMargin: 5

        readOnly: !newItemPage.isPageEditable
    }

    Label {
        id: label2
        height: 25
        text: "Password :"
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.top: textfield1.bottom
        anchors.topMargin: 25
    }

    TextField {
        id: textfield3
        height: 50
        anchors.right: parent.right
        anchors.rightMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: label3.bottom
        anchors.topMargin: 5

        readOnly: !newItemPage.isPageEditable
    }

    Label {
        id: label3
        height: 25
        text: "Description :"
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.top: textfield2.bottom
        anchors.topMargin: 25
    }

    Button {
        id: button1
        x: 34
        y: 495
        width: 250
        height: 50

        text: newItemPage.isPageEditable ? "Save the data" : "Ok"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: textfield3.top
        anchors.bottomMargin: -215

        onClicked: {
            // save the data to the model
            if (newItemPage.isPageEditable)
                cryptor.addPair(textfield1.text, textfield2.text, textfield3.text)


//            itemsModel.append({"login": textfield1.text, "password": textfield2.text, "description": textfield3.text})

            // reset values of fields
            textfield1.text = ""
            textfield2.text = ""
            textfield3.text = ""

            appWindow.pageStack.pop()
        }
    }


}
