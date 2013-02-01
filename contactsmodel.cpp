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

#include "contactsmodel.h"

#include <cntdb.h>
#include <cntitem.h>
#include <cntfldst.h>
#include <cntdef.h>
#include <QTimer>

bool appentrylessthan3(const AppEntry &s1, const AppEntry &s2)
{
    if(s1.rank < s2.rank)
        return true;
    else if ( (s1.rank == s2.rank) && (s1.caption < s2.caption) )
        return true;
    else
        return false;
}

contactsModel::contactsModel(QObject *parent) :
    QObject(parent),
    m_pbkEngine(NULL),
    m_number1(""),
    m_number2(""),
    m_contacts(new  QList<AppEntry>())
{
    LS("contactsModel::contactsModel() >>");
    QTimer::singleShot(10, this, SLOT(constructModel()));
    LS("contactsModel::contactsModel() >>");
}

contactsModel::~contactsModel()
{
    LS("contactsModel::~constructModel() >>");

    if(m_contacts)
        {
        m_contacts->clear();
        delete m_contacts;
        m_contacts = NULL;
        }

    if(m_pbkEngine)
        {
        delete m_pbkEngine;
        m_pbkEngine = NULL;
        }

    LS("contactsModel::~constructModel() <<");
}

void contactsModel::constructModel()
{
    LS("contactsModel::constructModel() >>");

    TBuf<256> orgContactFile( KNullDesC );
    CContactDatabase::GetDefaultNameL( orgContactFile );
    orgContactFile.LowerCase();

    TRAPD(err, m_pbkEngine = CContactDatabase::OpenL( orgContactFile ););
    //RDebug::Print(_L("%S"), &orgContactFile);
    LS(QString::number(err));

    //dont emit signal right now. main model cant handle this.
    //emit modelReady();

    LS("contactsModel::constructModel() <<");
}

void contactsModel::ExecuteAction(QVariant variant)
{
    LS("contactsModel::ExecuteAction =>>");
    TContactItemId id = variant.toLongLong();

    CContactItem* item = m_pbkEngine->ReadContactL(id);
    if(item == NULL)
        return;

    CContactItemFieldSet& contactfields = item->CardFields();

    TInt index = contactfields.Find(KUidContactFieldPhoneNumber);
    int i = 0;
    m_number1 = "";
    m_number2 = "";

    while(index >= 0)
        {
        i++;
        const CContactItemField& field1 = contactfields[index];
        TBuf<100> fname;
        fname.Copy(field1.TextStorage()->Text());

        //RDebug::Print(_L("%S"), &fname);

        if(i == 1)
            setnumber1(QString((QChar*)fname.Ptr(), fname.Length()));
        else if (i == 2)
            setnumber2(QString((QChar*)fname.Ptr(), fname.Length()));
        else
            break;

        index = contactfields.FindNext(KUidContactFieldPhoneNumber, index+1);
        }

    if(i == 1) //only one number is added. generate event to clear the second event.
        setnumber2("");
    else if (i== 0)
        {setnumber2("");setnumber1("");}
    else
        {}

    delete item;
    LS("contactsModel::ExecuteAction <<");
}

void contactsModel::setSearchString(const QString& a_searchString)
{
    LS("contactsModel::setSearchString =>>");
    m_searchString = a_searchString;
    QTimer::singleShot(1, this, SLOT(Search()));
    LS("contactsModel::setSearchString <<");
}

void contactsModel::Search()
{
    LS("contactsModel::Search =>>");
    QList<AppEntry> results;
    QString s = m_searchString;

    if(m_pbkEngine != NULL || m_searchString.isEmpty())
        {
        //search string will never cross 30. so this is safe.
        TBuf<50> sbuf(m_searchString.utf16());
        CContactItemFieldDef* fields = new CContactItemFieldDef();
        fields->AppendL(KUidContactFieldGivenName);
        fields->AppendL(KUidContactFieldFamilyName);
        CContactIdArray* array = NULL;
        TRAPD(err, array = m_pbkEngine->FindLC(sbuf, fields); CleanupStack::Pop(array); );

        int rank1results = 0;

        for(int i=0; array && i < array->Count(); i++)
            {
            CContactItem* item = m_pbkEngine->ReadContactL((*array)[i]);
            CContactItemFieldSet& contactfields = item->CardFields();

            QString firstName;
            QString secondName;

            TInt index = contactfields.Find(KUidContactFieldGivenName);
            if(index >= 0)
                {
                TPtrC ptr = contactfields[index].TextStorage()->Text();
                firstName = QString((QChar*)ptr.Ptr(), ptr.Length());
                }

            index = contactfields.Find(KUidContactFieldFamilyName);
            if(index >= 0)
                {
                TPtrC ptr = contactfields[index].TextStorage()->Text();
                secondName = QString((QChar*)ptr.Ptr(), ptr.Length());
                }

            AppEntry p;

            //contacts model quota is 3 ranks
            if(firstName.startsWith(m_searchString, Qt::CaseInsensitive) || secondName.startsWith(m_searchString, Qt::CaseInsensitive))
                {
                if(rank1results <= CONTACTSMODELQUOTA)
                    {p.rank = 1; rank1results++;}
                else
                    p.rank = 2;
                }
            else
                {
                //only 1,2,3 are allowed.
                p.rank = 3;//Max(3, 10 - m_searchString.length());
                }

            if(firstName.isEmpty() && secondName.isEmpty())
                p.caption = "";
            else
                p.caption = firstName + " " + secondName;

            p.uid = (*array)[i];
            p.fullName = "";
            p.iscontact = true;

            if(p.rank != 3)
                results.prepend(p);
            else
                results.append(p);

            delete item;
            }

        delete fields;
        delete array;
        }

    //sort and trim the results before sending.
    qSort(results.begin(), results.end(), appentrylessthan3);

    while(results.count() > MAXRESULTS)
        results.removeLast();

//    for(int j=0;j<results.count();j++)
//        {
//        LS(results[j].caption);
//        LS(results[j].rank);
//        }

    emit HandleSearchResults(s, results, 0, false);

    LS("contactsModel::Search << "<<QString::number(results.count()));
}
