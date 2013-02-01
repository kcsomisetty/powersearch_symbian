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
#ifndef MODEL_H
#define MODEL_H

#include <QtCore>
#include<QTimer>
#include <QAbstractItemModel>

#include "config.h"
#include "applistmodel.h"
#include "contactsmodel.h"

class ImageProvider;
class CCallDialer;

class Model: public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(  bool p_showkeyboard
                 READ showkeyboard
                 WRITE setshowkeyboard
                 NOTIFY showkeyboardChanged)

    Q_PROPERTY(  bool p_modelsready
                 READ modelsready
                 WRITE setmodelsready
                 NOTIFY modelsreadyChanged)

    Q_PROPERTY(  QString p_error
                 READ error
                 WRITE seterror
                 NOTIFY errorChanged)

public:
    Model(ImageProvider*);
    ~Model();

    virtual int rowCount(const QModelIndex& index ) const;
    virtual QVariant data(const QModelIndex& index, int role ) const;


    contactsModel* cntModel() {return m_contactsModel;}
    bool modelsready() {return m_modelsready;}
    void setmodelsready(bool ready) { m_modelsready = ready; emit modelsreadyChanged();}

    bool showkeyboard() {return m_showkeyboard;}

    void setshowkeyboard(bool state) {
        m_showkeyboard = state;
        emit showkeyboardChanged();
        }

    QString error() {return m_error;}

    void seterror(const QString msg) {
        m_error = msg;
        emit errorChanged();
    }

    void CallDialedL(int err);

    Q_INVOKABLE int setSearchString(const QString& searchString);
    Q_INVOKABLE void ItemSelected(int index);
    Q_INVOKABLE void MoveToBackground();
    Q_INVOKABLE void voicecall(QString number);
    Q_INVOKABLE void sendsms(QString number);

signals:
    void canQuit();
    void showkeyboardChanged();
    void busyChanged();
    void errorChanged();
    void modelsreadyChanged();

public slots:
    void HandleSearchComplete(const QString& searchString, QList<AppEntry>& map, int nwerror, bool cachedresults = false);
    void quitNow();
    void modelReadyHandle();
private slots:

private:
    QList<AppEntry>* m_searchresults;
    QString m_searchString;
    QString m_error;
    bool m_showkeyboard;
    bool m_modelsready;
    applistModel* m_applistModel;
    contactsModel* m_contactsModel;
    ImageProvider* m_ip; //Model is the parent
    bool m_clearresults;
    CCallDialer* iCallDialer;
};

#endif // MODEL_H
