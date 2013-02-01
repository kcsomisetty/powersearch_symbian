#Powersearch is search app for symbian smartphones
#Copyright (C) 2010 Krishna Somisetty

#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#cutewiki is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

#cusrule.pkg_prerules = \
#"%{\"Somisetty\"}"\
#" "\
#:\"Somisetty\""

DEPLOYMENT += cusrule

symbian {
    TARGET.UID3 = 0x2003AFFD
    TARGET.EPOCHEAPSIZE = 0x20000 0x1000000 #16kb - 1mb
    TARGET.CAPABILITY += PowerMgmt ReadUserData NetworkServices
    TARGET.EPOCSTACKSIZE = 0x14000
    LIBS += -lcone -leikcore -lws32 -lapparc -lapgrfx -laknicon -laknskins -letel3rdparty -lSendui -lcntmodel
    ICON = psearch.svg
    }

DEPLOYMENT.display_name = Power Search
DEPLOYMENT.installer_header = 0x2002CCCF

VERSION = 1.0.0

# Allow network access on Symbian
#symbian:

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
#CONFIG += mobility
#MOBILITY += contacts

SOURCES += main.cpp \
model.cpp \
    imageprovider.cpp \
    applistmodel.cpp \
    contactsmodel.cpp \
    makecall.cpp
HEADERS += model.h \
    config.h \
    imageprovider.h \
    applistmodel.h \
    contactsmodel.h \
    makecall.h


# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()
OTHER_FILES += \
    ui.qml \ 
    HMDelegate.qml \
    ErrorDialog.qml \
    CustomInput.qml \
    KBDelegate.qml


RESOURCES += PowerSearch.qrc

QT += declarative
