import QtQuick 2.2
import QtQuick.Controls.Styles 1.2

ButtonStyle {
    id: globalButtonStyle;
    background: Rectangle {
        implicitWidth: 70;
        implicitHeight: 28;
        border.width: (control.hovered || control.pressed) ? 2 : 1;
        border.color: control.hovered ? "#a0a0a0" : "#808080";
        radius: 4
        gradient: Gradient {
            GradientStop {
                position: 0 ;
                color: control.pressed ? "#A0A0A0" : "#D0D0D0";
            }
            GradientStop {
                position: 1 ;
                color: control.pressed ? "#D0D0D0" : "white";
            }
        }
    }
    label: Text {
        text: control.text;
        font.pointSize: 14;
        anchors.fill: parent;
        horizontalAlignment: Text.AlignHCenter;
        verticalAlignment: Text.AlignVCenter;
    }
}
