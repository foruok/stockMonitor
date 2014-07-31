import QtQuick 2.0
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Window {
    id: reminder;
    minimumWidth: 300;
    minimumHeight: 200;
    width: 300;
    height: 200;
    x: 0;
    y: 0;
    visible: true;
    title: qsTr("reminder setting");
    signal canceled();
    signal applied();

    Component.onDestruction: console.log("reminder destruction");
    onClosing:{
        destroy(10);
    }

    Text {
        anchors.right: parent.horizontalCenter;
        anchors.rightMargin: 4;
        anchors.bottom: parent.verticalCenter;
        text: qsTr("Lose");
        width: 100;
        height: 25;
        horizontalAlignment: Text.AlignRight;
        font.pixelSize: 16;
    }

    Rectangle {
        id: loseFrame;
        anchors.left: parent.horizontalCenter;
        anchors.leftMargin: 4;
        anchors.bottom: parent.verticalCenter;
        width: 100;
        height: 25;
        border.width: 1;
        border.color: "#2020A0";

        TextInput {
            id: loseEditor;
            anchors.left: parent.left;
            anchors.leftMargin: 2;
            anchors.right: parent.right;
            anchors.rightMargin: 2;
            anchors.verticalCenter: parent.verticalCenter;
            height: 25;
            width: parent.width - 4;
            verticalAlignment: TextInput.AlignVCenter;
            font.pixelSize: 16;
            validator: DoubleValidator{}
            //text: currentStock.loss;
            //Component.onCompleted: text = currentStock.loss;
        }
    }

    Text {
        anchors.right: parent.horizontalCenter;
        anchors.rightMargin: 4;
        anchors.bottom: loseFrame.top;
        anchors.bottomMargin: 4;
        text: qsTr("Gain");
        width: 100;
        height: 25;
        horizontalAlignment: Text.AlignRight;
        font.pixelSize: 16;
    }

    Rectangle {
        id: gainFrame;
        anchors.left: parent.horizontalCenter;
        anchors.leftMargin: 4;
        anchors.bottom: loseFrame.top;
        anchors.bottomMargin: 4;
        width: 100;
        height: 25;
        border.width: 1;
        border.color: "#2020A0";

        TextInput {
            id: gainEditor;
            anchors.left: parent.left;
            anchors.leftMargin: 2;
            anchors.right: parent.right;
            anchors.rightMargin: 2;
            anchors.verticalCenter: parent.verticalCenter;
            height: 25;
            width: parent.width - 4;
            verticalAlignment: TextInput.AlignVCenter;
            font.pixelSize: 16;
            validator: DoubleValidator{}
            //text: currentStock.gain;
        }
    }

    Text {
        anchors.bottom: gainFrame.top;
        anchors.bottomMargin: 4;
        anchors.horizontalCenter: parent.horizontalCenter;
        height: 26;
        font.pixelSize: 16;
        text: currentStock.name() + " : " + currentStock.price();
    }

    Button {
        id: cancel;
        anchors.right: parent.horizontalCenter;
        anchors.rightMargin: 4;
        anchors.top: parent.verticalCenter;
        anchors.topMargin: 8;
        width: 60;
        height: 25;
        text: qsTr("Cancel");
        onClicked: reminder.canceled();
    }
    Button {
        id: apply;
        anchors.left: parent.horizontalCenter;
        anchors.leftMargin: 4;
        anchors.top: parent.verticalCenter;
        anchors.topMargin: 8;
        width: 60;
        height: 25;
        text: qsTr("Apply");
        onClicked: {
            currentStock.loss = loseEditor.text;
            currentStock.gain = gainEditor.text;
            reminder.applied();
        }
    }
}
