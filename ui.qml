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

Rectangle {
    id: mother
    width: 360
    height: 640
    color: "black"

    property bool numericmode: false
    property bool ignoretextchanged: false

Item {
    id: searchview
    x: 0
    width: mother.width
    height: mother.height
    anchors.top: mother.top

    Rectangle {
        id: searchinput
        height: parent.height * 0.08
        width: parent.width
        property alias text: searchtext.text
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.left: parent.left
        radius: 2

        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightgray" }
            GradientStop { position: 1.0; color: "gray" }
            }

        Rectangle {
            id: searchtextholder
            anchors.left: parent.left
            anchors.margins: 4
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width * 0.99
            height: parent.height * 0.8;
            color: "white"
            radius: 8

            CustomInput {
                id: searchtext
                focus: true
                anchors.fill: parent
                subtext: "Search"
                text: ""

                onTextChanged: {
                    if(!ignoretextchanged)
                        appModel.setSearchString(searchtext.text)
                    else
                        ignoretextchanged = false;
                    }
                }
            }
        } //info pane

    Rectangle {
        id: searchresultsitem
        anchors.top: searchinput.bottom
        anchors.left: parent.left
        color: "black"

        width: parent.width * 0.99
        property int withkbheight: parent.height * 0.47
        height: (appModel.p_showkeyboard) ? withkbheight : parent.height * 0.80

        HMDelegate {id: delegate3 }

        ListView {
            id: searchresults
            clip: true
            anchors.fill: parent
            anchors.margins: 5
            model: appModel
            delegate: delegate3
                }
            }

    Item {
        id: keyboard
        width: parent.width * 0.99
        height: parent.height * 0.45
        anchors.bottom: parent.bottom
        visible: appModel.p_showkeyboard
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            anchors.fill: parent
            color: "black"
        }

        KBDelegate {
            id: kbdelegateitem
        }

        ListView {
            id: row1
            orientation: Qt.Horizontal
            width: parent.width
            height: parent.height * 0.27
            anchors.top: parent.top
            anchors.left: parent.left
            interactive: false
            model: kbmodelrow1
            delegate: kbdelegateitem
        }

        ListModel {
            id: kbmodelrow1
            ListElement { character: "q"; number: "1";}
            ListElement { character: "w"; number: "2";}
            ListElement { character: "e"; number: "3";}
            ListElement { character: "r"; number: "4";}
            ListElement { character: "t"; number: "5";}
            ListElement { character: "y"; number: "6";}
            ListElement { character: "u"; number: "7";}
            ListElement { character: "i"; number: "8";}
            ListElement { character: "o"; number: "9";}
            ListElement { character: "p"; number: "0";}
            }

        ListView {
            id: row2
            anchors.top: row1.bottom
            orientation: Qt.Horizontal
            width: parent.width * 0.9
            height: parent.height * 0.27
            anchors.horizontalCenter: parent.horizontalCenter
            interactive: false
            model: kbmodelrow2
            delegate: kbdelegateitem
        }

        ListModel {
            id: kbmodelrow2
            ListElement { character: "a"; number: "@";}
            ListElement { character: "s"; number: "#";}
            ListElement { character: "d"; number: "$";}
            ListElement { character: "f"; number: "!";}
            ListElement { character: "g"; number: "&";}
            ListElement { character: "h"; number: "*";}
            ListElement { character: "j"; number: "-";}
            ListElement { character: "k"; number: "+";}
            ListElement { character: "l"; number: ".";}
        }

        Item {
            id: row3
            width: parent.width
            height: parent.height * 0.27
            anchors.top: row2.bottom

            Item {
                id: numericmodeitem
                width: parent.width * 0.14
                height: parent.height * 0.95
                anchors.left: parent.left
                //anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter

                Rectangle {
                    anchors.fill: parent
                    color: numericMA.pressed ? "lightgray" : "#4d4d4d"
                    radius: 3
                    Text {
                        font.pixelSize: 25
                        text: numericmode ? "A#" : "1#"
                        color: numericMA.pressed ? "black":"white"
                        anchors.centerIn: parent
                        }

                    MouseArea {
                        id: numericMA
                        anchors.fill: parent
                        onPressed: {
                            if(mother.numericmode)
                                mother.numericmode = false;
                            else
                                mother.numericmode = true;
                            }
                        }
                    }
                }

            ListView {
                id: row3keys
                orientation: Qt.Horizontal
                width: parent.width * 0.70
                height: parent.height
                anchors.left: numericmodeitem.right
                anchors.leftMargin: 3
                interactive: false
                model: kbmodelrow3
                delegate: kbdelegateitem
                }

            Rectangle {
                id:backitem
                color: backMA.pressed ? "lightgray" : "#4d4d4d"
                width: parent.width * 0.16
                height: parent.height * 0.95
                anchors.left: row3keys.right
                anchors.verticalCenter: parent.verticalCenter
                radius: 2

                Image {
                    width: 40
                    height: 30
                    anchors.centerIn: parent
                    source: "clear.png"
                    smooth: true
                    }
                MouseArea {
                    id: backMA
                    anchors.fill: parent
                    onPressAndHold: {
                        searchtext.text = ""
                        }

                    onReleased: {
                        chopString(searchtext.text);
                        }
                    }
                }
            }

        ListModel {
            id: kbmodelrow3
            ListElement { character: "z"; number: "(";}
            ListElement { character: "x"; number: ")";}
            ListElement { character: "c"; number: "%";}
            ListElement { character: "v"; number: ":";}
            ListElement { character: "b"; number: "?";}
            ListElement { character: "n"; number: "'"}
            ListElement { character: "m"; number: "/";}
        }

        Item {
            id: row4
            anchors.top: row3.bottom
            anchors.topMargin: 2
            width: parent.width
            height: parent.height * 0.20

            Item {
                id: spaceitem
                width: parent.width * 0.68
                height: parent.height * 0.95
                anchors.top: parent.top
                anchors.topMargin:(parent.height - height) / 2
                anchors.horizontalCenter: parent.horizontalCenter

                Rectangle {
                    anchors.fill: parent
                    color: spaceMA.pressed ? "lightgray" : "#4d4d4d"
                    radius: 3
                    Text {
                        font.pixelSize: 25
                        text: "space"
                        color: spaceMA.pressed ? "black":"white"
                        anchors.centerIn: parent
                        }

                    MouseArea {
                        id: spaceMA
                        anchors.fill: parent
                        onPressed: {
                            var t = " "
                            addchar(t);
                            }
                        }
                    }
                }
            }

    Rectangle {
        id: highlighter
        color: "#0d6bb0"
        height: 50
        width: 50
        radius: 2
        visible: (highlightertext.text == "") ? false : true
        z: 5

        Text {
            id: highlightertext
            anchors.centerIn: parent
            anchors.margins: 10
            font.pixelSize: 30
            color: "white"
            text: ""
            onTextChanged: {
                if(text != "")
                    {
                    highlightertexthider.running = true;
                    }
                }
            }

        Timer {
            id: highlightertexthider
            interval: 330; running: false; repeat: false
            onTriggered: {
                highlightertext.text = ""
                }
            }
        }
    }

    Rectangle {
        id: settingsbutton
        color: "#4d4d4d"
        width:  parent.width * 0.15
        height: row4.height * 0.88
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        //anchors.verticalCenter: parent.verticalCenter
        Image {
            width: 50
            height: parent.height
            anchors.centerIn: parent
            source: "showmoreoptions.png"
            smooth: true
            scale: settingsMA.pressed ? 0.8 : 1.0
            onScaleChanged: {
                if(scale == 1.0)
                    {
                    appModel.MoveToBackground();
                    ignoretextchanged = true;
                    searchtext.text = "";
                    }
                }
            }

        MouseArea {
            id: settingsMA
            anchors.fill: parent
            }
        }

    Rectangle {
        id: exitbutton
        color: "#4d4d4d"
        width:  parent.width * 0.15
        height: row4.height * 0.88
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Image {
            width: 50
            height: parent.height
            anchors.centerIn: parent
            source: "exit.png"
            smooth: true
            scale: exitMA.pressed ? 0.8 : 1.0
            onScaleChanged: {
                if(scale == 1)
                    Qt.quit();
                }
        }

        MouseArea {
            id: exitMA
            anchors.fill: parent
            }
        }

    Behavior on x {
        NumberAnimation{duration: 250}
    }
}

