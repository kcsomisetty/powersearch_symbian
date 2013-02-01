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

#include "applistmodel.h"
#include <APACMDLN.H>
#include <QTimer>

bool appentrylessthan2(const AppEntry &s1, const AppEntry &s2)
{
    if(s1.rank < s2.rank)
        return true;
    else if ( (s1.rank == s2.rank) && (s1.caption < s2.caption) )
        return true;
    else
        return false;
}

applistModel::applistModel(QObject *parent) :
    QObject(parent),
    m_appslist(NULL)
{
    LS("applistModel::applistModel() >>");
    int err = m_session.Connect();

    if(KErrNone != err)
        {
        emit modelReady();
        }
    else
        {
        QTimer::singleShot(10, this, SLOT(constructappslist()));
        m_appslist = new  QList<AppEntry>();
        }

    LS("applistModel::applistModel() <<");
}

applistModel::~applistModel()
{
    LS("applistModel::~applistModel >>");
    if(m_appslist)
        {
        m_appslist->clear();
        delete m_appslist;
        m_appslist = NULL;
        }

    m_session.Close();
    LS("applistModel::~applistModel <<");
}

void applistModel::constructappslist()
{
    LS("applistModel::constructappslist() >>");

    int err = m_session.GetAllApps();
    if(KErrNone != err)
        return;

    TApaAppInfo appInfo;
    err = m_session.GetNextApp(appInfo);

    while(err == KErrNone)
        {
        TApaAppCapabilityBuf buf;
        m_session.GetAppCapability(buf, appInfo.iUid);
        if(buf().iAppIsHidden == EFalse && appInfo.iUid.iUid != APPUID)
            {
            QString caption((QChar*)appInfo.iCaption.Ptr(),appInfo.iCaption.Length());
            QString fullName((QChar*)appInfo.iFullName.Ptr(),appInfo.iFullName.Length());
            LS(caption);

            if(!caption.isEmpty())
                {
                AppEntry p;
                p.caption = caption;
                p.fullName = fullName;
                p.uid = appInfo.iUid.iUid;
                p.rank = 1; //does not matter. should be same value for alphabatical sorting
                m_appslist->append(p);
                }
            }

        err = m_session.GetNextApp(appInfo);
        }

    for(int i=0; i < m_appslist->count();i++)
        m_uidlist.append(m_appslist->at(i).uid);

    qSort(m_appslist->begin(), m_appslist->end(), appentrylessthan2);
    emit modelReady();

    LS("applistModel::constructappslist() <<");
}

void applistModel::ExecuteAction(QVariant variant)
{
    LS("applistModel::ExecuteAction =>>");
    QString fullName = variant.toString();

    CApaCommandLine* cmdLine = CApaCommandLine::NewL();
    TBuf<256> fn(fullName.utf16());
    cmdLine->SetExecutableNameL(fn);
    cmdLine->SetCommandL(EApaCommandRun);
    //TODO: StartApp returns faillure code. handle it to show error
    m_session.StartApp(*cmdLine);
    delete cmdLine;

    LS("applistModel::ExecuteAction <<");
}

void applistModel::setSearchString(const QString& a_searchString)
{
    LS("applistModel::setSearchString =>>");
    m_searchString = a_searchString;
    QTimer::singleShot(1, this, SLOT(Search()));
    LS("applistModel::setSearchString <<");
}

void applistModel::Search()
{
    LS("applistModel::Search =>>");
    QList<AppEntry> results;
    QString s = m_searchString;

    if(m_appslist != NULL  || m_searchString.isEmpty())
        {
        int rank1results = 0;
        for(int i=0; i < m_appslist->count() ; i++)
            {
            QString appname = m_appslist->at(i).caption;
            if(appname.startsWith(s, Qt::CaseInsensitive))
                {
                AppEntry p = m_appslist->at(i);
                p.iscontact = false;
                if(rank1results <= APPSMODELQUOTA)
                    {
                    rank1results++;
                    p.rank = 1;
                    }
                else
                    {
                    p.rank = 2;
                    }

                results.prepend(p);
                }
            else if(appname.contains(s, Qt::CaseInsensitive))
                {
                AppEntry p = m_appslist->at(i);
                p.iscontact = false;
                p.rank = 3;
                results.append(p);
                }
            else
                {//ignore
                }
            }
        }

    //sort and trim the results before sending.
    qSort(results.begin(), results.end(), appentrylessthan2);

    while(results.count() > MAXRESULTS)
        results.removeLast();

//    for(int j=0;j<results.count();j++)
//        {
//        LS(results[j].caption);
//        LS(results[j].rank);
//        }

    emit HandleSearchResults(s, results, 0, false);
    LS("applistModel::Search << "<<QString::number(results.count()));
}

QList<qint64>& applistModel::uids()
{
    return m_uidlist;
}
