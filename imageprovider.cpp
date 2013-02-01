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

#include "imageprovider.h"
#include <APAID.H>
#include <APGCLI.H>
#include <aknsutils.h>
#include <akniconutils.h>
#include <akniconutils.h>

ImageProvider::ImageProvider(QObject* parent)
    : QObject(parent),
      QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
      m_iconlist(new QMap<qint64, QPixmap*>())
    {
    }

ImageProvider::~ImageProvider()
    {
    LS("ImageProvider::~ImageProvider >>");
    if(m_iconlist)
        {
        m_iconlist->clear();
        delete m_iconlist;
        m_iconlist = NULL;
        }
    LS("ImageProvider::~ImageProvider <<");
    }

void ImageProvider::constructicons (QList<qint64>& uidlist)
    {
    LS("ImageProvider::constructicons >>");

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TSize targetSize(50, 50);

    for(int i=0;i < uidlist.count();i++)
        {
        TUid appuid;
        appuid.iUid = uidlist[i];

        CFbsBitmap *icon = 0;
        CFbsBitmap *mask = 0;

        TRAPD(err, AknsUtils::CreateAppIconLC(skin, appuid, EAknsAppIconTypeList , icon, mask); CleanupStack::Pop();CleanupStack::Pop(););

        if(err != KErrNone)
            continue;

        QPixmap* pixmap = new QPixmap();

        if (icon) {
            TInt error = AknIconUtils::SetSize(icon, targetSize, EAspectRatioNotPreserved);
            if (!error)
                *pixmap = QPixmap::fromSymbianCFbsBitmap(icon);
            }

        if (mask && !pixmap->isNull()) {
            TInt error = AknIconUtils::SetSize(mask, targetSize, EAspectRatioNotPreserved);
            if (!error)
                pixmap->setAlphaChannel(QPixmap::fromSymbianCFbsBitmap(mask));
            }
        m_iconlist->insert(appuid.iUid, pixmap);
        }

    uidlist.clear();

    LS("ImageProvider::constructicons <<");
    }

QImage	ImageProvider::requestImage ( const QString & id, QSize * size, const QSize & requestedSize )
    {
    }

QPixmap	ImageProvider::requestPixmap ( const QString & id, QSize * size, const QSize & requestedSize )
{
    LS("ImageProvider::requestPixmap >>");
    QPixmap pmap = 0;

    QMap<qint64, QPixmap*>::iterator iter = m_iconlist->find(id.toLongLong());
    if (iter != m_iconlist->end())
        pmap = *iter.value();

    return pmap;

    LS("ImageProvider::requestPixmap <<");    
}

void ImageProvider::releaseMap()
{
    LS("ImageProvider::releaseMap >>");

    QMap<qint64, QPixmap*>::iterator iter = m_iconlist->begin();
    while(iter != m_iconlist->end())
        {
        delete iter.value(); //only the pixmap not the iterator entry
        iter++;
        }

    LS("ImageProvider::releaseMap <<");
}