Rectangle {
    id: detailsview
    radius: 5
    width: mother.width * 0.9
    height: mother.height * 0.8
    anchors.left: searchview.right
    anchors.leftMargin: (parent.width - width) / 2
    anchors.top: parent.top
    anchors.topMargin: (parent.height - height) / 2
    color: "#33ffffff"
    border.color: "white"
    border.width: 1

    Text {
        id: cntname
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: "white"
        font.bold: true
        font.pixelSize: 25
        width:  parent.width * 0.85;
        height: parent.height * 0.1;
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10;
        elide: Text.ElideRight
        text: ""
        }

    Rectangle {
        id: number1call
        anchors.top: cntname.bottom
        anchors.topMargin: 10;
        anchors.left: parent.left
        anchors.leftMargin: (parent.width - width) / 2
        width: cntModel.p_number1 == "" ? 0 : parent.width * 0.8;
        height: cntModel.p_number1 == "" ? 0 : parent.height * 0.15;
        color: number1button.pressed ? "#0d6bb0" : "#11ffffff"
        border.color: "white"
        border.width: 1
        radius: 5

        Image {
            source: "call.png"
            height: parent.height == 0 ? 0 : 32
            width: 32
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            }

        Text {
            anchors.centerIn: parent
            width: parent.width * 0.75
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
            font.bold: true
            font.pixelSize: 20
            text: cntModel.p_number1
            }

        MouseArea {
            id: number1button
            anchors.fill: parent

            onReleased: {
                mother.state = "";
                appModel.voicecall(cntModel.p_number1);
                }
            }
        }

    Rectangle {
        id: number1sms
        anchors.top: number1call.bottom
        anchors.topMargin: 10;
        anchors.left: parent.left
        anchors.leftMargin: (parent.width - width) / 2
        width: cntModel.p_number1 == "" ? 0 : parent.width * 0.8;
        height: cntModel.p_number1 == "" ? 0 : parent.height * 0.15;
        color: number1smsbutton.pressed ? "#0d6bb0" : "#11ffffff"
        border.color: "white"
        border.width: 1
        radius: 5

        Image {
            source: "sms.png"
            height: parent.height == 0 ? 0 : 32
            width: 32
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            }


        Text {
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
            font.bold: true
            font.pixelSize: 20
            text: cntModel.p_number1
            width: parent.width * 0.7
            elide: Text.ElideRight
            }

        MouseArea {
            id: number1smsbutton
            anchors.fill: parent

            onReleased: {
                mother.state = "";
                appModel.sendsms(cntModel.p_number1);
                }
            }
        }

    Rectangle {
        id: number2call
        anchors.top: number1sms.bottom
        anchors.topMargin: 10;
        anchors.left: parent.left
        anchors.leftMargin: (parent.width - width) / 2
        width: cntModel.p_number2 == "" ? 0 : parent.width * 0.8;
        height: cntModel.p_number2 == "" ? 0 : parent.height * 0.15;
        color: number2button.pressed ? "#0d6bb0" : "#11ffffff"
        border.color: "white"
        border.width: 1
        radius: 5

        Image {
            source: "call.png"
            height: parent.height == 0 ? 0 : 32
            width: 32
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            }


        Text {
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
            font.bold: true
            font.pixelSize: 20
            text: cntModel.p_number2
            width: parent.width * 0.75
            elide: Text.ElideRight
            }

        MouseArea {
            id: number2button
            anchors.fill: parent

            onReleased: {
                mother.state = "";
                appModel.voicecall(cntModel.p_number2);
                }
            }
        }

    Rectangle {
        id: number2sms
        anchors.top: number2call.bottom
        anchors.topMargin: 10;
        anchors.left: parent.left
        anchors.leftMargin: (parent.width - width) / 2
        width: cntModel.p_number2 == "" ? 0 : parent.width * 0.8;
        height: cntModel.p_number2 == "" ? 0 : parent.height * 0.15;
        color: number2smsbutton.pressed ? "#0d6bb0" : "#11ffffff"
        border.color: "white"
        border.width: 1
        radius: 5

        Image {
            source: "sms.png"
            height: parent.height == 0 ? 0 : 32
            width: 32
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            }

        Text {
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
            font.bold: true
            font.pixelSize: 20
            text: cntModel.p_number2
            width: parent.width * 0.7
            elide: Text.ElideRight
            }

        MouseArea {
            id: number2smsbutton
            anchors.fill: parent

            onReleased: {
                mother.state = "";
                appModel.sendsms(cntModel.p_number2);
                }
            }
        }

    Rectangle {
        id: backrect
        anchors.top: number2sms.bottom
        anchors.topMargin: 10;
        anchors.left: parent.left
        anchors.leftMargin: (parent.width - width) / 2
        width: parent.width * 0.8;
        height: parent.height * 0.1;
        color: backbutton.pressed ? "#0d6bb0" : "#11ffffff"
        border.color: "white"
        border.width: 1
        radius: 5

        Text {
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
            font.bold: true
            font.pixelSize: 20
            text: "back"
            }

        MouseArea {
            id: backbutton
            anchors.fill: parent

            onReleased: {
                mother.state = "";
                }
            }
        }
    }


