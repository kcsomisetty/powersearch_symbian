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

Component {
    Rectangle {
        //property color specialcolor: special ? "#60ffffff" : "#00ffffff"
        color: stringMA.pressed ? "#0d6bb0": "#00ffffff"
        width: parent.width * 0.99
        radius: 4
        height: searchresults.height * 0.20;

        Image {
            id: appicon
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: iscontact ? 30: 50
            height: iscontact ? 30 : 50
            source: iscontact ? "file.gif" : "image://model/"+uid;
            }

        Item {
            id: resultstitle
            anchors.left: appicon.right
            anchors.leftMargin: 15
            width: parent.width - appicon.width
            height: parent.height

            Text {
                anchors.verticalCenter: parent.verticalCenter
                color: "white";
                font.pixelSize: 25
                text: title
                elide: Text.ElideRight;
                }
            }

        MouseArea {
            id: stringMA
            anchors.fill: parent
            onReleased: {
                if(title == "Search in Google.com")
                    {
                    appModel.searchInternet(searchtext.text);
                    }
                else
                    {
                    appModel.ItemSelected(index);

                    if(iscontact)
                        {
                        cntname.text = title
                        mother.state = "showdetails";
                        }
                    }
                }
            }
        }
    }
