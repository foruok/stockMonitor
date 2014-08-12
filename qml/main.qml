import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2

Rectangle {
    objectName: "rootWindow";
    width: 800;
    height: 480;
    x: 0;
    y: 0;
    visible: true;
    id: root;
    property Component reminderComponent: null;
    property var reminderWindow: null;
    function showReminder(){
        if(reminderComponent == null){
            reminderComponent = Qt.createComponent("reminder.qml");
        }
        if(reminderWindow == null){
            reminderWindow = reminderComponent.createObject(
                        root,
                        {
                            "width": 300, "height": 200,
                            "x": (width - 300)/2,
                            "y": (height - 200)/2
                        }
                        );
            reminderWindow.canceled.connect(onReminderSettingCanceled);
            reminderWindow.applied.connect(onReminderSettingApplied);
        }
    }

    function onReminderSettingCanceled(){
        reminderWindow.destroy();
        reminderWindow = null;
        update();
    }

    function onReminderSettingApplied(){
        reminderWindow.destroy();
        reminderWindow = null;
        stockTable.model.saveState();
        update();
    }

    Row {
        id: actionArea;
        anchors.top: parent.top;
        anchors.topMargin: 4;
        anchors.left: parent.left;
        anchors.leftMargin: 4;
        spacing: 8;
        height: 28;
        Text {
            text: qsTr("Stock Code");
            height: 28;
            verticalAlignment: Text.AlignVCenter;
            font.pointSize: 12;
        }
        Rectangle {
            border.width: 1;
            border.color: "#2020A0";
            width: 88;
            height: 28;

            TextInput {
                id: stockCodeEdit;
                maximumLength: 6;
                validator: IntValidator{}
                color: "blue";
                text: "000156";
                anchors.left: parent.left;
                anchors.leftMargin: 2;
                anchors.right: parent.right;
                anchors.rightMargin: 2;
                anchors.verticalCenter: parent.verticalCenter;
                height: parent.height;
                width: parent.width - 4;
                verticalAlignment: TextInput.AlignVCenter;
                font.pointSize: 14;
            }
        }
        Button {
            width: 70;
            height: 28;
            text: qsTr("Add");
            style: Qt.createComponent("GradientButtonStyle.qml");
            onClicked: {
                if(stockCodeEdit.length == 6)
                {
                    stockTable.model.addStock(stockCodeEdit.text);
                }
            }
        }
        Text {
            text: qsTr("add comments");
            height: 28;
            verticalAlignment: Text.AlignVCenter;
            font.pointSize: 12;
            color: "darkgray";
        }
    }

    TableView {
        id: stockTable;
        anchors.left: parent.left;
        anchors.top: actionArea.bottom;
        anchors.topMargin: 8;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        style: TableViewStyle {
            id: tstyle;
            backgroundColor: "white";
            alternateBackgroundColor: "#F6F6F6";
            textColor: "black";
            headerDelegate: Canvas {
                implicitWidth: 10;
                implicitHeight: 32;
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.fillStyle = "#E8E8EC";
                    ctx.fillRect(0, 0, width, height);
                    ctx.strokeStyle = "#D7D7DF";
                    ctx.lineWidth = 2;
                    ctx.save();
                    ctx.beginPath();
                    ctx.moveTo(0, 1);
                    ctx.lineTo(canvasSize.width, 1);
                    ctx.stroke();
                    ctx.beginPath();
                    ctx.moveTo(0, canvasSize.height - 1);
                    ctx.lineTo(canvasSize.width, canvasSize.height - 1);
                    ctx.stroke();
                    ctx.font = "16pt sans-serif";
                    ctx.textAlign = "right";
                    ctx.fillStyle = "#000099";
                    ctx.textBaseline = "middle";
                    ctx.fillText(styleData.value, width - 2, height/2);
                    ctx.restore();
                }
            }
            itemDelegate: Text {
                text: styleData.value;
                verticalAlignment: Text.AlignVCenter;
                horizontalAlignment: Text.AlignRight;
                font.pointSize: 13;
                //color: stockTable.colorOf(styleData.row);
                elide: styleData.elideMode;
                onTextChanged: {
                    switch(styleData.column){
                    case 2:
                    case 3:
                    case 4:
                        color = stockTable.colorOf(styleData.row);
                        break;
                    }
                }
            }
            rowDelegate: Rectangle {
                height: 30; // row height
                color: styleData.selected ? "lightsteelblue" :
                  (styleData.alternate ? tstyle.backgroundColor
                                           : tstyle.alternateBackgroundColor);
            }
        }

        TableViewColumn{
            role: "code";
            title: qsTr("Code");
            width: 86;
            movable: false;
            delegate: Text {
                color: "#551A8B";
                text: styleData.value;
                font.pointSize: 13;
                verticalAlignment: Text.AlignVCenter;
                horizontalAlignment: Text.AlignRight;
            }
        }
        TableViewColumn{
            role: "name";
            title: qsTr("Name");
            width: 90;
            movable: false;
            delegate: Text {
                color: "#551A8B";
                text: styleData.value;
                font.pointSize: 13;
                verticalAlignment: Text.AlignVCenter;
                horizontalAlignment: Text.AlignRight;
            }
        }
        TableViewColumn{
            role: "current";
            title: qsTr("Current");
            width: 80;
            movable: false;
        }
        TableViewColumn{
            role: "offset";
            title: qsTr("Offset");
            width: 80;
            movable: false;
        }
        TableViewColumn{
            role: "percent";
            title: qsTr("Percent");
            width: 80;
            movable: false;
        }

        TableViewColumn{
            role: "upDown";
            title: qsTr("Up/Down");
            width: 150;
            movable: false;
        }
        TableViewColumn{
            role: "gainAndLose";
            title: qsTr("Gain/Lose");
            width: 150;
            movable: false;
            delegate: Text{
                text: styleData.value;
                verticalAlignment: Text.AlignVCenter;
                horizontalAlignment: Text.AlignRight;
                font.pointSize: 13;
                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
                        stockTable.model.updateCurrentStock(styleData.row);
                        root.showReminder();
                    }
                }
            }
        }
        TableViewColumn{
            width: 40;
            movable: false;
            delegate: Canvas {
                implicitHeight: 20;
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.strokeStyle = "#CC0000";
                    ctx.lineWidth = 3;
                    ctx.save();
                    ctx.beginPath();
                    ctx.translate(width/2, height/2);
                    ctx.moveTo(-8, -8);
                    ctx.lineTo(8, 8);
                    ctx.moveTo(-8, 8);
                    ctx.lineTo(8, -8);
                    ctx.stroke();
                    ctx.restore();
                }
                MouseArea {
                    anchors.fill: parent;
                    onClicked: if(root.reminderWindow == null){
                            stockTable.model.remove(styleData.row);
                        }
                }
            }
        }

        function colorOf(row){
            switch(model.stateOf(row)){
            case 0:
                return "black";
            case 1:
                return "red";
            case -1:
                return "green";
            }
        }

        model: stockModel;
    }
}
