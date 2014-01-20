import QtQuick 1.1
import com.nokia.meego 1.0

import Cryptor 1.0

PageStackWindow {
    id: appWindow

    initialPage: loginPage
    showToolBar: pageStack.currentPage != loginPage

    property string mainPassword : ""

    MainPage {
        id: mainPage
    }

    LoginPage {
        id: loginPage
    }

    NewItemPage {
        id: newItemPage
    }

    ToolBarLayout {
        id: commonTools
        visible: true//

        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    ListModel {
        id: itemsModel

    }

    MyCryptor {
        id: cryptor

    }

    Connections{
        target: cryptor
        onPairDecripted: {
            itemsModel.append({"login": login, "password": password, "description": description})
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack

        MenuLayout {
            MenuItem { text: qsTr("Clear")
                onClicked: {
                    cryptor.clearAllPairs();
                    itemsModel.clear();
                }
            }
            MenuItem { text: qsTr("")
                //onClicked: mainPage.pageStack.pop()
            }
            MenuItem { text: qsTr("Logout")
                onClicked: {
                    itemsModel.clear();
                    mainPage.pageStack.pop()
                }
            }
            MenuItem { text: qsTr("Exit")
                onClicked: Qt.quit()
            }
        }
    }
}
