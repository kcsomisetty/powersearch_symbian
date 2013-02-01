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

#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QObject>
#include <QVariant>

#include "config.h"

class CContactDatabase;

class contactsModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(  QString p_number1
                 READ number1
                 WRITE setnumber1
                 NOTIFY number1Changed)

    Q_PROPERTY(  QString p_number2
                 READ number2
                 WRITE setnumber2
                 NOTIFY number2Changed)

public:
    explicit contactsModel(QObject *parent = 0);
    ~contactsModel();
    void ExecuteAction(QVariant variant);
    void setSearchString(const QString& a_searchString);    

    void setnumber1(QString number)
        {
        m_number1 = number;
        emit number1Changed();
        }

    QString number1()
        {
        return m_number1;
        }

    void setnumber2(QString number)
        {
        m_number2 = number;
        emit number2Changed();
        }

    QString number2()
        {
        return m_number2;
        }

signals:
    void number1Changed();
    void number2Changed();
    void modelReady();
    void HandleSearchResults(const QString& searchString, QList<AppEntry>& map, int nwerror, bool cachedresults = false);

public slots:
    void constructModel();
    void Search();

private:
    QList<AppEntry>* m_contacts;
    QString m_searchString;
    QString m_number1;
    QString m_number2;
    CContactDatabase* m_pbkEngine;
};

#endif // CONTACTSMODEL_H
