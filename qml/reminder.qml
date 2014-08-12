import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Rectangle {
    id: reminder;
    border.width: 2;
    border.color: "darkgray";
    signal canceled();
    signal applied();

    Component.onDestruction: console.log("reminder destruction");

    Text {
        id: loseLabel;
        anchors.right: parent.horizontalCenter;
        anchors.rightMargin: 4;
        anchors.bottom: parent.verticalCenter;
        anchors.bottomMargin: -24;
        text: qsTr("Lose");
        width: 100;
        height: 30;
        horizontalAlignment: Text.AlignRight;
        verticalAlignment: Text.AlignVCenter;
        font.pointSize: 16;
    }

    Rectangle {
        id: loseFrame;
        anchors.left: parent.horizontalCenter;
        anchors.leftMargin: 4;
        anchors.bottom: loseLabel.bottom;
        width: 100;
        height: 30;
        border.width: 1;
        border.color: "#2020A0";

        TextInput {
            id: loseEditor;
            anchors.left: parent.left;
            anchors.leftMargin: 2;
            anchors.right: parent.right;
            anchors.rightMargin: 2;
            anchors.verticalCenter: parent.verticalCenter;
            height: parent.height;
            width: parent.width - 4;
            verticalAlignment: TextInput.AlignVCenter;
            font.pointSize: 18;
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
        height: 30;
        horizontalAlignment: Text.AlignRight;
        verticalAlignment: Text.AlignVCenter;
        font.pointSize: 16;
    }

    Rectangle {
        id: gainFrame;
        anchors.left: parent.horizontalCenter;
        anchors.leftMargin: 4;
        anchors.bottom: loseFrame.top;
        anchors.bottomMargin: 4;
        width: 100;
        height: 30;
        border.width: 1;
        border.color: "#2020A0";

        TextInput {
            id: gainEditor;
            anchors.left: parent.left;
            anchors.leftMargin: 2;
            anchors.right: parent.right;
            anchors.rightMargin: 2;
            anchors.verticalCenter: parent.verticalCenter;
            height: parent.height;
            width: parent.width - 4;
            verticalAlignment: TextInput.AlignVCenter;
            font.pointSize: 18;
            validator: DoubleValidator{}
            //text: currentStock.gain;
        }
    }

    Text {
        anchors.bottom: gainFrame.top;
        anchors.bottomMargin: 4;
        anchors.horizontalCenter: parent.horizontalCenter;
        height: 26;
        font.pointSize: 16;
        text: currentStock.name() + " : " + currentStock.price();
    }

    Button {
        id: cancel;
        anchors.right: parent.horizontalCenter;
        anchors.rightMargin: 4;
        anchors.top: loseLabel.bottom;
        anchors.topMargin: 16;
        width: 70;
        height: 28;
        style: Qt.createComponent("GradientButtonStyle.qml");
        text: qsTr("Cancel");
        onClicked: reminder.canceled();
    }
    Button {
        id: apply;
        anchors.left: parent.horizontalCenter;
        anchors.leftMargin: 4;
        anchors.top: cancel.top;
        width: 70;
        height: 28;
        style: Qt.createComponent("GradientButtonStyle.qml");
        text: qsTr("Apply");
        onClicked: {
            currentStock.loss = loseEditor.text;
            currentStock.gain = gainEditor.text;
            reminder.applied();
        }
    }
}
