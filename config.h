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

#ifndef POWERSEARCH_CONFIG
#define POWERSEARCH_CONFIG

#include <QtDebug>

#define LS(x)
//#define LS(x) qDebug()<<x

#define APPUID 0x2003AFFD
#define CONFIGFILE "c:\\data\\powersearch.config"
#define MAXRESULTS 15
#define VERSION 1
#define ONLYAPPS "c:\\data\\onlyapps.txt"
#define NOTHING "c:\\data\\nothing.txt"
#define APPSMODELQUOTA 1
#define CONTACTSMODELQUOTA 2

struct AppEntry {
    QString caption;
    QString fullName;
    qint64 uid;
    bool iscontact;
    int rank;
};

#endif //POWERSEARCH_CONFIG
