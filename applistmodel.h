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

#ifndef APPLISTMODEL_H
#define APPLISTMODEL_H

#include <QObject>
#include <APAID.H>
#include <APGCLI.H>
#include <QVariant>

#include "config.h"

class applistModel : public QObject
{
    Q_OBJECT
public:
    explicit applistModel(QObject *parent = 0);
    ~applistModel();
    void ExecuteAction(QVariant variant);
    void setSearchString(const QString& a_searchString);
    QList<qint64>& uids();

signals:
    void modelReady();
    void HandleSearchResults(const QString& searchString, QList<AppEntry>& map, int nwerror, bool cachedresults = false);

public slots:
    void constructappslist();    
    void Search();

private:
    RApaLsSession m_session;
    QList<AppEntry>* m_appslist;
    QString m_searchString;
    QList<qint64> m_uidlist;
};

#endif // APPLISTMODEL_H
