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

#include <QDesktopServices>
#include <QUrl>

#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <apgtask.h>
#include <e32std.h>
#include <apgwgnam.h>
#include <Etel3rdParty.h>
#include <sendui.h>
#include <senduiconsts.h>
#include <cmessagedata.h>
#endif

#include "makecall.h"
#include "model.h"
#include "imageprovider.h"

bool appentrylessthan(const AppEntry &s1, const AppEntry &s2)
{
    if(s1.rank < s2.rank)
        return true;
    else if ( (s1.rank == s2.rank) && (s1.caption < s2.caption) )
        return true;
    else
        return false;
}

Model::Model(ImageProvider* ip):
        m_showkeyboard(true),        
        m_contactsModel(NULL),
        m_applistModel(NULL),
        m_error(""),
        m_searchString(""),
        m_modelsready(false),
        m_searchresults(new QList<AppEntry>()),
        m_ip(ip),
        m_clearresults(false),
        iCallDialer(NULL)
{
    LS("Model::Model =>>");

    QHash<int, QByteArray> roles;
    roles[0] = "title";
    roles[1] = "uid";
    roles[2] = "iscontact";

    setRoleNames(roles);
    roles.clear();

    QFile fileA( NOTHING );
    if(!fileA.exists())
        m_applistModel = new applistModel(this);
    fileA.close();

    QFile file( ONLYAPPS );
    if(!file.exists())
        m_contactsModel  = new contactsModel(this);
    file.close();

    if(m_applistModel)
        {
        QObject::connect(m_applistModel, SIGNAL(modelReady()), this, SLOT(modelReadyHandle()));
        QObject::connect(m_applistModel, SIGNAL(HandleSearchResults(QString,QList<AppEntry>&,int,bool)), this, SLOT(HandleSearchComplete(QString,QList<AppEntry>&,int,bool)));
        }

    if(m_contactsModel)
        {
        QObject::connect(m_contactsModel, SIGNAL(modelReady()), this, SLOT(modelReadyHandle()));
        QObject::connect(m_contactsModel, SIGNAL(HandleSearchResults(QString,QList<AppEntry>&,int,bool)), this, SLOT(HandleSearchComplete(QString,QList<AppEntry>&,int,bool)));
        }

    if(m_contactsModel == NULL && m_applistModel == NULL)
        modelReadyHandle();

    LS("Model::Model <<");
}

Model::~Model()
{
    LS("Model::~Model =>>");
    if(iCallDialer)
        {
        delete iCallDialer;
        iCallDialer = NULL;
        }

    //TODO: is clear sufficient?
    m_searchresults->clear();
    delete m_searchresults;

    LS("Model::~Model <<");
}

int Model::rowCount(const QModelIndex& /*index*/ ) const
{
    //return Min(MAXRESULTS, m_searchresults->count());
    return m_searchresults->count();
}

QVariant Model::data(const QModelIndex& index, int role ) const
{
    if (!index.isValid())
        return QVariant();

    if (role == 0)
        return QVariant((*m_searchresults)[index.row()].caption);
    else if (role == 1)
        return QVariant((*m_searchresults)[index.row()].uid);
    else if (role == 2)
        return QVariant((*m_searchresults)[index.row()].iscontact);
    else
        {return QVariant();}
}

int Model::setSearchString(const QString& a_searchString)
{
    LS("Model::setSearchString =>>");
    m_searchString =  a_searchString.trimmed();
    LS("Search for: "<<m_searchString);

    if(m_searchString.isEmpty())
        {
        emit beginResetModel();
        m_searchresults->clear();
        emit endResetModel();
        return 0;
        }

    m_clearresults = true;

    if(m_applistModel)
        m_applistModel->setSearchString(m_searchString);

    if(m_contactsModel)
        m_contactsModel->setSearchString(m_searchString);

    LS("Model::setSearchString <<");
}

void Model::HandleSearchComplete(const QString& searchString, QList<AppEntry>& results, int nwerror, bool cachedresults)
{
    LS("Model::HandleSearchComplete =>> "<<searchString);

    emit beginResetModel();

    if(m_clearresults)
        m_searchresults->clear();

    for(int i=0;i < results.count(); i++)
        {
        AppEntry& p = results[i];
        if(p.rank == 1)
            m_searchresults->prepend(p);
        else
            m_searchresults->append(p);
        }

    //now sort the results if this is the call from second model.
    if(m_clearresults)
        {
        m_clearresults = false;
        }
    else
        {
        qSort(m_searchresults->begin(), m_searchresults->end(), appentrylessthan);
        }

    for(int j=0;j<m_searchresults->count();j++)
        {
        LS((*m_searchresults)[j].caption);
        LS((*m_searchresults)[j].rank);
        }

    emit endResetModel();

    LS("Model::HandleSearchComplete << ");
}

void Model::quitNow()
{
    LS("Model::quitNow() =>>");
    m_ip->releaseMap();
    emit canQuit();
    LS("Model::quitNow() <<");
}

void Model::ItemSelected(int index)
{
LS("Model::launchApp() >>");
AppEntry& entry = (*m_searchresults)[index];
if(entry.iscontact)
    {
    if(m_contactsModel)
        m_contactsModel->ExecuteAction(QVariant(entry.uid));
    }
else
    {
    if(m_applistModel)
        m_applistModel->ExecuteAction(QVariant(entry.fullName));
    }

LS("Model::launchApp() <<");
}


void Model::MoveToBackground()
{
    RWsSession& wsSession = CEikonEnv::Static()->WsSession();
    TUid uid;
    uid.iUid = APPUID;
    TApaTaskList list (wsSession);
    TApaTask ap = list.FindApp(uid);
    if(ap.Exists())
        ap.SendToBackground();
}

void Model::modelReadyHandle()
{
    LS("Model::modelReadyHandle =>>");
    //only apps model is sending this callback. contacts model will not emit this signal.
    if(m_applistModel)
        {
        m_ip->constructicons(m_applistModel->uids());
        m_applistModel->uids().clear();
        }

    setmodelsready(true);
    LS("Model::modelReadyHandle <<");
}

void Model::voicecall(QString number)
{
    LS("Model::voicecall =>>");
    if(iCallDialer)
        delete iCallDialer;

    //hope number.length() can not be more than 100.
    if(number.length() > 199)
        return;

    TBuf<200> phoneNumber1(number.utf16());
    iCallDialer = CCallDialer::NewL(this, phoneNumber1);

    LS("Model::voicecall <<");
}

void Model::sendsms(QString num)
{
   LS("Model::sendsms =>>");
   if(num.length() > 199)
       return;

   CSendUi* sendAppUi = CSendUi::NewLC();
   CMessageData* message = CMessageData::NewLC();

   TBuf<200> number;
   number.Copy(num.utf16());
   message->AppendToAddressL(number, KNullDesC);

   sendAppUi->CreateAndSendMessageL(KSenduiMtmSmsUid, message, KNullUid, ETrue);

   CleanupStack::PopAndDestroy(2); //sendAppUi & message
   LS("Model::sendsms <<");
}

void Model::CallDialedL(int err)
{
    LS("Model::CallDialedL =>>"<<QString::number(err));
    delete iCallDialer;
    iCallDialer = NULL;
    LS("Model::CallDialedL <<");
}