ErrorDialog {
    id: syserrordialog
    z: 5
    anchors.top: parent.top
    anchors.topMargin: parent.height * 0.20
    anchors.horizontalCenter: parent.horizontalCenter
    }

Text {
    anchors.top: parent.top
    anchors.topMargin: parent.height * 0.33
    anchors.left: parent.left
    anchors.leftMargin: (parent.width - width) / 2
    font.pixelSize: 25
    text: "Loading..."
    color: "white"
    font.bold: true
    width: 75
    height: 30
    visible: appModel.p_modelsready ? false : true
    }

states: [
    State {
        name: "showdetails";
        PropertyChanges {target: searchview; x: -mother.width-5;}
        }
    ]

transitions: [
    Transition {
        from: ""; to: "showdetails"
        NumberAnimation {property : "x"; duration: 100;}
        },
    Transition {
        from: "showdetails"; to: ""
        NumberAnimation {property : "x"; duration: 1;}
        }
    ]

function addchar(txt)
{
    var str = searchtext.text;
    if(str.length >= 30) //only 30 characters allowed.
        return;

    var previousposition = searchtext.position;
    //console.log(previousposition)
    var t = str.substring(0, searchtext.position);
    t = t + txt;
    t = t + str.substring(searchtext.position, str.length)
    searchtext.text = t;
    searchtext.position = previousposition+1;
}

function chopString(str)
    {    
    var previousposition = searchtext.position;
    var t = str.substring(0, searchtext.position-1);
    t = t + str.substring(searchtext.position, str.length)
    searchtext.text = t;
    searchtext.position = previousposition-1;
    }
}
