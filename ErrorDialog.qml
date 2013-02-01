//Powersearch is search app for symbian smartphones
//Copyright (C) 2010 Krishna Somisetty

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Powersearch is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

import QtQuick 1.0

Item {
    width: mother.width * 0.9
    height: mother.height * 0.28
    property alias text: message.text
    visible: text != "" ? true : false
    z:5
		
    Rectangle {
        anchors.fill: parent
        color: "gray"
        radius: 5
        }

    Image {
        id: warningicon
        source: "warning.png"
        anchors.left: parent.left
        anchors.top: parent.top
        width: 38
        height: 38
        anchors.leftMargin: 15
        anchors.topMargin: 15
        }

    Text {
        id: message
        anchors.left: warningicon.right
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.leftMargin: 10
        width: parent.width - warningicon.width - 45 //(icon's left margin + text's leftmargin + some space on right side)
        height: parent.height * 0.66
        color: "black"
        font.bold: true
        font.pixelSize: 20
        font.wordSpacing: 6
        wrapMode: Text.Wrap
        text: appModel.p_error;
        }

    TextButton {
        id: okbutton
        anchors.bottom: parent.bottom
        //anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.8
        height: parent.height * 0.30
        anchors.bottomMargin: height * 0.44
        anchors.left: parent.left
        anchors.leftMargin: parent.width * 0.1
        text: "OK";
        onClicked: {
        //FIXME: make this generic and delink this with property
        appModel.p_error = "";
            }
        }
    }
